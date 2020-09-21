
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                            main.c
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                                                    Forrest Yu, 2005
							Brian.Z & Kerr & Ray & TT & Young,  2020   		
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


#define DELAY_TIME 5000
#define NULL ((void*)0)


/*****************************************************************************
 *                               New Functions List
 *****************************************************************************/
PUBLIC void anim();
PUBLIC void commandList();
PUBLIC void copyStr(char* tar, char* src, int lenth);
PUBLIC void noSuchCom();
PUBLIC void processManage();


/*****************************************************************************
 *                               kernel_main
 *****************************************************************************/
/**
 * jmp from kernel.asm::_start. 
 * 
 *****************************************************************************/
PUBLIC int kernel_main()
{
	disp_str("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n"
		 "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");

	int i, j, eflags, prio;
        u8  rpl;
        u8  priv; /* privilege */

	struct task * t;
	struct proc * p = proc_table;

	char * stk = task_stack + STACK_SIZE_TOTAL;

	for (i = 0; i < NR_TASKS + NR_PROCS; i++,p++,t++) {
		p->pid=i;
		if (i >= NR_TASKS + NR_NATIVE_PROCS) {
			p->p_flags = FREE_SLOT;
			continue;
		}

	        if (i < NR_TASKS) {     /* TASK */
                        t	= task_table + i;
                        priv	= PRIVILEGE_TASK;
                        rpl     = RPL_TASK;
                        eflags  = 0x1202;/* IF=1, IOPL=1, bit 2 is always 1 */
			prio    = 15;
                }
                else {                  /* USER PROC */
                        t	= user_proc_table + (i - NR_TASKS);
                        priv	= PRIVILEGE_USER;
                        rpl     = RPL_USER;
                        eflags  = 0x202;	/* IF=1, bit 2 is always 1 */
			prio    = 5;
                }

		strcpy(p->name, t->name);	/* name of the process */
		p->p_parent = NO_TASK;

		if (strcmp(t->name, "TestA") != 0) {
			p->ldts[INDEX_LDT_C]  = gdt[SELECTOR_KERNEL_CS >> 3];
			p->ldts[INDEX_LDT_RW] = gdt[SELECTOR_KERNEL_DS >> 3];

			/* change the DPLs */
			p->ldts[INDEX_LDT_C].attr1  = DA_C   | priv << 5;
			p->ldts[INDEX_LDT_RW].attr1 = DA_DRW | priv << 5;
		}
		else {		/* INIT process */
			unsigned int k_base;
			unsigned int k_limit;
			int ret = get_kernel_map(&k_base, &k_limit);
			assert(ret == 0);
			init_desc(&p->ldts[INDEX_LDT_C],
				  0, /* bytes before the entry point
				      * are useless (wasted) for the
				      * INIT process, doesn't matter
				      */
				  (k_base + k_limit) >> LIMIT_4K_SHIFT,
				  DA_32 | DA_LIMIT_4K | DA_C | priv << 5);

			init_desc(&p->ldts[INDEX_LDT_RW],
				  0, /* bytes before the entry point
				      * are useless (wasted) for the
				      * INIT process, doesn't matter
				      */
				  (k_base + k_limit) >> LIMIT_4K_SHIFT,
				  DA_32 | DA_LIMIT_4K | DA_DRW | priv << 5);
		}

		p->regs.cs = INDEX_LDT_C << 3 |	SA_TIL | rpl;
		p->regs.ds =
			p->regs.es =
			p->regs.fs =
			p->regs.ss = INDEX_LDT_RW << 3 | SA_TIL | rpl;
		p->regs.gs = (SELECTOR_KERNEL_GS & SA_RPL_MASK) | rpl;
		p->regs.eip	= (u32)t->initial_eip;
		p->regs.esp	= (u32)stk;
		p->regs.eflags	= eflags;

		p->ticks = p->priority = prio;

		p->p_flags = 0;
		p->p_msg = 0;
		p->p_recvfrom = NO_TASK;
		p->p_sendto = NO_TASK;
		p->has_int_msg = 0;
		p->q_sending = 0;
		p->next_sending = 0;

		for (j = 0; j < NR_FILES; j++)
			p->filp[j] = 0;

		stk -= t->stacksize;
	}

	k_reenter = 0;
	ticks = 0;

	p_proc_ready	= proc_table;

	init_clock();
        init_keyboard();

	restart();

	while(1){}
}


/*****************************************************************************
 *                                get_ticks
 *****************************************************************************/
PUBLIC int get_ticks()
{
	MESSAGE msg;
	reset_msg(&msg);
	msg.type = GET_TICKS;
	send_recv(BOTH, TASK_SYS, &msg);
	return msg.RETVAL;
}

/*****************************************************************************
 *                                get_ticks
 *****************************************************************************
																	Kerr 2020
 *****************************************************************************/
PUBLIC void clear()
{
	clear_screen(0, console_table[current_console].cursor);
	console_table[current_console].crtc_start = 0;
	console_table[current_console].cursor = 0;
}

/*======================================================================*
                               TestA
 *======================================================================*
							                        Kerr & Brian.Z  2020
 *======================================================================*/
void TestA()
{
	char buff[128];
	//char command_3[100], command_4[100], command_5[100];
	int fd_stdin  = open("/dev_tty0", O_RDWR);
	assert(fd_stdin  == 0);
	int fd_stdout = open("/dev_tty0", O_RDWR);
	assert(fd_stdout == 1);
	anim();
	commandList();

	while (1) {
		printf("master@yeezyOS: $ ");

		int r = read(fd_stdin, buff, 70);
		buff[r] = 0;

		if (!strcmp(buff, "help"))
		{
			clear();
			commandList();
		}
		else if (!strcmp(buff, "clear"))
		{
			clear();
		}
		else if (!strcmp(buff, "game"))
		{
			gameList();
		}
		else if (!strcmp(buff, "calcul"))
		{
			clear();
			calculator_main(&fd_stdin);
			commandList();
			continue;
		}
		else if (!strcmp(buff, "process")) 
		{
			clear();
			processManage();
		}
		else if (!strcmp(buff, "file")) 
		{
			clear();
			runFileManage(fd_stdin);
			clear();
			commandList();
		}
		else if (!strcmp(buff, "sudoku")) 
		{
			sudoku_main(&fd_stdin);
			commandList();
		}
		else if (!strcmp(buff, "calend"))
		{
			calendar_main(&fd_stdin);
			commandList();
		}
		else if (!strcmp(buff, "bwchess"))
		{
			bwchess_main(&fd_stdin);
			commandList();
		}
		else if (!strcmp(buff, "carry"))
		{
			carrycraft_main(&fd_stdin);
			commandList();
		}
		else if (!strcmp(buff, "tic"))
		{
			ticTacToe_main(&fd_stdin);
			commandList();
		}else if (!strcmp(buff, "mine"))
		{
			mine_main(fd_stdin);
			commandList();
		}
		else   //找不到所输入的指令
		{
			clear();
			noSuchCom();
		}
	}	
	for(;;);
}

void copyStr(char* tar, char* src, int lenth)
{
	assert(tar != NULL && src != NULL);

	char* temp = tar;
	int i = 0;
	while (i++ < lenth && (*temp++ = *src++) != '\0');

	if (*(temp) != '\0') {
		*temp = '\0';
	}
}

void noSuchCom()
{
	clear();
	printf("      ====================================================================\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      #                 *******                  ---NOTICE---            #\n");
	printf("      #          ***   **                                                #\n");
	printf("      #             * **                            Sorry                #\n");
	printf("      #              **                                                  #\n");
	printf("      #          *********                  There's no such command      #\n");
	printf("      #        *************                                             #\n");
	printf("      #       ***************             You can get more information   #\n");
	printf("      #       ***************                                            #\n");
	printf("      #        *************                     by inputting            #\n");
	printf("      #           *******                                                #\n");
	printf("      #                                             [help]               #\n");
	printf("      #                                                                  #\n");
	printf("      #   *   *   ******   ******   ******   *   *   *******    ****     #\n");
	printf("      #    * *    *        *            *     * *    *     *   *         #\n");
	printf("      #     *     *****    *****       *       *     *     *    ****     #\n");
	printf("      #     *     *        *          *        *     *     *        *    #\n");
	printf("      #     *     ******   ******   ******     *     *******    ****     #\n");
	printf("      ====================================================================\n");
	printf("\n\n");
}

void anim(){
	clear();
	printf("      ====================================================================\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      #          #   #                                                   #\n");
	printf("      #           # #                                                    #\n");
	printf("      #            #                                                     #\n");
	printf("      #            #                                                     #\n");
	printf("      #            #                                                     #\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      ====================================================================\n");
	milli_delay(DELAY_TIME);
	clear();
	
	printf("      ====================================================================\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      #          #   # ######                                            #\n");
	printf("      #           # #  #                                                 #\n");
	printf("      #            #   #####                                             #\n");
	printf("      #            #   #                                                 #\n");
	printf("      #            #   ######                                            #\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      ====================================================================\n");
	milli_delay(DELAY_TIME);
	clear();
	
	printf("      ====================================================================\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      #          #   # ###### ######                                     #\n");
	printf("      #           # #  #      #                                          #\n");
	printf("      #            #   #####  #####                                      #\n");
	printf("      #            #   #      #                                          #\n");
	printf("      #            #   ###### ######                                     #\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      ====================================================================\n");
	milli_delay(DELAY_TIME);
	clear();

	printf("      ====================================================================\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      #          #   # ###### ###### ######                              #\n");
	printf("      #           # #  #      #          #                               #\n");
	printf("      #            #   #####  #####     #                                #\n");
	printf("      #            #   #      #        #                                 #\n");
	printf("      #            #   ###### ###### ######                              #\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      ====================================================================\n");
	milli_delay(DELAY_TIME);
	clear();

	printf("      ====================================================================\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      #          #   # ###### ###### ###### #   #                        #\n");
	printf("      #           # #  #      #          #   # #                         #\n");
	printf("      #            #   #####  #####     #     #                          #\n");
	printf("      #            #   #      #        #      #                          #\n");
	printf("      #            #   ###### ###### ######   #                          #\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      ====================================================================\n");
	milli_delay(DELAY_TIME);
	clear();

	printf("      ====================================================================\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      #                                           #                      #\n");
	printf("      #          #   # ###### ###### ###### #   # #                      #\n");
	printf("      #           # #  #      #          #   # #  #                      #\n");
	printf("      #            #   #####  #####     #     #                          #\n");
	printf("      #            #   #      #        #      #                          #\n");
	printf("      #            #   ###### ###### ######   #                          #\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      ====================================================================\n");
	milli_delay(DELAY_TIME);
	clear();

	printf("      ====================================================================\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      #                                           #                      #\n");
	printf("      #          #   # ###### ###### ###### #   # #                      #\n");
	printf("      #           # #  #      #          #   # #  #                      #\n");
	printf("      #            #   #####  #####     #     #   #                      #\n");
	printf("      #            #   #      #        #      #   #                      #\n");
	printf("      #            #   ###### ###### ######   #   #                      #\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      ====================================================================\n");
	milli_delay(DELAY_TIME);
	clear();

	printf("      ====================================================================\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      #                                           #                      #\n");
	printf("      #          #   # ###### ###### ###### #   # #                      #\n");
	printf("      #           # #  #      #          #   # #  #                      #\n");
	printf("      #            #   #####  #####     #     #   #                      #\n");
	printf("      #            #   #      #        #      #   #                      #\n");
	printf("      #            #   ###### ###### ######   #   ####                   #\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      ====================================================================\n");
	milli_delay(DELAY_TIME);
	clear();

	printf("      ====================================================================\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      #                                           #                      #\n");
	printf("      #          #   # ###### ###### ###### #   # #                      #\n");
	printf("      #           # #  #      #          #   # #  #                      #\n");
	printf("      #            #   #####  #####     #     #   #                      #\n");
	printf("      #            #   #      #        #      #   #                      #\n");
	printf("      #            #   ###### ###### ######   #   #######                #\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      ====================================================================\n");
	milli_delay(DELAY_TIME);
	clear();

	printf("      ====================================================================\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      #                                           #                      #\n");
	printf("      #          #   # ###### ###### ###### #   # #                      #\n");
	printf("      #           # #  #      #          #   # #  #     #                #\n");
	printf("      #            #   #####  #####     #     #   #     #                #\n");
	printf("      #            #   #      #        #      #   #     #                #\n");
	printf("      #            #   ###### ###### ######   #   #######                #\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      ====================================================================\n");
	milli_delay(DELAY_TIME);
	clear();

	printf("      ====================================================================\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      #                                           #    ##                #\n");
	printf("      #          #   # ###### ###### ###### #   # #     #                #\n");
	printf("      #           # #  #      #          #   # #  #     #                #\n");
	printf("      #            #   #####  #####     #     #   #     #                #\n");
	printf("      #            #   #      #        #      #   #     #                #\n");
	printf("      #            #   ###### ###### ######   #   #######                #\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      ====================================================================\n");
	milli_delay(DELAY_TIME);
	clear();

	printf("      ====================================================================\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      #                                           #######                #\n");
	printf("      #          #   # ###### ###### ###### #   # #     #                #\n");
	printf("      #           # #  #      #          #   # #  #     #                #\n");
	printf("      #            #   #####  #####     #     #   #     #                #\n");
	printf("      #            #   #      #        #      #   #     #                #\n");
	printf("      #            #   ###### ###### ######   #   #######                #\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      ====================================================================\n");
	milli_delay(DELAY_TIME);
	clear();

	printf("      ====================================================================\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      #                                           #######                #\n");
	printf("      #          #   # ###### ###### ###### #   # #     #  ####          #\n");
	printf("      #           # #  #      #          #   # #  #     # #              #\n");
	printf("      #            #   #####  #####     #     #   #     #  ####          #\n");
	printf("      #            #   #      #        #      #   #     #      #         #\n");
	printf("      #            #   ###### ###### ######   #   #######  ####          #\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      #            Designed by YoungYoung Kerr Brian TT Ray              #\n");
	printf("      #                                                                  #\n");
	printf("      ====================================================================\n");
	milli_delay(DELAY_TIME);
	clear();

	printf("      ====================================================================\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      #                                           #######                #\n");
	printf("      #          #   # ###### ###### ###### #   # #     #  ####          #\n");
	printf("      #           # #  #      #          #   # #  #     # #              #\n");
	printf("      #            #   #####  #####     #     #   #     #  ####          #\n");
	printf("      #            #   #      #        #      #   #     #      #         #\n");
	printf("      #            #   ###### ###### ######   #   #######  ####          #\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      #                                                                  #\n");
	printf("      #            Designed by YoungYoung Kerr Brian TT Ray              #\n");
	printf("      #                         TONGJI UNIVERSITY                        #\n");
	printf("      ====================================================================\n");
	milli_delay(DELAY_TIME);


}

void commandList()
{
	clear();
	printf("      ====================================================================\n");
	printf("      #                                         Welcome to               #\n");
	printf("      #                                          yeezyOS                 #\n");
	printf("      #                  ******                                          #\n");
	printf("      #          ***   **                   [COMMAND LIST]               #\n");
	printf("      #             * **            $ help --- show the command list     #\n");
	printf("      #              **             $ clear --- clear the cmd            #\n");
	printf("      #          *********          $ calend --- show the month          #\n");
	printf("      #        *************        $ calcul --- calculator              #\n");
	printf("      #       ***************       $ process --- process manager        #\n");
	printf("      #       ***************       $ file --- file manager              #\n");
	printf("      #        *************        $ game --- 5 games                   #\n");
	printf("      #           *******           $      -tic ---tic-tac-toe           #\n");
	printf("      #                             $      -bwchess --- black and white  #\n");
	printf("      #  * *  ****  ****  ***  * *  $      -sudoku --- sudoku game       #\n");
	printf("      #   *   ***   ***    *    *   $      -carry --- push boxes         #\n");
	printf("      #   *   ****  ****  ***   *   $      -mine --- mine sweeping       #\n");
	printf("      #                                                                  #\n");
	printf("      #            Designed by YoungYoung Kerr Brian TT Ray              #\n");
	printf("      #                       TONGJI UNIVERSITY                          #\n");
	printf("      ====================================================================\n");	    

	printf("\n\n");
}

void gameList()
{
	clear();
	printf("      ====================================================================\n");
	printf("      #                                                                  #\n");
	printf("      #                                          Welcome to              #\n");
	printf("      #                  ******                   yeezyOS                #\n");
	printf("      #          ***   **                                                #\n");
	printf("      #             * **                         [GAME LIST]             #\n");
	printf("      #              **             $                                    #\n");
	printf("      #          *********          $     -bwchess --- black and white   #\n");
	printf("      #        *************        $                                    #\n");
	printf("      #       ***************       $     -carry --- push boxes          #\n");
	printf("      #       ***************       $                                    #\n");
	printf("      #        *************        $     -tic --- tic-tac-toe           #\n");
	printf("      #           *******           $                                    #\n");
	printf("      #                             $     -sudoku --- sudoku game        #\n");
	printf("      #  * *  ****  ****  ***  * *  $                                    #\n");
	printf("      #   *   ***   ***    *    *   $     -mine --- mine sweeping        #\n");
	printf("      #   *   ****  ****  ***   *   $                                    #\n");
	printf("      #                                                                  #\n");
	printf("      #            Designed by YoungYoung Kerr Brian TT Ray              #\n");
	printf("      #                       TONGJI UNIVERSITY                          #\n");
	printf("      ====================================================================\n");	    

	printf("\n\n");
}

/*======================================================================*
                               TestB
 *======================================================================*/
void TestB()
{
	for(;;);
}

/*======================================================================*
                               TestC
 *======================================================================*/
void TestC()
{
	for(;;);
}

/*****************************************************************************
 *                                panic
 *****************************************************************************/
PUBLIC void panic(const char *fmt, ...)
{
	int i;
	char buf[256];

	/* 4 is the size of fmt in the stack */
	va_list arg = (va_list)((char*)&fmt + 4);

	i = vsprintf(buf, fmt, arg);

	printl("%c !!panic!! %s", MAG_CH_PANIC, buf);

	/* should never arrive here */
	__asm__ __volatile__("ud2");
}

