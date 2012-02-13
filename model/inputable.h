#ifndef INPUTABLE_H
#define INPUTABLE_H

#include "oscpkt.hh"
#include "udp.hh"
#include <QString>
#include <QThread>
#include <QDebug>

using namespace oscpkt;

class Inputable : public QThread
{
   // Q_OBJECT

public:
    Inputable();
    ~Inputable();
    //thread
    void setRunnable(const bool &b);
    virtual void run() = 0;

protected:
    Message *message; //message to send
    PacketReader *reader; //write the message
    UdpSocket *socket; //socket to open the connection

    // thread
    bool runnable;
};

#endif // INPUTABLE_H
