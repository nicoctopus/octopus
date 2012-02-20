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
    Port(const quint32 &id, const quint16 &portNb, const bool &active);

    /* Getter & Setter */
    quint16 getPortNumber();
    void setPortNumber(const quint16 &pn);
    virtual const char* className();

private:
protected:
    quint16 portNumber; // numero du port
};

#endif // PORT_H
