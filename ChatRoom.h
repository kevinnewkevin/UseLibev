//
// Created by pc on 2017/7/22.
//

#ifndef USELIBEV_CHATROOM_H
#define USELIBEV_CHATROOM_H

#include "Client.h"
#include <vector>
#include <unistd.h>
#include "ev.h"

class ChatRoom {
public:
    static ChatRoom* getInstance() {
        if(!m_instance) {
            m_instance = new ChatRoom;
        }
        return m_instance;
    }

    int initAccept();
    static void accept_cb(struct ev_loop *loop, struct ev_io *w, int revents);

    ~ChatRoom() {
        delete m_instance;
        close(m_fd);
        ev_break(m_loop, EVBREAK_ALL);
    }
private:
    ChatRoom() : m_fd(-1) {
        m_loop = ev_default_loop(0);
        m_userList.clear();
    }
    ChatRoom(const ChatRoom& rhs) {}
    const ChatRoom& operator=(const ChatRoom& rhs) {}

private:
    static ChatRoom* m_instance;
    std::vector<Client*> m_userList;
    struct ev_loop* m_loop;
    int m_fd;

    ev_io m_accept;
};


#endif //USELIBEV_CHATROOM_H
