#include "jointmvt.h"
quint32 JointMvt::idJointMvtStatic = 0;

/**
  *   CONSTRUCTEURS
  **/
JointMvt::JointMvt() {
    this->idJointMvt = ++idJointMvtStatic;
    this->listPositions = new QList<Position*>();
    this->jointReference = new Joint();
}

JointMvt::JointMvt(const JointMvt &jointMvt)
{
    this->idJointMvt = jointMvt.idJointMvt;
    this->idJointRef = jointMvt.idJointRef;
    this->idMovement = jointMvt.idMovement;
    //this->jointReference = new Joint(*(jointMvt.jointReference));
    this->jointReference = new Joint();
    this->listPositions = new QList<Position*>();
    for(int i = 0 ; i < jointMvt.listPositions->size() ; i++)
        this->listPositions->append(new Position(*(jointMvt.listPositions->at(i))));
}

JointMvt::JointMvt(const quint32 &idJointRef, const quint32 &idMovement)
{
    this->idJointMvt = ++idJointMvtStatic;
    this->idJointRef = idJointRef;
    this->idMovement = idMovement;
    this->jointReference = new Joint();
    this->listPositions = new QList<Position*>();
}

JointMvt::JointMvt(const quint32 &idJointRef, const quint32 &idMovement, Joint *jointReference)
{
    this->idJointMvt = ++idJointMvtStatic;
    this->idJointRef = idJointRef;
    this->idMovement = idMovement;
    this->jointReference = jointReference;
    this->listPositions = new QList<Position*>();
}

/**
  *   GETTERS
  **/
Joint* JointMvt::getJointRef()
{
    return this->jointReference;
}

quint32 JointMvt::getIdJointMvt()
{
    return this->idJointMvt;
}

quint32 JointMvt::getIdMovement()
{
    return this->idMovement;
}

quint32 JointMvt::getIdJointRef()
{
    return this->idJointRef;
}

QList<Position*>* JointMvt::getListPositions()
{
    return listPositions;
}

void JointMvt::setJointRef(Joint *JointReference)
{
    this->jointReference = JointReference;
}

void JointMvt::addPosition(Position *position)
{
    this->listPositions->append(position);
}

void JointMvt::updateIdMovement(const quint32 &idMovement)
{
    this->idMovement = idMovement;
}

void JointMvt::updateIdJointMvt(const quint32 &idJointMvt)
{
    this->idJointMvt = idJointMvt;
    for(int i = 0 ; i < this->listPositions->size() ; i++)
    {
	this->listPositions->at(i)->updateIdJointMvt(idJointMvt);
    }
}


/**
  *   DESTRUCTEURS
  **/
JointMvt::~JointMvt()
{
    for(int i = 0 ; i < this->listPositions->size() ; i++)
	delete(this->listPositions->at(i));
    delete(this->listPositions);
    delete(this->jointReference);
}

/**
  *   METHODES
  **/
 /* ajouter la derniere position du buffer (Joint pere) a la liste des positions fixes du JointMvt courant*/
 void JointMvt::savePositionFixe()
 {
     listPositions->append(this->jointReference->getBufferPositions()->last());
 }


 /**
   *    SERIALISATION
   **/
 //Pour save
 QDataStream & operator << (QDataStream & out, const JointMvt &valeur)
 {
    // std::cout << "Entree operator << JointMvt" << std::endl;
     out << valeur.idJointMvt;
     out << valeur.idJointRef;
     out << valeur.idMovement;
     return out;
 }

 // Pour load
 QDataStream & operator >> (QDataStream & in, JointMvt &valeur)
 {
    // std::cout << "Entree operator >> JointMvt" << std::endl;
     in >> valeur.idJointMvt;
     in >> valeur.idJointRef;
     in >> valeur.idMovement;
     return in;
 }
