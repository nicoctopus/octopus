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


Position* Position::ecartPivot(Position const *pos, Position *posEcart)
{
    posEcart->x = this->x - pos->x;
    if(posEcart->x == 0)
	posEcart->x = 1;
    posEcart->y = this->y - pos->y;
    if(posEcart->y == 0)
	posEcart->y = 1;
    posEcart->z = this->z - pos->z;
    if(posEcart->z == 0)
	posEcart->z = 1;
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
    this->id = NULL;
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



