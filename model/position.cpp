#include "position.h"
#include "qDebug"
quint32 Position::idPositionsStatic = 0;

/**
  *   CONSTRUCTEUR
  **/
Position::Position()
{
}

Position::Position(const float &x, const float &y, const float &z, const Position &previousPosition)
{
    this->idJoinMvt = 0;
    this->id = 0;
    this->x = (qint32) COEF_FLOAT2INT * x;
    this->y = (qint32) COEF_FLOAT2INT * y;
    this->z = (qint32) COEF_FLOAT2INT * z;
    this->dx = std::abs((int)(this->x - previousPosition.x)) / INTERVAL_TIME;
    this->dy = std::abs((int)(this->y - previousPosition.y)) / INTERVAL_TIME;
    this->dz = std::abs((int)(this->z - previousPosition.z)) / INTERVAL_TIME;
}

Position::Position(const float &x, const float &y, const float &z)
{
    this->idJoinMvt = 0;
    this->id = 0;
    this->x = (qint32) COEF_FLOAT2INT * x;
    this->y = (qint32) COEF_FLOAT2INT * y;
    this->z = (qint32) COEF_FLOAT2INT * z;
    this->dx = 0;//std::abs(this->x)/INTERVAL_TIME;
    this->dy = 0;//std::abs(this->y)/INTERVAL_TIME;
    this->dz = 0;//std::abs(this->z)/INTERVAL_TIME;
}

Position::Position (const quint16 &idJoinMvt, const float &x, const float &y, const float &z, const float &a, const float &b, const float &c)
{
    this->id = ++idPositionsStatic;
    this->idJoinMvt = idJoinMvt;
    this->x = (qint32) COEF_FLOAT2INT * x;
    this->y = (qint32) COEF_FLOAT2INT * y;
    this->z = (qint32) COEF_FLOAT2INT * z;
    this->a = (qint32) COEF_FLOAT2INT * a;
    this->b = (qint32) COEF_FLOAT2INT * b;
    this->c = (qint32) COEF_FLOAT2INT * c;
    this->dx = 0;
    this->dy = 0;
    this->dz = 0;
}

Position::Position(const quint16 &idJoinMvt, const float &x, const float &y, const float &z,
		   const Position &previousPosition, const float &a, const float &b, const float &c)
{
    this->id = ++idPositionsStatic;
    this->idJoinMvt = idJoinMvt;
    this->x = (qint32) COEF_FLOAT2INT * x;
    this->y = (qint32) COEF_FLOAT2INT * y;
    this->z = (qint32) COEF_FLOAT2INT * z;
    this->a = (qint32) COEF_FLOAT2INT * a;
    this->b = (qint32) COEF_FLOAT2INT * b;
    this->c = (qint32) COEF_FLOAT2INT * c;
    this->dx = std::abs((int)(this->x - previousPosition.x)) / INTERVAL_TIME;
    this->dy = std::abs((int)(this->y - previousPosition.y)) / INTERVAL_TIME;
    this->dz = std::abs((int)(this->z - previousPosition.z)) / INTERVAL_TIME;
}

Position::Position(const quint16 &idJoinMvt, const qint32 &x, const qint32 &y, const qint32 &z,
		   const qint32 &dx, const qint32 &dy, const qint32 &dz, const qint32 &a, const qint32 &b, const qint32 &c)
{
    this->id = ++idPositionsStatic;
    this->idJoinMvt = idJoinMvt;
    this->x = x;
    this->y = y;
    this->z = z;
    this->dx = dx;
    this->dy = dy;
    this->dz = dz;
    this->a = a;
    this->b = b;
    this->c = c;
}

Position::Position (const Position &position)
{
    this->id = position.id;
    this->idJoinMvt = position.idJoinMvt;
    this->x = position.x;
    this->y = position.y;
    this->z = position.z;
    this->a = position.a;
    this->b = position.b;
    this->c = position.c;
    this->dx = position.dx;
    this->dy = position.dy;
    this->dz = position.dz;
}
/**
*      GETTERS
**/
qint32 Position::getX()
{
    return this->x;
}

qint32 Position::getY()
{
    return this->y;
}

qint32 Position::getZ()
{
    return this->z;
}

qint32 Position::getDx(){
    return this->dx;
}

qint32 Position::getDy()
{
    return this->dy;
}

qint32 Position::getDz()
{
    return this->dz;
}

quint16 Position::getId()
{
    return this->id;
}

qint32 Position::getA()
{
    return this->a;
}

qint32 Position::getB()
{
    return this->b;
}

qint32 Position::getC()
{
   return this->c;
}

quint16 Position::getIdJointMvt()
{
    return this->idJoinMvt;
}

void Position::updateIdJointMvt(const quint32 &idJointMvt)
{
    this->idJoinMvt = idJointMvt;
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
void Position::setDx(const qint32 &a) {
    this->dx = a;
}
void Position::setDy(const qint32 &a) {
    this->dy = a;
}
void Position::setDz(const qint32 &a) {
    this->dz = a;
}


/**
  *  DESTRUCTEUR
  **/

Position::~Position(){

}

//save
QDataStream & operator << (QDataStream & out, const Position &valeur)
{
    //std::cout << "Entrée operator << Position" << std::endl;
    out << valeur.id << valeur.idJoinMvt
	<< valeur.x << valeur.y << valeur.z
	<< valeur.a << valeur.b << valeur.c
	<< valeur.dx << valeur.dy << valeur.dz;
    return out;
}

//load
QDataStream & operator >> (QDataStream & in, Position &valeur)
{
   // std::cout << "Entree operator >> Position" << std::endl;
    in >> valeur.id;
    in >> valeur.idJoinMvt;
    in >> valeur.x;
    in >> valeur.y;
    in >> valeur.z;
    in >> valeur.a;
    in >> valeur.b;
    in >> valeur.c;
    in >> valeur.dx;
    in >> valeur.dy;
    in >> valeur.dz;
    return in;
}
