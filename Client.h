//
// Created by pc on 2017/7/22.
//

#ifndef USELIBEV_CLIENT_H
#define USELIBEV_CLIENT_H

#include <string>
#include "ev.h"
#include "ChatRoom.h"
#include <queue>

class Client {
public:
    Client(struct ev_loop* loop, std::string ip, unsigned int port, int fd) : m_ip(ip), m_port(port), m_fd(fd)
    {
        m_ev_read.data = this;
        ev_io_init(&m_ev_read, read_cb, m_fd, EV_READ);
        ev_io_start(loop, &m_ev_read);

        m_ev_write.data = this;
        ev_io_init(&m_ev_write, write_cb, m_fd, EV_WRITE);
        ev_io_start(loop, &m_ev_write);
    }

    static void read_cb(struct ev_loop *loop, struct ev_io *w, int revents);
    static void write_cb(struct ev_loop *loop, struct ev_io *w, int revents);
    int getMessage(std::string& message);
    int addMessage(const std::string& message);

private:
    std::string m_ip;
    unsigned int m_port;
    int m_fd;
    ev_io m_ev_read;
    ev_io m_ev_write;
    std::queue<std::string> m_sendList;
};


#endif //USELIBEV_CLIENT_H
