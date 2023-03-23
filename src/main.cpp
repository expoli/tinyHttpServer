// firstWebServer.cpp: 定义应用程序的入口点。
//

#include "http/http.h"
#include "utils/utils.h"
#include "config.h"

// 实现网络的初始化
// 返回值：套接字（服务器端的套接字）
// 参数：port 表示端口
//			*port 的值是 0，那么就自动分配一个可用的端口
int startup(unsigned short *port) {
  // 具体流程
  /*
  1. 网络通信初始化（windows需要做）
  2. 创建套接字
  3. 设置套接字属性，端口可复用（可略）
  4. 帮定套接字和网络地址
  5. 创建监听队列
  */
  WSAData data;
  int ret = WSAStartup(MAKEWORD(1, 1), // 1.1 版本的协议
                       &data);
  if (ret) { // 失败
    error_die("WAS error!");
  }

  int server_socket =
      socket(PF_INET, // 套接字类型
             SOCK_STREAM, // 数据包类型（流数据专用通道、报文数据）
             IPPROTO_TCP); // 使用的协议
  if (server_socket == -1) {
    // 打印错误提示、并结束程序
    error_die("Socket create error!");
  }
  // 设置端口可复用
  int opt = 1;
  ret = setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, (const char *)&opt,
                   sizeof(opt));
  if (ret == -1) {
    error_die("setsockopt");
  }

  // 配置服务器端的网络地址
  struct sockaddr_in server_addr;
  memset(&server_addr, 0, sizeof(server_addr));
  server_addr.sin_family = AF_INET; // 网络地址类型
  server_addr.sin_port = htons(*port);
  server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

  // 绑定套接字
  ret =
      bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr));
  if (ret < 0) {
    error_die("bind server addr");
  }

  // 动态分配一个端口
  int nameLen = sizeof(server_addr);
  if (*port == 0) {
    ret = getsockname(server_socket, (struct sockaddr *)&server_addr, &nameLen);
    if (ret < 0) {
      error_die("get port");
    }
    *port = server_addr.sin_port;
  }

  // 创建监听队列
  ret = listen(server_socket, 5);
  if (ret < 0) {
    error_die("listen");
  }

  return server_socket;
}

// 用户服务函数
DWORD WINAPI accept_request(LPVOID arg) {
  // 解析 HTTP
  char buff[BUFF_SIZE_LEN];

  int client = (SOCKET)arg; // 客户端套接字

  // 读取一行数据
  // "GET / HTTP/1.1\n"
  int char_num = get_line(client, buff, sizeof(buff));
//  PRINTF(buff);

  char method[METHOD_BUFF_LEN];
  // 控制遍历缓存中的数据，获取方法（可以用 switch case
  int j = 0, i = 0;
  while (!isspace(buff[j]) && i < sizeof method - 1) {
    method[i++] = buff[j++];
  }
  method[i] = 0; //'\0'
  PRINTF(method);

  // 检查一下请求的方法，是否是本服务器支持的方法
  if (stricmp("GET", method) && stricmp("POST", method)) {
    PRINTF("unsupported method")
    return 0;
  }

  // 解析资源文件的路径
  // www.rock.com/abc/CursorTest.html
  char url[URL_BUFF_LEN];
  i = 0; // 重置游标
  // 跳过资源路径前面的空格（现在还没有作用
  while (isspace(buff[j]) && j < sizeof(buff)) {
    j++;
  }
  while (!isspace(buff[j]) && i < sizeof(url) - 1 && j < sizeof(buff)) {
    url[i++] = buff[j++];
  }
  url[i] = 0;
  PRINTF(url);

  char path[PATH_LEN] = "";
  sprintf(path, "%s%s",WWW_WORK_DIR, url);
  // 如果 URL 最后一个字符是 / 则代表访问 index.html
  if (path[strlen(path) - 1] == '/') {
    strcat(path, "index.html");
  }
  // 如果请求的是一个目录，则默认访问目录下面的 index.html
  struct stat status;
  if (stat(path, &status) == -1) {
    // notfound(client);
    not_found(client,path);
  } else {
    // 位操作看是否是目录
    if ((status.st_mode & S_IFMT) == S_IFDIR) {
      // 如果是目录，添加 index.html
      strcat(path, "/index.html");
    }
    // 发送文件的接口 server_file(client, path);
    PRINTF(path);
    server_file(client,path);
  }
  // 关闭套接字
  closesocket(client);
  return 0;
}

int main() {
  unsigned short port = 8000;
  int server_sock = startup(&port);

  printf("httpd server started, listening %d port. \n", port);

  struct sockaddr_in client_addr;
  int client_addr_len = sizeof(client_addr);

  while (1) {
    // 阻塞式等待用户通过流览器发起访问
    // 新的套接字只对这个客户端使用
    int client_socket =
        accept(server_sock, (struct sockaddr *)&client_addr, &client_addr_len);
    if (client_socket < 0) {
      error_die("accapt");
    }
    // 使用 client_sock 对用户进行服务（收发消息)
    // 创建一个线程，进行私密一对一的服务
    // 进程可以包含多个线程
    DWORD thread_id = 0;
    CreateThread(0, 0, accept_request, (void *)client_socket, 0, &thread_id);
  }
  return 0;
}
