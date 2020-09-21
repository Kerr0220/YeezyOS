# YeezyOS

操作系统课程设计

# 1 项目描述

## 1.1 项目目的

​		操作系统课程设计旨在让我们通过学习并实现一个简单而功能完善的操作系统，来加深对操作系统中进程管理、内存管理、文件管理等概念及原理的理解，并真正了解一个操作系统是如何从无到有、一步步实现的。

## 1.2 开发环境


* **操作系统:** Ubuntu 64bits*(使用 VMware虚拟机)*
* **操作系统模拟器:** Bochs开源模拟器
* **代码编辑器:** Visual Studio Code
* **代码语言:** C语言、汇编语言

## 1.3 项目选题及达成指标


* **项目选题：**完成《Orange's：一个操作系统的实现》项目要求
* **达成指标：**
  * **B级：**本系统对文件系统进行重新实现，新增代码量达到相关模块代码的一半。
  * **C级：**在参考源码上实现了系统级应用，如磁盘、工具台、进程管理等，通过调用较多系统API实现对系统的检测和控制。
  * **D级：**在参考源码上实现了用户级应用，包括五个小游戏、日历、计算器。



## 2. 系统功能及使用说明


* 在`yeezyOS`打开终端
* 输入`bochs`->`6`->`c`
* 进入`yeezyOS`操作系统

### 2.1 开机动画

![图片](https://uploader.shimo.im/f/0j9mV38MKffhlwNX.gif)

### 2.2 帮助界面/主界面

​		菜单显示系统提供的所有功能编号及概要。输入各命令或其编号，进入相应功能界面。

​		若想要回到`帮助界面/主界面`，在命令行中输入“help”按下回车即可。

![图片](https://uploader.shimo.im/f/Orfjoapph5J4Kltl.png!thumbnail)

### 2.3 清屏功能

​		当需要清屏时，输入`clear`按下回车即可。

![图片](https://uploader.shimo.im/f/sZDSILJ4mZyIvnGH.png!thumbnail)![图片](https://uploader.shimo.im/f/DeOrH03N6VgK41IB.png!thumbnail)

### 2.4 进程管理

**描述**

​		用户输入指令**“process”**，进入进程管理功能，系统将打印出当前所有进程的PID、进程名称、优先级（15表示系统级进程，5表示用户级进程）以及运行状态。

![图片](https://uploader.shimo.im/f/DvbFidoyh2aL1DV3.png!thumbnail)

**功能实现**


* 调用``process_manager()``，对进程列表proc_table[]``进行遍历，逐个打印每个进程的信息，其中宏``NR_TASKS + NR_PROCS``表示所有进程的数量。

### 2.5 文件管理

**描述：**

runFileManage():文件系统

本文件系统模拟Linux的部分简单文件操作，设置初始用户为home，实现了可长期保存的多级目录下的文件系统。主要包含操作有：查看该目录下所有目录及文件名（ls），创建普通文件（touch），创建目录文件（mkdir），删除文件（rm），读文件（rd），写文件（wt），进入多级目录（cd）。

用户通过在1号控制台中输入**“file”**指令进入文件管理系统。

下面是文件系统包含命令帮助界面：

![图片](https://uploader.shimo.im/f/QOoKQwse13Ywe8W6.png!thumbnail)

#### 2.5.1 ls

**描述**

用户进入文件系统后，输入指令**“ls”，**系统打印出当前⽂件⽬录下的全部⽂件及⽬录。

**实现方法**

调⽤ls()⽅法，找到当前⽬录下的全部⽂件名。

![图片](https://uploader.shimo.im/f/6cl5VcIFbOLchPuZ.png!thumbnail)

#### 2.5.2 mkdir

**描述**

用户进入文件系统后，输入指令**“mkdir + filename”，**系统即在当前目录下创建一个新的目录文件并命名为filename。若创建成功，系统提示“DIR created”的消息，若创建失败则系统提示“Failed to create a new directory”的消息。用户可在该子目录下进行等同于根目录的操作。

**实现方法**

根据传入的type类型（I_DIRECTORY），通过调⽤open()⽅法创建对应类型的新目录，根据⽅法返回值判断是否创建成功并给⽤户发出反馈消息。

![图片](https://uploader.shimo.im/f/YJc0xJvhyCH6o0ZN.png!thumbnail)

#### 2.5.3 touch

**描述**

⽤户输⼊“**touch + filename**”指令，在当前⽬录下创建⼀个新的⽂件并命名，若创建成功则系 统提示“file created successfully！”的消息，若创建失败则系统提示“failed to create a new file...”的消息

**实现方法**

根据传入的type类型（I_REGULAR），通过调⽤open()⽅法创建对应类型的新⽂件，根据⽅法返回值判断是否创建成功并给⽤户发出反馈消息。

![图片](https://uploader.shimo.im/f/NBSDovSOk0yiQtWY.png!thumbnail)

#### 2.5.4 remove（rm）

**描述**

用户进入文件系统某一目录后，可以输入**“rm”**在当前目录下进行文件删除操作。若删除普通文件，则输入操作**“rm filename”**，系统执行删除操作并返回是否删除成功标志；若删除目录文件，则输入操作**“rm -r dir_name”**，系统执行删除目录操作并返回是否成功删除标志。

**实现方法**

通过调用unlink()API实现文件删除，通过递归调用dir_unlink()实现目录文件的级联删除。

![图片](https://uploader.shimo.im/f/2XlNuRKNpr3vK3k5.png!thumbnail)

#### 2.5.5 write（wt）

**描述**

⽤户输⼊**“wt+filename”**指令，系统在定位到⽬标⽂件后显示⽂件路径，之后⽤户输⼊要写⼊⽂件的内容并回⻋，系统显示写入byte数，完成写⽂件操作。

**实现方法**

调用open()⽅法定位⽂件，然后调⽤write()⽅法对⽂件进⾏写操作。

![图片](https://uploader.shimo.im/f/yCbIAkaIMIrMONra.png!thumbnail)

#### 2.5.6 read（rd）

**描述**

⽤户输⼊**“rd+filename”**指令，查看该文件内容，若文件已写入内容，则显示文件内容；若文件已创建但为空（未写入内容），则打印空行；若文件未查找到或文件打开失败，则提示用户**“fail to open！”**。

**实现方法**

调用open()⽅法定位⽂件，然后调⽤read()⽅法对⽂件进⾏读操作。

![图片](https://uploader.shimo.im/f/WT9LqHNBvTujOMsr.png!thumbnail)

#### 2.5.7 cd

**描述**

⽤户输⼊**“cd+directory”**指令，系统定位到该相对路径所在的目录并检索其是否存在，如果存在则将cur_dir变量后追加用户所输入的相对路径，在此后进行操作时，控制台屏幕上显示的当前目录为cd后进入的目录，以后操作中涉及的目录均以此为基础进行定位。

**实现方法**

调用open()方法确认要进入的目录是否存在，如果存在则有strcat()函数更新cur_dir。

![图片](https://uploader.shimo.im/f/yyr6VpjkvHz97k2N.png!thumbnail)

### 2.6 游戏及工具

#### 2.6.1 游戏界面

用户输入指令**"game"**，进入游戏界面，控制台将显示所有游戏简介及相应指令。

![图片](https://uploader.shimo.im/f/LZrBx9J6SgkyAJlL.png!thumbnail)

#### 2.6.2 sudoku（数独）

**操作描述**


* 用户输入指令**“sudoku”**，进入数独游戏界面，控制台将显示游戏的操作说明，用户按下回车键即开始游戏

![图片](https://uploader.shimo.im/f/eDWk552vFO9kCmdm.png!thumbnail)


* 用户可以通过指令在数独棋盘中输入或删除数字，每次操作后将打印新的棋盘

![图片](https://uploader.shimo.im/f/wFOChTYMeEsNsvfH.png!thumbnail)


* 用户成功完成数独时，将显示游戏获胜信息

**功能实现**


* 采用深度优先搜索，调用getmap()随机生成一个数独棋盘，并随机取一定数量单元格作为初始给定的单元格
* 调用sudoku_main()接收用户的指令进行游戏
  * 每次用户完成操作后，调用``printmap()``打印新棋盘
  * 若当前已被填满，则调用``check_win()``判断用户是否获胜

#### 2.6.3 bwchess（黑白棋）

**操作描述**

* 用户输入指令**“bwchess”**，进入黑白棋游戏界面，控制台将显示游戏的操作说明，用户按下回车键即开始游戏

![图片](https://uploader.shimo.im/f/mF2xIXgQEGdViueu.png!thumbnail)

* 系统默认玩家持黑子，AI持白子，玩家可以选择输入1或0来选择先手或后手

![图片](https://uploader.shimo.im/f/6QljzWHh4koHw2IR.png!thumbnail)

* 用户可以通过指令输入将要落子的行列进行落子，每次操作后将打印新的棋盘

![图片](https://uploader.shimo.im/f/lk1Uycr3rIC6en8z.png!thumbnail)

* 当棋盘下满或双方无法钧无法落子时，系统将判定输赢并计算分数，显示游戏结算信息

**功能实现**


* 调用Exa()函数计算落子的步数用于最后计算分数
* 调用Calsore()函数通过计算最后棋子的个数来计算玩家最后的得分
* 调用show()函数来每次打印棋盘
* 调用bwchess_main()函数来接受用户指令进行游戏

#### 2.6.4 tic-tac-toe（井字棋）

**操作描述**


* 用户输入指令**“tic”**，进入井字棋游戏界面，控制台将显示井字棋的操作说明，用户按下回车键即开始游戏

![图片](https://uploader.shimo.im/f/g237H2hX2BiYxJsB.png!thumbnail)

* 游戏支持两位玩家进行对战，两位玩家依次输入数字1-9（表示落子的位置）进行落子（若输入则可退出游戏小程序）

![图片](https://uploader.shimo.im/f/cuZ8Hjhihkhd3gIH.png!thumbnail)

* 若其中一位玩家的三颗棋子连成一线，则该玩家获胜；若棋盘已满而无人获胜，则平局，将在屏幕显示最终游戏结果

![图片](https://uploader.shimo.im/f/NSqMS2EmqeZynFcx.png!thumbnail)

**功能实现**


* 调用``ticTacToe_main()``输出井字棋棋盘，接收用户输入的指令执行相应操作，并判断游戏最终结果

#### 2.6.5 mine（扫雷）

**操作描述**

* 用户输入指令**“mine”**，进入扫雷游戏界面，控制台将显示游戏选项，用户输入`1`即开始游戏，输入`0`退出游戏

![图片](https://uploader.shimo.im/f/u00f9wMXxvCwU4M3.png!thumbnail)

* 用户可以通过指令在扫雷棋盘中翻开指定块，每次操作后将打印新的棋盘

![图片](https://uploader.shimo.im/f/GeQew6LfGXcHl5tw.png!thumbnail)

* 用户成功排除所有雷（10个）时，将显示游戏获胜信息

![图片](https://uploader.shimo.im/f/961I4xYQcFlyqr3R.png!thumbnail)

* 用户“踩”到雷则游戏失败

![图片](https://uploader.shimo.im/f/2PAeKCZnqOs3i1xi.png!thumbnail)

**功能实现**

* 调用InitBoard()生成两个棋盘，一个用于显示给用户，一个用于记录哪里埋雷
* 调用mine_main()接收用户的指令进行游戏

#### 2.6.5 carrycraft（推箱子)

**操作描述**

* 用户输入指令**“carrycraft”**，进入推箱子游戏界面，控制台将显示游戏的操作说明，用户按下回车键即开始游戏

![图片](https://uploader.shimo.im/f/LiRe8HGehVoqgBq9.png!thumbnail)

* 玩家通过wasd输入来实现控制人物进行上下左右的行走

![图片](https://uploader.shimo.im/f/fQMMwd60RQEbh7kZ.png!thumbnail)

* 状态图根据玩家的指令修改相应信息，每次操作后将打印新的地图

![图片](https://uploader.shimo.im/f/55me2i1K6rvRXvou.png!thumbnail)

* 当所有的箱子都放置到目标地点后或者任务无法移动，系统将判定输赢，显示游戏结算信息

**功能实现**

* 调用drawmain()来打印地图信息
* 调用win()函数来判断玩家是否胜利
* 调用carry_main()函数来接受用户指令进行游戏

#### 2.6.6 calendar（日历）

**操作描述**

* 用户输入指令**“calend”**，进入日历应用界面，控制台将显示日历的操作说明

![图片](https://uploader.shimo.im/f/zooEtMyugZ8vrK4S.png!thumbnail)

* 用户输入要查询的年月日

![图片](https://uploader.shimo.im/f/jL8iqJ0hxF96BoZh.png!thumbnail)

* 系统输出查询的日期具体是星期几，并且输出该月份的日历

![图片](https://uploader.shimo.im/f/n65R9Dc6KqxhBLVf.png!thumbnail)

**功能实现**

* 调用weekday()函数来判断输入日期是星期几
* 调用display_month来打印该月的日历
* 调用calendar_main()函数来接受用户指令进行查询

#### 2.6.7 calculator（计算器）

**操作描述**

* 用户输入指令**“calcul”**，进入计算器应用界面，控制台将显示计算器的操作说明

# ![图片](https://uploader.shimo.im/f/Ca2S8gETxxa8nbNE.png!thumbnail)


* 输入要进行的操作

![图片](https://uploader.shimo.im/f/Mo7JVO8pEXWYUsb6.png!thumbnail)


* 输入要计算的表达式或数字

![图片](https://uploader.shimo.im/f/2zomRbHEStbdQxn7.png!thumbnail)

**功能实现**


* 调用get_option(int *fd_stdin)函数让用户选择要进行的操作
* 用户输入要计算的数字
* 调用print_result()函数打印计算结果

## 3. 项目结构

```
│  80m.img
│  80m.img.gz
│  a.img
│  bochsrc
│  kernel.bin
│  krnl.map
│  Makefile
│
├─.vs
│  │  ProjectSettings.json
│  │  slnx.sqlite
│  │  VSWorkspaceState.json
│  │
│  └─YezzyOS
│      └─v16
│              Browse.VC.db
│
├─boot
│  │  boot.asm
│  │  boot.bin
│  │  loader.asm
│  │  loader.bin
│  │
│  └─include
│          fat12hdr.inc
│          load.inc
│          pm.inc
│
├─fs
│      disklog.c
│      link.c
│      main.c
│      misc.c
│      open.c
│      read_write.c
│
├─include
│  │  anim.h
│  │  rand.h
│  │  stdio.h
│  │  string.h
│  │  type.h
│  │
│  └─sys
│          config.h
│          console.h
│          const.h
│          fs.h
│          global.h
│          hd.h
│          keyboard.h
│          keymap.h
│          proc.h
│          protect.h
│          proto.h
│          sconst.inc
│          tty.h
│
├─kernel
│      clock.c
│      console.c
│      global.c
│      hd.c
│      i8259.c
│      kernel.asm
│      keyboard.c
│      main.c
│      proc.c
│      protect.c
│      start.c
│      systask.c
│      tty.c
│
├─lib
│      close.c
│      exit.c
│      getpid.c
│      klib.c
│      kliba.asm
│      ls.c
│      misc.c
│      open.c
│      printf.c
│      read.c
│      string.asm
│      syscall.asm
│      syslog.c
│      unlink.c
│      vsprintf.c
│      wait.c
│      write.c
│
├─mm
│      forkexit.c
│      main.c
│
├─scripts
│      genlog
│      splitgraphs
│
└─user
        app.c
        system.c
```

## 作者

| 学号    | 姓名           |
| :------ | :------------- |
| 1853829 | 杨雨辰（组长） |
| 1850250 | 赵浠明         |
| 1851343 | 季潇熠         |
| 1852137 | 张艺腾         |
| 1851202 | 雷泓           |