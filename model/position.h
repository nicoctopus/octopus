#ifndef POSITION_H
#define POSITION_H

#include <QtGlobal>
#include <iostream>
#include "utils.h"
#include <QMetaType>

class Position
{
private:
    quint16 id;
    quint32 idJoinMvt;
    qint32 x;
    qint32 y;
    qint32 z;
    qint32 dx;
    qint32 dy;
    qint32 dz;
    qint32 a;
    qint32 b;
    qint32 c;
    friend QDataStream & operator << (QDataStream &, const Position &);
    friend QDataStream & operator >> (QDataStream &, Position &);

public:
    Position();
    Position (const Position &position);
    Position (const quint32 &idJoinMvt, const float &x, const float &y, const float &z, const float &a, const float &b, const float &c);
    Position(const quint32 &idJoinMvt, const float &x, const float &y, const float &z,
	     const Position &previousPosition, const float &a, const float &b, const float &c);
    Position(const quint32 &idJoinMvt, const qint32 &x, const qint32 &y, const qint32 &z,
	     const qint32 &dx, const qint32 &dy, const qint32 &dz, const qint32 &a, const qint32 &b, const qint32 &c);
    Position(const float &x, const float &y, const float &z, const Position &previousPosition);
    Position(const float &x, const float &y, const float &z);
    qint32 getX();
    qint32 getY();
    qint32 getZ();
    qint32 getDx();
    qint32 getDy();
    qint32 getDz();
    quint16 getId();
    qint32 getA();
    qint32 getB();
    qint32 getC();
    void setA(const qint32 &a);
    void setB(const qint32 &a);
    void setC(const qint32 &a);
    void setX(const qint32 &a);
    void setY(const qint32 &a);
    void setZ(const qint32 &a);
    void setDx(const qint32 &a);
    void setDy(const qint32 &a);
    void setDz(const qint32 &a);
    void setIdJointMvt(const quint32 &idJointMvt);

    Position* ecartPivot (const Position &pos, Position* posEcart);

    quint16 getIdJointMvt();
    ~Position();
    void updateIdJointMvt(const quint32 &idJointMvt);
    void updateId(const quint32 &id);
    //VARIABLE STATIC ID
    static quint32 idPositionsStatic;
};

Q_DECLARE_METATYPE(Position)
QDataStream & operator << (QDataStream & out, const Position &valeur);
QDataStream & operator >> (QDataStream & in, Position &valeur);
#endif // POSITION_H
