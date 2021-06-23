# C语言生成日志管理器介绍 kslogs

V0.0.13.20210623

------

## 项目简介

## 文件目录

* 这是单独的一篇文章，要什么文件目录

## 用法详解

* 查看示例代码 logs.c
  * kslogs_file 结构体用以存储一份日志管理状态，这些状态之间互不干扰，可以创建多份这样的结构体，如果需要对不同的内容做日志输出，比如将性能的输出输出到一个文件，将警告输出到一个文件。
  * 只需要执行 初始化kslogs_file_init。 制定一个目录，kslogs_file_start启动即可，启动完成将会创建一个线程，同样意味着，在多线程环境中，写入日志将会是安全的。
  * KSLOGS_SEND 是一份宏定义，用来帮助输出文档就像 printf 一样用的悠闲自在
  * path可能的情况
    * 绝对地址，无所谓
    * 相对地址，无所谓
    * 目录，将会在这个目录下按不同的天数生成日志，每天一份，如果跨年重叠了天数就追加文件，很适合日志将会很长的情况，天数的定义取决于linux本身的时间

```c
int main(int argv, char** argc)
{
    char  path[] = "./";            // 使用目录
    //  char path[] = "./mylog.log";    // 使用文件
    s_kslogs_file kslogs_file;
    printf("************ %lld *************\n", kslogs_getms());
    kslogs_file_init(&kslogs_file, path);
    kslogs_file_start(&kslogs_file);
    KSLOGS_SEND(&kslogs_file, "hello my friend, to %s", path);
    kslogs_file_clean(&kslogs_file);
    printf("************ %lld *************\n", kslogs_getms());
    return 0;
}
```

## 源码结构说明

```
[kira@kira-virtual-machine src]$ ll
总用量 48
drwxrwxr-x 2 kira kira 4096 6月  23 15:56 ./
drwxrwxr-x 8 kira kira 4096 6月  23 15:56 ../
-rwxrwxr-x 1 kira kira   87 5月  27 16:51 build.sh*
-rw-rw-r-- 1 kira kira 2239 6月  23 15:56 kslogs_basic.c
-rw-rw-r-- 1 kira kira  951 6月  23 15:56 kslogs_basic.h
-rw-rw-r-- 1 kira kira  551 6月  23 15:56 kslogs.c
-rw-rw-r-- 1 kira kira  185 5月  28 16:04 kslogs_config.c
-rw-rw-r-- 1 kira kira  737 5月  28 16:04 kslogs_config.h
-rw-rw-r-- 1 kira kira 6497 6月  23 15:56 kslogs_file.c
-rw-rw-r-- 1 kira kira 1107 6月  23 15:56 kslogs_file.h
-rw-rw-r-- 1 kira kira   74 5月  27 14:32 kslogs.h
[kira@kira-virtual-machine src]$ 
```

* src 目录
  * build.sh 编译当前目录
  * kslogs_basic.c 最基本的对文本读写的部分，无论时文件，内存，等，最底层的部分
  * kslogs.c 对外提供接口，其他程序调用时，仅仅需要包含这个源代码的头文件即可
  * kslogs_config 调试、配置等，如有需要，可以修改这个源代码的头文件
  * kslogs_file 使用文件形式对日志进行操作，这也是目前这个仓库的主要用法

## 注意

## 关于作者

>Autho: KiraSkyler
>Email: kiraskyler@outlook.com / kiraskyler@qq.com

## 贡献者/贡献组织

## 鸣谢

## 版权信息

> 该项目签署了GPL 授权许可，详情请参阅官网

>This program is free software: you can redistribute it and/or modify
>it under the terms of the GNU General Public License as published by
>the Free Software Foundation, either version 3 of the License, or
>(at your option) any later version.
>This program is distributed in the hope that it will be useful,
>but WITHOUT ANY WARRANTY; without even the implied warranty of
>MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
>GNU General Public License for more details.
>You should have received a copy of the GNU General Public License
>along with this program.  If not, see <https://www.gnu.org/licenses/>.

## 更新日志

* V0.0.13.20210623