# Linux设计实例 --  IO管理
[TOC]

承接上文关于IO管理的介绍，我们来介绍Linux在网络IO的用户接口上是如何设计通用性IO接口API的。

linux给用户提供了三种 同步IO读写函数（异步读写函数暂时不讨论）：

- 阻塞型(blocking) `read()/write()`
- 非阻塞型(non-blocking) `read()/write()`
- 多路复用IO `select()/poll()/epoll()/kqueue()`

## read()/write()
其中，前两种读写函数很好理解，阻塞型IO会在`recvfrom()`中阻塞而陷入系统内核态。以下摘自Linux Man Page对于`recvfrom(2)`的描述:

```
ssize_t recvfrom(int sockfd, void *buf, size_t len, int flags, struct sockaddr *src_addr, socklen_t *addrlen)

ssize_t recvmsg(int sockfd, struct msghdr *msg, int flags)
```
> The recvfrom() and recvmsg() calls are used to receive messages from a socket, and may be used to receive data on a socket whether or not it is connection-oriented.

可以看出，系统会在**数据包准备好**和**数据包拷贝完成**的两种中断受理完成的情况下返回。

其read IO模型流程如下：

```sequence
Note left of 用户态:调用read
用户态->内核态:recvfrom调用()
Note right of 内核态:数据包没有准备好
内核态->内核态:收到中断：数据包准备好了，开始读数据包
Note right of 内核态:复制完成
内核态->用户态:recvfrom()返回
```

## select()/poll()
然而最有意思的是多路复用模型`select()`，通过分析`select()`的机制，我们可以看到操作系统的**二阶段受理中断**的过程。以下摘自Linux Man Page对`select(2)`的描述：
```
 int select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout);
```
> select() and pselect() allow a program to monitor multiple file descriptors, waiting until one or more of the file descriptors become "ready" for some class of I/O operation (e.g., input possible).  A file descriptor is considered ready if it is possible to perform a corresponding I/O operation (e.g., read(2) without blocking, or a sufficiently small write(2)).

可以看出，`select()`有两个系统调用`recvfrom()`，因为`select()`可以同时监视多个设备句柄，`readfds`，`writefds`，`exceptfds`同时作为“可读事件”，“可写事件”和“错误事件”输入参数。当我监视的多个IO设备中有某个事件被触发，就从阻塞状态返回。此时用户态再发送系统调用`recvfrom()`，操作系统会将数据从内核拷贝到用户内存中并返回。这种每次受理一组事件的模型被称为“事件驱动模型”。在这个机制下，我们可以看到操作系统对IO设备中断的二阶段受理过程。

其read IO模型如下:
```sequence
Note left of 用户态:调用select
Note left of 用户态:监视一组可读fd
用户态->内核态:recvfrom调用()
Note right of 内核态:数据包没有准备好
Note right of 内核态:有个fd数据包准备好了
内核态->用户态:收到中断，返回可读
Note left of 用户态: 从可读fd开始read() 
用户态->内核态:recvfrom调用()
Note right of 内核态:开始读数据包
Note right of 内核态:复制完成
内核态->用户态:recvfrom()返回
```

`poll()`系统调用与`select()`类似，不同点在于`poll()`指定一个`pollfd`的结构体来监听可读、可写、异常事件，因此它的API接口原型如下：
```
int poll(struct pollfd* fds, nfds_t nfds, int timeout)
```
其中，结构体`pollfd`的定义如下：
```
struct pollfd 
{
	int fd;
	short events;
	short revents;
}
```
`fd`表示指定的文件描述符，`events` 告诉`poll`需要监听哪些事件，`revents`由内核通知应用程序实际发生了哪些事件。

其余的参数和函数返回值的含义与`select()`相同。

## epoll()函数族

虽然`epoll()`也是Linux的I/O复用函数，将`epoll()`单独拿出来说明，是因为它在底层实现和使用上与`select()`、`poll()`有很大区别。并且epoll是使用一组函数来实现I/O复用，因此称作epoll函数族。

### select()的设计缺陷

试想一下，如果我们需要找到某个`select()`返回的触发相应可读事件的文件描述符该怎么做？下面给出一个示例代码：
```
int ret = select(fd, readfds, writefds, exceptfds, timeout);
/*遍历所有可能的文件描述符*/
for (int i = 0; i < FD_SETSIZE; ++i) {
	if (FD_ISSET(i, readfds)) {
		/*找到被设置为可读的文件描述符i*/
		int sockfd = i;
		...
	}
}
```

从上述代码可以看出，如果要找到相应的文件描述符，需要遍历整个文件描述符表，而文件描述符表的大小在linux系统的默认设置为
```
#define __FD_SETSIZE 1024
```
先不说`select()`能监视的设备句柄的上限只有1024个，**每次触发了响应事件，都得遍历这1024位来找出哪一个设备文件描述符触发了响应事件**，这是一个效率较低的方式。

### epoll()的内核事件表

相比于`select()/poll() `使用固定长度的数组来存放需要监视的若干文件描述符，epoll使用了一个额外的文件描述符来代替，这个文件描述符唯一标识一个内核事件表。epoll把所有用户关心的文件描述符上的事件都放在内核里的一个事件表中，从而无需像于`select()/poll() `那样每次调用都要重复地传入文件描述符集。

这个内核事件表的文件描述符使用`epoll_create()`来创建：
```
int epoll_create(int size)
```
该函数返回内核事件表的文件描述符。**它会作为内核事件表epfd传入接下来的所有介绍的函数的第一个参数epfd内**。

下面的函数用于操作内核事件表：

```
int epoll_ctl(int epfd, int op, int fd, struct epoll_event *event)
```
其中，fd是要操作的文件描述符，op指定操作类型。我们主要看看event参数，它的结构体`struct epoll_event`定义为:

```
struct epoll_event
{
	__uint32_t events;
	epoll_data_t data;
}
```
其中，类型`epoll_data_t`定义为：

```
typedef union epoll_data 
{
	void *ptr;
	int fd;
	uint32_t u32;
	uint64_t u64;
} epoll_data_t;
```
通常，我们只会用这个union的成员fd，它指定了事件所从属的文件描述符。

### epoll_wait()
`epoll_wait()`是epoll系统调用的主要函数，它在一段时间内等待一组文件描述符的事件。它的API接口定义如下：

```
int epoll_wait (int epfd, struct epoll_event* events, int maxevents, int timeout)
```
其中，这个函数的参数events是触发了事件的文件描述符链表，它由内核修改，maxevents指定最多监听多少个事件（我们使用了`epoll_ctl()`注册了多少个事件进内核事件表就最多监听多少个，内核不知道这个链表最多有多长所以要调用者自己指定）。其他参数与`select()/poll()`含义相同。

**如果`poll_wait()`检测到事件，就将所有就绪的事件从内核事件表中复制到第二个参数events中，因此events的每一个元素必定是就绪的事件。这样就极大地提高了应用程序索引就绪的文件描述符的效率。**

我们通过代码来对比一下：
```
int ret = epoll_wait(epollfd, events, MAX_EVENT_NUMBER, timeout);
for (int i = 0; i < ret; ++i) {
		/*events的每个元素必定是就绪的*/
		int socked = events[i].data.fd;
		...
	}
}
```

## IO模型对比

通过上文对Linux在实现IO管理的API的分析和介绍，我们总结一下几种IO模型的差异：

IO模型 | 读写操作与阻塞阶段
------ | ------
阻塞IO | 阻塞于read()/write()读写函数
非阻塞IO | 程序不阻塞
IO复用 | 程序阻塞于IO复用系统调用，但可同时监听多个IO事件。对IO本身的读写操作时非阻塞
异步IO | 内和执行读写操作并触发读写完成事件。程序不阻塞

对于重点介绍的三种IO复用函数，我们用如下表总结：

系统调用 | select | poll | epoll
------ | ------ | ------ | ------
事件机制 | 用户传入可读、可写、异常事件，由内核修改，每次调用select重置这三个参数 | 用户传入pollfd.events事件集，由内核修改 | 内核通过内核事件表管理所有事件，用户无须传入感兴趣事件，由内核传出events反馈就绪事件
索引就绪文件描述符时间复杂度 | O(n) | O(n) | O(1)
工作模式 | 轮询O(n) | 轮询O(n) | 回调O(1)
最大支持文件描述符数 | 由__FD_SETSIZE指定，1024 | 由cat/proc/sys/fs/filemax指定，65535 | 由cat/proc/sys/fs/filemax指定，65535

### 什么时候使用epoll
操作系统内核的技术总是在不断向前发展的，epoll函数族机制是在Linux内核2.6版本之后才正式引入。**并非使用epoll()即性能最高，虽然其算法时间复杂度为O(1)，但是当监测的活跃文件描述符较多的时候，回调函数被触发地过于频繁，`epoll_wait()`的性能未必比`select()\poll()`高。因此`epoll_wait()`可以适用于连接数量多，但活跃连接较少的环境。**

通过本节课对Linux的IO接口的介绍，可以根据实际的应用情况使用合适的IO接口，以实现高性能的应用程序。


