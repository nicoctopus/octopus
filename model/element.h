#ifndef ELEMENT_H
#define ELEMENT_H

#include <QtGlobal>
#include <QString>
#include <QObject>

class Element
{
public:
    Element();
    Element(const int &id, const QString &name, const bool &active, qint16 xBlackBoard, qint16 yBlackBoard);
    Element(const Element &element);
    quint32 getId();
    virtual void updateId(const quint32 &id);
    void setName(const QString &n);
    QString getName();
    virtual const char* className() = 0;
    bool isActive();
    void setActive(const bool &active);
    void setPosXBlackBoard(qint16 x);
    void setPosYBlackBoard(qint16 y);
    qint16 getPosXBlackBoard();
    qint16 getPosYBlackBoard();

    ~Element();
protected:
    quint32 idElement;
    QString nameElement;
    bool active;
    qint16 posXBlackBoard;
    qint16 posYBlackBoard;

};

#endif // ELEMENT_H
