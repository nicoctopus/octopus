#include "jointmvt.h"
quint32 JointMvt::idJointMvtStatic = 0;

/**
  *   CONSTRUCTEURS
  **/
JointMvt::JointMvt() {
    this->idJointMvt = ++idJointMvtStatic;
    this->jointReference = NULL;
    this->listPositions = new QList<Position*>();
}

JointMvt::JointMvt(const JointMvt &jointMvt)
{
    this->idJointMvt = jointMvt.idJointMvt;
    this->idJointRef = jointMvt.idJointRef;
    this->jointReference = NULL;
    this->listPositions = new QList<Position*>();
    for(int i = 0 ; i < jointMvt.listPositions->size() ; i++)
	this->listPositions->append(new Position(*jointMvt.listPositions->at(i)));
}

JointMvt::JointMvt(const quint32 &idJointRef)
{
    this->idJointMvt = ++idJointMvtStatic;
    this->idJointRef = idJointRef;
    this->jointReference = NULL;
    this->listPositions = new QList<Position*>();
}

JointMvt::JointMvt(const quint32 &idJointRef, Joint *jointReference)
{
    this->idJointMvt = ++idJointMvtStatic;
    this->idJointRef = idJointRef;
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

quint32 JointMvt::getIdJointMvt() const
{
    return this->idJointMvt;
}

quint32 JointMvt::getIdJointRef() const
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

void JointMvt::updateIdJointMvt(const quint32 &idJointMvt)
{
    this->idJointMvt = idJointMvt;
}

void JointMvt::addPosition(Position *position)
{
    this->listPositions->append(position);
}

/**
  *   DESTRUCTEURS
  **/
JointMvt::~JointMvt()
{
    delete this->jointReference;
    for(int i = 0 ; i < this->listPositions->size() ; i++)
	delete this->listPositions->at(i);
    delete this->listPositions;
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
     //qDebug() << "Entree operator save JointMvt" << endl;
     out << valeur.idJointMvt;
     out << valeur.idJointRef;
     out << valeur.listPositions->size();
     //qDebug() << "listpositionsize" << valeur.listPositions.size() << endl;
     for(int i = 0 ; i < valeur.listPositions->size() ; i++)
	 out << *(valeur.listPositions->at(i));
     return out;
 }

 // Pour load
 QDataStream & operator >> (QDataStream & in, JointMvt &valeur)
 {
     //qDebug() << "Entree operator load JointMvt" << endl;
     int sizeTemp;
     in >> valeur.idJointMvt;
     in >> valeur.idJointRef;
     in >> sizeTemp;
     for(int i = 0 ; i < sizeTemp ; i++)
     {
	 Position position;
	 in >> position;
	 valeur.listPositions->append(new Position(position));
     }
     //qDebug() << "fin operator load JointMvt" << valeur.idJointMvt << endl;
     return in;
 }
