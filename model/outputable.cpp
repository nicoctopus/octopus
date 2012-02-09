#include "outputable.h"

Outputable::Outputable() {
    message = new Message();
    writer = new PacketWriter();
    socket = new UdpSocket();

    // thread
    runnable = false;
}

 /* ajouter, formatter puis envoyer le message sur le reseau */
bool Outputable::send() {
    writer->init();
    writer->addMessage(*message);
    socket->sendPacket(writer->packetData(), writer->packetSize());
    //qDebug() << " socket : "<< socket->isOk() << endl;
    return socket->isOk();
}

/* se connecter a un hote et un port */
bool Outputable::connectTo(QString *host, int p) {
    //qDebug() << " connect : "<< socket->connectTo(host->toStdString(), p) << endl;
    return socket->connectTo(host->toStdString(), p);
}

/* vider le contenu du message */
void Outputable::cleanMessage() {
    message->clear();
}

void Outputable::setMessage(const QString &s) {
    message = new Message(s.toStdString());
}

void Outputable::pushQString(const QString &s) {
    message->pushStr(s.toStdString());
}

void Outputable::pushQInt32(const quint32 &i) {
    message->pushInt32(i);
}

void Outputable::pushQInt64(const quint64 &i) {
    message->pushInt64(i);
}

/**
  * THREAD
  */
void Outputable::setRunnable(const bool &b) {
    this->runnable = b;
}
