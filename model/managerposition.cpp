#include "managerposition.h"


ManagerPosition::ManagerPosition() : Manager()
{
    this->initSystem();
    this->listPositionsTemp = new QList<Position*>();
}

void ManagerPosition::save(Position *position, QSettings &fichierPosition)
{
    QString key = QString::number(position->getId());
    fichierPosition.setValue(key, qVariantFromValue(*position));
}

void ManagerPosition::loadAll()
{
    QSettings fichierPosition("position.ini", QSettings::IniFormat);
    for(int i = 0 ; i < fichierPosition.allKeys().size() ; i++)
    {
	Position *temp = new Position(fichierPosition.value(fichierPosition.allKeys().at(i), qVariantFromValue(Position())).value<Position>());
	this->listPositionsTemp->append(temp);
    }

    for(int i = 0; i < this->listPositionsTemp->size() ; i++)
	for(int j = i + 1 ; j < this->listPositionsTemp->size() ; j++)
	    if(this->listPositionsTemp->at(i)->getId() > this->listPositionsTemp->at(j)->getId())
		this->listPositionsTemp->swap(i,j);
}

void ManagerPosition::remove(QList<Position*> *listPositionsToDelete)
{
    QSettings fichierPosition("position.ini", QSettings::IniFormat);
    for(int i = 0 ; i < listPositionsToDelete->size() ; i++)
    {
	fichierPosition.remove(QString::number(listPositionsToDelete->at(i)->getId()));
	//fichierPosition.remove(QString::number(this->listPositionsTemp->last()->getId()));
	//this->listPositionsTemp->last()->updateId(listPositionsToDelete->at(i)->getId());
	//this->save(this->listPositionsTemp->last(), fichierPosition);
	Position::idPositionsStatic--;
    }
    fichierPosition.sync();
}

void ManagerPosition::saveAll()
{
    QFile::remove("position.ini");
    QSettings fichierPosition("position.ini", QSettings::IniFormat);
    for(int i = 0 ; i < this->listPositionsTemp->size() ; i++)
	this->save(this->listPositionsTemp->at(i), fichierPosition);
    fichierPosition.sync();
}

QList<Position*>* ManagerPosition::getListPositionTemp()
{
    return this->listPositionsTemp;
}

void ManagerPosition::initSystem()
{
    qRegisterMetaTypeStreamOperators<Position>("Position");
    qMetaTypeId<Position>();
}

ManagerPosition::~ManagerPosition()
{
    for(int i = 0 ; i < this->listPositionsTemp->size() ; i++)
	delete(this->listPositionsTemp->at(i));
    delete(this->listPositionsTemp);
}
