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

#define MYPORT 7000
#define BUFFER_SIZE 1024

int main() {
  /// 定义 sockfd
  int sock_cli = socket(AF_INET, SOCK_STREAM, 0);

  /// 定义 sockaddr_in
  struct sockaddr_in servaddr;
  memset(&servaddr, 0, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(MYPORT);  // 服务器端口
  servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");  // 服务器 ip，inet_addr 用于 IPv4 的 IP 转换（十进制转换为二进制）
  // 127.0.0.1 是本地预留地址
  // 连接服务器，成功返回 0，错误返回 -1
  if (connect(sock_cli, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0) {
    perror("connect");
    exit(1);
  }

  char sendbuf[BUFFER_SIZE];
  char recvbuf[BUFFER_SIZE];

  while (fgets(sendbuf, sizeof(sendbuf), stdin) !=
         NULL) { /*每次读取一行，读取的数据保存在 buf 指向的字符数组中，成功，则返回第一个参数 buf；*/
    send(sock_cli, sendbuf, strlen(sendbuf), 0);  /// 发送
    if (strcmp(sendbuf, "exit\n") == 0) break;
    recv(sock_cli, recvbuf, sizeof(recvbuf), 0);  /// 接收
    fputs(recvbuf, stdout);

    memset(sendbuf, 0, sizeof(sendbuf));  // 接受或者发送完毕后把数组中的数据全部清空（置 0）
    memset(recvbuf, 0, sizeof(recvbuf));
  }
  close(sock_cli);
  return 0;
}
/*
在 TCP 三次握手完成后会进入等待连接队列，等待服务端调用 accpet 与之建立连接，这时候是 server 端调用 accept
跟客户端建立通信，客户端并不需要调用 accpet，因为有很多个客户端要跟服务端建立连接，这时候服务端就会有一个队列，
对已经经过三次握手的才可以建立连接（类似缓存信息），这个是由服务端来确认的，客户端并不知道什么时候服务端才能跟它建立连接，
在服务端没有调用 accept 与之连接或者还未排队到它，只能是一直等待，直到服务端准备好了才能跟客户端建立连接，
所以主动权在服务端
*/
