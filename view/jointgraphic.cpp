#include "jointgraphic.h"

JointGraphic::JointGraphic() : QGraphicsRectItem()
{
}

QString JointGraphic::getName(){
    return name;
}

void JointGraphic::setName(QString name){
    this->name = name;
}

int JointGraphic::getXOrigin(){
    return xOrigin;
}

int JointGraphic::getYOrigin(){
    return yOrigin;
}

void JointGraphic::setXOrigin(int xOrigin){
    this->xOrigin = xOrigin;
}

void JointGraphic::setYOrigin(int yOrigin){
    this->yOrigin = yOrigin;
}
