#include <iostream>
#include "ChatRoom.h"
//#include "easylogging++.h"

//INITIALIZE_EASYLOGGINGPP

int main() {
    ChatRoom* cr = ChatRoom::getInstance();
    cr->initAccept();

    return 0;
}