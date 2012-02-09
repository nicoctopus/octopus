#ifndef PORT_H
#define PORT_H

#include <QtGlobal>
#include "element.h"

/**
  *
  */

class Port : public Element
{
public:
    Port();
    Port(const quint16 &portNb, const bool &active);

    /* Getter & Setter */
    quint16 getPortNumber();
    void setPortNumber(const quint16 &pn);
    virtual const char* className();
    static quint32 idPortStatic;

private:
protected:
    quint16 portNumber; // numero du port
};

#endif // PORT_H
