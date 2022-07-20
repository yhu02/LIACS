/* EdFS -- An educational file system
 *
 * Copyright (C) 2017,2019  Leiden University, The Netherlands.
 */

/*
5-29-2022
Yvo Hu              s2962802
Seyed Saqlain Zeidi s2982048
*/


#define FUSE_USE_VERSION 26

#include "edfs-common.h"

#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <limits.h>

#include <stdbool.h>
void write_block(edfs_image_t *img, const char **data_block, edfs_block_t block_nr, size_t *size, off_t *offset);

static bool
allocate_block(edfs_image_t *img, edfs_block_t *block_nr);

void update_bitmap(edfs_image_t *img, edfs_block_t block_nr, bool mode);

static int
edfs_get_parent_inode(edfs_image_t *img,
                      const char *path,
                      edfs_inode_t *parent_inode);

static inline edfs_image_t *
get_edfs_image(void)
{
  return (edfs_image_t *)fuse_get_context()->private_data;
}
/* Searches the file system hierarchy to find the inode for
 * the given path. Returns true if the operation succeeded.
 *
 * IMPORTANT: TODO: this function is not yet complete, you have to
 * finish it! See below.
 */
static bool
edfs_find_inode(edfs_image_t *img,
                const char *path,
                edfs_inode_t *inode)
{
  if (strlen(path) == 0 || path[0] != '/')
    return false;

  edfs_inode_t current_inode;
  edfs_read_root_inode(img, &current_inode);

  while (path && (path = strchr(path, '/')))
  {
    /* Ignore path separator */
    while (*path == '/')
      path++;

    /* Find end of new component */
    char *end = strchr(path, '/');
    if (!end)
    {
      int len = strnlen(path, PATH_MAX);
      if (len > 0)
        end = (char *)&path[len];
      else
      {
        /* We are done: return current entry. */
        *inode = current_inode;
        return true;
      }
    }

    /* Verify length of component is not larger than maximum allowed
     * filename size.
     */
    int len = end - path;
    if (len >= EDFS_FILENAME_SIZE)
      return false;

    /* Within the directory pointed to by parent_inode, find the
     * inode number for path, len.
     */
    edfs_dir_entry_t direntry = {
        0,
    };
    strncpy(direntry.filename, path, len);
    direntry.filename[len] = 0;

    if (direntry.filename[0] != 0)
    {
      /* TODO: visit the directory entries of parent_inode and look
       * for a directory entry with the same filename as
       * direntry.filename. If found, fill in direntry.inumber with
       * the corresponding inode number.
       *
       * Write a generic function which visits directory entries,
       * you are going to need this more often. Consider implementing
       * a callback mechanism.
       */

      bool found = false;
      edfs_get_parent_inode(img, path, &current_inode);

      const int DIR_SIZE = edfs_get_n_dir_entries_per_block(&img->sb);

      edfs_dir_entry_t dir[DIR_SIZE];

      for (int i = 0; i < EDFS_INODE_N_BLOCKS; i++)
      {
        if (current_inode.inode.blocks[i] == 0)
          continue;
        // Read all directory entries and iterate over them
        off_t offset = edfs_get_block_offset(&img->sb, current_inode.inode.blocks[i]);

        pread(img->fd, dir, sizeof(edfs_dir_entry_t) * DIR_SIZE, offset);
        for (int j = 0; j < DIR_SIZE; j++)
        {
          if (dir[j].inumber != 0)
          {
            // Match found
            if (strcmp(dir[j].filename, direntry.filename) == 0)
            {
              direntry.inumber = dir[j].inumber;
              found = true;
              goto out;
            }
          }
        }
      }
    out:

      if (found)
      {
        /* Found what we were looking for, now get our new inode. */
        current_inode.inumber = direntry.inumber;
        edfs_read_inode(img, &current_inode);
      }
      else
        return false;
    }

    path = end;
  }

  *inode = current_inode;

  return true;
}

static inline void
drop_trailing_slashes(char *path_copy)
{
  int len = strlen(path_copy);
  while (len > 0 && path_copy[len - 1] == '/')
  {
    path_copy[len - 1] = 0;
    len--;
  }
}

/* Return the parent inode, for the containing directory of the inode (file or
 * directory) specified in @path. Returns 0 on success, error code otherwise.
 *
 * (This function is not yet used, but will be useful for your
 * implementation.)
 */
static int
edfs_get_parent_inode(edfs_image_t *img,
                      const char *path,
                      edfs_inode_t *parent_inode)
{
  int res;
  char *path_copy = strdup(path);

  drop_trailing_slashes(path_copy);

  if (strlen(path_copy) == 0)
  {
    res = -EINVAL;
    goto out;
  }

  /* Extract parent component */
  char *sep = strrchr(path_copy, '/');
  if (!sep)
  {
    res = -EINVAL;
    goto out;
  }

  if (path_copy == sep)
  {
    /* The parent is the root directory. */
    edfs_read_root_inode(img, parent_inode);
    res = 0;
    goto out;
  }

  /* If not the root directory for certain, start a usual search. */
  *sep = 0;
  char *dirname = path_copy;

  if (!edfs_find_inode(img, dirname, parent_inode))
  {
    res = -ENOENT;
    goto out;
  }

  res = 0;

out:
  free(path_copy);

  return res;
}

/* Separates the basename (the actual name of the file) from the path.
 * The return value must be freed.
 *
 * (This function is not yet used, but will be useful for your
 * implementation.)
 */
static char *
edfs_get_basename(const char *path)
{
  char *res = NULL;
  char *path_copy = strdup(path);

  drop_trailing_slashes(path_copy);

  if (strlen(path_copy) == 0)
  {
    res = NULL;
    goto out;
  }

  /* Find beginning of basename. */
  char *sep = strrchr(path_copy, '/');
  if (!sep)
  {
    res = NULL;
    goto out;
  }

  res = strdup(sep + 1);

out:
  free(path_copy);

  return res;
}

/*
 * Implementation of necessary FUSE operations.
 */

static int
edfuse_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
               off_t offset, struct fuse_file_info *fi)
{
  edfs_image_t *img = get_edfs_image();
  edfs_inode_t inode = {
      0,
  };

  if (!edfs_find_inode(img, path, &inode))
    return -ENOENT;

  if (!edfs_disk_inode_is_directory(&inode.inode))
    return -ENOTDIR;

  filler(buf, ".", NULL, 0);
  filler(buf, "..", NULL, 0);

  const int DIR_SIZE = edfs_get_n_dir_entries_per_block(&img->sb);
  edfs_dir_entry_t dir[DIR_SIZE];

  for (int i = 0; i < EDFS_INODE_N_BLOCKS; i++)
  {
    if (inode.inode.blocks[i] == 0)
      continue;

    // Get block offset which is needed to find data block with directory entries
    offset = edfs_get_block_offset(&img->sb, inode.inode.blocks[i]);
    pread(img->fd, dir, img->sb.block_size, offset);
    for (int j = 0; j < DIR_SIZE; j++)
    {
      // If valid dir entry then print filename in dir entry
      if (dir[j].inumber != 0)
        filler(buf, dir[j].filename, NULL, 0);
    }
  }

  return 0;
}

static int
edfuse_mkdir(const char *path, mode_t mode)
{
  edfs_image_t *img = get_edfs_image();

  edfs_inode_t parent_inode = {
      0,
  };
  edfs_inode_t inode = {
      0,
  };

  edfs_block_t blocks[EDFS_INODE_N_BLOCKS] = {0};
  for (int i = 0; i < EDFS_INODE_N_BLOCKS; i++)
  {
    if (!allocate_block(img, &blocks[i]))
      return -ENOSPC;
  }

  edfs_get_parent_inode(img, path, &parent_inode);
  edfs_new_inode(img, &inode, EDFS_INODE_TYPE_DIRECTORY);

  const int DIR_SIZE = edfs_get_n_dir_entries_per_block(&img->sb);
  edfs_dir_entry_t dir[DIR_SIZE];
  off_t offset;
  bool found = false;

  for (int i = 0; i < EDFS_INODE_N_BLOCKS && found == false; i++)
  {
    // Get block offset which is needed to find data block with directory entries
    offset = edfs_get_block_offset(&img->sb, parent_inode.inode.blocks[i]);

    // Read dir entries from parent directory
    pread(img->fd, dir, img->sb.block_size, offset);
    for (int j = 0; j < DIR_SIZE; j++)
    {
      // Dir entry is empty
      if (dir[j].inumber == 0)
      {
        edfs_dir_entry_t dir_entry;
        found = true;

        // Valgrind complaining uninitialised values
        memset(dir_entry.filename, 0, sizeof(dir_entry.filename));

        // Copy filename
        strncpy(dir_entry.filename, edfs_get_basename(path), strlen(edfs_get_basename(path)) + 1);
        dir_entry.inumber = inode.inumber;

        // Write dir_entry to parent directory
        offset = edfs_get_block_offset(&img->sb, parent_inode.inode.blocks[i]) + j * sizeof(edfs_dir_entry_t);
        pwrite(img->fd, &dir_entry, sizeof(edfs_dir_entry_t), offset);

        // Copy block numbers to inode
        memcpy(inode.inode.blocks, blocks, EDFS_INODE_N_BLOCKS * sizeof(edfs_block_t));

        // Write inode to disk
        edfs_write_inode(img, &inode);
        return 0;
      }
    }
  }

  // Error directory is full
  return -EMLINK;
}

static int
edfuse_rmdir(const char *path)
{
  edfs_image_t *img = get_edfs_image();

  edfs_inode_t parent_inode = {
      0,
  };
  edfs_inode_t inode = {
      0,
  };

  if (!edfs_find_inode(img, path, &inode))
    return -ENOENT;

  if (!edfs_disk_inode_is_directory(&inode.inode))
    return -ENOTDIR;

  const int DIR_SIZE = edfs_get_n_dir_entries_per_block(&img->sb);
  edfs_dir_entry_t dir[DIR_SIZE];
  off_t offset;
  bool found = false;

  edfs_get_parent_inode(img, path, &parent_inode);
  for (int i = 0; i < EDFS_INODE_N_BLOCKS; i++)
  {
    // Get block offset which is needed to find data block with directory entries
    offset = edfs_get_block_offset(&img->sb, parent_inode.inode.blocks[i]);

    // Read dir entries from parent directory
    pread(img->fd, dir, img->sb.block_size, offset);
    for (int j = 0; j < DIR_SIZE; j++)
    {
      // Dir entry found
      if (dir[j].inumber == inode.inumber)
      {
        edfs_dir_entry_t dir_entry;
        // Valgrind is complaining
        memset(&dir_entry, 0, sizeof(edfs_dir_entry_t));

        // Delete entry in directory of parent
        dir_entry.inumber = 0;
        offset = edfs_get_block_offset(&img->sb, parent_inode.inode.blocks[i]) + sizeof(edfs_dir_entry_t) * j;
        pwrite(img->fd, &dir_entry, sizeof(edfs_dir_entry_t), offset);
        found = true;
      }
    }
  }

  if (found)
  {
    // Release allocated blocks;
    for (int i = 0; i < EDFS_INODE_N_BLOCKS; i++)
      update_bitmap(img, inode.inode.blocks[i], false);

    // Release inode
    inode.inode.type = EDFS_INODE_TYPE_FREE;
    edfs_write_inode(img, &inode);
    return 0;
  }

  // Error directory entry has not been found in parent directory
  return -ENOENT;
}

/* Get attributes of @path, fill @stbuf. At least mode, nlink and
 * size must be filled here, otherwise the "ls" listings appear busted.
 * We assume all files and directories have rw permissions for owner and
 * group.
 */
static int
edfuse_getattr(const char *path, struct stat *stbuf)
{
  int res = 0;
  edfs_image_t *img = get_edfs_image();

  memset(stbuf, 0, sizeof(struct stat));
  if (strcmp(path, "/") == 0)
  {
    stbuf->st_mode = S_IFDIR | 0755;
    stbuf->st_nlink = 2;
    return res;
  }

  edfs_inode_t inode;
  if (!edfs_find_inode(img, path, &inode))
    res = -ENOENT;
  else
  {
    if (edfs_disk_inode_is_directory(&inode.inode))
    {
      stbuf->st_mode = S_IFDIR | 0770;
      stbuf->st_nlink = 2;
    }
    else
    {
      stbuf->st_mode = S_IFREG | 0660;
      stbuf->st_nlink = 1;
    }
    stbuf->st_size = inode.inode.size;

    /* Note that this setting is ignored, unless the FUSE file system
     * is mounted with the 'use_ino' option.
     */
    stbuf->st_ino = inode.inumber;
  }

  return res;
}

/* Open file at @path. Verify it exists by finding the inode and
 * verify the found inode is not a directory. We do not maintain
 * state of opened files.
 */
static int
edfuse_open(const char *path, struct fuse_file_info *fi)
{
  edfs_image_t *img = get_edfs_image();

  edfs_inode_t inode;
  if (!edfs_find_inode(img, path, &inode))
    return -ENOENT;

  /* Open may only be called on files. */
  if (edfs_disk_inode_is_directory(&inode.inode))
    return -EISDIR;

  return 0;
}

static int
edfuse_create(const char *path, mode_t mode, struct fuse_file_info *fi)
{
  /* TODO: implement
   *
   * Create a new inode, attempt to register in parent directory,
   * write inode to disk.
   */
  return -ENOSYS;
}

/* Since we don't maintain link count, we'll treat unlink as a file
 * remove operation.
 */
static int
edfuse_unlink(const char *path)
{
  /* TODO: implement
   *
   * Validate @path exists and is not a directory; remove directory entry
   * from parent directory; release allocated blocks; release inode.
   */
  return -ENOSYS;
}

static int
edfuse_read(const char *path, char *buf, size_t size, off_t offset,
            struct fuse_file_info *fi)
{
  edfs_image_t *img = get_edfs_image();
  edfs_inode_t inode = {
      0,
  };

  if (!edfs_find_inode(img, path, &inode))
    return -ENOENT;

  const int BLOCK_N = img->sb.block_size / sizeof(edfs_block_t);

  edfs_block_t data_block[BLOCK_N];
  size_t size_it = 0;

  for (int i = 0; i < EDFS_INODE_N_BLOCKS; i++)
  {
    // First occurrence of a 0 means end of allocated indirect blocks. No holes allowed
    if (inode.inode.blocks[i] == 0)
      break;

    // Either the offset of a direct data block or the offset of an indirect block
    offset = edfs_get_block_offset(&img->sb, inode.inode.blocks[i]);

    // Direct data blocks
    if (!edfs_disk_inode_has_indirect(&inode.inode))
    {
      pread(img->fd, buf + size_it, img->sb.block_size, offset);
      size_it += img->sb.block_size;
    }
    else // Indirect blocks
    {
      pread(img->fd, data_block, img->sb.block_size, offset);
      for (int j = 0; j < BLOCK_N; j++)
      {
        // First occurrence of a 0 means end of allocated data blocks. No holes allowed
        if (data_block[j] == 0)
          break;

        offset = edfs_get_block_offset(&img->sb, data_block[j]);

        pread(img->fd, buf + size_it, img->sb.block_size, offset);
        size_it += img->sb.block_size;
      }
    }
  }

  return size_it;
}

static int
edfuse_write(const char *path, const char *buf, size_t size, off_t offset,
             struct fuse_file_info *fi)
{
  edfs_image_t *img = get_edfs_image();
  edfs_inode_t inode = {
      0,
  };

  if (!edfs_find_inode(img, path, &inode))
    return -ENOENT;

  const int BLOCK_N = img->sb.block_size / sizeof(edfs_block_t);
  size_t end_block_inode_index;
  size_t start_block_inode_index;
  size_t start_offset = offset;

  if (!edfs_disk_inode_has_indirect(&inode.inode)) // Direct blocks
  {
    end_block_inode_index = ((offset + size) / img->sb.block_size) + 1;
    start_block_inode_index = (offset / img->sb.block_size);
    // Writes past size of direct blocks, need to use indirect blocks
    if ((offset + size) > (EDFS_INODE_N_BLOCKS * img->sb.block_size))
    {
      edfs_block_t data_block[BLOCK_N];
      memset(data_block, 0, img->sb.block_size);

      for (int j = 0; j < EDFS_INODE_N_BLOCKS; j++)
      {
        if (inode.inode.blocks[j] == 0)
          break;
        // Save already allocated blocks in data block, and remove from inode
        data_block[j] = inode.inode.blocks[j];
        inode.inode.blocks[j] = 0;
      }

      // Allocate indirect block
      if (!allocate_block(img, &inode.inode.blocks[0]))
        return -ENOSPC;

      // Write data block to disk pointed at by indirect offset
      off_t indirect_offset = edfs_get_block_offset(&img->sb, inode.inode.blocks[0]);
      pwrite(img->fd, data_block, img->sb.block_size, indirect_offset);

      // Direct has been converted to indirect, now write the data
      inode.inode.type += EDFS_INODE_TYPE_INDIRECT;
      goto indirect;
    }
    else if ((offset + size) <= (EDFS_INODE_N_BLOCKS * img->sb.block_size)) // Does not write past size of direct blocks
    {
      for (int i = start_block_inode_index; i < end_block_inode_index; i++)
      {
        // Allocate direct block
        if (inode.inode.blocks[i] == 0)
          if (!allocate_block(img, &inode.inode.blocks[i]))
            return -ENOSPC;

        write_block(img, &buf, inode.inode.blocks[i], &size, &offset);
      }
    }
    // Update inode on disk
    if (inode.inode.size < offset)
      inode.inode.size = offset;

    edfs_write_inode(img, &inode);
  }
  else // Indirect blocks
  {
  indirect:
    // Initialise starting index for indirect block in inode
    end_block_inode_index = ((offset + size) / (img->sb.block_size * (img->sb.block_size / sizeof(edfs_block_t)))) + 1;
    start_block_inode_index = (offset / (img->sb.block_size * (img->sb.block_size / sizeof(edfs_block_t))));

    for (int i = start_block_inode_index; i < end_block_inode_index; i++)
    {
      // Allocate indirect block
      if (inode.inode.blocks[i] == 0)
        if (!allocate_block(img, &inode.inode.blocks[i]))
          return -ENOSPC;

      // Get the offset for the indirect block
      off_t indirect_offset = edfs_get_block_offset(&img->sb, inode.inode.blocks[i]);

      // Initialise indirect block containing block numbers pointed to by indirect block_nr
      edfs_block_t indirect_block[BLOCK_N];
      memset(indirect_block, 0, img->sb.block_size);

      // Read the stored indirect block from disk containing block numbers
      pread(img->fd, indirect_block, img->sb.block_size, indirect_offset);

      size_t end_block_indirect_index = (((offset + size) / (img->sb.block_size)) % (img->sb.block_size / sizeof(edfs_block_t))) + 1;
      size_t start_block_indirect_index = (offset / img->sb.block_size) % (img->sb.block_size / sizeof(edfs_block_t));

      for (int j = start_block_indirect_index; j < end_block_indirect_index && size != 0; j++)
      {
        // Allocate 1 block referenced inside indirect block
        if (indirect_block[j] == 0)
          if (!allocate_block(img, &indirect_block[j]))
            return -ENOSPC;

        write_block(img, &buf, indirect_block[j], &size, &offset);
      }
      // Update indirect data block
      pwrite(img->fd, indirect_block, img->sb.block_size, indirect_offset);
    }
    // Update inode on disk
    if (inode.inode.size < offset)
      inode.inode.size = offset;
    edfs_write_inode(img, &inode);
  }

  if (size == 0)
    return offset - start_offset;

  /* TODO: implement
   *
   * Write @size bytes of data from @buf to @path starting at @offset.
   * Allocate new blocks as necessary. You may have to fill holes! Update
   * the file size if necessary.
   */
  return -ENOSYS;
}

static int
edfuse_truncate(const char *path, off_t offset)
{
  edfs_image_t *img = get_edfs_image();
  edfs_inode_t inode = {
      0,
  };

  if (!edfs_find_inode(img, path, &inode))
    return -ENOENT;

  size_t filesize = 0;
  size_t size = 0;
  if (!edfs_disk_inode_has_indirect(&inode.inode))
  {
    for (int i = 0; i < EDFS_INODE_N_BLOCKS; i++)
    {
      if (inode.inode.blocks[i] != 0)
        filesize += img->sb.block_size;
      else
        break;
    }

    // Allocate blocks
    if (offset > filesize)
    {
      // Allocate direct blocks
      if (offset < (EDFS_INODE_N_BLOCKS * img->sb.block_size))
      {
      }
      else if (offset > (EDFS_INODE_N_BLOCKS * img->sb.block_size)) // Convert direct block to indirect blocks
      {
      }
    }
    else if (offset < filesize) // Deallocate direct blocks
    {
      // Initialise data_block
      char *buf = malloc(img->sb.block_size);
      memset(buf, 0, img->sb.block_size);

      size_t end_block_inode_index = filesize / img->sb.block_size;
      size_t start_block_inode_index = (offset / img->sb.block_size);

      for (int i = start_block_inode_index; i < end_block_inode_index; i++)
      {

        off_t bytes_until_next_block = img->sb.block_size - (offset % img->sb.block_size);

        write_block(img, (const char **)&buf, inode.inode.blocks[i], &size, &offset);

        // Full block deallocated, update bitmap
        if (offset % img->sb.block_size == 0)
          update_bitmap(img, inode.inode.blocks[i], false);

        inode.inode.blocks[i] = 0;

        // Update size
        if ((inode.inode.size - bytes_until_next_block) > 0)
          inode.inode.size -= bytes_until_next_block;
        else
          inode.inode.size = 0;
      }
      free(buf);
    }
    // Update inode on disk
    edfs_write_inode(img, &inode);
  }
  else // Indirect blocks
  {
    // Allocate indirect blocks
    if (offset > filesize)
    {
    }
    else if (offset < filesize) // Deallocate indirect blocks
    {
    }
  }
  /* TODO: implement
   *
   * The size of @path must be set to be @offset. Release now superfluous
   * blocks or allocate new blocks that are necessary to cover offset.
   */
  return 0;
}

/*
 * FUSE setup
 */

static struct fuse_operations edfs_oper =
    {
        .readdir = edfuse_readdir,
        .mkdir = edfuse_mkdir,
        .rmdir = edfuse_rmdir,
        .getattr = edfuse_getattr,
        .open = edfuse_open,
        .create = edfuse_create,
        .unlink = edfuse_unlink,
        .read = edfuse_read,
        .write = edfuse_write,
        .truncate = edfuse_truncate,
};

int main(int argc, char *argv[])
{
  /* Count number of arguments without hyphens; excluding execname */
  int count = 0;
  for (int i = 1; i < argc; ++i)
    if (argv[i][0] != '-')
      count++;

  if (count != 2)
  {
    fprintf(stderr, "error: file and mountpoint arguments required.\n");
    return -1;
  }

  /* Extract filename argument; we expect this to be the
   * penultimate argument.
   */
  /* FIXME: can't this be better handled using some FUSE API? */
  const char *filename = argv[argc - 2];
  argv[argc - 2] = argv[argc - 1];
  argv[argc - 1] = NULL;
  argc--;

  /* Try to open the file system */
  edfs_image_t *img = edfs_image_open(filename, true);
  if (!img)
    return -1;

  /* Start fuse main loop */
  int ret = fuse_main(argc, argv, &edfs_oper, img);
  edfs_image_close(img);

  return ret;
}

void update_bitmap(edfs_image_t *img, edfs_block_t block_nr, bool mode)
{
  // Retrieve the byte corresponding with the block number / 8; (1 byte = 8 bits)
  char new_byte = 0;
  edfs_block_t block_index = img->sb.bitmap_start + block_nr / 8;

  pread(img->fd, &new_byte, sizeof(char), block_index);

  // Remove or add the bit that corresponds with the block number
  if (mode == false)
    new_byte -= 1 << (block_nr % 8);
  else if (mode == true)
    new_byte += 1 << (block_nr % 8);

  // Write new byte
  pwrite(img->fd, &new_byte, sizeof(char), block_index);
}

static bool
allocate_block(edfs_image_t *img, edfs_block_t *block_nr)
{
  // Allocate buffer
  char bytes[img->sb.bitmap_size];

  // Fill buffer with bitmap
  pread(img->fd, bytes, img->sb.bitmap_size, img->sb.bitmap_start);

  // Find empty bit in bitmap
  for (int i = 0; i < img->sb.bitmap_size; i++)
  {
    for (int b = 0; b < 8; b++)
    {
      // Empty bit found
      if (!((bytes[i] >> b) & 1))
      {
        *block_nr = i * 8 + b;

        update_bitmap(img, *block_nr, true);
        return true;
      }
    }
  }
  // No free block found
  return false;
}

// Write block to disk at block_nr
// Size == 0 means edfuse_truncate() is being called
void write_block(edfs_image_t *img, const char **data_block, edfs_block_t block_nr, size_t *size, off_t *offset)
{
  off_t block_offset = *offset % img->sb.block_size;
  off_t block_offset_start = edfs_get_block_offset(&img->sb, block_nr);
  off_t bytes_until_next_block = img->sb.block_size - (*offset % img->sb.block_size);

  // Set bytes_until_next_block to size if it goes past size
  // Need to update this variable for write amount and incrementing offset and data_block for later use
  if (*size != 0)
    if (bytes_until_next_block > *size)
      bytes_until_next_block = *size;

  // Write bytes from datablock on disk
  pwrite(img->fd, *data_block, bytes_until_next_block, block_offset_start + block_offset);

  // Need to keep track of current position to write at the next position
  *offset += bytes_until_next_block;

  // Need to keep track of current position in data_block to read from
  if (*size != 0)
  {
    *data_block += bytes_until_next_block;
    *size -= bytes_until_next_block;
  }
}