//
// Created by pc on 2017/7/22.
//

#ifndef USELIBEV_CLIENT_H
#define USELIBEV_CLIENT_H

#include <string>

class Client {
public:
    Client(std::string ip, unsigned int port, int fd) : m_ip(ip), m_port(port), m_fd(fd)
    { }

private:
    std::string m_ip;
    unsigned int m_port;
    int m_fd;
};


#endif //USELIBEV_CLIENT_H
