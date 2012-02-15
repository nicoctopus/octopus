#include "managermovements.h"

ManagerMovements::ManagerMovements()
{
    this->initSystem();
    this->listMovements = new QList<Movement*>();
    this->managerJointMvt = new ManagerJointMvt();
}

ManagerMovements::~ManagerMovements()
{
    for(int i = 0 ; i < this->listMovements->size() ; i++)
	delete(this->listMovements->at(i));
    delete(this->listMovements);
    delete(this->managerJointMvt);
}

void ManagerMovements::saveAll()
{
    QFile::remove("movement.ini");
    QSettings fichierMovement("movement.ini", QSettings::IniFormat);
    for(int i = 0 ; i < this->listMovements->size() ; i++)
	this->save(this->listMovements->at(i), fichierMovement);
    fichierMovement.sync();
}

void ManagerMovements::loadAll()
{
    this->listMovements->clear();
    QSettings fichierMovement("movement.ini", QSettings::IniFormat);
    for(int i = 0 ; i < fichierMovement.allKeys().size() ; i++)
	this->listMovements->append(new Movement(fichierMovement.value(fichierMovement.allKeys().at(i), qVariantFromValue(Movement())).value<Movement>()));
    this->managerJointMvt->loadAll();
    this->dispacher();
}

void ManagerMovements::dispacher()
{
    for(int i = 0 ; i < this->listMovements->size() ; i++)
    {
	for(int j = 0 ; j < this->managerJointMvt->getListJointsMvts()->size() ; j++)
	{
	    JointMvt *jointMvtTemp = this->managerJointMvt->getListJointsMvts()->at(j);
	    if(this->listMovements->at(i)->getId() == jointMvtTemp->getIdMovement())
	    {
		for(int k = 0 ; k < this->managerJointMvt->getManagerPosition()->getListPositionTemp()->size() ; k++)
		{
		    if(jointMvtTemp->getIdJointMvt() == this->managerJointMvt->getManagerPosition()->getListPositionTemp()->at(k)->getIdJointMvt())
			jointMvtTemp->addPosition(this->managerJointMvt->getManagerPosition()->getListPositionTemp()->at(k));
		}
		this->listMovements->at(i)->addJointMvt(this->managerJointMvt->getListJointsMvts()->at(j));
	    }
	}
    }
    if(this->managerJointMvt->getManagerPosition()->getListPositionTemp()->isEmpty())
	Position::idPositionsStatic = 0;
    else{
	int tmp = 0;
	for(int i=0; i<managerJointMvt->getManagerPosition()->getListPositionTemp()->size();i++)
	    if (tmp < managerJointMvt->getManagerPosition()->getListPositionTemp()->at(i)->getId()) tmp = managerJointMvt->getManagerPosition()->getListPositionTemp()->at(i)->getId();
	Position::idPositionsStatic = tmp;
    }
    if(this->managerJointMvt->getListJointsMvts()->isEmpty())
	JointMvt::idJointMvtStatic = 0;
    else{
	int tmp = 0;
	for(int i=0; i<managerJointMvt->getListJointsMvts()->size();i++)
	    if(tmp < managerJointMvt->getListJointsMvts()->at(i)->getIdJointMvt()) tmp = managerJointMvt->getListJointsMvts()->at(i)->getIdJointMvt();
	JointMvt::idJointMvtStatic = tmp;

    }if(this->listMovements->isEmpty())
	Movement::idMovementStatic = 0;
    else {
	int tmp = 0;
	for(int i=0; i<listMovements->size(); i++)
	    if(tmp < listMovements->at(i)->getId()) tmp = listMovements->at(i)->getId();
	Movement::idMovementStatic = tmp;
    }
}

void ManagerMovements::save(Movement *movement, QSettings &fichierMovement)
{
    QString key = QString::number(movement->getId());
    fichierMovement.setValue(key, qVariantFromValue(*movement));
    this->managerJointMvt->save(movement->getListJointsMvt());
}

void ManagerMovements::remove(Movement *movement)
{
    qDebug() << "         debut removeMovement..." << endl;
    quint32 idTemp = movement->getId();
    this->managerJointMvt->remove(movement->getListJointsMvt());
    movement->getListJointsMvt()->clear();

    QSettings fichierMovement("movement.ini", QSettings::IniFormat);
    //On supprime le mouvement du fichier de serialisation

    fichierMovement.remove(QString::number(idTemp));
    //ON SUPPRIME LE MOUVEMENT ET SES COMPOSANTES de la liste
    for(int i = 0 ; i < this->listMovements->size() ; i++)
	if(this->listMovements->at(i)->getId() == idTemp)
	{
	    //delete(this->listMovements->at(i));
	    this->listMovements->removeAt(i);
	}
    if(this->getListMovements()->isEmpty() != true)
    {
	int idTemp2 = this->listMovements->at(0)->getId();
	for(int i = 1 ; i < this->listMovements->size() ; i++)
	    if(this->listMovements->at(i)->getId() > this->listMovements->at(i - 1)->getId())
		idTemp2 = this->listMovements->at(i)->getId();
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
    qDebug() << "         ...fin removeMovement" << endl;
}

void ManagerMovements::initSystem()
{
    qRegisterMetaTypeStreamOperators<Movement>("Movement");
    qMetaTypeId<Movement>();
}

void ManagerMovements::sortMovements()
{
    for(int i = 1 ; i < this->listMovements->size() ; i++)
	if(this->listMovements->at(i - 1)->getListJointsMvt()->at(0)->getListPositions()->size() >
		this->listMovements->at(i)->getListJointsMvt()->at(0)->getListPositions()->size())
	    this->listMovements->swap(i, i - 1);
}

void ManagerMovements::sortMovementsById()
{
    for(int i = 1 ; i < this->listMovements->size() ; i++)
	if(this->listMovements->at(i - 1)->getId() > this->listMovements->at(i)->getId())
	    this->listMovements->swap(i, i - 1);
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

ManagerJointMvt* ManagerMovements::getManagerJointMvt()
{
    return this->managerJointMvt;
}
