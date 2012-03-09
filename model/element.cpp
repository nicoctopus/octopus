#include "element.h"
#include <QDebug.h>

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
    this->posXBlackBoard = element.posXBlackBoard;
    this->posYBlackBoard = element.posYBlackBoard;
}

Element::Element(const int &id, const QString &name, const bool &active, qint16 xBlackBoard, qint16 yBlackBoard)
{
    this->idElement = id;
    this->nameElement = name;
    this->active = active;
    this->posXBlackBoard = xBlackBoard;
    this->posYBlackBoard = yBlackBoard;
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

void Element::setPosXBlackBoard(qint16 x){
    this->posXBlackBoard = x;
}

void Element::setPosYBlackBoard(qint16 y){
    this->posYBlackBoard = y;
}

qint16 Element::getPosXBlackBoard(){
    return this->posXBlackBoard;
}

qint16 Element::getPosYBlackBoard(){
    return this->posYBlackBoard;
}


/**
  *   DESTRUCTEUR
  **/
Element::~Element()
{

}
