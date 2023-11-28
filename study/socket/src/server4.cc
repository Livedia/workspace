/*
完美异步聊天服务端和客户端支持多用户
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
#include <iostream>
#include <list>
#include <thread>

#define PORT 7000
#define IP "127.0.0.1"

int s;
struct sockaddr_in servaddr;
socklen_t len;
std::list<int> li;  // 用 list 来存放套接字，没有限制套接字的容量就可以实现一个 server 跟若干个 client 通信

void getConn() {
  while (1) {
    int conn = accept(s, (struct sockaddr*)&servaddr, &len);
    li.push_back(conn);
    printf("%d\n", conn);
  }
}

void getData() {
  struct timeval tv;
  tv.tv_sec = 10;  // 设置倒计时时间
  tv.tv_usec = 0;
  while (1) {
    std::list<int>::iterator it;
    for (it = li.begin(); it != li.end(); ++it) {
      fd_set rfds;
      FD_ZERO(&rfds);
      int maxfd = 0;
      int retval = 0;
      FD_SET(*it, &rfds);
      if (maxfd < *it) {
        maxfd = *it;
      }
      retval = select(maxfd + 1, &rfds, NULL, NULL, &tv);
      if (retval == -1) {
        printf("select error\n");
      } else if (retval == 0) {
        // printf("not message\n");
      } else {
        char buf[1024];
        memset(buf, 0, sizeof(buf));
        int len = recv(*it, buf, sizeof(buf), 0);
        printf("%s", buf);
      }
    }
    sleep(1);
  }
}

void sendMess() {
  while (1) {
    char buf[1024];
    fgets(buf, sizeof(buf), stdin);
    // printf("you are send %s", buf);
    std::list<int>::iterator it;
    for (it = li.begin(); it != li.end(); ++it) {
      send(*it, buf, sizeof(buf), 0);
    }
  }
}

int main() {
  // new socket
  s = socket(AF_INET, SOCK_STREAM, 0);
  memset(&servaddr, 0, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(PORT);
  servaddr.sin_addr.s_addr = inet_addr(IP);
  if (bind(s, (struct sockaddr*)&servaddr, sizeof(servaddr)) == -1) {
    perror("bind");
    exit(1);
  }
  if (listen(s, 20) == -1) {
    perror("listen");
    exit(1);
  }
  len = sizeof(servaddr);

  // thread : while ==>> accpet
  std::thread t(getConn);
  t.detach();  // detach 的话后面的线程不同等前面的进程完成后才能进行，如果这里改为 join 则前面的线程无法判断结束，就会
  // 一直等待，导致后面的线程无法进行就无法实现操作
  // printf("done\n");
  // thread : input ==>> send
  std::thread t1(sendMess);
  t1.detach();
  // thread : recv ==>> show
  std::thread t2(getData);
  t2.detach();
  while (1)  // 做一个死循环使得主线程不会提前退出
  {
  }
  return 0;
}
/*这个跟前面的不一样的地方是，把获得连接套接字 getConn 和发送信息 sendMess 和接收信息 getData 放在三个函数中，创建
的三个线程分别对应处理三个函数，就可以使得 server 能跟若干个 client 通信*/
