#include "position.h"
#include "qDebug"
quint32 Position::idPositionsStatic = 0;
float Position::DIVISEUR = 0.1;
/**
  *   CONSTRUCTEUR
  **/
Position::Position()
{
}

Position::Position(const float &x, const float &y, const float &z)
{
    this->id = 0;
    this->x = (qint32) x * DIVISEUR;
    this->y = (qint32) y * DIVISEUR;
    this->z = (qint32) z * DIVISEUR;
}

Position::Position(const float &x, const float &y, const float &z, const float &a, const float &b, const float &c)
{
    this->id = ++idPositionsStatic;
    this->x = x * DIVISEUR;
    this->y = y * DIVISEUR;
    this->z = z * DIVISEUR;
    this->a = a * DIVISEUR;
    this->b = b * DIVISEUR;
    this->c = c * DIVISEUR;
}

Position::Position (const Position &position)
{
    this->id = position.id;
    this->x = position.x;
    this->y = position.y;
    this->z = position.z;
    this->a = position.a;
    this->b = position.b;
    this->c = position.c;
}


Position* Position::ecartPivot(Position *pos, Position *posEcart)
{
    //qDebug() << "ECART PIVOT test" << pos->getX() << endl;
    posEcart->setX(this->x - pos->getX());
    if(posEcart->getX() == 0)
	posEcart->setX(1);
    posEcart->setY(this->y - pos->getY());
    if(posEcart->getY() == 0)
	posEcart->setY(1);
    posEcart->setZ(this->z - pos->getZ());
    if(posEcart->getZ() == 0)
	posEcart->setZ(1);
    return posEcart;
}


/**
*      GETTERS
**/
qint32 Position::getX() const
{
    return this->x;
}

qint32 Position::getY() const
{
    return this->y;
}

qint32 Position::getZ() const
{
    return this->z;
}

quint16 Position::getId() const
{
    return this->id;
}

qint32 Position::getA() const
{
    return this->a;
}

qint32 Position::getB() const
{
    return this->b;
}

qint32 Position::getC() const
{
   return this->c;
}

void Position::updateId(const quint32 &id)
{
    this->id = id;
}

/**
  * SETTERS
  **/
void Position::setA(const qint32 &a) {
    this->a = a;
}
void Position::setB(const qint32 &a) {
    this->b = a;
}
void Position::setC(const qint32 &a) {
    this->c = a;
}
void Position::setX(const qint32 &a) {
    this->x = a;
}
void Position::setY(const qint32 &a) {
    this->y = a;
}
void Position::setZ(const qint32 &a) {
    this->z = a;
}


/**
  *  DESTRUCTEUR
  **/

Position::~Position(){
}

//save
QDataStream & operator << (QDataStream & out, const Position &valeur)
{
    //qDebug() << "Entree operator save Position" << endl;
    out << valeur.id
	<< valeur.x << valeur.y << valeur.z
	<< valeur.a << valeur.b << valeur.c;
    return out;
}

//load
QDataStream & operator >> (QDataStream & in, Position &valeur)
{
    //qDebug() << "Entree operator load Position" << endl;
    in >> valeur.id;
    in >> valeur.x;
    in >> valeur.y;
    in >> valeur.z;
    in >> valeur.a;
    in >> valeur.b;
    in >> valeur.c;
    return in;
}



