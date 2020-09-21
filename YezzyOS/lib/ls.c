/*************************************************************************//**
 *****************************************************************************
 * @file   ls.c
 * @brief  ls()
 * @author Young 
 * @date   Aug 22 2020
 *****************************************************************************
 *****************************************************************************/

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
#include "proto.h"

/*****************************************************************************
 *                                ls
 *****************************************************************************/
/**
 * List file and directory
 * 
 * @param pathname  The full path of the current directory.
 * 
 * @return number of files searched
 *****************************************************************************/
PUBLIC int ls(const char *pathname)
{
	MESSAGE msg;

	msg.type	= LS;

	msg.PATHNAME	= (void*)pathname;
	msg.NAME_LEN	= strlen(pathname);

	send_recv(BOTH, TASK_FS, &msg);
	assert(msg.type == SYSCALL_RET);

	return msg.RETVAL;
}
