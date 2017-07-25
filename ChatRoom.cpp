//
// Created by pc on 2017/7/22.
//

#include <iostream>
#include "ChatRoom.h"
#include <fcntl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <errno.h>
#include <cstring>
//#include "easylogging++.h"

using std::vector;
using std::cout;
using std::endl;

ChatRoom* ChatRoom::m_instance = NULL;

int ChatRoom::initAccept() {
    struct sockaddr_in addr;

    m_fd = socket(PF_INET, SOCK_STREAM, 0);
    if(m_fd < 0) {
       // LOG(ERROR) << "m_fd < 0 !";
        cout << "m_fd < 0 !" << endl;
        return 1;
    }

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8888);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    //设置端口快速回收
    //int on = 1;
    //if (setsockopt(_fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0)

    if(bind(m_fd, (struct sockaddr*) &addr, sizeof(addr)) < 0 ) {
       // LOG(ERROR) << "bind error!";
        cout << "bind error!" << endl;
        return 2;
    }

    fcntl(m_fd, F_SETFL, fcntl(m_fd, F_GETFL, 0) | O_NONBLOCK);
    listen(m_fd, 10000);

    m_accept.data = this;
    ev_io_init(&m_accept, accept_cb, m_fd, EV_READ);
    ev_io_start(m_loop, &m_accept);
    ev_run(m_loop, 0);

    //LOG(INFO) << "listen ok!";
    cout << "listen ok!" << endl;
    return 0;
}

void ChatRoom::accept_cb(struct ev_loop *loop, struct ev_io *w, int revents) {
    if(EV_ERROR & revents) {
        std::cout << "error ev." << std::endl;
        return;
    }

    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);

    int fd = accept(w->fd, (struct sockaddr*)&client_addr, &client_len);
    if(fd < 0) {
       // LOG(ERROR) << "accept fd error.";
        cout << "accept fd error." << endl;
        return;
    }

    ChatRoom* cr = static_cast<ChatRoom*> (w->data);

    //设置FD为非阻塞
    fcntl(fd, F_SETFL, fcntl(fd, F_GETFL, 0) | O_NONBLOCK);

    Client* cli = new (std::nothrow) Client(loop, inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port), fd);
    if(cli) {
        cr->m_userList.push_back(cli);
       // LOG(INFO) << inet_ntoa(client_addr.sin_addr) << ":" << ntohs(client_addr.sin_port) << " in room.";
        cout << inet_ntoa(client_addr.sin_addr) << ":" << ntohs(client_addr.sin_port) << " in room." << endl;
    } else {
        close(fd);
    }

}

void ChatRoom::sendAllUser(std::string message) {
    vector<Client*>::iterator iter = m_userList.begin();
    while(iter != m_userList.end()) {
		(*iter)->addMessage(message);
        ++iter;
    }
}
