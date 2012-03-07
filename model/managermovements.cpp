#include "managermovements.h"

ManagerMovements::ManagerMovements()
{
    this->initSystem();
    this->listMovements = new QList<Movement*>();
}

ManagerMovements::~ManagerMovements()
{
    for(int i = 0 ; i < this->listMovements->size() ; i++)
	delete(this->listMovements->at(i));
    delete(this->listMovements);
}

void ManagerMovements::saveAll()
{
    QFile::remove("movement.ini");
    QSettings fichierMovement("movement.ini", QSettings::IniFormat);
    for(int i = 0 ; i < this->listMovements->size() ; i++)
    {
	qDebug() << this->listMovements->at(i)->getListJointsMvt()->at(0)->getListPositions()->size();
	this->save(this->listMovements->at(i), fichierMovement);
    }
    fichierMovement.sync();
}

void ManagerMovements::loadAll()
{
    this->listMovements->clear();
    QSettings fichierMovement("movement.ini", QSettings::IniFormat);
    //qDebug() << "size" << fichierMovement.allKeys().size() << endl;
    for(int i = 0 ; i < fichierMovement.allKeys().size() ; i++)
    {
	Movement movement = fichierMovement.value(fichierMovement.allKeys().at(i), qVariantFromValue(Movement())).value<Movement>();
	this->listMovements->append(new Movement(movement));
    }
}

void ManagerMovements::save(Movement *movement, QSettings &fichierMovement)
{
    QString key = QString::number(movement->getId());
    //qDebug() << "managerMovement save" << movement->getListJointsMvt()->at(0)->getIdJointMvt() << endl;
    fichierMovement.setValue(key, qVariantFromValue(*movement));
}

void ManagerMovements::remove(Movement *movement)
{
    quint32 idTemp = movement->getId();
    //this->managerJointMvt->remove(movement->getListJointsMvt());
    movement->getListJointsMvt()->clear();


    QSettings fichierMovement("movement.ini", QSettings::IniFormat);
    //On supprime le mouvement du fichier de serialisation

    fichierMovement.remove(QString::number(idTemp));
    //ON SUPPRIME LE MOUVEMENT ET SES COMPOSANTES de la liste
    for(int i = 0 ; i < this->listMovements->size() ; i++)
	if(this->listMovements->at(i)->getId() == idTemp)
	{
	    this->listMovements->removeAt(i);
	    //delete(this->listMovements->at(i));
	}
    if(this->listMovements->size() >= 2)
    {
	int idTemp2 = this->listMovements->at(0)->getId();
	for(int i = 1 ; i < this->listMovements->size() ; i++)
	    if(this->listMovements->at(i)->getId() > this->listMovements->at(i - 1)->getId())
		idTemp2 = this->listMovements->at(i)->getId();
	//IDTEMP2 est la plus grosse ID de la liste
	//On supprime le dernier mouvement de la liste de mouvement du fichier de serialisation
	fichierMovement.remove(QString::number(idTemp2));
	for(int i = 0 ; i < this->listMovements->size() ; i++)
	    if(this->listMovements->at(i)->getId() == idTemp2)
	    {
		this->getListMovements()->at(i)->updateId(idTemp);
		this->save(this->getListMovements()->at(i), fichierMovement);
	    }
    }
    fichierMovement.sync();

    Movement::idMovementStatic--;
}

void ManagerMovements::initSystem()
{
    qRegisterMetaTypeStreamOperators<Movement>("Movement");
    qMetaTypeId<Movement>();
    qRegisterMetaTypeStreamOperators<JointMvt>("JointMvt");
    qMetaTypeId<JointMvt>();
    qRegisterMetaTypeStreamOperators<Position>("Position");
    qMetaTypeId<Position>();
}

void ManagerMovements::sortMovementsByPositionSize()
{
    for(int i = 0 ; i < this->listMovements->size() ; i++)
	for(int j = i + 1 ; j < this->listMovements->size() ; j++)
	    if(!this->listMovements->at(i)->getListJointsMvt()->isEmpty() && !this->listMovements->at(j)->getListJointsMvt()->isEmpty())
		if(!this->listMovements->at(i)->getListJointsMvt()->at(0)->getListPositions()->isEmpty() && !this->listMovements->at(j)->getListJointsMvt()->at(0)->getListPositions()->isEmpty())
		    if(this->listMovements->at(j)->getListJointsMvt()->at(0)->getListPositions()->size() <
			    this->listMovements->at(i)->getListJointsMvt()->at(0)->getListPositions()->size())
                        this->listMovements->swap(i, j);
}

void ManagerMovements::sortMovementsById()
{
    for(int i = 1 ; i < this->listMovements->size() ; i++)
	if(this->listMovements->at(i - 1)->getId() > this->listMovements->at(i)->getId())
	{
	    this->listMovements->at(i)->updateId(this->listMovements->at(i - 1)->getId());
	    this->listMovements->at(i - 1)->updateId(this->listMovements->at(i)->getId());
	    this->listMovements->swap(i, i - 1);
	}
}

QList<Movement*>*  ManagerMovements::getListMovementsByName()
{
    QList<Movement*> *listMovementsSortedByName = new QList<Movement*>(*this->listMovements);
    if(listMovementsSortedByName->size() > 1)
	for(int i = 0 ; i < listMovementsSortedByName->size() ; i++)
	    for(int j = i + 1 ; j < listMovementsSortedByName->size() ; j++)
		if(QString::localeAwareCompare(listMovementsSortedByName->at(i)->getName().toLower(), listMovementsSortedByName->at(j)->getName().toLower()) > 0)
		    listMovementsSortedByName->swap(i, j);
    return listMovementsSortedByName;


}

void ManagerMovements::addMovement(Movement *movement)
{
    this->listMovements->append(movement);
}

QList<Movement*>* ManagerMovements::getListMovements()
{
    return this->listMovements;
}

QList<Movement*>* ManagerMovements::getListMovementsActive()
{
    QList<Movement*>*listMovementActive = new QList<Movement*>();
    for(int i = 0 ; i < this->listMovements->size() ; i++)
	if(this->listMovements->at(i)->isActive() == true)
	    listMovementActive->append(this->listMovements->at(i));
    return listMovementActive;
}
