/* EdFS -- An educational file system
 *
 * Copyright (C) 2019  Leiden University, The Netherlands.
 */

#ifndef __EDFS_COMMON_H__
#define __EDFS_COMMON_H__

#include "edfs.h"

#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>


/* Structure to use as handle to an opened image file. */
typedef struct
{
  int fd;
  const char *filename;

  edfs_super_block_t sb;
} edfs_image_t;


void           edfs_image_close           (edfs_image_t *img);
edfs_image_t  *edfs_image_open            (const char   *filename,
                                           bool          read_super);



/*
 * Inode-related routines
 */


/* In-memory representation of an inode, we store inode number and
 * actual inode read from disk together in a structure.
 */
typedef struct
{
  edfs_inumber_t inumber;
  edfs_disk_inode_t inode;
} edfs_inode_t;


int            edfs_read_inode            (edfs_image_t *img,
                                           edfs_inode_t *inode);
int            edfs_read_root_inode       (edfs_image_t *img,
                                           edfs_inode_t *inode);
int            edfs_write_inode           (edfs_image_t *img,
                                           edfs_inode_t *inode);
int            edfs_clear_inode           (edfs_image_t *img,
                                           edfs_inode_t *inode);
edfs_inumber_t edfs_find_free_inode       (edfs_image_t *img);
int            edfs_new_inode             (edfs_image_t *img,
                                           edfs_inode_t *inode,
                                           edfs_inode_type_t type);

#endif /* __EDFS_COMMON_H__ */
