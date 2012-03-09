#ifndef ELEMENT_H
#define ELEMENT_H

#include <QtGlobal>
#include <QString>
#include <QObject>

class Element
{
public:
    Element();
    Element(const int &id, const QString &name, const bool &active);
    Element(const Element &element);
    quint32 getId();
    virtual void updateId(const quint32 &id);
    void setName(const QString &n);
    QString getName();
    virtual const char* className() = 0;
    bool isActive();
    void setActive(const bool &active);

    //Variables publiques
    quint16 r, g, b;

    ~Element();
protected:
    quint32 idElement;
    QString nameElement;
    bool active;

};

#endif // ELEMENT_H
