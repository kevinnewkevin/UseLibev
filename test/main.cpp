//
// Created by pc on 2017/7/24.
//
#include <iostream>
#include <string>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <cstdlib>
#include <cstring>
#include "ev.h"
#include <fcntl.h>

using std::cout;
using std::endl;
using std::string;
using std::cin;

int client_sockfd;

static void readCin_cb(struct ev_loop *loop, struct ev_io *w, int revents) {
    string message;
    cin >> message;
    ssize_t sendCount = write(client_sockfd, message.c_str(), message.length() + 1);
    cout << "message: " << message << " sendCount:" << sendCount << endl;
}

static void readFd_cb(struct ev_loop *loop, struct ev_io *w, int revents) {
    char recvBuf[256];
    int recvCount = recv(client_sockfd, recvBuf, sizeof(recvBuf), 0);
    cout << "recv: " << recvBuf << " recvCount:" << recvCount << endl;
}

int main(int argc, char** argv) {
    if(argc != 3) {
        cout << "input: ip port." << endl;
        return 1;
    }

    string ip = argv[1];
    int port = atoi(argv[2]);

    int len;
    struct sockaddr_in remote_addr; //服务器端网络地址结构体
//    char buf[BUFSIZ];  //数据传送的缓冲区
    memset(&remote_addr,0,sizeof(remote_addr)); //数据初始化--清零
    remote_addr.sin_family=AF_INET; //设置为IP通信
    remote_addr.sin_addr.s_addr=inet_addr(ip.c_str());//服务器IP地址
    remote_addr.sin_port=htons(port); //服务器端口号

    /*创建客户端套接字--IPv4协议，面向连接通信，TCP协议*/
    if((client_sockfd=socket(PF_INET,SOCK_STREAM,0))<0)
    {
        cout << "socket error" << endl;
        return 1;
    }

    if(connect(client_sockfd,(struct sockaddr *)&remote_addr,sizeof(struct sockaddr))<0)
    {
        cout << "connect error" << endl;
        return 1;
    }
    cout << "connected to server。/n" << endl;

    fcntl(client_sockfd, F_SETFL, fcntl(client_sockfd, F_GETFL, 0) | O_NONBLOCK);

    struct ev_loop* loop = ev_default_loop(0);
    ev_io ev_readCin;
    ev_io ev_readFd;

    ev_io_init(&ev_readFd, readFd_cb, client_sockfd, EV_READ);
    ev_io_init(&ev_readCin, readCin_cb, STDIN_FILENO, EV_READ);
    ev_io_start(loop, &ev_readFd);
    ev_io_start(loop, &ev_readCin);
    ev_run(loop, 0);

//    len=recv(client_sockfd,buf,BUFSIZ,0);//接收服务器端信息
//    buf[len]='/0';
//    printf("%s",buf); //打印服务器端信息
//
//    /*循环的发送接收信息并打印接收信息（可以按需发送）--recv返回接收到的字节数，send返回发送的字节数*/
//    while(1)
//    {
//        printf("Enter string to send:");
//        scanf("%s",buf);
//        if(!strcmp(buf,"quit")
//            break;
//        len=send(client_sockfd,buf,strlen(buf),0);
//        len=recv(client_sockfd,buf,BUFSIZ,0);
//        buf[len]='/0';
//        printf("received:%s/n",buf);
//    }

    /*关闭套接字*/
    close(client_sockfd);
    cout << "close" << endl;

    return 0;
}