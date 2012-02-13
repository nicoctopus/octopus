#ifndef JOINT_H
#define JOINT_H

#include <QDebug>
#include <QWidget>
#include "position.h"
#include <QMetaType>

class Joint
{
public:
    Joint();
    Joint(const Joint &joint);
    Joint(const QString &nom);
    Joint(const QString &nom, QList<Position*> *bufferPositions);
    quint16 getId();
    QString getNom();
    QString getMessageSynapse();
    QList<Position*>* getBufferPositions();
    void initializeBuffer();

    ~Joint();

    //METHODES
    void addPosition(const float &x, const float &y, const float &z);

    static quint32 idJointStatic;

private:
    quint16 id;
    QString nom;
    QList<Position*> *bufferPositions;
    QString messageSynapse;
    friend QDataStream & operator << (QDataStream &, const Joint &);
    friend QDataStream & operator >> (QDataStream &, Joint &);
};

Q_DECLARE_METATYPE(Joint)
QDataStream & operator << (QDataStream & out, const Joint &valeur);
QDataStream & operator >> (QDataStream & in, Joint &valeur);

#endif // JOINT_H
