<script src="{{ bower directory }}/bower-webfontloader/webfont.js" />
<script src="{{ bower directory }}/snap.svg/dist/snap.svg-min.js" />
<script src="{{ bower directory }}/underscore/underscore-min.js" />
<script src="{{ bower directory }}/js-sequence-diagrams/dist/sequence-diagram-min.js" />

# 第九节 IO管理(调度与控制概念)
[TOC]

OS除了要做进程管理、内存管理、文件管理之外，还要负责控制管理所有IO设备。例如发送命令、捕捉中断、处理异常。其一切调度策略其实围绕着一个核心：多道程序设计。

如何去理解IO模型下的多道程序设计呢？让我们从一个实际生活例子来看。

## IO问题的引出

假设你在旅游，而且正在一辆在夜间运行的火车上。在夜间，如何在正确的站点下车呢？一种方法是整晚都要醒着，然后注意到了哪一站。这样，你就不会错过你要到达的站点，但是这样会让你感到很疲倦。另外，你可以看一下时间表，估计一下火车到达目的地的时间，然后在一个稍早的时间点上设置闹铃，然后你就可以安心的睡会了。这个方法听起来也很不错，也没有错过你要下车的站点，但是当火车晚点的时候，你就要被过早的叫醒了。当然，闹钟的电池也可能会没电了，并导致你睡过站。理想的方式是，无论是早或晚，**只要当火车到站的时候，有人或其他东西能把你唤醒，就好了**。

## IO的控制与处理策略

可是，这和操作系统IO模型有什么关系呢？


好吧，让我们首先来看一张Berkeley大学EECS学院的一张赫赫有名的统计图：

![latency](https://camo.githubusercontent.com/77f72259e1eb58596b564d1ad823af1853bc60a3/687474703a2f2f692e696d6775722e636f6d2f6b307431652e706e67)

从图上可以看出：**引用一次1级缓存与磁盘磁头搜索一次相比，延迟差了2千万倍！！！**

然而缓存、磁盘这些设备都被操作系统管理，可以想象得出，如果操作系统没有一个好的控制机制，如果取一次L1缓存和刷一次磁盘放在同一次事物中，缓存事务必定要等磁盘刷完再运行，这个等待时间可以让缓存事务跑2千万次！

正是由于不同的设备拥有不同的响应时间，操作系统需要一种唤醒机制，让**到点**的设备给操作系统发一个信号，通知操作系统。

在操作系统的设计中，通常会采用两种策略：

- 轮询(Polling)
- 中断(Interrupt)

轮询方式相当于：当操作系统等待一个IO事务完成时，它可以持续的检查与IO设备共享的数据标志(用于做保护工作的互斥量)，直到设备完成工作时对这个标志进行重设。不过，**就是一种浪费**：操作系统消耗宝贵的执行时间持续的检查对应标志，并且当互斥量被上锁后，其他资源就没有办法获取锁，这样OS就会持续等待。这种情况类似于，保持清醒状态和列车驾驶员聊了一晚上：驾驶员不得不缓慢驾驶，因为你分散了他的注意力，所以火车需要更长的时间，才能到站。

更好的做法是采用中断的方式，使用发送信号的方式实现唤醒。中断可以细分为几种方式

- 中断：	
	- 普通IO中断
	- 直接存储器访问(DMA)中断
	- IO通道中断

那么，不同的中断方式的差别在哪里？请思考一下，如果IO有一次读写操作就出发一次中断，这种机制会有什么问题？当设备有数据来时，会给操作系统发送一个**“有数据包来了”**的信号，操作系统会从用户态陷入内核态，处理这个信号的数据，并且将数包报拷贝到某一块指定的地址空间，再返回用户态。

事实上，拷贝数据包到指定内存对用户态的进程来说其实也是一个意义不大且浪费资源的过程。用户态只是为了得到一个**结果**。因此，将拷贝的过程用缓存的方式代理运行，再把拷贝完成的**结果**通知给操作系统即可。

对于普通IO中断来说，每当设备完成IO操作时，就向CPU发送中断请求信号。这也是最传统的实现方式，假如在网络设备传输中收到了1000个字符，就要受理1000次中断。

DMA方式是从IO设备中整块读取／写入数据放入控制器的内部缓冲区，再核实有没有发生读／写错误，再给CPU产生一次中断信号。因此，存储控制器还需要知道内存的start memory address和offset size。DMA的缺陷是它只能**串行**地读／写块信息，如果数据不在相同连续的内存地址时，就需要产生多次中断。

IO通道方式解决了DMA的缺陷，IO通道有**自己的指令系统**，可以并行、独立地处理设备的信息传输，使外设的读写能并行操作。

## Linux里IO读写的操作

口说无凭，让我们来从Linux对IO读写操作来一探究竟。

linux给用户提供了三种同步IO读写函数（异步读写函数暂时不讨论）：

- 阻塞型(blocking) `read/write`
- 非阻塞型(non-blocking) `read/write`
- 多路复用IO `select/poll/epoll/kqueue`

其中，前两种读写函数很好理解，阻塞型IO会在`recvfrom()`中阻塞而陷入系统内核态。以下摘自Linux Man Page对于`recvfrom(2)`的描述:

```
ssize_t recvfrom(int sockfd, void *buf, size_t len, int flags, struct sockaddr *src_addr, socklen_t *addrlen)

ssize_t recvmsg(int sockfd, struct msghdr *msg, int flags)
```
> The recvfrom() and recvmsg() calls are used to receive messages from a socket, and may be used to receive data on a socket whether or not it is connection-oriented.

可以看出，系统会在**数据包准备好**和**数据包拷贝完成**的两种中断受理完成的情况下返回。

其read IO模型流程如下：

<script> 
  var d = Diagram.parse("
Note left of 用户态:调用read
用户态->内核态:recvfrom调用()
Note right of 内核态:数据包没有准备好
内核态->内核态:收到中断：数据包准备好了，开始读数据包
Note right of 内核态:复制完成
内核态->用户态:recvfrom()返回
");
  var options = {theme: 'simple'};
  d.drawSVG('diagram', options);
</script>

<!-- comment -->
```sequence
Note left of 用户态:调用read
用户态->内核态:recvfrom调用()
Note right of 内核态:数据包没有准备好
内核态->内核态:收到中断：数据包准备好了，开始读数据包
Note right of 内核态:复制完成
内核态->用户态:recvfrom()返回
```
<!-- more -->

然而最有意思的是多路复用模型`select()`，通过分析`select()`的机制，我们可以看到操作系统的**二阶段受理中断**的过程。以下摘自Linux Man Page对`select(2)`的描述：
```
 int select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout);
```
> select() and pselect() allow a program to monitor multiple file descriptors, waiting until one or more of the file descriptors become "ready" for some class of I/O operation (e.g., input possible).  A file descriptor is considered ready if it is possible to perform a corresponding I/O operation (e.g., read(2) without blocking, or a sufficiently small write(2)).

可以看出，`select()`有两个系统调用`recvfrom()`，因为`select()`可以同时监视多个设备句柄，`readfds`，`writefds`，`exceptfds`同时作为“可读事件”，“可写事件”和“错误事件”输入参数。当我监视的多个IO设备中有某个事件被触发，就从阻塞状态返回。此时用户态再发送系统调用`recvfrom()`，操作系统会将数据从内核拷贝到用户内存中并返回。这种每次受理一组事件的模型被称为“事件驱动模型”。在这个机制下，我们可以看到操作系统对IO设备中断的二阶段受理过程。

其read IO模型如下:

<script> 
  var d = Diagram.parse("
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
");
  var options = {theme: 'simple'};
  d.drawSVG('diagram', options);
</script>

<!-- comment -->
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
<!-- more -->
## IO设备控制软件的设计原则

分析以上的IO模型，我们可以以管窥豹式地看出IO设备控制的设计需要遵循下面的几种原则：

- 并行性

操作系统会同时管理多个IO设备，支持多个设备的并发操作，则意味着有利于提高操作系统的性能。这也是操作系统多道程序设计永恒的设计原则。

- 独立性

IO软件与硬件依赖分离，将软件独立于设备，可以提高设备管理软件的设计效率。

- 统一接口

文件、网络、外接设备等一切含有IO操作的设备，Linux都可以用统一的函数调用来操作。这也是Linux“一切皆为文件”的原则。当需要对某种设备交互时，用户不用关心它是什么设备，统一用“文件描述符”(一个整数序列)来描述它。因此Linux的一切文件系统的设计思想可以与之通用。

