#include "joint.h"

quint32 Joint::idJointStatic = 0;
/**
  * CONSTRUCTEURS
  **/
Joint::Joint()
{
    this->bufferPositions = new QList<Position*>();
    this->id = ++idJointStatic;
}

Joint::Joint(const QString &nom, QList<Position*> *bufferPositions)
{
    this->id = ++idJointStatic;
    this->nom = nom;
    this->bufferPositions = bufferPositions;
    if(this->nom == "lefthand")
	this->messageSynapse = "/lefthand_trackjointpos";
    else if(this->nom == "righthand")
	this->messageSynapse = "/righthand_trackjointpos";
    else if(this->nom == "leftelbow")
	this->messageSynapse = "/leftelbow_trackjointpos";
    else if(this->nom == "rightelbow")
	this->messageSynapse = "/rightelbow_trackjointpos";
    else if(this->nom == "leftfoot")
	this->messageSynapse = "/leftfoot_trackjointpos";
    else if(this->nom == "rightfoot")
	this->messageSynapse = "/rightfoot_trackjointpos";
    else if(this->nom == "leftknee")
	this->messageSynapse = "/leftknee_trackjointpos";
    else if(this->nom == "rightnknee")
	this->messageSynapse = "/rightknee_trackjointpos";
    else if(this->nom == "head")
	this->messageSynapse = "/head_trackjointpos";
    else if(this->nom == "torso")
	this->messageSynapse = "/torso_trackjointpos";
}
Joint::Joint(const Joint &joint)
{
    this->id = joint.id;
    this->nom = joint.nom;
    this->bufferPositions = new QList<Position*>(*(joint.bufferPositions));
}

Joint::Joint(const QString &nom)
{
    this->id = ++idJointStatic;
    this->nom = nom;
    this->bufferPositions = new QList<Position*>();
    if(this->nom == "lefthand")
	this->messageSynapse = "/lefthand_trackjointpos";
    else if(this->nom == "righthand")
	this->messageSynapse = "/righthand_trackjointpos";
    else if(this->nom == "leftelbow")
	this->messageSynapse = "/leftelbow_trackjointpos";
    else if(this->nom == "rightelbow")
	this->messageSynapse = "/rightelbow_trackjointpos";
    else if(this->nom == "leftfoot")
	this->messageSynapse = "/leftfoot_trackjointpos";
    else if(this->nom == "rightfoot")
	this->messageSynapse = "/rightfoot_trackjointpos";
    else if(this->nom == "leftknee")
	this->messageSynapse = "/leftknee_trackjointpos";
    else if(this->nom == "rightknee")
	this->messageSynapse = "/rightknee_trackjointpos";
    else if(this->nom == "head")
	this->messageSynapse = "/head_trackjointpos";
    else if(this->nom == "torso")
	this->messageSynapse = "/torso_trackjointpos";
}

/**
  *     GETTERS
  **/
quint16 Joint::getId()
{
    return this->id;
}

QString Joint::getNom()
{
    return this->nom;
}

QList<Position*>* Joint::getBufferPositions()
{
    return this->bufferPositions;
}

/*void Joint::setBufferPositions(BufferPositions *listPositionTemp)
{
    this->bufferPositions = listPositionTemp;
}*/

/**
  *    DESTRUCTEUR
  **/

Joint::~Joint()
{
    for(int i = 0 ; i < this->bufferPositions->size() ; i++)
        delete(this->bufferPositions->at(i));
    delete(this->bufferPositions);
}

/**
  *       METHODES
  **/
// methode qui sert a rajouter une position temporaire recue par synapse dans la liste des positions temporaires et de calculer
// une derivee grace a la valeur a  t-1
void Joint::addPosition(const float &x, const float &y, const float &z)
{
    if(this->bufferPositions->length() < SIZE_MAX_BUFFERS && this->bufferPositions->length() >= 1){
	this->bufferPositions->append(new Position(x, y, z));
    }
    else if (this->bufferPositions->length() == 0){
        this->bufferPositions->append((new Position(x, y, z)));
    }
    else if(this->bufferPositions->length() >= SIZE_MAX_BUFFERS)
    {
        this->bufferPositions->removeFirst();
	this->bufferPositions->append(new Position(x, y, z));
    }
}

/**
  Initialiser le buffer avec des positions a 0
  **/
void Joint::initializeBuffer() {
    for(int i=0; i<SIZE_MAX_BUFFERS; i++) {
	bufferPositions->append(new Position(0,0,0,0,0,0));
    }
}

QString Joint::getMessageSynapse()
{
    return this->messageSynapse;
}

//Pour save
QDataStream & operator << (QDataStream & out, const Joint &valeur)
{
    //std::cout << "Entrée operator << Joint" << std::endl;
    out <</* "AF_UNSPEC3" << */valeur.id;
    out << valeur.nom;
    return out;
}

// Pour load
QDataStream & operator >> (QDataStream & in, Joint &valeur)
{
    //std::cout << "Entrée operator >> Joint" << std::endl;
    in >> valeur.id;
    in >> valeur.nom;
    return in;

}
