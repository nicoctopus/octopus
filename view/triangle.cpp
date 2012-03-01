#include "triangle.h"
#include <time.h>


Triangle::Triangle(quint16 x, quint16 y, QColor* color, QGraphicsScene *scene, SampleAudio *sampleAudio) : QGraphicsPolygonItem()

{

    QPen pen;
    pen.setBrush(Qt::white);
    pen.setWidth(6);
    pen.setStyle(Qt::SolidLine);
    pen.setCapStyle(Qt::RoundCap);
    pen.setJoinStyle(Qt::RoundJoin);


    srand(time(NULL)); //Attention multiple SRAND
    QPolygon polygon;
    polygon << QPoint(0, 50) << QPoint(50, 50) << QPoint(25, 0);
    this->setPolygon(polygon);
    this->setFlags(/*QGraphicsItem::ItemIsMovable |*/ QGraphicsItem::ItemIsSelectable);
    this->sampleAudio = sampleAudio;
    this->setPen(pen);
    this->setBrush(QColor(rand()%167,198, rand()%26+67, 255));
    this->setPos(x, y);
    this->setZValue(5);
    scene->addItem(this);
    this->contextMenu = false;
}

SampleAudio* Triangle::getSampleAudio()
{
    return this->sampleAudio;
}

bool Triangle::getContextMenu()
{
    return this->contextMenu;
}

void Triangle::setContextMenu(bool contextMenu)
{
    this->contextMenu = contextMenu;
}
