#ifndef OUTPUTABLE_H
#define OUTPUTABLE_H

#include "oscpkt.hh"
#include "udp.hh"
#include <QString>
#include <QThread>
#include <QDebug>

using namespace oscpkt;

class Outputable : public QThread
{
public:
    Outputable();
    virtual bool send();
    virtual bool connectTo(QString *host, int p);
    void cleanMessage();
    void setMessage(const QString &s);
    void pushQString(const QString &s);
    void pushQInt32(const quint32 &i);
    void pushQInt64(const quint64 &i);

    //thread
    void setRunnable(const bool &b);
    virtual void run() = 0;

private:
    Message *message; //message to send
    PacketWriter *writer; //write the message
    UdpSocket *socket; //socket to open the connection

protected:
    // thread
    bool runnable;

};

#endif // OUTPUTABLE_H
