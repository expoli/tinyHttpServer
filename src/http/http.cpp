//
// Created by tcy on 2023/3/23.
//

#include "http.h"
#include "../file/file.h"

// 从指定的客户端套接字 sock ，读取一行数据，保存到 buff 中
// 返回实际读取到的字节数
int get_line(int sock, char *buff, int size) {
  char c = 0;
  int i = 0;
  // http 中的换行是 \r\n
  while (i < size - 1 && c != '\n') {
    int n = recv(sock, &c, 1, 0);
    if (n > 0) {
      if (c == '\r') {
        n = recv(sock, &c, 1, MSG_PEEK); // 瞟一眼，不从缓冲区中拿出来
        if (n > 0 && c == '\n') {
          recv(sock, &c, 1, 0);
        } else {
          c = '\n';
        }
      }
      buff[i++] = c;
    } else {
      // todo
      c = '\n';
    }
  }
  buff[i] = 0; // '\0'
  return i;
}

// 资源不存在
void not_found(int client) {
  // todo 资源不存在信息
}

// 资源不存在
void not_found(int client, const char * file) {
  const int BUFF_LEN = 1024;
  char buff[BUFF_LEN];
  // todo 资源不存在信息
  // 判断是否提供了 404 页面
  struct stat status;
  // 如果没有提供 404 页面，则直接服务器返回 404
  int char_num = 1;
  // 丢掉所有未读取的数据包内容
  // 数据包最后一行是一个空行
  while (char_num > 0 && strcmp("\n", buff)) {
    char_num = get_line(client, buff, sizeof(buff));
    //      PRINTF(buff);
  }
  // 发送响应包的头信息
  headers(client,404,"404.html");
  // 发送  404 网页内容
  sprintf(buff, "<!DOCTYPE html>\n"
                "<html lang=\"en\">\n"
                "<head>\n"
                "    <meta charset=\"UTF-8\">\n"
                "    <title>404 NOT FOUND!</title>\n"
                "</head>\n"
                "<body>\n"
                "<h2> 404 NOT FOUND! </h2>"
                "</body>\n"
                "</html>");
  send(client,buff, strlen(buff),0);
}



// 发送文件头
void headers(int client, int code, const char *type){
  // 发送响应包的头信息
  const int BUFF_LEN = 1024;
  char buff[BUFF_LEN];

  switch (code) {
  case 200:
    sprintf(buff, "HTTP/1.0 200 OK\r\n");
    break ;
  case 404:
    sprintf(buff, "HTTP/1.0 404 Not Found\r\n");
    break ;
  default:
    sprintf(buff, "HTTP/1.0 200 OK\r\n");
  }
  send(client,buff, strlen(buff),0);

  strcpy(buff,"Server: tcy http server 0.1\r\n");
  send(client,buff, strlen(buff),0);

  //  strcpy(buff,"Content-type: text/html\r\n");
  sprintf(buff,"Content-type: %s\r\n", type);
  send(client,buff, strlen(buff),0);

  strcpy(buff,"\r\n");
  send(client,buff, strlen(buff),0);
}


void server_file(int client, const char *file_name) {
  const int BUFF_LEN = 1024;
  char buff[BUFF_LEN];
  int char_num = 1;
  // 把请求数据包的剩余数据读取完
  while (char_num > 0 && strcmp("\n", buff)) {
    char_num = get_line(client, buff, sizeof(buff));
  }

  FILE *resource = nullptr;

  if (file_has_spec_suffix(file_name)){
    resource = fopen(file_name, "rb");
  } else {
    resource = fopen(file_name, "r");
  }

  if (resource == nullptr) {
    not_found(client, file_name);
  } else {
    // 正式发送资源给流览器
    headers(client, 200, get_file_head_type(file_name));
    // 发送请求的资源信息
    cat(client,resource);

    printf("file send done!");
  }
  // 关闭文件指针
  fclose(resource);
}

void cat(int client, FILE *file) {
  const int BUFF_LEN = 4096;
  char buff[BUFF_LEN];
  long count = 0;
  while (true){
    // 实际读取成功的字节数
    int num = fread(buff, sizeof (char ), sizeof (buff), file);
    if (num <= 0){
      break ;
    }
    send(client, buff, num, 0);
    count += num;
  }
  printf("already send %ld bytes to browser\n",count);
}
