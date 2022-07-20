/* EdFS -- An educational file system
 *
 * Copyright (C) 2019  Leiden University, The Netherlands.
 */

/*
5-29-2022
Yvo Hu              s2962802
Seyed Saqlain Zeidi s2982048
*/
#ifndef __EDFS_H__
#define __EDFS_H__

#include <stdbool.h>
#include <stdint.h>
#include <unistd.h>


/*
 * General defines
 */

typedef uint32_t edfs_inumber_t;

typedef uint16_t edfs_block_t;
#define EDFS_MAX_BLOCKS (1 << (sizeof(edfs_block_t) * 8))

#define EDFS_MAX_BLOCK_SIZE (1 << 13)
#define EDFS_MIN_BLOCK_SIZE (1 << 9)

/* Block 0 is always in use for the boot block, so we can use it
 * as invalid block marker in inodes.
 */
#define EDFS_BLOCK_INVALID 0


/*
 * Super block
 */

/* The super block is always stored at offset 512 from the start of
 * the disk / image file.
 */
#define EDFS_SUPER_BLOCK_OFFSET 512

#define EDFS_MAGIC 0x00133700f00d0037ULL

typedef struct
{
  uint64_t magic;
  uint16_t version;

  uint16_t block_size;  /* technically supports blocks up to 64 KB, but
                         * we cap at 8 KB, see defines above.
                         */
  edfs_block_t n_blocks;

  uint32_t bitmap_start; /* offset from start of device; in bytes */
  uint32_t bitmap_size;  /* in bytes */

  uint32_t inode_table_start; /* offset from start of device; in bytes */
  uint32_t inode_table_size;  /* in bytes */
  uint32_t inode_table_n_inodes;

  /* Inode hosting the root directory of the file system. */
  edfs_inumber_t root_inumber;
} __attribute__((__packed__)) edfs_super_block_t;



/*
 * Inode
 */


typedef enum
{
  EDFS_INODE_TYPE_FREE = 0,
  EDFS_INODE_TYPE_FILE,
  EDFS_INODE_TYPE_DIRECTORY,

  EDFS_INODE_TYPE_INDIRECT = 1 << 7    /* Flag to indicate block pointers
                                        * are indirect blocks.
                                        */
} edfs_inode_type_t;


#define EDFS_INODE_N_BLOCKS 2   /* NB: Increasing this value will break
                                 * compatibility.
                                 */

/* Padded to be 16 bytes in size, with 5 reserved bytes available for
 * future expansion.
 */
typedef struct
{
  edfs_inode_type_t type : 8;
  uint8_t reserved[3];

  uint32_t size;

  edfs_block_t blocks[EDFS_INODE_N_BLOCKS];
  uint16_t reserved2[2];
} __attribute__((__packed__)) edfs_disk_inode_t;


/*
 * Directory entry
 */

/* We aim for directory entries of 64 bytes in size. */

/* Filename includes a null-terminator, so the effective size
 * of a filename is 59 bytes.
 */
#define EDFS_FILENAME_SIZE (64 - sizeof(edfs_inumber_t))

typedef struct
{
  edfs_inumber_t inumber;
  char filename[EDFS_FILENAME_SIZE];
} __attribute__((__packed__)) edfs_dir_entry_t;



/*
 * Assorted utility functions
 */

static inline uint32_t
edfs_get_super_block_offset(void)
{
  /* Super block is located at offset 512, after the boot block. */
  return 512;
}

static inline uint32_t
edfs_get_size(const edfs_super_block_t *sb)
{
  return sb->block_size * sb->n_blocks;
}

static inline int
edfs_get_n_dir_entries_per_block(const edfs_super_block_t *sb)
{
  return sb->block_size / sizeof(edfs_dir_entry_t);
}

static inline int
edfs_get_n_blocks_per_indirect_block(const edfs_super_block_t *sb)
{
  return sb->block_size / sizeof(edfs_block_t);
}

static inline uint32_t
edfs_get_block_offset(const edfs_super_block_t *sb, edfs_block_t block)
{
  return sb->block_size * block;
}

static inline off_t
edfs_get_inode_offset(edfs_super_block_t *sb, edfs_inumber_t inumber)
{
  return sb->inode_table_start + inumber * sizeof(edfs_disk_inode_t);
}

static inline bool
edfs_dir_entry_is_empty(const edfs_dir_entry_t *entry)
{
  if (entry->filename[0] == 0 || entry->inumber == 0)
    return true;

  return false;
}

static inline bool
edfs_disk_inode_is_directory(const edfs_disk_inode_t *inode)
{
  return inode->type == EDFS_INODE_TYPE_DIRECTORY;
}

static inline bool
edfs_disk_inode_has_indirect(const edfs_disk_inode_t *inode)
{
  return (inode->type & EDFS_INODE_TYPE_INDIRECT) == EDFS_INODE_TYPE_INDIRECT;
}

#endif /* __EDFS_H__ */

