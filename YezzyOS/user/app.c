
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                            app.c
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
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
#include "rand.h"
#include "string.h"

/*************************************************************
 * @title calendar 日历
 * @author TT
 * ***********************************************************/

PUBLIC int isleap(int year);
PUBLIC int max_day(int year, int month);
PUBLIC int total_day(int year, int month, int day);
PUBLIC int weekday(int year, int month, int day);
PUBLIC void display_week(int year, int month, int day);
PUBLIC void display_month(int year, int month, int day);
PUBLIC int calendar_main(int *fd_stdin);
PUBLIC void calendar_intro();

PUBLIC int isleap(int year)
{
    if ((year % 4 == 0) && (year % 100 != 0) || (year % 400 == 0))
        return 1;
    else
        return 0;
}

PUBLIC int max_day(int year, int month)
{
    int day[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (isleap(year) == 1)
        day[1] = 29;
    return day[month - 1];
}

PUBLIC int total_day(int year, int month, int day)
{
    int sum = 0;
    int i = 1;
    for (i = 1; i < month; i++)
        sum += max_day(year, i);
    sum += day;
    return sum;
}
PUBLIC int weekday(int year, int month, int day)
{
    int count;
    count = (year - 1) + (year - 1) / 4 - (year - 1) / 100 + (year - 1) / 400 + total_day(year, month, day);
    count %= 7;
    return count;
}

PUBLIC void display_week(int year, int month, int day)
{
    clear();
    printf("\n\n");

    int count;
    count = weekday(year, month, day);
    switch (count)
    {
    case 0:
        printf("    %d-%d-%d is Sunday\n", year, month, day);
        break;
    case 1:
        printf("    %d-%d-%d is Monday\n", year, month, day);
        break;
    case 2:
        printf("    %d-%d-%d is Tuesday\n", year, month, day);
        break;
    case 3:
        printf("    %d-%d-%d is Wednesday\n", year, month, day);
        break;
    case 4:
        printf("    %d-%d-%d is Thursday\n", year, month, day);
        break;
    case 5:
        printf("    %d-%d-%d is Friday\n", year, month, day);
        break;
    case 6:
        printf("    %d-%d-%d is Saturday\n", year, month, day);
        break;

        printf("error\n");
    }
}

static char month_dis[12][12] = {"January", "February", "March", "April", "May",
                                 "June", "July", "August", "September",
                                 "October", "November", "December"};

PUBLIC void display_month(int year, int month, int day)
{

    int i = 0, j = 1;
    int week, max;
    week = weekday(year, month, 1);
    max = max_day(year, month);
    printf("\n                 %d - %s", year, month_dis[month - 1]);
    printf("\n");
    printf("\n     S     M     T     W     T     F     S\n\n");
    for (i = 0; i < week; i++)
        printf("      ");
    for (j = 1; j <= max; j++)
    {
        printf("%6d", j);
        if (i % 7 == 6)
            printf("\n\n");
        i++;
    }
    printf("\n\n");
}
PUBLIC int calendar_main(int *fd_stdin)
{
    int year, month, day;
    char y[5], m[3], d[3]; // year, month, day

    calendar_intro();

    printf("Please enter year:");
    int p = read(*fd_stdin, y, 512);

    y[p] = 0;
    year = 0;
    int i = 0;
    while (i < p)
    {
        year *= 10;
        year += (y[i++] - '0');
    }

    printf("Please enter month:");
    p = read(*fd_stdin, m, 512);
    m[p] = 0;
    month = 0;
    i = 0;
    while (i < p)
    {
        month *= 10;
        month += (m[i++] - '0');
    }

    printf("Please enter day:");
    p = read(*fd_stdin, d, 512);
    d[p] = 0;
    day = 0;
    i = 0;
    while (i < p)
    {
        day *= 10;
        day += (d[i++] - '0');
    }

    if (month < 1 || month > 12 || day < 1 || day > 31)
    {
        printf("error!");
        return -1;
    }

    display_week(year, month, day);
    display_month(year, month, day);

    printf("\nPress ENTER to continue...\n");
    read(*fd_stdin, d, 512);

    return 0;
}

void calendar_intro()
{
    clear();
    printf("      ====================================================================\n");
    printf("      #                                         Welcome to               #\n");
    printf("      #                                          Calendar                #\n");
    printf("      #                  ******                                          #\n");
    printf("      #          ***   **                   [ INTRODCUTION ]             #\n");
    printf("      #             * **            $                                    #\n");
    printf("      #              **             $     You can look up any date       #\n");
    printf("      #          *********          $         in this calendar.          #\n");
    printf("      #        *************        $                                    #\n");
    printf("      #       ***************       $  Step 1: Input the year, month and #\n");
    printf("      #       ***************       $          day that you want to look #\n");
    printf("      #        *************        $          up.                       #\n");
    printf("      #           *******           $                                    #\n");
    printf("      #                             $  Step 2: The month of the data     #\n");
    printf("      #  * *  ****  ****  ***  * *  $          will be displayed.        #\n");
    printf("      #   *   ***   ***    *    *   $                                    #\n");
    printf("      #   *   ****  ****  ***   *   $                                    #\n");
    printf("      #                                                                  #\n");
    printf("      #            Designed by YoungYoung Kerr Brian TT LeiHong          #\n");
    printf("      #                       TONGJI UNIVERSITY                          #\n");
    printf("      ====================================================================\n");

    printf("\n");
}

/*************************************************************
 * @title calculator 计算器
 * @author Kerr
 * ***********************************************************/

#define False 0
#define True 1
#define EMPTY_C '\0'
#define EMPTY_NUM -999999
#define SIZE 50
#define bool int
PUBLIC int get_option();
PUBLIC void print_result(int num1, int num2, int result, int option);
PUBLIC void calculator_list();
PUBLIC int calculator_main();
// 用于存放表达式中分离出来的每一项的结构体
struct C{
	int data;	//data
	int tag;	//1->number 0->operator
};
// 用于存放操作数的栈
int num_stack[100] = { 0 };
int num_index = -1;

bool isEmpty_num() {
	return num_index == -1;
}

void num_stack_push(int num) {
	num_index++;
	num_stack[num_index] = num;
}

int num_stack_pop(void) {
	if (isEmpty_num()) {
	printf("1111111111111\n");
		return EMPTY_NUM;
	}
	int num = num_stack[num_index];
	num_stack[num_index] = 0;
	num_index--;
	return num;
}

void num_clear(void) {
	memset(num_stack, 0, sizeof(num_stack));
	num_index = -1;
	return;
}


// 用于存放操作符的栈
char op_stack[100] = "\0";
int op_index = -1;

bool isEmpty_op() {
	return op_index == -1;
}

void op_stack_push(char ch) {
	op_index++;
	op_stack[op_index] = ch;
}

char op_stack_pop(void) {
	if (isEmpty_op()) {
		printf("222222222222\n");
		return EMPTY_C;
	}
	char ch = op_stack[op_index];
	op_stack[op_index] = '\0';
	op_index--;
	return ch;
}

char op_stack_top(void) {
	if (isEmpty_op()) {
		return EMPTY_C;
	}
	return op_stack[op_index];
}

void op_stack_clear(void) {
	memset(op_stack, '\0', sizeof(op_stack));
	op_index = -1;
}

//
char bucket_stack[100] = "\0";
int bucket_index = -1;

bool isEmpty_bucket() {
	return bucket_index == -1;
}

void bucket_stack_push(char ch) {
	bucket_index++;
	bucket_stack[bucket_index] = ch;
}

char bucket_stack_pop(void) {
	if (isEmpty_bucket()) {
		return EMPTY_C;
	}
	char ch = bucket_stack[bucket_index];
	bucket_index--;
	return ch;
}

void bucket_clear(void) {
	memset(bucket_stack, '\0', sizeof(bucket_stack));
	bucket_index = -1;
}

//栈内优先级
int priorityInStack(char c)
{
	switch (c)
	{
	case '#':return 0;
	case '(':return 1;
	case '+':case '-':return 3;
	case '*':case '/':return 5;
	case ')':return 6;
	}
}
//栈外优先级
int priorityOutStack(char c)
{
	switch (c)
	{
	case '#':return 0;
	case ')':return 1;
	case '+':case '-':return 2;
	case '*':case '/':return 4;
	case '(':return 6;
	}
}
// 添加结尾标识
void addEndTag(char* exp) {
	int i = strlen(exp);
	exp[i] = ' ';
	exp[i + 1] = '#';
}
// 是否是运算符
bool isOp(char c)
{
	if (c == '+' || c == '-' || c == '*' || c == '/' || c == '(' || c == ')' || c == '#') {
		return True;
	}
	return False;
}
// 是否是数字
bool isSingleNum(char c)
{
	return (c >= '0' && c <= '9');
}
bool isNum(char* exp)
{
	char c = exp[0];
	if (c == '+' && strlen(exp) > 1)
		//  +i 只存 i
	{
		exp = exp + 1;	//删除+
		c = exp[0];		//更新一下c
	}

	if (isSingleNum(c) || (c == '-' && strlen(exp) > 1))
		//储存各种数字, 包括正数,负数,浮点数
	{
		return True;
	}
	return False;
}
int current = 0;
struct C transNext(char* exp)
{
	char temp[50] = "\0";
	char c;
	int index = 0;

	for (int i = current; i < strlen(exp); ++i)
	{
		c = exp[i];
		if (c != ' ')
		{
			temp[index] = c;
			index++;	//因为不同对象以空格隔开,所以只要不是空格就加到_next
		}
		else
		{
			while (exp[i] == ' ') {
				i++;
			}
			current = i;	//_current指向下一个位置,结束当前对象的寻找
			break;
		}
	}

	if (isOp(temp[0])) {
		struct C d;
		d.data = temp[0];
		d.tag = 0;
		return d;
	}
	else {
		struct C d;
		d.data=(*temp)-'0';
		d.tag = 1;
		return d;
	}
}
int operate(int a, char op, int b)
{
	switch (op)
	{
	case '+':
		return a + b;
		break;
	case '-':
		return a - b;
		break;
	case '*':
		return a * b;
		break;
	case '/':
		return a / b;
		break;
	default:
		return a + b;
		break;
	}
}
int calculate(struct C rlt[], int size)
{
	num_clear();
	for (int i = 0; i < size; ++i) {
		if (rlt[i].tag == 1) {//如果是数字就压入栈中
			num_stack_push(rlt[i].data);
		}
		else {// 如果是操作符，就从栈中弹出两个数字做运算
			int right = num_stack_pop();
			int left = num_stack_pop();
			num_stack_push(operate(left, rlt[i].data, right));// 再将计算结果压入栈中
		}
	}
	return num_stack_pop();
}

int advanceCalculate(char* origin_exp) {
	char exp[30] = "\0";
	int pos = 0;
	for (int i = 0; i < strlen(origin_exp); ++i) {
		if (isOp(origin_exp[i])) {
			exp[pos] = ' ';
			++pos;
			exp[pos] = origin_exp[i];
			++pos;
			exp[pos] = ' ';
			++pos;
		}
		else if (isSingleNum(origin_exp[i])) {
			exp[pos] = origin_exp[i];
			++pos;
		}
	}
	// 初始化
	num_clear();
	op_stack_clear();
	current = 0;

	struct C result[100];
	int index = 0;

	// 在表达式尾部添加结束标识符
	addEndTag(exp);

	op_stack_push('#');
	struct C elem = transNext(exp);
	while (!isEmpty_op()) {
		char ch = elem.data;

		if (elem.tag == 1) {		//如果是操作数, 直接读入下一个内容
			result[index] = elem;
			index++;
			elem = transNext(exp);
		}
		else if (elem.tag == 0) {	//如果是操作符,判断ch的优先级priorityOutStack和当前栈顶操作符的优先级priorityInStack
			char topch = op_stack_top();
			if (priorityInStack(topch) < priorityOutStack(ch)) {		//当前操作符优先级大,将ch压栈,读入下一个内容
				op_stack_push(ch);
				elem = transNext(exp);
			}
			else if (priorityInStack(topch) > priorityOutStack(ch)) {	//当前优先级小,推展并输出到结果中
				struct C buf;
				buf.data = op_stack_pop();
				buf.tag = 0;
				result[index] = buf;
				index++;
			}
			else {
				if (op_stack_top() == '(') {	//如果退出的是左括号则读入下一个内容
					elem = transNext(exp);
				}
				op_stack_pop();
			}
		}
	}

	return calculate(result, index);
}

// 括号是否匹配
bool checkBuckets(char* exp) {

	char ch = '\0';

	for (int i = 0; i < strlen(exp); ++i) {
		if (exp[i] == '(') {
			bucket_stack_push('(');
		}
		else if (exp[i] == ')') {
			ch = bucket_stack_pop();
			if (ch == EMPTY_C || ch != '(') {
				printf("Buckets in the exprssion you input do not match.\n");
				return False;
			}
		}
	}
	return isEmpty_bucket();
}

/*判断表达式是否有非法符号*/
bool checkOp(char* exp) {
	for (int i = 0; i < strlen(exp); ++i) {
		if (isSingleNum(exp[i]) || isOp(exp[i]) || exp[i] == ' ') {
			continue;
		}
		else {
			printf("The operator we support: [+-*/()], you have input %c.\n", exp[i]);
			return False;
		}
	}
	return True;
}
void advcCal(char* expression)
{
	if (!strcmp(expression, "NULL")) {
		printf("Sorry, please input a math expressioin.\n");
	}
	else {
		// 检查符号是否合法
		if (!checkOp(expression)) {
			printf("Please check the expression and try again.\n");
			printf("\n");
			return;
		}
		// 检查括号是否匹配
		bucket_clear();
		if (!checkBuckets(expression)) {
			printf("Please check the expression and try again.\n");
			printf("\n");
			return;
		}
		int result = advanceCalculate(expression);
		printf("The result is %d\n", result);
	printf("\n");
	}
}
PUBLIC int calculator_main(int *fd_stdin)
{
    int option, num1, num2, result;
    char a[20], b[20]; // num1,um2

    calculator_list();
    read(*fd_stdin, a, 512);
    clear();

    do
    {
        option = get_option(fd_stdin);
        if (option == 0)
            break;
        if(option == 5){
            char exp[30]="\0";
            printf("please input an math expression:");
            int p = read(*fd_stdin, exp, 512);
            advcCal(exp);
        }else{
             do
            {
                printf("\nplease input a number:");
                int p = read(*fd_stdin, a, 512);
                a[p] = 0;
                num1 = 0;
                int i = 0;
                while (i < p)
                {
                    num1 *= 10;
                    num1 += (a[i++] - '0');
                }

                printf("\nplease input another number:");
                p = read(*fd_stdin, b, 512);
                b[p] = 0;
                num2 = 0;
                i = 0;
                while (i < p)
                {
                    num2 *= 10;
                    num2 += (b[i++] - '0');
                }

                if (option == 4 && num2 == 0)
                {
                    printf("\nsorry!! divid can not be 0");
                }
                else
                {
                    switch (option)
                    {
                    case 1:
                        result = num1 + num2;
                        break;
                    case 2:
                        result = num1 - num2;
                        break;
                    case 3:
                        result = num1 * num2;
                        break;
                    case 4:
                        result = num1 / num2;
                    }
                    print_result(num1, num2, result, option);
                }
            } while (option == 4 && num2 == 0);
        }
    } while (option != 0);

    return 0;
}

int get_option(int *fd_stdin)
{
    int option;
    char order[2]; 

    do
    {
        printf("\n **************************");
        printf("\n ******   1.add      ******");
        printf("\n ******   2.minus    ******");
        printf("\n ******   3.mutiply  ******");
        printf("\n ******   4.division ******");
        printf("\n ******   5.advance ******");
        printf("\n ******   0.exit     ******");
        printf("\n **************************");

        printf("\nplease choose a function:");
        int p = read(*fd_stdin, order, 512);
        order[p] = 0;
        option = order[0] - '0';

        if (option < 0 || option > 5)
        {
            clear();
            printf("sorry we have no such command, please reinput!\n");
        }
    } while (option < 0 || option > 5);

    return option;
}

void print_result(int num1, int num2, int result, int option)
{
    char operator;
    switch (option)
    {
    case 1:
        operator= '+';
        break;
    case 2:
        operator= '-';
        break;
    case 3:
        operator= '*';
        break;
    case 4:
        operator= '/';
        break;
    }
    printf("\n** %d %c %d = %d **\n", num1, operator, num2, result);
}

void calculator_list()
{
    clear();
    printf("      ====================================================================\n");
    printf("      #                                         Welcome to               #\n");
    printf("      #                                         Calculator               #\n");
    printf("      #                  ******                                          #\n");
    printf("      #          ***   **                       [ COMMAND ]              #\n");
    printf("      #             * **            $ 1 --- Add                          #\n");
    printf("      #              **             $                                    #\n");
    printf("      #          *********          $ 2 --- Minus                        #\n");
    printf("      #        *************        $                                    #\n");
    printf("      #       ***************       $ 3 --- Multiply                     #\n");
    printf("      #       ***************       $                                    #\n");
    printf("      #        *************        $ 4 --- Divide                       #\n");
    printf("      #           *******           $                                    #\n");
    printf("      #                             $ 5 --- Advance                      #\n");
    printf("      #  * *  ****  ****  ***  * *  $                                    #\n");
    printf("      #   *   ***   ***    *    *   $ 0 --- Quit                         #\n");
    printf("      #   *   ****  ****  ***   *   $                                    #\n");
    printf("      #                                  Now press ENTER to continue!    #\n");
    printf("      #            Designed by YoungYoung Kerr Brian TT LeiHong          #\n");
    printf("      #                       TONGJI UNIVERSITY                          #\n");
    printf("      ====================================================================\n");

    printf("\n\n");
}



/*************************************************************
 * @title bwchess 黑白棋
 * @author TT
 * ***********************************************************/



#define MAXSIZE 8 //  设置棋盘最大尺寸
 //
void youxi();
int Exa(char board[][MAXSIZE], int arrput[][MAXSIZE], char); //计算步数
void Print(char board[][MAXSIZE], int, int, char);
int CalSore(char board[][MAXSIZE], char);                     //计算成绩
int Hint(char board[][MAXSIZE], int arrput[][MAXSIZE], char); //最佳算法
void Foeplay(char board[][MAXSIZE], int arrput[][MAXSIZE], char);
void Show(char board[][MAXSIZE]); //显示棋盘
void bwchess_list();
void bwchess_intro();

int bwchess_main(int* fd_stdin)
{
    char board[MAXSIZE][MAXSIZE];
    int arrput[MAXSIZE][MAXSIZE] = { 0 };
    int row, col, x, y;
    int count = 0;
    int level = 0;
    int cross = 0;
    int score[2];
    char ok;

    int order = 0;
    int v, m = 1;
    //初始界面

    bwchess_list();
    printf("master@yeezyOS:~/bwchess $ ");

    char o[2], r[2], c[2]; // order,row,column
    int p = read(*fd_stdin, o, 512);
    o[p] = 0;
    v = o[0] - '0';

    if (v == 3)
        return 0;
    if (v == 1)
    {

        bwchess_intro();
        p = read(*fd_stdin, o, 512);
        v = 2;
    }
    if (v == 2)
    {

        clear();
        printf("========================AI BW-CHESS========================\n\n");

        printf("||     you will be the black one- ( X )              ||\n");
        printf("||     i will be the white one- ( O )                ||\n");


        printf("==========================================================\n");

        printf(" enter 1 to be the fisrt, enter 0 to be the later : ");
        p = read(*fd_stdin, o, 512);
        o[p] = 0;
        order = o[0] - '0';

        if (order == 0)
            level = 1;
        if (level == 0)
        {
            level = 1;
        }
        else
        {
            level = 0;
        }
        count = 4;
        for (row = 0; row < MAXSIZE; row++) //棋盘初始
        {
            for (col = 0; col < MAXSIZE; col++)
            {
                board[row][col] = 0;
            }
        }
        board[MAXSIZE / 2 - 1][MAXSIZE / 2 - 1] = board[MAXSIZE / 2][MAXSIZE / 2] = -1;
        board[MAXSIZE / 2 - 1][MAXSIZE / 2] = board[MAXSIZE / 2][MAXSIZE / 2 - 1] = 1;
        Show(board);
        int i = 0;
        for (i = count; count < (MAXSIZE * MAXSIZE) && cross < 2;)
        {
            if (level == 1)
            {
                level = 0;
                if (Exa(board, arrput, 2))
                {
                    while (1)
                    {
                        printf("\nenter your action(r):");
                        p = read(*fd_stdin, r, 512);
                        r[p] = 0;

                        if (!strcmp(r, "Q"))
                        {
                            return;
                        }

                        x = r[0] - '0';

                        printf("\nenter your action(c):");
                        p = read(*fd_stdin, c, 512);
                        c[p] = 0;
                        y = c[0];

                        if (!strcmp(c, "Q"))
                        {
                            return;
                        }

                        x--;
                        if (y >= 'a')
                        {
                            y = y - 'a' + 1;
                        }
                        else
                        {
                            y = y - 'A' + 1;
                        }
                        y--;
                        if (x >= 0 && y >= 0 && x < MAXSIZE && y < MAXSIZE && arrput[x][y])
                        {
                            Print(board, x, y, 2);
                            count++;
                            break;
                        }
                        else
                        {
                            printf("\nwrong order try again .\n");
                        }
                    }
                    Show(board); //更新棋盘
                }
                else if (++cross < 2)
                {
                    printf("\n is my turn.\n");
                }
                else
                {
                    printf("\ngame over.\n");
                }
            }
            else
            {
                level = 1;
                if (Exa(board, arrput, 1))
                {
                    cross = 0;
                    Foeplay(board, arrput, 1);
                    count++;
                    Show(board);
                }
                else
                {
                    if (++cross < 2)
                    {
                        printf("\ni cannot move,you turn\n");
                    }
                    else
                    {
                        printf("\ngame over.");
                    }
                }
            }
        }
        Show(board);
        score[0] = score[1] = 0;
        for (row = 0; row < MAXSIZE; row++) //计算分数
        {
            for (col = 0; col < MAXSIZE; col++)
            {
                score[0] = score[0] + (board[row][col] == -1);
                score[1] = score[1] + (board[row][col] == 1);
            }
        }
        printf("final score:\n");
        printf("white:%d\nblack:%d\n", score[1], score[0]);
        if (score[1] > score[0])
        {
            printf("\nha ha ha ,you lose \n\n");
        }
        else
        {
            printf("\noh,i will be back!\n\n");
        }
    }

    //scanf_s("%c", &ok);
}

void Show(char board[][MAXSIZE])
{
    clear();

    int row, col;
    printf("\n   ");
    for (col = 0; col < MAXSIZE; col++)
    {
        printf("%c  ", col + 'a');
    }
    for (row = 0; row < MAXSIZE; row++)
    {
        printf("\n ");
        for (col = 0; col < MAXSIZE; col++)
        {
            printf("+--");
            if (col == (MAXSIZE - 1))
                printf("+\n");
        }
        printf("%c", row + '1');
        for (col = 0; col < MAXSIZE; col++)
        {
            if (board[row][col] == 1)
                printf("| O");
            else if (board[row][col] == -1)
                printf("| x");
            else
            {
                printf("|  ");
            }
            if (col == (MAXSIZE - 1))
                printf("|");
        }
        if (row == (MAXSIZE - 1))
        {
            printf("\n ");
            for (col = 0; col < MAXSIZE; col++)
            {
                printf("+--");
                if (col == (MAXSIZE - 1))
                    printf("+\n");
            }
        }
    }
}

int Exa(char board[][MAXSIZE], int arrput[][MAXSIZE], char level)
{
    int rowd, cold, row, col, x, y = 0;
    int step = 0;
    char foe;
    if (level == 1)
        foe = -1; //对手
    else
        foe = 1;
    char player = -1 * foe;
    for (row = 0; row < MAXSIZE; row++)
    {
        for (col = 0; col < MAXSIZE; col++)
        {
            arrput[row][col] = 0;
        }
    }
    for (row = 0; row < MAXSIZE; row++)
    {
        for (col = 0; col < MAXSIZE; col++)
        {
            if (board[row][col] != 0)
                continue;
            for (rowd = -1; rowd <= 1; rowd++)
                for (cold = -1; cold <= 1; cold++)
                {
                    //判断是否越界
                    if (row + rowd < 0 || row + rowd >= MAXSIZE || col + cold < 0 || col + cold >= MAXSIZE || (rowd == 0 && cold == 0))
                    {
                        continue;
                    }
                    if (board[row + rowd][col + cold] == foe)
                    {
                        x = row + rowd;
                        y = col + cold;
                        while (1)
                        {
                            x += rowd;
                            y += cold;
                            if (x < 0 || x >= MAXSIZE || y < 0 || y >= MAXSIZE)
                            {
                                break;
                            }
                            if (board[x][y] == 0)
                            {
                                break;
                            }
                            if (board[x][y] == player)
                            {
                                arrput[row][col] = 1;
                                step++;
                                break;
                            }
                        }
                    }
                }
        }
    }
    return step;
}

void Print(char board[][MAXSIZE], int row, int col, char level)
{
    int rowd = 0, cold = 0, x = 0, y = 0;
    char foe;
    if (level == 1)
        foe = -1;
    else
        foe = 1;
    char player = -1 * foe;
    board[row][col] = player;
    for (rowd = -1; rowd <= 1; rowd++)
    {
        for (cold = -1; cold <= 1; cold++)
        {
            if (row + rowd < 0 || row + rowd >= MAXSIZE || col + cold < 0 || col + cold >= MAXSIZE || (rowd == 0 && cold == 0)) //�ж��Ƿ�Խ��
            {
                continue;
            }
            if (board[row + rowd][col + cold] == foe)
            {
                x = row + rowd;
                y = col + cold;
                while (1)
                {
                    x += rowd;
                    y += cold;
                    if (x < 0 || x >= MAXSIZE || y < 0 || y >= MAXSIZE)
                    {
                        break;
                    }
                    if (board[x][y] == 0)
                    {
                        break;
                    }
                    if (board[x][y] == player)
                    {
                        while (board[x -= rowd][y -= cold] == foe)
                        {
                            board[x][y] = player;
                        }
                        break;
                    }
                }
            }
        }
    }
}

int CalSore(char board[][MAXSIZE], char level) //计算成绩
{
    int score = 0;
    int row, col;
    char foe;
    if (level == 1)
        foe = -1;
    else
        foe = 1;
    char player = -1 * foe;
    for (row = 0; row < MAXSIZE; row++)
        for (col = 0; col < MAXSIZE; col++)
        {
            score = score - (board[row][col] == foe);
            score = score + (board[row][col] == player);
        }
    return score;
}

int Hint(char board[][MAXSIZE], int arrput[][MAXSIZE], char level) //最佳走法
{
    int row, col, i, j;
    char board1[MAXSIZE][MAXSIZE] = { 0 };
    int maxscore = 0;
    int score = 0;
    char foe;
    if (level == 1)
        foe = -1;
    else
        foe = 1;
    for (row = 0; row < MAXSIZE; row++)
        for (col = 0; col < MAXSIZE; col++)
        {
            if (!arrput[row][col])
                continue;
            for (i = 0; i < MAXSIZE; i++)
                for (j = 0; j < MAXSIZE; j++)
                {
                    board1[i][j] = board[i][j];
                }
            Print(board1, row, col, level);
            score = CalSore(board1, level);
            if (maxscore < score)
                maxscore = score;
        }
    return maxscore;
}

void Foeplay(char board[][MAXSIZE], int arrput[][MAXSIZE], char level)
{
    int row, col, row1, col1, i, j;
    int score = 0, minscore = 100;
    char board1[MAXSIZE][MAXSIZE];
    int arrput1[MAXSIZE][MAXSIZE];
    char foe;
    if (level == 1)
        foe = -1;
    else
        foe = 1;
    for (row = 0; row < MAXSIZE; row++)
    {
        for (col = 0; col < MAXSIZE; col++)
        {
            if (arrput[row][col] == 0)
            {
                continue;
            }
            for (i = 0; i < MAXSIZE; i++)
            {
                for (j = 0; j < MAXSIZE; j++)
                {
                    board1[i][j] = board[i][j];
                }
            }
            Print(board1, row, col, level);
            Exa(board1, arrput1, foe);
            score = Hint(board1, arrput1, foe);
            if ((row == 0 && col == 0) || (row == 0 && col == MAXSIZE - 1 || row == MAXSIZE - 1 && col == 0 || row == MAXSIZE - 1 && col == MAXSIZE - 1))
            {
                minscore = score;
                row1 = row;
                col1 = col;
            }
            else if (row == 0 || row == MAXSIZE - 1 || col == 0 || col == MAXSIZE - 1)
            {
                minscore = score;
                row1 = row;
                col1 = col;
            }
            else if (score < minscore)
            {
                minscore = score;
                row1 = row;
                col1 = col;
            }
        }
    }
    Print(board, row1, col1, level);
}

void bwchess_list()
{
    clear();
    printf("      ====================================================================\n");
    printf("      #                                         Welcome to               #\n");
    printf("      #                               BWchess --- Black & White Chess    #\n");
    printf("      #                  ******                                          #\n");
    printf("      #          ***   **                   [COMMAND LIST]               #\n");
    printf("      #             * **            $ 1 --- Introduction                 #\n");
    printf("      #              **             $     You'd better take a look       #\n");
    printf("      #          *********          $     before playing.                #\n");
    printf("      #        *************        $                                    #\n");
    printf("      #       ***************       $ 2 --- Start the game               #\n");
    printf("      #       ***************       $                                    #\n");
    printf("      #        *************        $ Q --- Quit during the game         #\n");
    printf("      #           *******           $                                    #\n");
    printf("      #                             $ Press any other key to quit now    #\n");
    printf("      #  * *  ****  ****  ***  * *  $                                    #\n");
    printf("      #   *   ***   ***    *    *   $                                    #\n");
    printf("      #   *   ****  ****  ***   *   $                                    #\n");
    printf("      #                                                                  #\n");
    printf("      #            Designed by YoungYoung Kerr Brian TT LeiHong          #\n");
    printf("      #                       TONGJI UNIVERSITY                          #\n");
    printf("      ====================================================================\n");

    printf("\n\n");
}

void bwchess_intro()
{
    clear();
    printf("      ====================================================================\n");
    printf("      #                                         Welcome to               #\n");
    printf("      #                               BWchess --- Black & White Chess    #\n");
    printf("      #                  ******                                          #\n");
    printf("      #          ***   **                   [ INTRODCUTION ]             #\n");
    printf("      #             * **            $                                    #\n");
    printf("      #              **             $ 1. With the next chess, you must   #\n");
    printf("      #          *********          $    convert at least one of the      #\n");
    printf("      #        *************        $    opponent's chess to your own,   #\n");
    printf("      #       ***************       $    OR you have to skip the turn!   #\n");
    printf("      #       ***************       $                                    #\n");
    printf("      #        *************        $ 2. If both of players can't move,  #\n");
    printf("      #           *******           $    the game ends.                  #\n");
    printf("      #                             $                                    #\n");
    printf("      #  * *  ****  ****  ***  * *  $ 3. At last, the one who have the   #\n");
    printf("      #   *   ***   ***    *    *   $    more chess will be the winner.  #\n");
    printf("      #   *   ****  ****  ***   *   $ Now press ENTER to start the game! #\n");
    printf("      #                                                                  #\n");
    printf("      #            Designed by YoungYoung Kerr Brian TT LeiHong          #\n");
    printf("      #                       TONGJI UNIVERSITY                          #\n");
    printf("      ====================================================================\n");

    printf("\n\n");
}



/*************************************************************
 * @title carrycraft 推箱子
 * @author TT
 * ***********************************************************/

int map[9][11] = {
    {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
    {0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0},
    {0, 1, 0, 3, 3, 3, 3, 3, 0, 1, 0},
    {0, 1, 0, 3, 0, 3, 0, 3, 0, 1, 1},
    {0, 1, 0, 0, 0, 2, 0, 0, 3, 0, 1},
    {1, 1, 0, 1, 1, 1, 1, 0, 3, 0, 1},
    {1, 0, 4, 4, 4, 4, 4, 1, 0, 0, 1},
    {1, 0, 4, 4, 4, 4, 4, 0, 0, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0} };



int drawmain();
int move();
int win();
void craft_list();


PUBLIC int carrycraft_main(int* fd_stdin)
{
    char o[2];

    craft_list();
    read(*fd_stdin, o, 512);

    while (1)
    {

        drawmain();
        int p = read(*fd_stdin, o, 512);
        o[p] = 0;
        char push = o[0];
        int count, caw;
        for (int i = 0; i < 9; i++)
        {
            for (int j = 0; j < 11; j++)
            {
                if (map[i][j] == 2 || map[i][j] == 6)
                {
                    count = i;
                    caw = j;
                }
            }
        }
        if (push == 'w')
        {

            if (map[count - 1][caw] == 0 || map[count - 1][caw] == 4)
            {
                map[count][caw] -= 2;
                map[count - 1][caw] += 2;
            }
            else if (map[count - 1][caw] == 3 || map[count - 1][caw] == 7)
            {
                if (map[count - 2][caw] == 0 || map[count - 2][caw] == 4)
                {
                    map[count][caw] -= 2;
                    map[count - 1][caw] -= 1;
                    map[count - 2][caw] += 3;
                }
            }
        }

        if (push == 's')
        {
            if (map[count + 1][caw] == 0 || map[count + 1][caw] == 4)
            {
                map[count][caw] -= 2;
                map[count + 1][caw] += 2;
            }

            else if (map[count + 2][caw] == 0 || map[count + 2][caw] == 4)
            {
                if (map[count + 1][caw] == 3 || map[count + 1][caw] == 7)
                {
                    map[count][caw] -= 2;
                    map[count + 1][caw] -= 1;
                    map[count + 2][caw] += 3;
                }
            }
        }

        if (push == 'a')
        {
            if (map[count][caw - 1] == 0 || map[count][caw - 1] == 4)
            {
                map[count][caw] -= 2;
                map[count][caw - 1] += 2;
            }

            else if (map[count][caw - 2] == 0 || map[count][caw - 2] == 4)
            {
                if (map[count][caw - 1] == 3 || map[count][caw - 1] == 7)
                {
                    map[count][caw] -= 2;
                    map[count][caw - 1] -= 1;
                    map[count][caw - 2] += 3;
                }
            }

        }

        if (push == 'd')
        {
            if (map[count][caw + 1] == 0 || map[count][caw + 1] == 4)
            {
                map[count][caw] -= 2;
                map[count][caw + 1] += 2;
            }

            else if (map[count][caw + 2] == 0 || map[count][caw + 2] == 4)
            {
                if (map[count][caw + 1] == 3 || map[count][caw + 1] == 7)
                {
                    map[count][caw] -= 2;
                    map[count][caw + 1] -= 1;
                    map[count][caw + 2] += 3;
                }
            }
            //break;
        }

        if(push == 'q')
        {
            return;
        }
    }
    return 0;
}


int drawmain()
{
    clear();
    int i, j;
    win(); //判断输赢
    for (i = 0; i < 9; i++)
    {
        printf("                   ");
        for (j = 0; j < 11; j++)
        {
            switch (map[i][j])
            {
            case 0:
                printf("  "); //空白的位置
                break;
            case 1:
                printf("W "); //墙
                break;
            case 2:
                printf("P "); //代表人
                break;
            case 3:
                printf("C "); //代表箱子
                break;
            case 4:
                printf("T "); //代表目标地址
                break;
            case 6:
                printf("P "); //代表人和目标地址重合
                break;
            case 7:
                printf("G "); //代表箱子和目标地址重合
                break;
            }
        }
        printf("\n");
    }
    return 0;
}


int win()
{
    int k = 0;
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 11; j++)
        {
            if (map[i][j] == 3)
                k++;
        }
    }
    if (k == 0)
        printf("congratulation,you win��\n");
    return 0;
}

void craft_list()
{
    printf("      ====================================================================\n");
    printf("      #                                     Welcome to                   #\n");
    printf("      #                   Carrycraft ---  Carry the craft to the target! #\n");
    printf("      #                  ******                                          #\n");
    printf("      #          ***   **                        [ COMMAND ]             #\n");
    printf("      #             * **            $   In the map, 'W' means the wall   #\n");
    printf("      #              **             $               'P' means the player #\n");
    printf("      #          *********          $               'C'means the craft   #\n");
    printf("      #        *************        $               'T'means the target  #\n");
    printf("      #       ***************       $      'G'means the craft at target  #\n");
    printf("      #       ***************       $                                    #\n");
    printf("      #        *************        $   ENTER w to go up                 #\n");
    printf("      #           *******           $   ENTER s to go down               #\n");
    printf("      #                             $   ENTER a to go left               #\n");
    printf("      #  * *  ****  ****  ***  * *  $   ENTER d to go right              #\n");
    printf("      #   *   ***   ***    *    *   $   ENTER q to quit                  #\n");
    printf("      #   *   ****  ****  ***   *   $ Now press ENTER to start the game! #\n");
    printf("      #                                                                  #\n");
    printf("      #            Designed by YoungYoung Kerr Brian TT LeiHong          #\n");
    printf("      #                       TONGJI UNIVERSITY                          #\n");
    printf("      ====================================================================\n");

    printf("\n\n");
}

/*************************************************************
 * @title sudoku 数独
 * @author Brian.Z
 * ***********************************************************/


int map_copy[9][9];   // 数独棋盘,递归创建时使用
int sudoku_map[9][9]; // 数独棋盘
int player[9][9];     // 玩家填入的数字

/**
 * 棋盘单元格的状态
 *      GIVEN 初始给定的单元格
 *      CORRECT 已填入正确的数字
 *      WRONG 已填入错误的数字
 *      EMPTY 未填入数字
 * */
int status[9][9];

#define GIVEN 0
#define CORRECT 1
#define WRONG 2
#define EMPTY 3

// linux中gcc编译不支持bool类型，要么用下面的宏定义，要么用g++编译

#define bool int
#define false 0
#define true 1
#define NULL ((void *)0)

/***** 建立数独棋盘 *****/

PUBLIC int getStart(int i, int j);            // 获取单元格所在九宫格位置
PUBLIC bool isLegal(int i, int j);            // 判断当前单元格填入数字后是否合法
PUBLIC void dfs(int i, int j, bool *success); // 搜索算法建立数独棋盘
PUBLIC void copy_map();                       // 成功建立棋盘后，将棋盘复制到map[][]中
PUBLIC void getmap();                         // 获取数独棋盘

PUBLIC void printmap(); // 打印数独棋盘

PUBLIC bool check_win(); // 判断当前玩家是否获胜
PUBLIC void sudoku_main();

/* 获取单元格所在九宫格位置 */
PUBLIC int get_start(int i, int j)
{
    if (i >= 0 && i < 3)
    {
        if (j >= 0 && j < 3)
            return 0;
        if (j >= 3 && j < 6)
            return 1;
        if (j >= 6 && j < 9)
            return 2;
    }
    if (i >= 3 && i < 6)
    {
        if (j >= 0 && j < 3)
            return 3;
        if (j >= 3 && j < 6)
            return 4;
        if (j >= 6 && j < 9)
            return 5;
    }
    if (i >= 6 && i < 9)
    {
        if (j >= 0 && j < 3)
            return 6;
        if (j >= 3 && j < 6)
            return 7;
        if (j >= 6 && j < 9)
            return 8;
    }
    return -1;
}

/* 判断当前单元格填入数字后是否合法 */
PUBLIC bool is_legal(int i, int j)
{
    int start[9][2] = {{0, 0}, {0, 3}, {0, 6}, {3, 0}, {3, 3}, {3, 6}, {6, 0}, {6, 3}, {6, 6}};

    int st = get_start(i, j);
    int a = start[st][0], b = start[st][1];

    /* 九宫格内不能有重复数字 */
    for (int m = a; m < a + 3; m++)
    {
        for (int n = b; n < b + 3; n++)
        {
            if (m == i && n == j)
                break;
            if (map_copy[m][n] == map_copy[i][j])
                return false;
        }
    }

    for (int k = 0; k < i; k++)
    {
        /* 所属列不能有相同数字 */
        if (map_copy[k][j] == map_copy[i][j])
            return false;
    }

    for (int k = 0; k < j; k++)
    {
        /* 所属行不能有相同数字 */
        if (map_copy[i][k] == map_copy[i][j])
            return false;
    }
    return true;
}

/* 搜索算法建立数独棋盘 */
PUBLIC void dfs(int i, int j, bool *success)
{
    if (*success)
    {
        return;
    }

    if (i == 9 && j == 0)
    {
        /* 成功建立棋盘 */
        *success = true;
        copy_map();
        return;
    }

    int next_i = j == 8 ? i + 1 : i;
    int next_j = j == 8 ? 0 : j + 1;

    /**
     *  建立1-9的随机数组进行遍历
     *  
     *  but bochs中导入time.h有点问题
     */
    for (int k = 9; k > 0; k--)
    {
        map_copy[i][j] = k;
        if (is_legal(i, j))
        {
            dfs(next_i, next_j, success);
        }
        map_copy[i][j] = 0;
    }
}

/* 获取数独棋盘 */
PUBLIC void get_map()
{
    bool success = false;
    dfs(0, 0, &success);
}

/* 成功建立棋盘后，将棋盘复制到map[][]中 */
PUBLIC void copy_map()
{
    srand(1010);
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            sudoku_map[i][j] = map_copy[i][j];

            /* 随机给定初始单元格，设定数量为30个 */
            if (rand() % 81 < 30)
            {
                /* 给定单元格状态为GIVEN */
                player[i][j] = sudoku_map[i][j];
                status[i][j] = GIVEN;
            }
            else
            {
                status[i][j] = EMPTY;
            }
        }
    }
}

/* 打印数独棋盘 */
PUBLIC void print_map()
{
    for (int i = 0; i < 9; i++)
    {
        printf("                   ");

        if (i % 3 == 0)
        {
            printf("-------------------------------------\n");
        }
        else
        {
            printf("|-----------|-----------|-----------|\n");
        }

        printf("                   ");
        for (int j = 0; j < 9; j++)
        {
            if (status[i][j] == EMPTY)
            {
                printf("|%3c", ' ');
            }
            else if (status[i][j] == GIVEN)
            {
                /* 对于初始单元格，在数字右上角加上^加以区分 */
                printf("|%2d%1c", player[i][j], '^');
            }
            else
            {
                printf("|%2d%1c", player[i][j], ' ');
            }
        }
        printf("|\n");
    }
    printf("                   ");
    printf("-------------------------------------\n");
    printf("\n");
}
PUBLIC void sudoku_list();

/* 判定玩家是否获胜 */
PUBLIC bool check_win()
{
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            if (status[i][j] == WRONG || status[i][j] == EMPTY)
                return false;
        }
    }
    return true;
}

PUBLIC void sudoku_main(int *fd_stdin)
{
    /* init the game */
    get_map();

    char order[2];         // 指令
    char r[2], c[2], v[2]; // row,column,value

    /* press ENTER to start the game */
    sudoku_list();
    int start = read(*fd_stdin, order, 512);
    print_map();

    while (1)
    {
        printf("[E] to enter a number [D] to delete a number [Q] to quit the game [P] to print the map\n");
        printf("Please enter the order: ");

        /* read */
        int p = read(*fd_stdin, order, 512);
        order[p] = 0;

        if (strcmp(order, "E") == 0)
        {
            /* 输入数字 */
            int row, col, val;
            printf("Please enter the row of the grid you want to complete: ");
            p = read(*fd_stdin, r, 512);
            r[p] = 0;
            row = r[0] - '0';

            printf("Please enter the column of the grid you want to complete: ");
            p = read(*fd_stdin, c, 512);
            c[p] = 0;
            col = c[0] - '0';

            if (status[row - 1][col - 1] != EMPTY)
            {
                printf("Sorry, grid[%d, %d] has been filled in.\n", row, col);
                print_map();
                continue;
            }

            printf("Please enter the number you want to fill in: ");
            p = read(*fd_stdin, v, 512);
            v[p] = 0;
            val = v[0] - '0';

            if (val == sudoku_map[row - 1][col - 1])
            {
                /* 输入正确 */
                status[row - 1][col - 1] = CORRECT;
            }
            else
            {
                /* 输入错误 */
                status[row - 1][col - 1] = WRONG;
            }
            player[row - 1][col - 1] = val;
        }
        else if (strcmp(order, "D") == 0)
        {
            /* 删除数字 */
            int row, col;
            printf("Please enter the row of the grid you want to delete: ");
            p = read(*fd_stdin, r, 512);
            r[p] = 0;
            row = r[0] - '0';

            printf("Please enter the column of the grid you want to delete: ");
            p = read(*fd_stdin, c, 512);
            c[p] = 0;
            col = c[0] - '0';

            if (status[row - 1][col - 1] == GIVEN)
            {
                /* 初始给定数字不能删除 */
                printf("Sorry, Grid[%d, %d] cannot be deleted.\n\n", row, col);
                continue;
            }
            else
            {
                status[row - 1][col - 1] = EMPTY;
                player[row - 1][col - 1] = 0;
            }
        }
        else if (strcmp(order, "Q") == 0)
        {
            printf("\n\n\n");
            break;
        }
        else if (strcmp(order, "P") != 0)
        {
            printf("False Command!\n\n");
            continue;
        }

        printf("\n\n\n\n\n\n\n\n\n");
        print_map();

        /* 判定是否获胜 */
        if (check_win())
        {
            printf("            Congratulation!! You successfully complete the sudoku!!\n");
            break;
        }
    }
}

PUBLIC void sudoku_list()
{
    clear();
    printf("      ====================================================================\n");
    printf("      #                                 Welcome to                       #\n");
    printf("      #                        Sudoku --- An Interesting Puzzle Game     #\n");
    printf("      #                  ******                                          #\n");
    printf("      #          ***   **                   [COMMAND LIST]               #\n");
    printf("      #             * **            $ E --- ENTER a number               #\n");
    printf("      #              **             $     you need to enter the location #\n");
    printf("      #          *********          $     and the number step by step.   #\n");
    printf("      #        *************        $                                    #\n");
    printf("      #       ***************       $ D --- DELETE a number              #\n");
    printf("      #       ***************       $     you need to enter the location #\n");
    printf("      #        *************        $     of the grid you want to delete.#\n");
    printf("      #           *******           $                                    #\n");
    printf("      #                             $ P --- PRINT the map                #\n");
    printf("      #  * *  ****  ****  ***  * *  $                                    #\n");
    printf("      #   *   ***   ***    *    *   $ Q --- QUIT the game                #\n");
    printf("      #   *   ****  ****  ***   *   $ Now press ENTER to start the gmae! #\n");
    printf("      #                                                                  #\n");
    printf("      #            Designed by YoungYoung Kerr Brian TT LeiHong          #\n");
    printf("      #                       TONGJI UNIVERSITY                          #\n");
    printf("      ====================================================================\n");

    printf("\n\n");
}


/*************************************************************
 * @title tic-tac-toe 井字棋
 * @author Ray
 * ***********************************************************/

PUBLIC int ticTacToe_list();
PUBLIC int ticTacToe_main(int *fd_stdin)
{
    int player = 0;
    int winner = 0;
    int number = 0;
    int row = 0;
    int column = 0;


    char tic_tac_toe[3][3] = {
        {'1', '2', '3'},
        {'4', '5', '6'},
        {'7', '8', '9'}};
    
    //让双方玩家轮流输入自己想要标志的位置
    int i;
    char o[2];

    ticTacToe_list();
    read(*fd_stdin, o, 512);
    clear();

    for (i = 0; i < 9 && winner == 0; i++)
    {
        printf("\n");
        printf("                       ");
        printf(" %c | %c | %c \n", tic_tac_toe[0][0], tic_tac_toe[0][1], tic_tac_toe[0][2]);
        printf("                       ");
        printf("---+---+---\n");
        printf("                       ");
        printf(" %c | %c | %c \n", tic_tac_toe[1][0], tic_tac_toe[1][1], tic_tac_toe[1][2]);
        printf("                       ");
        printf("---+---+---\n");
        printf("                       ");
        printf(" %c | %c | %c \n", tic_tac_toe[2][0], tic_tac_toe[2][1], tic_tac_toe[2][2]);
        player = i % 2 + 1;
        do
        {
            printf("\nif you want to exit, please input 0.\n");
            printf("\nplayer %c", (player == 1) ? 'A' : 'B');
            printf(" input a number of the grid to put your %c piece:", (player == 1) ? 'X' : 'O');
            int p = read(*fd_stdin, o, 512);
            o[p] = 0;
            number = o[0] - '0';
            if(number==0) return 0;
            row = (number - 1) / 3;    //行的索引码
            column = (number - 1) % 3; //列的索引码
        } while (number < 0 || number > 9 || tic_tac_toe[row][column] > '9');
        tic_tac_toe[row][column] = (player == 1) ? 'X' : 'O';
        //检查此玩家是否获胜
        //检查对角线上该玩家是否获胜
        if ((tic_tac_toe[0][0] == tic_tac_toe[1][1] && tic_tac_toe[0][0] == tic_tac_toe[2][2]) ||
            (tic_tac_toe[0][2] == tic_tac_toe[1][1] && tic_tac_toe[0][2] == tic_tac_toe[2][0]))
        {
            winner = player;
        }
        //检查横或竖上该玩家是否获胜
        else
        {
            for (int line = 0; line < 3; line++)
            {
                if ((tic_tac_toe[line][0] == tic_tac_toe[line][1] && tic_tac_toe[line][0] == tic_tac_toe[line][2]) ||
                    (tic_tac_toe[0][line] == tic_tac_toe[1][line] && tic_tac_toe[0][line] == tic_tac_toe[2][line]))
                {
                    winner = player;
                }
            }
        }
        clear();
    }
    //公布最后得分面板
    printf("\n");
    printf("                       ");
    printf(" %c | %c | %c \n", tic_tac_toe[0][0], tic_tac_toe[0][1], tic_tac_toe[0][2]);
    printf("                       ");
    printf("---+---+---\n");
    printf("                       ");
    printf(" %c | %c | %c \n", tic_tac_toe[1][0], tic_tac_toe[1][1], tic_tac_toe[1][2]);
    printf("                       ");
    printf("---+---+---\n");
    printf("                       ");
    printf(" %c | %c | %c \n", tic_tac_toe[2][0], tic_tac_toe[2][1], tic_tac_toe[2][2]);
    //打印最后胜利者结果
    if (winner == 0)
    {
        printf("\nWhat a pity! Nobody win the game!\n");
    }
    else
    {
        printf("\nCongratulation! The winner is %c!\n", (winner == 1) ? 'A' : 'B');
    }

    printf("\n\nPress ENTER to continue...\n");
    read(*fd_stdin, o, 512);

    return 0;
}

PUBLIC int ticTacToe_list()
{
    clear();
    printf("      ====================================================================\n");
    printf("      #                                 Welcome to                       #\n");
    printf("      #                             Tic-Tac-Toe --- an easy and fun game #\n");
    printf("      #                  ******                                          #\n");
    printf("      #          ***   **                                                #\n");
    printf("      #             * **            $     Welcome to play tic-tac-toe!   #\n");
    printf("      #              **             $                                    #\n");
    printf("      #          *********          $     Have fun with your friends!    #\n");
    printf("      #        *************        $                                    #\n");
    printf("      #       ***************       $                                    #\n");
    printf("      #       ***************       $    Press ENTER to start the game   #\n");
    printf("      #        *************        $                                    #\n");
    printf("      #           *******           $                                    #\n");
    printf("      #                             $                                    #\n");
    printf("      #  * *  ****  ****  ***  * *  $                                    #\n");
    printf("      #   *   ***   ***    *    *   $                                    #\n");
    printf("      #   *   ****  ****  ***   *   $                                    #\n");
    printf("      #                                                                  #\n");
    printf("      #            Designed by YoungYoung Kerr Brian TT LeiHong          #\n");
    printf("      #                       TONGJI UNIVERSITY                          #\n");
    printf("      ====================================================================\n");

    printf("\n\n");
}

/*************************************************************
 * @title mine 扫雷
 * @author Kerr
 * ***********************************************************/
#define ROW 9//界面的长宽
#define COL 9
#define ROWS ROW+2
#define COLS COL+2
#define EASYCOUNT 10//雷的个数

PUBLIC void InitBoard(char board[ROWS][COLS], int rows, int cols, char set);
PUBLIC void DispalyBoard(char board[ROWS][COLS], int row, int col);
PUBLIC void SetMine(char mine[ROWS][COLS], int row, int col);
PUBLIC void FindMine(char mine[ROWS][COLS], char show[ROWS][COLS], int row, int col, int fd_stdin);
int mine_list()
{
    clear();
    printf("      ====================================================================\n");
    printf("      #                                 Welcome to                       #\n");
    printf("      #                           Mine sweeping--- an easy and fun game  #\n");
    printf("      #                  ******                                          #\n");
    printf("      #          ***   **                                                #\n");
    printf("      #             * **            $     Welcome to play Mine sweeping! #\n");
    printf("      #              **             $                                    #\n");
    printf("      #          *********          $     Have fun with your friends!    #\n");
    printf("      #        *************        $                                    #\n");
    printf("      #       ***************       $                                    #\n");
    printf("      #       ***************       $       ********************         #\n");
    printf("      #        *************        $       ****** 1.play ******         #\n");
    printf("      #           *******           $       ********************         #\n");
    printf("      #                             $       ****** 0.quit ******         #\n");
    printf("      #  * *  ****  ****  ***  * *  $       ********************         #\n");
    printf("      #   *   ***   ***    *    *   $                                    #\n");
    printf("      #   *   ****  ****  ***   *   $                                    #\n");
    printf("      #                                                                  #\n");
    printf("      #            Designed by YoungYoung Kerr Brian TT LeiHong          #\n");
    printf("      #                       TONGJI UNIVERSITY                          #\n");
    printf("      ====================================================================\n");

    printf("\n\n");
}
void InitBoard(char board[ROWS][COLS], int rows, int cols, char set)//为数组初始化，设计展示的界面
{
	int i = 0;
	int j = 0;
	for (i = 0; i < rows; i++)
	{
		for (j = 0; j < cols; j++)
		{
			board[i][j] = set;  //set可以更改
		}
	}
}
void DispalyBoard(char board[ROWS][COLS], int row, int col) //打印数组函数
{
	int i = 0;
	int j = 0;
	for (i = 0; i <= row; i++)
	{
		printf("%d ", i);
	}
	printf("\n");
	for (i = 1; i <= row; i++)
	{
		printf("%d", i);
		for (j = 1; j <= col; j++)
		{
			printf("%2c", board[i][j]);
		}
		printf("\n");
	}
}
void SetMine(char mine[ROWS][COLS], int row, int col) //布置雷
{
	int count = EASYCOUNT;
	while (count)
	{
		int x = rand() % row + 1;
		int y = rand() % col + 1;
		if (mine[x][y] == '0')
		{
			mine[x][y] = '1';
			count--;
		}
	}
}
//在玩家输入坐标时，判断该坐标有没有雷，如果没有显示周围一圈的雷数
void ShowMineCount(char mine[ROWS][COLS], char show[ROWS][COLS], int row, int col, int x, int y, int* win)
{
    if (x < 1 || y < 1 || x > row || y > col) return;
    if (show[x][y] != '*') return;
	int mine_count = (mine[x - 1][y] +
		mine[x - 1][y - 1] +
		mine[x][y - 1] +
		mine[x + 1][y - 1] +
		mine[x + 1][y] +
		mine[x][y + 1] +
		mine[x + 1][y + 1] +
		mine[x - 1][y + 1] - 8 * '0');
    show[x][y] = mine_count + '0';
    (*win)++;
    if (mine_count == 0)
    {
        int dirx[8] = {1,1,0,-1,-1,-1,0,1};
        int diry[8] = {0,1,1,1,0,-1,-1,-1};
        for (int i = 0; i < 8; i++)
        {
            ShowMineCount(mine, show, row, col, x + dirx[i], y + diry[i], win);
        }
    }
}
void FindMine(char mine[ROWS][COLS], char show[ROWS][COLS], int row, int col, int fd_stdin)
{
	int x = 0;
	int y = 0;
	int win = 0;
	int p;
	char xn[2];
	char yn[2];
    printf("\nCurrent score is %d.\n", win);
    printf("You'll win when score reaches 71!\n\n");
	while (win < (row * col - EASYCOUNT))
	{
        printf("(Enter 0 to quit)\n");
		printf("please input x:>");
		p = read(fd_stdin, xn, 512);
        x=xn[0]-'0';
        if(x == 0)
        {
            return;
        }

		printf("please input y:>");
		p = read(fd_stdin, yn, 512);
		y=yn[0]-'0';
        if(y == 0)
        {
            return;
        }

		if (x >= 1 && x <= row && y >= 1 && y <= col)
		{
			if (mine[x][y] == '1')
			{
				clear();
				printf("Boom!!!  Game over!\n");
				DispalyBoard(mine, row, col);
				break;
			}
			else
			{
                ShowMineCount(mine, show, row, col, x, y, &win);
				clear();
				DispalyBoard(show, row, col);
                printf("\nCurrent score is %d.\n", win);
                printf("You'll win when score reaches 71!\n\n");
			}
			if (win == row * col - EASYCOUNT)
			{
                clear();
                DispalyBoard(mine, row, col);
				printf("\nYou win! Congratulation!!\n\n");
			}
		}
		else
		{
			printf("wrong index:>\n");
		}
	}
}
void menu()//菜单功能
{
	printf("************************\n");
	printf("******** 1.new game ****\n");
	printf("************************\n");
	printf("******** 0.quit ********\n");
	printf("************************\n");
}
void game(int fd_stdin)
{
	//雷的信息储存
	//1.布置好的雷的信息：
	char mine[ROWS][COLS] = { 0 };
	//2.排查出的雷的信息：
	char show[ROWS][COLS] = { 0 };
	//初始化：
	InitBoard(mine, ROWS, COLS, '0');
	InitBoard(show, ROWS, COLS, '*');
	//布置雷：
	SetMine(mine, ROW, COL);
	//打印布置好的雷
	clear();
	DispalyBoard(show, ROW, COL);
	//排查雷
	FindMine(mine, show, ROW, COL, fd_stdin);
}
PUBLIC int mine_main(int fd_stdin)
{
	clear();
	srand(1012);
	char o[2];
	int input = -1;
	mine_list();
	do
	{
		printf("please input your choice: ");
		int p = read(fd_stdin, o, 512);
		input=o[0]-'0';
		switch (input)
		{
		case 1:
			game(fd_stdin);
			break;
		case 0:
			printf("quit game：>\n");
			break;
		default:
			printf("wrong index ,please input again:>\n");
			break;
		}
		mine_list();
	} while (input);
	return 0;
}
