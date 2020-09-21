/*************************************************************************//**
 *****************************************************************************
 * @file   misc.c
 * @brief  
 * @author original: Forrest Y. Yu
 * 		   modify: Young 
 * @date   original: 2008
 * 		   modify: The Aug 22 2020
 *****************************************************************************
 *****************************************************************************/

/* Orange'S FS */

#include "type.h"
#include "stdio.h"
#include "const.h"
#include "protect.h"
#include "string.h"
#include "fs.h"
#include "proc.h"
#include "tty.h"
#include "console.h"
#include "global.h"
#include "keyboard.h"
#include "proto.h"
#include "hd.h"
#include "fs.h"

/*****************************************************************************
 *                                get_file
 *****************************************************************************/
/**
 * Search the file in current directory by name and return the inode_nr.
 *
 * @param[in] filename, current directory inode, file mode
 * @return         Ptr to the i-node of the file if successful, otherwise zero.
 * 
 * @author Young (modify)
 *****************************************************************************/
PUBLIC int get_file(char* filename, struct inode* dir_inode, u32 mode)
{
	if (filename[0] == 0)	
		return dir_inode->i_num;

	int dir_blk0_nr = dir_inode->i_start_sect;  
	int nr_dir_blks = (dir_inode->i_size + SECTOR_SIZE - 1) / SECTOR_SIZE;
	int nr_dir_entries =    
	  dir_inode->i_size / DIR_ENTRY_SIZE; /**
					       * including unused slots
					       * (the file has been deleted
					       * but the slot is still there)
					       */
	int i, j, m = 0;
	struct dir_entry * pde;
	int dev = dir_inode->i_dev;
	for (i = 0; i < nr_dir_blks; i++) 
	{
		RD_SECT(dev, dir_blk0_nr + i);
		pde = (struct dir_entry *)fsbuf;
		for (j = 0; j < SECTOR_SIZE / DIR_ENTRY_SIZE; j++, pde++) 
		{
			//check every directory entry
			if (strcmp(filename, pde->name) == 0)
			{
				int result = pde->inode_nr;
				struct inode * pin = get_inode(dev, pde->inode_nr);
				if (pin->i_mode == mode || mode == 0)
				{
					put_inode(pin);
					return result;
				}
				put_inode(pin);
				RD_SECT(dev, dir_blk0_nr + i);
			}
				
			if (++m > nr_dir_entries)
				break;
		}
		if (m > nr_dir_entries) /* all entries have been iterated */
			break;
	}

	/* file not found */
	return 0;
}
/*****************************************************************************
 *                                get_list
 *****************************************************************************/
/**
 * Search all files or directory in current directory and display their name
 *
 * @param[in]  current directory inode, file mode
 * @return         number of files searched
 * @author Young 
 *****************************************************************************/

PUBLIC int get_list(struct inode * dir_inode, u32 mode, int out)
{
	
	if (mode != I_DIRECTORY && mode != I_REGULAR) return 0;
	int file_cnt = 0;
	int dir_blk0_nr = dir_inode->i_start_sect; 
	int nr_dir_blks = (dir_inode->i_size + SECTOR_SIZE - 1) / SECTOR_SIZE;
	int nr_dir_entries =    
	  dir_inode->i_size / DIR_ENTRY_SIZE; /**
					       * including unused slots
					       * (the file has been deleted
					       * but the slot is still there)
					       */
	int i, j, m = 0;
	struct dir_entry * pde;
	int dev = dir_inode->i_dev;
	// search every sectors
	for (i = 0; i < nr_dir_blks; i++) 
	{
		RD_SECT(dev, dir_blk0_nr + i);
		pde = (struct dir_entry *)fsbuf;
		// read every directory entry
		for (j = 0; j < SECTOR_SIZE / DIR_ENTRY_SIZE; j++,pde++) 
		{
			char tem_name[64];
			strcpy(tem_name, pde->name);
			struct inode * pin = get_inode(dev, pde->inode_nr);
			if (tem_name[0] != 0 && tem_name[0] != '.' && tem_name[0] != ' ')
			{
				if (mode == I_DIRECTORY)
				{
					if (pin->i_mode == I_DIRECTORY) 
					{
						if (out) printl("      |%12s   |%12s    |\n",tem_name,"directory");
						file_cnt++;
					}
				}
				else 
				{
					if (pin->i_mode != I_DIRECTORY)
					{
						if (out) printl("      |%12s   |%12s    |\n",tem_name,"file");
						file_cnt++;
					}
				}
			}
			put_inode(pin);
			RD_SECT(dev, dir_blk0_nr + i);
			if (++m > nr_dir_entries) break;
				
		}
		/* all entries have been iterated */
		if (m > nr_dir_entries) break; 
	}
	return file_cnt;
}
/*****************************************************************************
 *                                search_file
 *****************************************************************************/
/**
 * Search the file in given full path and return the inode_nr.
 *
 * @param[in]      path
 * @return         Ptr to the i-node of the file if successful, otherwise zero.
 * 
 * @author Young (modify)
 *****************************************************************************/

PUBLIC int search_file(char * path)
{
	char * _path = path;
	if (path[0] == '$' || path[0] == '#') _path++;
	char filename[MAX_PATH];
	memset(filename, 0, MAX_FILENAME_LEN);
	struct inode * dir_inode;
	if (strip_path(filename, _path, &dir_inode) != 0)
		return 0;
	if (path[0] == '$')    // directory file
	{
		return get_file(filename, dir_inode, I_DIRECTORY);
	}
	if (path[0] == '#')   // regular file
	{
		return get_file(filename, dir_inode, I_REGULAR);
	}
	return get_file(filename, dir_inode, 0);  //system file
}

/*****************************************************************************
 *                                strip_path
 *****************************************************************************/
/**
 * Get the basename from the fullpath
 * (support file system supporting more than one levels)
 *
 * This routine should be called at the very beginning of file operations
 * such as open(), read() and write(). It accepts the full path and returns
 * two things: the basename and a ptr of the root dir's i-node.
 *
 * e.g. After stip_path(filename, "/blah", ppinode) finishes, we get:
 *      - filename: "blah"
 *      - *ppinode: root_inode
 *      - ret val:  0 (successful)
 *
 * Currently an acceptable pathname should begin with at most one `/'
 * preceding a filename.
 *
 * Filenames may contain any character except '/' and '\\0'.
 *
 * @param[out] filename The string for the result.
 * @param[in]  pathname The full pathname.
 * @param[out] ppinode  The ptr of the dir's inode will be stored here.
 * 
 * @return Zero if success, otherwise the pathname is not valid.
 * 
 * @author Young (modify)
 *****************************************************************************/
PUBLIC int strip_path(char * filename, const char * pathname,
		      struct inode** ppinode)
{
	const char * s = pathname;
	char * t = filename;
	struct inode * dir_inode = get_inode(ROOT_DEV, ROOT_INODE);
	struct inode * next_inode;
	
	if (s == 0)
		return -1;

	if (*s == '/' || *s == '$' || *s == '#')
		s++;

	if (*s == '/')
		s++;

	while (*s) /* check each character */
	{		
		if (*s == '/')    //获得一级文件目录，此时名称存储在filename中
		{
			*t = 0;
			int dev = dir_inode->i_dev;
			int dir_inode_nr = get_file(filename, dir_inode, I_DIRECTORY);
			next_inode = get_inode(dev, dir_inode_nr);  //寻找当前级目录对应节点
			put_inode(dir_inode);
			dir_inode = next_inode;
			t = filename;   
			s++;
		}
		*t++ = *s++;
		/* if filename is too long, just truncate it */
		if (t - filename >= MAX_FILENAME_LEN)
			break;
	}
	*t = 0;

	*ppinode = dir_inode;

	return 0;
}

