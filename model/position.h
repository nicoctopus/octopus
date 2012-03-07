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
    Position (const float &x, const float &y, const float &z, const float &a, const float &b, const float &c);
    Position(const float &x, const float &y, const float &z);
    qint32 getX() const;
    qint32 getY() const;
    qint32 getZ() const;
    quint16 getId() const;
    qint32 getA() const;
    qint32 getB() const;
    qint32 getC() const;
    void setX(const qint32 &a);
    void setY(const qint32 &a);
    void setZ(const qint32 &a);
    void setA(const qint32 &a);
    void setB(const qint32 &a);
    void setC(const qint32 &a);

    Position* ecartPivot (Position const *pos, Position *posEcart);

    ~Position();
    void updateId(const quint32 &id);
    //VARIABLE STATIC ID
    static quint32 idPositionsStatic;
};

Q_DECLARE_METATYPE(Position)
QDataStream & operator << (QDataStream & out, const Position &valeur);
QDataStream & operator >> (QDataStream & in, Position &valeur);
#endif // POSITION_H
