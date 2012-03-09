#include "port.h"

Port::Port() : Element()
{
}

Port::Port(const quint16 &portNb, const bool &active)
    : Element(0, QString::number(portNb), active, 0, 0)
{
    this->portNumber = portNb;
}
Port::Port(const quint32 &id, const quint16 &portNb, const bool &active)
    : Element(id, QString::number(portNb), active, 0, 0)
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
