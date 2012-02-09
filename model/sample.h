#ifndef SAMPLE_H
#define SAMPLE_H

#include <QWidget>
#include "element.h"

class Sample : public Element
{
protected:
    QString fileURL;
    quint32 idMovement;
public:
    Sample();
    Sample(const quint32 &id, const QString &name, const QString &fileURL, const quint32 &idMovement, const bool &active);

    /* Getters */
    QString getFileURL();
    quint32 getIdMovement();
    /* Setters */
    void setFileURL(const QString &file);
    void updateIdMovement(const quint32 &idMovement);
    const char* className();
};

#endif // SAMPLE_H
