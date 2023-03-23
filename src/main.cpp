// firstWebServer.cpp: 定义应用程序的入口点。
//

#include <cstdio>
#include "utils/utils.h"
#include "server/server.h"



int main() {
  unsigned short port = 8000;
  Server server;
  SOCKET server_sock = server.startup(&port);

  printf("httpd server started, listening %d port. \n", port);

  struct sockaddr_in client_addr;
  int client_addr_len = sizeof(client_addr);

  while (true) {
    // 阻塞式等待用户通过流览器发起访问
    // 新的套接字只对这个客户端使用
    SOCKET client_socket =
        accept(server_sock, (struct sockaddr *)&client_addr, &client_addr_len);
    if (client_socket == SOCKET_ERROR) {
      error_die("accapt");
    }
    // 使用 client_sock 对用户进行服务（收发消息)
    // 创建一个线程，进行私密一对一的服务
    // 进程可以包含多个线程
    DWORD thread_id = 0;
    CreateThread(0, 0, Server::accept_request, (void *)client_socket, 0, &thread_id);
  }
  return 0;
}
