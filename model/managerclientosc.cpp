#include "managerclientosc.h"

ManagerClientOSC::ManagerClientOSC() : Manager()
{
    this->initSystem();
    this->listClientOSC = new QList<ClientOSC*>();
}


QList<ClientOSC*>* ManagerClientOSC::getListClientsOSC()
{
    return this->listClientOSC;
}

QList<ClientOSC*>* ManagerClientOSC::getListClientsOSCActive()
{
    QList<ClientOSC*>* listPortsActive = new QList<ClientOSC*>();
    for(int i = 0 ; i < this->listClientOSC->size() ; i++)
	if(this->listClientOSC->at(i)->isActive() == true)
	    listPortsActive->append(this->listClientOSC->at(i));
    return listPortsActive;
}

void ManagerClientOSC::loadAll()
{
    this->listClientOSC->clear();

    QSettings fichierClientOSC("clientOSC.ini", QSettings::IniFormat);
    for(int i = 0 ; i < fichierClientOSC.allKeys().length() ; i++)
    {
	ClientOSC *temp = new ClientOSC(fichierClientOSC.value(fichierClientOSC.allKeys().at(i), qVariantFromValue(ClientOSC())).value<ClientOSC>());
	this->listClientOSC->append(temp);
    }
}

void ManagerClientOSC::saveClientOSC(ClientOSC *clientOSC, QSettings &fichierClientOSC)
{
    QString key = QString::number(clientOSC->getId());
    fichierClientOSC.setValue(key, qVariantFromValue(*clientOSC));
}

void ManagerClientOSC::saveAll()
{
    QSettings fichierClientsOSC("clientOSC.ini", QSettings::IniFormat);
    for(int i = 0 ; i < this->listClientOSC->size() ; i++)
    {
	this->saveClientOSC(this->listClientOSC->at(i), fichierClientsOSC);
    }
    fichierClientsOSC.sync();
}

void ManagerClientOSC::removeClientOSC(QList<ClientOSC*> *listClientOSCToDelete)
{
    QSettings fichierClientOSC("clientOSC.ini", QSettings::IniFormat);
    for(int i = 0 ; i < listClientOSCToDelete->size() ; i++)
    {
	fichierClientOSC.remove(QString::number(listClientOSCToDelete->at(i)->getId()));
    }
    fichierClientOSC.sync();
}

void ManagerClientOSC::initSystem()
{
    qRegisterMetaTypeStreamOperators<ClientOSC>("ClientOSC");
    qMetaTypeId<ClientOSC>();
}

ManagerClientOSC::~ManagerClientOSC()
{
    for(int i = 0 ; i < this->listClientOSC->size() ; i++)
	delete(this->listClientOSC->at(i));
    delete(this->listClientOSC);
}
