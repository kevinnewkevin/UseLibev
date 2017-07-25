//
// Created by pc on 2017/7/22.
//

#include "Client.h"
#include <sys/socket.h>
#include <iostream>

using std::string;
using std::cout;
using std::endl;

void Client::read_cb(struct ev_loop *loop, struct ev_io *w, int revents) {
    Client* client = static_cast<Client*> (w->data);
    char recvBuf[256];
    int recvCount = recv(client->m_fd, recvBuf, sizeof(recvBuf), 0);
    ChatRoom::getInstance()->sendAllUser(recvBuf);
    cout << "recv: " << recvBuf << " recvCount:" << recvCount << endl;
}

void Client::write_cb(struct ev_loop *loop, struct ev_io *w, int revents) {
    Client* client = static_cast<Client*> (w->data);
    if(client->m_sendList.empty()) {
        ev_io_stop(loop, &client->m_ev_write);
        return;
    }

    string message;
    client->getMessage(message);

    ssize_t sendCount = write(client->m_fd, message.c_str(), message.length() + 1);
    cout << "send:" << message  <<" ip:" << client->m_ip << " port:" << client->m_port << endl;
}

int Client::getMessage(std::string &message) {
    if(m_sendList.empty()) {
        return 1;
    }
    message = m_sendList.front();
    m_sendList.pop();
    return 0;
}

int Client::addMessage(const std::string & message)
{
    if(m_sendList.empty()) {
        ev_io_start(ChatRoom::getInstance()->getLoop(), &m_ev_write);
    }
    m_sendList.push(message);
	return 0;
}
