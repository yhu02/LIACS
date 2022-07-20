/* EdFS -- An educational file system
 *
 * Copyright (C) 2019  Leiden University, The Netherlands.
 */

#include "edfs-common.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

/*
 * EdFS image management
 */

void
edfs_image_close(edfs_image_t *img)
{
  if (!img)
    return;

  if (img->fd >= 0)
    close(img->fd);

  free(img);
}

/* Read and verify super block. */
static bool
edfs_read_super(edfs_image_t *img)
{
  if (pread(img->fd, &img->sb, sizeof(edfs_super_block_t), EDFS_SUPER_BLOCK_OFFSET) < 0)
    {
      fprintf(stderr, "error: file '%s': %s\n",
              img->filename, strerror(errno));
      return false;
    }

  if (img->sb.magic != EDFS_MAGIC)
    {
      fprintf(stderr, "error: file '%s': EdFS magic number mismatch.\n",
              img->filename);
      return false;
    }

  /* Simple sanity check of size of file system image. */
  struct stat buf;

  if (fstat(img->fd, &buf) < 0)
    {
      fprintf(stderr, "error: file '%s': stat failed? (%s)\n",
              img->filename, strerror(errno));
      return false;
    }

  if (buf.st_size < edfs_get_size(&img->sb))
    {
      fprintf(stderr, "error: file '%s': file system size larger than image size.\n",
              img->filename);
      return false;
    }

  /* FIXME: implement more sanity checks? */

  return true;
}

edfs_image_t *
edfs_image_open(const char *filename, bool read_super)
{
  edfs_image_t *img = malloc(sizeof(edfs_image_t));

  img->filename = filename;
  img->fd = open(img->filename, O_RDWR);
  if (img->fd < 0)
    {
      fprintf(stderr, "error: could not open file '%s': %s\n",
              img->filename, strerror(errno));
      edfs_image_close(img);
      return NULL;
    }

  /* Load super block into memory. */
  if (read_super && !edfs_read_super(img))
    {
      edfs_image_close(img);
      return NULL;
    }

  return img;
}


/*
 * Inode-related routines
 */

/* Read inode from disk, inode->inumber must be set to the inode number
 * to be read from disk.
 */
int
edfs_read_inode(edfs_image_t *img,
                edfs_inode_t *inode)
{
  if (inode->inumber >= img->sb.inode_table_n_inodes)
    return -ENOENT;

  off_t offset = edfs_get_inode_offset(&img->sb, inode->inumber);
  return pread(img->fd, &inode->inode, sizeof(edfs_disk_inode_t), offset);
}

/* Reads the root inode from disk. @inode must point to a valid
 * inode structure.
 */
int
edfs_read_root_inode(edfs_image_t *img,
                     edfs_inode_t *inode)
{
  inode->inumber = img->sb.root_inumber;
  return edfs_read_inode(img, inode);
}

/* Writes @inode to disk, inode->inumber must be set to a valid
 * inode number to which the inode will be written.
 */
int
edfs_write_inode(edfs_image_t *img, edfs_inode_t *inode)
{
  if (inode->inumber >= img->sb.inode_table_n_inodes)
    return -ENOENT;

  off_t offset = edfs_get_inode_offset(&img->sb, inode->inumber);
  return pwrite(img->fd, &inode->inode, sizeof(edfs_disk_inode_t), offset);
}

/* Clears the specified inode on disk, based on inode->inumber.
 */
int
edfs_clear_inode(edfs_image_t *img, edfs_inode_t *inode)
{
  if (inode->inumber >= img->sb.inode_table_n_inodes)
    return -ENOENT;

  off_t offset = edfs_get_inode_offset(&img->sb, inode->inumber);

  edfs_disk_inode_t disk_inode;
  memset(&disk_inode, 0, sizeof(edfs_disk_inode_t));
  return pwrite(img->fd, &disk_inode, sizeof(edfs_disk_inode_t), offset);
}

/* Finds a free inode and returns the inumber. NOTE: this does NOT
 * allocate the inode. Only after a valid inode has been written
 * to this inumber, this inode is allocated in the table.
 */
edfs_inumber_t
edfs_find_free_inode(edfs_image_t *img)
{
  edfs_inode_t inode = { .inumber = 1 };

  while (inode.inumber < img->sb.inode_table_n_inodes)
    {
      if (edfs_read_inode(img, &inode) > 0 &&
          inode.inode.type == EDFS_INODE_TYPE_FREE)
        return inode.inumber;

      inode.inumber++;
    }

  return 0;
}

/* Create a new inode. Searches for a free inode in the inode table (returns
 * -ENOSPC if the inode table is full). @inode is initialized accordingly.
 */
int
edfs_new_inode(edfs_image_t *img,
               edfs_inode_t *inode,
               edfs_inode_type_t type)
{
  edfs_inumber_t inumber;

  inumber = edfs_find_free_inode(img);
  if (inumber == 0)
    return -ENOSPC;

  memset(inode, 0, sizeof(edfs_inode_t));
  inode->inumber = inumber;
  inode->inode.type = type;

  return 0;
}
