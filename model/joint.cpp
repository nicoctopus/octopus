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
    std::cout << "entree addPosition  " << std::endl;
    if(this->bufferPositions->length() < SIZE_MAX_BUFFERS && this->bufferPositions->length() >= 1){
	this->bufferPositions->append(new Position(x, y, z, *this->bufferPositions->last()));
	std::cout << "cas normal" << std::endl;
    }
    else if (this->bufferPositions->length() == 0){
	this->bufferPositions->append((new Position(x, y, z)));
	std::cout << "moins de un element" << std::endl;
    }
    else if(this->bufferPositions->length() >= SIZE_MAX_BUFFERS)
    {
	std::cout << "liste trop grande" << std::endl;
	this->bufferPositions->removeFirst();
	this->bufferPositions->append(new Position(x, y, z, *(this->bufferPositions->last())));
    }
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
