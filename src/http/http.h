//
// Created by tcy on 2023/3/23.
//

#ifndef CURSORCPPWEB_HTTP_H
#define CURSORCPPWEB_HTTP_H

#include <sys/stat.h>
#include <sys/types.h>
#include <cstdio>
// 网络通信需要包含的头文件、需要加载的库文件。
#include <winsock2.h>
#pragma comment(lib, "WS2_32.lib")

void not_found(int client, const char * file);
int get_line(int sock, char *buff, int size);
void cat(int client, FILE *pIobuf);
void server_file(int client, const char *file_name);
void headers(int client, int code, const char * type);

#endif // CURSORCPPWEB_HTTP_H
