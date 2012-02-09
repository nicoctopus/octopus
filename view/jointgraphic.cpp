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
