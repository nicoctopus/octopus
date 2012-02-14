#include "port.h"

quint32 Port::idPortStatic = 0;

Port::Port() : Element()
{
}

Port::Port(const quint16 &portNb, const bool &active) : Element(++idPortStatic, QString::number(portNb), active)
{
    this->portNumber = portNb;
}

void Port::setPortNumber(const quint16 &pn) {
    this->portNumber = pn;
}

quint16 Port::getPortNumber() {
    return portNumber;
}

const char* Port::className() {
    return "Port";
}
