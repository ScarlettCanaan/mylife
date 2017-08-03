# 事件处理设计模式 -- IO管理

承接上文，在学习了Linux的IO管理接口之后，可以运用Linux的IO接口来编写应用程序了。

通常应用程序需要处理各类I/O事件、信号以及定时事件。如何高效地处理各类I/O事件，基于操作系统对I/O事件处理的设计，也衍生了一套事件处理的设计模式。接下来简单介绍两种事件处理模式：Reactor和Proactor。

## 问题的引出

如何处理多路IO事件，或许最直观的方法是使用多线程，每个IO事件就开一个线程来处理，实现*per-thread-per-connection*。这个问题看似得到了解决，然而，让我们仔细思考一下，多线程其实不能很好地处理这些需求：

- **高性能**： 多线程涉及到上下文切换、数据同步与互斥、数据拷贝等操作，性能低下。
- **系统复杂度**：多线程会让系统架构变得复杂
- **可移植性**：多线程不能运行在不支持多线程架构的操作系统上

因此，需要引用满足高性能、系统简单需求的模式来优化事件处理的架构。

## IO复用机制的组成结构

让我们来分析一下IO复用的组成结构。

首先，IO复用系统需要**事件**（Event）和发送事件的对象，在操作系统中通常以**Handle**（句柄）表示事件对象。例如某个handle可能代表一个网络连接、打开的文件或者一个定时器。某个网络连接句柄可能会发送connection或者read事件。

其次，IO复用系统需要**事件处理者**（Event Handler），它是各类IO事件的最终受理者，处理各类读写请求。通常一个IO复用系统会包含各种不同种类的Event Handler ， 以处理各种特定种类的IO事件请求。

最后，IO复用系统需要一个**事件分享者**（Event Demultiplexer），它的作用是将各个IO事件请求分发给各个事件处理者（Event Handler）。开发者在开始的时候需要在分享器那里注册感兴趣的事件，并提供相应的处理者(event handlers)，或者是回调函数; 事件分享器在适当的时候会将请求的事件分发给这些handler或者回调函数。

这种IO处理和事件逻辑处理分离的组成结构，可以用来描述一台服务器或者服务器集群的框架。

## Reactor Patten

### Reactor的原理

Reactor模式是这样一种模式，其主线程只监听所注册的handle是否有事件发生，如果有就立即将该事件通知到工作线程。数据读写、连接建立、事件处理均在工作线程完成。

###一个login server的实例

关于Reactor模式的解释，Douglas C. Schmidt的文章给了很好的[例子](http://www.dre.vanderbilt.edu/%7Eschmidt/PDF/reactor-siemens.pdf)。文中使用一个login server实现Reactor模式来模拟多个client连接受理。

文中reactor模式的架构如图所示：
<div align="center">
<img src="https://github.com/ScarlettCanaan/mylife/tree/master/fuckOS/BD24B24F-4D76-41D9-94C4-95985DA05437.png" alt=""/><><br/>
</div>

图中的**Synchronous Event Demultiplexer**和**Event Handler**为核心组件，Synchronous Event Demultiplexer使用了select()作为消息循环的底层事件监听函数。它阻塞或者轮询等待Handle中的事件的到来。当select()返回时，Synchronous Event Demultiplexer将改变Handle的状态，并由Synchronous Event Demultiplexer的上层模块**Initial Dispatcher**分发给相应的Event Handler，即回调Event Handler的handle_event()。

结合文中的流程图加以理解过程。
<div align="center">
<img src="https://github.com/ScarlettCanaan/mylife/tree/master/fuckOS/ACF78F0A-588C-4E3E-A20E-696DAA410DA5.png" alt=""/><><br/>
</div>

### 同步IO模型的Reactor的工作流程

上文的login server是一个比较完整的设计，接下来将介绍仅以eopll_wait为例，实现Reactor模式下read／write的核心工作流程：

1. 主线程忘epoll内核事件表中注册handle上的读就绪事件
2. 主线程调用epoll_wait等待socket上的读就绪事件
3. 当socket有数据可读时，epoll_wait通知主线程，主线程将handle的可读事件放入*请求队列*
4. 睡眠在请求队列上的worker线程被唤醒，从handle读取数据，并处理客户请求，再往epoll内核事件表注册写就绪事件
5. 主线程调用epoll_wait等待handle可写
6. 当handle可写时，epoll_wait通知主线程，主线程将handle可写事件放入请求队列
7. 睡眠在请求队列的worket线程被唤醒，往handle写入服务器写请求结果

## Proactor Patten

与Reactor模式不同的是，Proactor模式的所有IO操作都由主线程和内核处理，工作线程仅处理业务逻辑。

单纯这么解释可能没有办法理解Proactor模式与Reactor模式相比的实用意义在哪里，那么考虑一下这种情况：

使用Reactor模式实现的IO事件处理模型，**对于IO的读写全在worker线程进行**。假如有两个事件共享一个handle并且交由一个worker线程处理，那么后来的事件必须等待前一个IO事件读写完成才能继续，假如前一个IO事件是一个在网络中传输的大文件的话，这无疑会使得性能降低。

考虑到这种情况的发生，与其用Reactor模式还不如用最原始的*per-thread-per-connection*更高效，至少对于两个IO事件是由两个线程受理。

对于Reactor模式的缺点，有没有更好的方式去解决呢？事实上，对于应用程序来说，IO读写操作的过程是不需要关心的，因此将它交给内核处理，进一步简化worker线程的负担，这便是Proactor模式。因此，Proactor模式是原生的异步处理模式，与Linux的aio_read()和aio_write()等异步IO接口相容性更高。

### 同步IO模型的改进Proactor的工作流程

上面提到Proactor是纯异步模式，使用异步IO接口更好。在操作系统没有提供异步IO接口的情况下，我们也可以用同步IO接口去模拟出Proactor模式。下面是使用同步IO接口（epoll_wait）模拟Proactor的工作流程：

1. 主线程往epoll内核事件表中注册handle上的读就绪事件
2. 主线程调用epoll_wait等待handle上有数据可读
3. **当handle有数据可读时，epoll_wait通知主线程。主线程从handle中读取数据，直到没有更多数据可读，然后将读到的数据封装成请求对象并插入请求队列**
4. 睡眠在请求队列上的worker线程被唤醒，获得请求对象并，并处理客户请求，再往epoll内核事件表注册写就绪事件
5. 主线程调用epoll_wait等待handle有数据可写
6. 当handle可写时，epoll_wait通知主线程。主线程往handle上写处理客户请求的结果

对比Reactor模式的工作流程，可以看出不同点在于Event Demultiplexer这个角色的职责，它是分发**事件**，还是分发**结果**。对Proactor模式而言，worker线程获得的不是**事件**，而是直接获得了**数据读写的结果**，如此一来worker线程的工作就只有业务逻辑了。

### 异步IO模型的Proactor的工作流程

上文的同步IO模型模拟Proactor看似可用，实际上只是一个玩具。因为IO读写的负载放在主线程并没有从实质上解决问题，而且主线程还存在**单点负载问题**。因此，最好的做法是将IO读写过程放在内核运行，即异步IO。

如下使用异步IO模型（aio_read和aio_write）的Proactor的工作流程：

1. 主线程调用aio_read函数向内核注册handle的读完成事件（一般来说要包括告诉内核用户缓冲区的位置，以及读操作完成时如何通知应用程序，我们以信号为例）
2. 主线程继续处理其他逻辑
3. 当handle上的数据被读入用户缓冲区后，内核向应用程序发送一个信号，以通知应用程序数据已可用
4. 应用程序预先定义好的信号处理函数选择一个worker线程受理客户请求。工作线程处理完客户请求后，调用aio_write函数向内核注册写完成事件
5. 主线程继续处理其他逻辑
6. 当用户缓冲区的数据被写入完成之后，内核向应用程序发送一个信号，以通知应用程序数据已发送。
7. 应用程序预先定义好的信号处理函数选择一个worker线程做善后处理

## 事件处理模式总结

学习与使用模式并不是真的能给应用程序带来多大的性能提升（PS:性能的提升其实是有的，比如Reactor模式减少了多余的工作线程的创建，比如[此篇论文](http://www.eecs.harvard.edu/~mdw/papers/seda-sosp01.pdf)分析了线程的增长对性能的损耗），模式有其自己的特性：解耦、提升复用性、模块化、可移植性、事件驱动、细力度的并发控制等等。

抛开系统机制大谈空谈模式其实是毫无意义的，因此模式的章节放在IO管理的最后，从认识原理--认识Linux实践，最后再作为IO设计的思想的升华，学习操作系统的设计模式。

对事件处理模式以及整个操作系统IO管理的内容在此结束。

