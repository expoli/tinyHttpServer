//
// Created by tcy on 2023/3/23.
//

#ifndef TINYHTTPSERVER_SERVER_H
#define TINYHTTPSERVER_SERVER_H


#include <winsock.h>

class Server{
    struct sockaddr_in server_addr{};
    SOCKET server_socket{};
    SOCKET bind_socket{};
    WSAData data{};
    int opt = 1;
public:
    SOCKET startup(unsigned short *port);
    static DWORD WINAPI accept_request(LPVOID arg);
};


#endif //TINYHTTPSERVER_SERVER_H
