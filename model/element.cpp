#include "element.h"

/**
  *   CONSTRUCTEURS
  **/
Element::Element()
{
}

Element::Element(const Element &element)
{
    this->idElement = element.idElement;
    this->nameElement = element.nameElement;
}

Element::Element(const int &id, const QString &name, const bool &active)
{
    this->idElement = id;
    this->nameElement = name;
    this->active = active;
}

/**
  *   GETTERS SETTEURS
  **/
quint32 Element::getId() {
    return idElement;
}

void Element::setName(const QString &n) {
    this->nameElement = n;
}

QString Element::getName() {
    return nameElement;
}

void Element::updateId(const quint32 &id)
{
    this->idElement = id;
}

bool Element::isActive() {
    return active;
}

void Element::setActive(const bool &active)
{
    this->active = active;
}

/**
  *   DESTRUCTEUR
  **/
Element::~Element()
{

}
