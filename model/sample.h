#ifndef SAMPLE_H
#define SAMPLE_H

#include <QWidget>
#include "element.h"

class Sample : public Element
{
protected:
    QString fileURL;
    QList<quint32> *listIdMovement;
public:
    Sample();
    Sample(const quint32 &id, const QString &name, const QString &fileURL, const quint32 &idMovement, const bool &active, const qint16 &xBlackBoard, const qint16 &yBlackBoard);
    Sample(const quint32 &id, const QString &name, const QString &fileURL, const QList<quint32> &listIdMovement, const bool &active, const qint16 &xBlackBoard, const qint16 &yBlackBoard);
    Sample(const quint32 &id, const QString &name, const QString &fileURL, const bool &active, const qint16 &xBlackBoard, const qint16 &yBlackBoard);


    /* Getters */
    QString getFileURL();
    QList<quint32>* getListIdMovement();
    /* Setters */
    void setFileURL(const QString &file);
    void updateIdMovement(const quint32 &newId);
    void removeId(const quint32 &idMovement);
    const char* className();
};

#endif // SAMPLE_H
