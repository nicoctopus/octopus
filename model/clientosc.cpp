#include "clientosc.h"

ClientOSC::ClientOSC() {

}

ClientOSC::ClientOSC(const quint16 &portNb, const QString &host, const bool &active) : Port(portNb, active) {
    this->nameElement = QString::number(portNb);
    this->host = host;
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
