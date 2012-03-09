#include "clientosc.h"
quint32 ClientOSC::idClientOSCStatic = 0;

ClientOSC::ClientOSC() : Port()
{
    this->datas = new DataBITG();
    this->msg = new QList<MessageSynapse*>();
    this->listIdMovement = new QList<quint16>();
    this->active = false;
}

ClientOSC::ClientOSC(const quint16 &portNb, const QString &host, const bool &active)
    : Port(++idClientOSCStatic,  portNb, active) {
    this->host = host;
    this->datas = new DataBITG();
    this->msg = new QList<MessageSynapse*>();
    this->listIdMovement = new QList<quint16>();
    this->setRunnable(false);

}

ClientOSC::ClientOSC(const ClientOSC &copie) : Port(copie.idElement, copie.portNumber, copie.active)
{
    this->host = copie.host;
    this->listIdMovement = new QList<quint16>();
    for(int i = 0 ; i < copie.listIdMovement->size() ; i++)
	this->listIdMovement->append(copie.listIdMovement->at(i));
    this->msg = new QList<MessageSynapse*>();
    for(int i = 0 ; i < copie.msg->size() ; i++)
	this->msg->append(copie.msg->at(i));
    this->datas = new DataBITG(copie.datas->getBarycenter(), copie.datas->getIdMovement(), copie.datas->getTime(), copie.datas->getGap());
}

ClientOSC::~ClientOSC()
{

    for (int i = 0 ; i < this->msg->size() ; i++)
	delete(msg->at(i));
    delete (this->msg);
    delete(this->listIdMovement);
    delete(this->datas);
}

void ClientOSC::setHost(const QString &h) {
    this->host = h;
}

void ClientOSC::setMsgSynapse(QList<MessageSynapse*>* messages) {
    this->msg = messages;
}

void ClientOSC::setDataBITG(DataBITG* d) {
    this->datas = d;
}

/** Ajouter une donnee DataBITG et la formater pour envoyer en OSC */
bool ClientOSC::sendDataBITG() {
    this->cleanMessage();
    this->setMessage(QString("/bitg"));
    this->pushQInt32(this->datas->getBarycenter());
    this->pushQInt32(this->datas->getIdMovement());
    this->pushQInt32(this->datas->getTime());
    this->pushQInt32(this->datas->getGap());

    return (this->connectTo(&host, portNumber)*(this->Outputable::send()));
}

bool ClientOSC::sendMsgSynapse() {
    bool b = true;
    for(int i=0;i<msg->size(); i++) {
        this->cleanMessage(); //nettoyer le message pour en envoyer un nouveau
        this->setMessage(this->msg->at(i)->getStart()); //Debut du message (exemple : "/lefthand_trackjointpos")
        this->pushQInt32(this->msg->at(i)->getParams());
        b *= (this->connectTo(&host, portNumber)*(this->Outputable::send()));
    }
    return b;
}

bool ClientOSC::send(QString *host) {
    return (this->connectTo(host, portNumber)*(this->Outputable::send()));
}

const char* ClientOSC::className() {
    return "ClientOSC";
}

/**
  * Thread
  */
void ClientOSC::run() {

    //qDebug()<< "Alloooo ca tourne AUSSI !! " << endl;


    while(runnable) {
        //qDebug()<< "Alloooo ça tourne grave AUSSI !! " << runnable << endl;

        //requetes pour Synapse
        if(this->msg->size()>0) {
            this->sendMsgSynapse();
        }
        //message BITG
        else if (this->datas->getIdMovement()>-1) {
            this->sendDataBITG();
        }
        sleep(2);
    }

   // exec();
}

void ClientOSC::updateIdMovement(const quint16 &newId)
{
    this->listIdMovement->append(newId);
}

void ClientOSC::removeIdMovement(const quint16 &idMovement)
{
    for(int i = 0 ; i < this->listIdMovement->size() ; i++)
	if(this->listIdMovement->at(i) == idMovement)
	{
	    this->listIdMovement->removeAt(i);
	}
}

QList<quint16>* ClientOSC::getListIdMovement()
{
    return this->listIdMovement;
}

QDataStream & operator << (QDataStream &out, const ClientOSC &valeur)
{
    //qDebug() << "Entree operator << ClientOSC" << endl;
    out << valeur.idElement;
    out << static_cast<QList <quint16> >(*(valeur.listIdMovement));
    out << valeur.nameElement;
    out << valeur.active;
    out << valeur.host;
    out << valeur.portNumber;
    return out;
}

QDataStream & operator >> (QDataStream &in, ClientOSC &valeur)
{
    //qDebug() << "Entree operator >> ClientOSC" << endl;
    in >> valeur.idElement;
    QList<quint16> *list = new QList<quint16>();
    in >> *list;
    valeur.getListIdMovement()->append(*list);
    in >> valeur.nameElement;
    in >> valeur.active;
    in >> valeur.host;
    in >> valeur.portNumber;
    return in;
}
