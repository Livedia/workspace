/*
实现的功能是 client 到 server 的半双工通信，server 只能接受接收 client 发送过来的消息，但是不能向 client 发送消息
*/
#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/shm.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <cstring>
#include <iostream>
#include <thread>

#define PORT 7000
#define QUEUE 20  // 连接请求队列
int conn;
void thread_task() {}

int main() {
  /* socket() 函数，获取一个 socket 描述符，若成功则返回一个 sockfd（套接字描述符，唯一标识一个 socket)
  sockfd = socket file descriptor 套接字文件描述符
  函数声明：
  int  socket(int protofamily, int type, int protocol);
  返回 sockfd，创建失败会返回 -1，形参含义如下：
  - protofamily: 协议簇，常用如下
      - AF_INET（IPV4 网络，32 位 ip，常用协议包含 TCP/UDP）
      - AF_INET6（IPV6 网络，128 位 ip，常用协议包含 TCP/UDP）
      - AF_LOCAL（或称 AF_UNIX，本地进程通信，使用文件系统路径作为套接字地址，用于同一台机器上的进程间通信 Unix 域
                  socket）
      - AF_NETLINK（用于与 Linux 内核通信）
  - type: socket 类型，常用如下
      - SOCK_STREAM(面向连接的、可靠的、基于字节流的 socket 类型，提供了类似文件的读写操作，确保数据的
                    可靠传输和按顺序接收，TCP 使用)
      - SOCK_DGRAM(无连接的、不可靠的、基于数据报的 socket 类型，支持通过数据报发送和接收离散的数据包，
                   但不保证数据的可靠性、有序性和完整性，UDP 使用)
      - SOCK_RAW(原始的 socket 类型，提供网络协议的直接访问，允许应用程序直接发送和接收原始的网络数据包，
                  绕过传输层和应用层协议的处理，通常需要特权权限，需要自行处理协议头部)
      - SOCK_SEQPACKET(面向连接的、可靠的、有序的、基于数据报的 socket 类型，类似 SOCK_STREAM 但是保留的数据报的特性
                       适用于需要保持消息边界的应用程序，例如传输文件或者多媒体数据)
  - protocol: 协议，常用如下
      - 0 表示根据协议簇和类型来自动选择合适的协议
      - IPPROTO_TCP，传输控制协议（TCP）
      - IPPROTO_UDP，用户数据报协议（UDP）
      - IPPROTO_ICMP，Internet 控制消息协议
      - IPPROTO_IP，Internet 协议
      - IPPROTO_IPV6，Internet 协议版本 6
  */
  // 创建一个 IPV4 协议 TCP 的 socket
  int ss = socket(AF_INET, SOCK_STREAM, 0);
  printf("Create a sockfd, value is:%d\n", ss);

  // 一般是储存地址和端口的。用于信息的显示及存储使用
  struct sockaddr_in server_sockaddr;
  /*设置 sockaddr_in 结构体中相关参数*/
  server_sockaddr.sin_family = AF_INET;
  // 将一个无符号短整型数值转换为网络字节序，即大端模式 (big-endian)
  server_sockaddr.sin_port = htons(PORT);
  // printf("%d\n",INADDR_ANY);
  // INADDR_ANY 就是指定地址为 0.0.0.0 的地址，这个地址事实上表示不确定地址，或“所有地址”、“任意地址”。
  // 一般来说，在各个系统中均定义成为 0 值。
  // 将主机的无符号长整形数转换成网络字节顺序。
  server_sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);

  /* bind 函数
    函数声明：
    int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
    形参说明：
    - sockfd：即 socket 描述字，bind() 函数就是将给这个描述字绑定一个名字
    - addr：一个 const struct sockaddr *指针，指向要绑定给 sockfd 的协议地址，这个地址结构根据地址创建 socket
            时的地址协议族的不同而不同，如 ipv4 对应的是：
            struct sockaddr_in {
              sa_family_t    sin_family; // address family: AF_INET
              in_port_t      sin_port;   // port in network byte order
              struct in_addr sin_addr;   // internet address
            };

            struct in_addr {
              uint32_t       s_addr;     // address in network byte order
            };
    - addrlen：对应地址的长度
  */
  if (bind(ss, (struct sockaddr*)&server_sockaddr, sizeof(server_sockaddr)) == -1) {
    perror("bind");
    exit(1);
  }
  /* listen 函数
    调用 socket()、bind() 之后就会调用 listen() 来监听这个 socket
    socket() 函数创建的 socket 默认是一个主动类型的，listen 函数将 socket 变为被动类型的，等待客户的连接请求
    listen 函数后这个套接字就变成了监听套接字
    函数声明：
    int listen(int sockfd, int backlog);
    形参说明：
    - sockfd：要监听的 socket 描述字
    - backlog：对应的 socket 能排队的最大连接个数
  */

  if (listen(ss, QUEUE) == -1) {
    perror("listen");
    exit(1);
  }
  printf("bind port:%d, listen ...\n", server_sockaddr.sin_port);

  struct sockaddr_in client_addr;
  socklen_t length = sizeof(client_addr);

  /* accept 函数
  TCP 服务器端依次调用 socket()、bind()、listen() 之后，就会监听指定的 socket 地址了
  TCP 客户端依次调用 socket()、connect() 之后就向 TCP 服务器发送了一个连接请求
  TCP 服务器监听到这个请求之后，就会调用 accept() 函数取接收请求，这样连接就建立好了
  如果 accept 成功返回，则服务器与客户已经正确建立连接了，此时服务器通过 accept 返回的套接字来完成与客户的通信
  accept 默认会阻塞进程，直到有一个客户连接建立后返回，它返回的是一个新的可用的套接字，这个套接字是连接套接字。
  后面与客户端直接的通信都通过这个新的连接套接字来实现
  函数声明：
  int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen); //返回连接 connect_fd
  形参说明：
   - sockfd：socket 描述字
   - addr：客户端的地址相关信息，输出参数，返回得到
   - addrlen：客户端地址相关信息大小
  */

  /// 成功返回非负描述字，出错返回 -1
  conn = accept(ss, (struct sockaddr*)&client_addr, &length);
  // 如果 accpet 成功，那么其返回值是由内核自动生成的一个全新描述符，代表与所返回客户的 TCP 连接。
  // accpet 之后就会用新的套接字 conn，conn 与客户端直接建立了点点连接，可以通过 recv、send 等方法来进行通信了
  if (conn < 0) {
    perror("connect");
    exit(1);
  }

  /* 网络 I/O
  - read()/write()
  - recv()/send()
  - readv()/writev()
  - recvmsg()/sendmsg()
  - recvfrom()/sendto()
  头文件：
  #include <unistd.h>
  包含方法：
  ssize_t read(int fd, void *buf, size_t count);
  ssize_t write(int fd, const void *buf, size_t count);

  头文件：
  #include <sys/socket.h>
  #include <sys/types.h>
  包含方法：
  ssize_t send(int sockfd, const void *buf, size_t len, int flags);
  ssize_t recv(int sockfd, void *buf, size_t len, int flags);
  ssize_t sendto(int sockfd, const void *buf, size_t len, int flags, const struct sockaddr *dest_addr,
  socklen_t addrlen);
  ssize_t recvfrom(int sockfd, void *buf, size_t len, int flags, struct sockaddr *src_addr, socklen_t *addrlen);

  ssize_t sendmsg(int sockfd, const struct msghdr *msg, int flags);
  ssize_t recvmsg(int sockfd, struct msghdr *msg, int flags);

  推荐使用 recvmsg()/sendmsg() 函数
  */

  char buffer[1024];
  // 创建另外一个线程
  // std::thread t(thread_task);
  // t.join();
  // char buf[1024];
  // 主线程
  while (true) {
    // 这里把 send 注释掉了，所以这个程序中 server 只能是接收 client 端的数据并能给 client
    // 发送数据，即使不注释掉也没用，因为没有对是否有数据传入和传入
    // 进行判断所以按照下面的代码这样写，每次都要先让 server 输入后才能输出 client 传过来的数据，若是 server
    // 不输入则程序无法向下走就没有 client 发送过来的输出，
    // 而且每次显示也只能是一行，这样显示就全是错的了，所以就需要 select 和 FD_ISSET 的判断了
    //  memset(buf, 0 ,sizeof(buf));
    //  if(fgets(buf, sizeof(buf),stdin) != NULL) {
    //      send(conn, buf, sizeof(buf), 0);
    //  }

    memset(buffer, 0, sizeof(buffer));
    int len = recv(conn, buffer, sizeof(buffer), 0);  // 从 TCP 连接的另一端接收数据。
    /*该函数的第一个参数指定接收端套接字描述符；
    第二个参数指明一个缓冲区，该缓冲区用来存放 recv 函数接收到的数据；
    第三个参数指明 buf 的长度；
    第四个参数一般置 0*/
    if (strcmp(buffer, "exit\n") == 0)  // 如果没有收到 TCP 另一端发来的数据则跳出循环不输出
    {
      break;
    }
    printf("recv msg:%s", buffer);  // 如果有收到数据则输出数据
    // 必须要有返回数据，这样才算一个完整的请求
    char send_msg[1024] = "server has been received:";
    strcat(send_msg, buffer);
    printf("send msg:%s", send_msg);            // 如果有收到数据则输出数据
    send(conn, send_msg, strlen(send_msg), 0);  // 向 TCP 连接的另一端发送数据。
  }
  close(conn);  // 因为 accpet 函数连接成功后还会生成一个新的套接字描述符，结束后也需要关闭
  close(ss);  // 关闭 socket 套接字描述符
  return 0;
}
