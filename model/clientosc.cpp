#include "clientosc.h"

ClientOSC::ClientOSC() : Port()
{
    this->datas = new DataBITG();
    this->msg = new QList<MessageSynapse*>();
    this->active = false;
}

ClientOSC::ClientOSC(const quint16 &portNb, const QString &host, const bool &active) : Port(portNb, active) {
    this->host = host;
    this->datas = new DataBITG();
    this->msg = new QList<MessageSynapse*>();
}

ClientOSC::ClientOSC(const ClientOSC &copie) : Port(copie.portNumber, copie.active)
{
    this->host = copie.host;
    this->idMovement = copie.idMovement;
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
    while(runnable) {
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
    exec();
}

void ClientOSC::updateIdMovement(quint16 idMovement)
{
    this->idMovement = idMovement;
}

QDataStream & operator << (QDataStream &out, const ClientOSC &valeur)
{
    //qDebug() << "Entree operator << ClientOSC" << endl;
    out << valeur.idElement;
    out << valeur.idMovement;
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
    in >> valeur.idMovement;
    in >> valeur.nameElement;
    in >> valeur.active;
    in >> valeur.host;
    in >> valeur.portNumber;
    return in;
}
