# kslogs

V0.0.4.20210527

------

## 项目简介

>对指定文件位置生成log日志，使用多线程
>
>可以同时对多份日志文件进行写入

## 文件目录

```
[kira@kira-virtual-machine kslogs]$ ll
总用量 24
drwxrwxr-x 4 kira kira 4096 5月  27 17:01 ./
drwxrwxr-x 7 kira kira 4096 5月  27 14:08 ../
-rw-rw-r-- 1 kira kira  645 5月  27 17:01 Makefile
-rw-rw-r-- 1 kira kira 1311 5月  27 17:02 Readme.md
drwxrwxr-x 2 kira kira 4096 5月  27 17:00 src/ 源代码
drwxrwxr-x 3 kira kira 4096 5月  27 16:50 tools/ 用于git，代码对齐等的工具
[kira@kira-virtual-machine kslogs]$ 
```

## 效果预览

* 日志文件，写入时添加了时间戳

```
[ts,Thu May 27 16:52:30 2021]hw_cpu stat fitst, stat = 8, 10
hw_cpu get cpu cnt diff 4
[ts,Thu May 27 16:52:40 2021][ts,Thu May 27 16:54:19 2021]hw_cpu stat fitst, stat = 8, 10
hw_cpu get cpu cnt diff 4
[ts,Thu May 27 16:54:20 2021]HELLO WORD
[ts,Thu May 27 16:54:21 2021]HELLO WORD
[ts,Thu May 27 16:54:22 2021]HELLO WORD
[ts,Thu May 27 16:54:23 2021]HELLO WORD
[ts,Thu May 27 16:54:24 2021]HELLO WORD
[ts,Thu May 27 16:54:25 2021]HELLO WORD
[ts,Thu May 27 16:54:26 2021]HELLO WORD
[ts,Thu May 27 16:54:27 2021]HELLO WORD
[ts,Thu May 27 16:54:29 2021]HELLO WORD
[ts,Thu May 27 16:54:30 2021]HELLO WORD
[ts,Thu May 27 16:54:30 2021]
./log/agent_main.log (END)
```

## 使用方法

* make all
* sudo make install

```
[kira@kira-virtual-machine kslogs]$ make all
gcc -c src/kslogs_basic.c -O2 -Wall
gcc -c src/kslogs_file.c -O2 -Wall
src/kslogs_file.c: In function ‘kslogs_file_main_write’:
src/kslogs_file.c:78:5: warning: ignoring return value of ‘write’, declared with attribute warn_unused_result [-Wunused-result]
     write(kslogs_file->fd, time_asc, strlen(time_asc));
     ^~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
src/kslogs_file.c:79:5: warning: ignoring return value of ‘write’, declared with attribute warn_unused_result [-Wunused-result]
     write(kslogs_file->fd, mess, mess_len);
     ^~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
sgcc -c src/kslogs.c -O2 -Wall
mkdir -p include/kslogs lib 
cp src/*.h include/kslogs/
ar rcs libkslogs.a kslogs_basic.o kslogs_file.o kslogs.o
cp libkslogs.a lib/
[kira@kira-virtual-machine kslogs]$ sudo make install
[sudo] kira 的密码： 
cp -r include/kslogs /usr/include/ 
cp lib/libkslogs.a /usr/lib/
[kira@kira-virtual-machine kslogs]$ 
```

## Makefile使用说明

* 参考[briskgreen/conf-c: 基于C语言的轻量级读取/创建配置文件的函数库 (github.com)](https://github.com/briskgreen/conf-c)
* .o .a文件位置比较乱，也没什么结构，又不是不能用

## 示例代码

* src/kslogs.c中主函数main即是

## 安装说明

## 使用说明

## 维护说明

## 注意

## 关于作者

>Author: KiraSkyler
>Email: kiraskyler@outlook.com / kiraskyler@qq.com

## 贡献者/贡献组织

## 鸣谢

## 版权信息

> 当前项目为私有项目，如果您在任何地方看到此项目，请联系作者确认合法性，未经作者允许对该文件除删除外所有操作均视为非法
>

## 更新日志

* V0.0.4.20210527
  * 支持写入到文件类型的log日志，支持多线程与多份日志的管理，自动添加了时间戳