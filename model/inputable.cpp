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
