#include "myqline.h"

MyQLine::MyQLine():QLine()
{
}

MyQLine::MyQLine(const QPoint &pt1, const QPoint &pt2): QLine(pt1,pt2)
{
}

QString MyQLine::getNamePointA(){
    return namePointA;
}

int MyQLine::getXAOrigin(){
    return xAOrigin;
}

int MyQLine::getYAOrigin(){
    return yAOrigin;
}

void MyQLine::setNamePointA(QString namePointA){
    this->namePointA = namePointA;
}

void MyQLine::setXAOrigin(int xAOrigin){
    this->xAOrigin = xAOrigin;
}

void MyQLine::setYAOrigin(int yAOrigin){
    this->yAOrigin = yAOrigin;
}

QString MyQLine::getNamePointB(){
    return namePointB;
}

int MyQLine::getXBOrigin(){
    return xBOrigin;
}

int MyQLine::getYBOrigin(){
    return yBOrigin;
}

void MyQLine::setNamePointB(QString namePointB){
    this->namePointB = namePointB;
}

void MyQLine::setXBOrigin(int xBOrigin){
    this->xBOrigin = xBOrigin;
}

void MyQLine::setYBOrigin(int yBOrigin){
    this->yBOrigin = yBOrigin;
}

void MyQLine::setGraphicsLine(QGraphicsLineItem* graphicsLine){
    this->graphicsLine = graphicsLine;
}

QGraphicsLineItem* MyQLine::getGraphicsLine(){
    return graphicsLine;
}
void MyQLine::setLastXa(int lastXa){
    this->lastXa = lastXa;
}

void MyQLine::setLastYa(int lastYa){
    this->lastYa = lastYa;
}
void MyQLine::setLastXb(int lastXb){
    this->lastXb = lastXb;
}
void MyQLine::setLastYb(int lastYb){
    this->lastYb = lastYb;
}
int MyQLine::getLastXa(){
    return lastXa;
}

int MyQLine::getLastYa(){
    return lastYa;
}
int MyQLine::getLastXb(){
    return lastXb;
}
int MyQLine::getLastYb(){
    return lastYb;
}
void MyQLine::setLastPositionsToOrigins(){
    this->lastXa = xAOrigin;
    this->lastYa = yAOrigin;
    this->lastXb = xBOrigin;
    this->lastYb = yBOrigin;
}
