
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                            system.c
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                                                    Forrest Yu, 2005
							            Brian.Z & Ray & Young,  2020   		
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

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

#define NULL ((void*)0)

/*======================================================================*
                            	Process Manager
 *======================================================================*
							                        	 Brian.Z    2020
 *======================================================================*/

void processManage()
{
    int i;
	printf("=============================================================================\n\n");
	printf("                               Process Manager                               \n\n");
    printf("=============================================================================\n");
    printf("%9s      |%10s      |%12s    |%10s  \n", "PID", "name", "priority", "running?");

    printf("-----------------------------------------------------------------------------\n");
    for (i = 0; i < NR_TASKS + NR_PROCS; ++i)
    {
        /* 读取proc_table数组获取进程信息 */
        if (proc_table[i].p_flags != FREE_SLOT)
            printf("%8d        %10s      %10d             %s\n", proc_table[i].pid, proc_table[i].name, proc_table[i].priority, proc_table[i].p_flags == FREE_SLOT ? "NO" : "YES");
    }
    printf("=============================================================================\n\n");
}

/*======================================================================*
                            	File System
 *======================================================================*
							                         Ray & Young    2020
 *======================================================================*/

void runFileManage(int fd_stdin)
{
	
	fs_welcome();
	char _curdir[512] = "/";
	char _rdbuf[256];
	char _cmd[20];
	char _filename1[128];
	char _filename2[128];

	read(fd_stdin, _rdbuf, 512);
	fsComList();
	for (;;)
	{
		printf("master@yeezyOS: $%s", _curdir);  // 打印当前路径
		int r = read(fd_stdin, _rdbuf, 512);
		_rdbuf[r] = 0;

		// 解析命令
		int pos = 0;
		while (_rdbuf[pos] != ' ' && _rdbuf[pos] != 0)  // 读取指令
		{
			_cmd[pos] = _rdbuf[pos];
			pos++;
		}
		_cmd[pos] = 0;
		if (_rdbuf[pos] != 0)  // 指令还未结束
		{
			pos++;
			int len = pos;
			while (_rdbuf[pos] != ' ' && _rdbuf[pos] != 0)  // 读取第一个参数（文件名或指令属性）
			{
				_filename1[pos - len] = _rdbuf[pos];
				pos++;
			}
			_filename1[pos - len] = 0;
		}
		else      // 不存在第一个参数，将其置空
		{
			_filename1[0] = 0;
		}
		if (_rdbuf[pos] != 0)  // 指令还未结束
		{
			pos++;
			int len = pos;
			while (_rdbuf[pos] != ' ' && _rdbuf[pos] != 0)  // 读取第二个参数（操作由参时的文件名）
			{
				_filename2[pos - len] = _rdbuf[pos];
				pos++;
			}
			_filename2[pos - len] = 0;
		}
		else     // 不存在第二个参数，将其置空
		{
			_filename2[0] = 0;
		}
		
		//比对用户输入内容执行相应命令
		// ls 展示当下目录所有文件与目录
		// cd 进入目录
		// mkdir 创建目录
		// touch 创建普通文件
		// wt 写文件
		// rd 读文件
		// rm 删除文件或目录
		// help 帮助界面
		// exit 退出文件系统
		if (strcmp(_cmd, "ls") == 0)
		{
			cmd_ls(_curdir);
		}
		else if (strcmp(_cmd, "cd") == 0)
		{
			cd(_curdir, _filename1);
		}
		else if (strcmp(_cmd, "mkdir") == 0)
		{
			mkdir(_curdir, _filename1);
		}
		else if (strcmp(_cmd, "touch") == 0)
		{
			touch(_curdir, _filename1);
		}
		else if (strcmp(_cmd, "rm") == 0)
		{
			//0 means regular file, 1 means dir file
			if (strcmp(_filename1, "-r") == 0) 
			{
				rm(_curdir, _filename2, 1);
			}
			else {
				rm(_curdir, _filename1, 0);
			}
		}
		else if (strcmp(_cmd, "wt") == 0)
		{
			wt(_curdir, _filename1);
		}
		else if (strcmp(_cmd, "rd") == 0)
		{
			rd(_curdir, _filename1);
		}
		else if(strcmp(_cmd, "help") == 0)
		{
			clear();
			fsComList();
		}
		else if(strcmp(_cmd, "exit")==0)
		{
			break;
		}
		else printf("error: no command named %s\n", _cmd);
	}
}

void fsComList()
{
	clear();
	printf("==============================================================================\n");
    printf("Command List              :\n");
    printf("1. ls                     : List all files in current directory\n");
    printf("2. touch     [filename]   : Create a new file in current directory\n");
    printf("3. rm        [filename]   : Delete a file in current directory\n");
    printf("4. rd        [filename]   : Print the content of a file in current directory\n");
    printf("5. wt        [filename]   : Write new content at the end of the file\n");
    printf("6. mkdir     [dirname]    : Create a new directory in current directory\n");
    printf("7. cd        [dirname]    : Go to a directory in current directory\n");
	printf("8. help                   : Display the command list\n");
	printf("9. exit                   : Exit the file system\n");
    printf("==============================================================================\n");
	printf("\n\n");
}

void fs_welcome()
{
    clear();
    printf("      ====================================================================\n");
    printf("      #                                         Welcome to               #\n");
    printf("      #                                        File Manager              #\n");
    printf("      #                  ******                                          #\n");
    printf("      #          ***   **                       [ COMMAND ]              #\n");
    printf("      #             * **            $                                    #\n");
    printf("      #              **             $ 1 --- ls                           #\n");
    printf("      #          *********          $ 2 --- touch [filename]             #\n");
    printf("      #        *************        $ 3 --- rm    [filename]             #\n");
    printf("      #       ***************       $ 4 --- rd    [filename]             #\n");
    printf("      #       ***************       $ 5 --- wt    [filename]             #\n");
    printf("      #        *************        $ 6 --- mkdir [filename]             #\n");
    printf("      #           *******           $ 7 --- cd    [filename]             #\n");
    printf("      #                             $ 8 --- help                         #\n");
    printf("      #  * *  ****  ****  ***  * *  $ 9 --- exit                         #\n");
    printf("      #   *   ***   ***    *    *   $                                    #\n");
    printf("      #   *   ****  ****  ***   *   $    Now press ENTER to continue     #\n");
    printf("      #                                                                  #\n");
    printf("      #            Designed by YoungYoung Kerr Brian TT LeiHong          #\n");
    printf("      #                       TONGJI UNIVERSITY                          #\n");
    printf("      ====================================================================\n");

    printf("\n\n");
}


void cmd_ls(char * cur_dir)
{	
	printf("      ==================================\n");
	printf("      |%12s   |%12s    |\n","file_name","type");
	printf("      |---------------|----------------|\n");
	if (ls(cur_dir) < 0) printf("ls error");
	printf("      ==================================\n\n");
}

void cd(char * cur_dir, char * dir_name)
{
	//处理特殊目录
	if (dir_name[0] <= 0)
	{
		printf("cd:: error: command cd need one parameter.\n");
		return;
	}
	if (dir_name[0] == ' ')
	{
		printf("cd:: error: directory cannot start with space.\n");
		return;
	}
	if (dir_name[0] == '.')
	{
		if (dir_name[1] == '.')
		{
			int i = 0;
			for (; ; i++)
			{
				if (cur_dir[i] == '/' && cur_dir[i + 1] == 0)
				{
					break;
				}
			}
			if (i > 0)
			{
				cur_dir[i--] = 0;
				for (; ; i--)
				{
					if (cur_dir[i] == '/') break;
					cur_dir[i] = 0;
				}
			}
		}
		return;
	}
	char path[32] = "$";
	int i = 0;
	for (; ; i++)
	{
		if (cur_dir[i] == 0) break;
		path[i + 1] = cur_dir[i];
	}
	i++;
	for (int j = 0; ; j++)
	{
		if (dir_name[j] == 0) break;
		path[i++] = dir_name[j];
	}
	path[i] = 0;
	int fd = open(path, O_RDWR);
	if (fd == -1)
	{
		printf("No such directory.\n");
		return;
	}
	close(fd);
	strcat(cur_dir, dir_name);
	strcat(cur_dir, "/");
}

void mkdir(char * cur_dir, char * dir_name)
{
	if (dir_name[0] <= 0)
	{
		printf("mkdir:: error: command mkdir need one parameter.\n");
		return;
	}
	if (dir_name[0] == ' ')
	{
		printf("mkdir:: error: directory cannot start with space.\n");
		return;
	}
	char path[32] = "$";  //文件目录标识符
	int i = 0;
	for (; ; i++)
	{
		if (cur_dir[i] == 0) break;
		path[i + 1] = cur_dir[i];
	}
	i++;
	for (int j = 0; ; j++)
	{
		if (dir_name[j] == 0) break;
		path[i++] = dir_name[j];
	}
	path[i] = 0;
	int fd = open(path, O_CREAT);
	if (fd != -1)
	{
		close(fd);
	}
	else
	{
		printf("mkdir error: directory exists.\n");
	}
}

void touch(char * cur_dir, char * filename)
{
	if (filename[0] <= 0)
	{
		printf("touch:: error: command touch need one parameter.\n");
		return;
	}
	if (filename[0] == ' ')
	{
		printf("touch:: error: file name cannot start with space.\n");
		return;
	}
	char path[32] = "#";   //普通目录标识符
	int i = 0;
	for (; ; i++)
	{
		if (cur_dir[i] == 0) break;
		path[i + 1] = cur_dir[i];
	}
	i++;
	for (int j = 0; ; j++)
	{
		if (filename[j] == 0) break;
		path[i++] = filename[j];
	}
	path[i] = 0;
	int fd = open(path, O_CREAT);
	if (fd != -1)
	{
		close(fd);
	}
	else
	{
		printf("touch error: file exists.\n");
	}

}

void rm(char * cur_dir, char * filename, int flag) 
{
	char path[32] = "#";
	
	if (flag == 1) 
	{
		path[0] = '$';
	}
	int i = 0;
	for (; ; i++)
	{
		if (cur_dir[i] == 0) break;
		path[i + 1] = cur_dir[i];
	}
	i++;
	for (int j = 0; ; j++)
	{
		if (filename[j] == 0) break;
		path[i++] = filename[j];
	}
	path[i] = 0;
	int fd = unlink(path);
	if (fd == 0)
	{
		printf("Remove success: %s removed!\n", filename);
	}
	else if (fd == -2)
	{
		printf("Remove fail: cannot find %s!\n", filename);
	}
	else if (fd != -3)
	{
		printf("Failed to remove %s!\n", filename);
	}
}

void wt(char * cur_dir, char * filename) 
{
	char path[32] = "#";
	int i = 0;
	for (; ; i++)
	{
		if (cur_dir[i] == 0) break;
		path[i + 1] = cur_dir[i];
	}
	i++;
	for (int j = 0; ; j++)
	{
		if (filename[j] == 0) break;
		path[i++] = filename[j];
	}
	path[i] = 0;
	int fd = open (path, O_RDWR);
	if (fd == -1)
	{
		printf("Failed to open %s!\n", filename);
		return;
	}
	char tty_name[] = "/dev_tty0";
	int fd_stdin  = open(tty_name, O_RDWR);
	if (fd_stdin == -1)
	{
		printf("Failed to write file!\n");
		return;
	}
	char writeBuf[4096];  // 写缓冲区
	printf("Enter the content:\n");
	int final = read(fd_stdin, writeBuf, 4096);
	writeBuf[final] = 0;
	write(fd, writeBuf, final + 1);
	printf("\nTotally write %d bytes in the file : %s\n",final,writeBuf);  
	close(fd);
}

void rd(char*cur_dir, char * filename)
{
	char path[32] = "#";
	int i = 0;
	for (; ; i++)
	{
		if (cur_dir[i] == 0) break;
		path[i + 1] = cur_dir[i];
	}
	i++;
	for (int j = 0; ; j++)
	{
		if (filename[j] == 0) break;
		path[i++] = filename[j];
	}
	path[i] = 0;
	int fd = open (path, O_RDWR);
	if(fd==-1){
		printf("Failed to open %s!\n", filename);
        return;
	}
	char buf[4096];
    int rdFlag = read(fd, buf, 4096);
    if (rdFlag == -1)  // 读取文件内容失败
    {
        printf("Failed to read file!\n");
        close(fd);
        return;
    }

    printf("Content of file %s :\n", filename);
    display_file(filename,buf);
    close(fd);
}

void display_file(char* filename, char* buf)
{
	printf("      ==================================\n");
	printf("      |%17s%15s|\n",filename," ");
	printf("      ----------------------------------\n");

	int count = 0;
	char tem[33];

	while(buf[count] != NULL)
	{
		copyStr(tem, buf, 32);
		printf("       %s\n", tem);
		buf += 32;
		count += 32;
	}

	printf("       %s\n",buf);
	printf("      ==================================\n\n");
}

