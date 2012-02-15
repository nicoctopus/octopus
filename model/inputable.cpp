#include "inputable.h"

Inputable::Inputable()
{
    reader = new PacketReader();
    socket = new UdpSocket();
    message = new Message();

    // thread
    runnable = false;

}

void Inputable::setRunnable(const bool &b) {
    this->runnable = b;
}


Inputable::~Inputable()
{
    delete(this->message);
    delete(this->reader);
    delete(this->socket);
}
