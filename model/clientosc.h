#ifndef CLIENTOSC_H
#define CLIENTOSC_H

#include <QMetaType>
#include "port.h"
#include "outputable.h"
#include "databitg.h"
#include "messagesynapse.h"

class ClientOSC : public Port, public Outputable
{
public:
    ClientOSC();
    ClientOSC(const quint16 &portNb, const QString &host, const bool &active);
    ClientOSC(const ClientOSC &copie);
    ~ClientOSC();
    void setHost(const QString &h);
    QString getHost(){return this->host;}
    void setDataBITG(DataBITG* d);
    void setMsgSynapse(QList<MessageSynapse*>* messages);
    void updateIdMovement(quint16 idMovement);
    bool sendDataBITG();
    bool sendMsgSynapse();
    bool send(QString *host);

    virtual const char* className();
    virtual void run();

private:
    QList<MessageSynapse*>* msg;
    DataBITG *datas;
    QString host;
    quint16 idMovement;
    friend QDataStream & operator << (QDataStream &, const ClientOSC &);
    friend QDataStream & operator >> (QDataStream &, ClientOSC &);
};

Q_DECLARE_METATYPE(ClientOSC)
QDataStream & operator << (QDataStream & out, const ClientOSC &valeur);
QDataStream & operator >> (QDataStream & in, ClientOSC &valeur);

#endif // CLIENTOSC_H
