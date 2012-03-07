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
    JointMvt(const quint32 &idJointRef, Joint *jointReference);
    JointMvt(const quint32 &idJointRef);
    QList<Position*>* getListPositions();
    Joint* getJointRef();
    quint32 getIdJointMvt() const;
    quint32 getIdJointRef() const;
    void setJointRef(Joint *jointReference);
    ~JointMvt();
    void savePositionFixe();
    void updateIdJointMvt(const quint32 &idJointMvt);
    void addPosition(Position *position);

    //VARIABLE STATIC ID
    static quint32 idJointMvtStatic;

private:
    QList<Position*>* listPositions;
    Joint *jointReference;
    quint32 idJointRef;
    quint32 idJointMvt;
    friend QDataStream & operator << (QDataStream &, const JointMvt &);
    friend QDataStream & operator >> (QDataStream &, JointMvt &);
};

Q_DECLARE_METATYPE(JointMvt)
QDataStream & operator << (QDataStream & out, const JointMvt &valeur);
QDataStream & operator >> (QDataStream & in, JointMvt &valeur);

#endif // JOINTMVT_H
