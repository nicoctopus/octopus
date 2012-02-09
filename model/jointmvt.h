#ifndef JOINTMVT_H
#define JOINTMVT_H

#include <QList>
#include <QMetaType>
#include "joint.h"
#include "position.h"

class JointMvt
{
public:
    JointMvt();
    JointMvt(const JointMvt &jointMvt);
    JointMvt(const quint32 &idJointRef, const quint32 &idMovement, Joint *jointReference);
    JointMvt(const quint32 &idJointRef, const quint32 &idMovement);
    QList<Position*>* getListPositions();
    Joint* getJointRef();
    quint32 getIdJointMvt();
    quint32 getIdJointRef();
    quint32 getIdMovement();
    void setJointRef(Joint *jointReference);
    void updateIdMovement(const quint32 &idMovement);
    ~JointMvt();
    void savePositionFixe();
    void addPosition(Position *position);
    void updateIdJointMvt(const quint32 &idJointMvt);

    //VARIABLE STATIC ID
    static quint32 idJointMvtStatic;

private:
    QList<Position*> *listPositions;
    Joint *jointReference;
    quint32 idJointRef;
    quint32 idJointMvt;
    quint32 idMovement;
    friend QDataStream & operator << (QDataStream &, const JointMvt &);
    friend QDataStream & operator >> (QDataStream &, JointMvt &);
};

Q_DECLARE_METATYPE(JointMvt)
QDataStream & operator << (QDataStream & out, const JointMvt &valeur);
QDataStream & operator >> (QDataStream & in, JointMvt &valeur);

#endif // JOINTMVT_H
