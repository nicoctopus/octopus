#include "diamond.h"

Diamond::Diamond(quint16 x, quint16 y, QColor* color, QGraphicsScene *scene, ClientOSC* port) : QGraphicsPolygonItem()
{

    QPen pen;
    pen.setBrush(Qt::white);
    pen.setWidth(6);
    pen.setStyle(Qt::SolidLine);
    pen.setCapStyle(Qt::RoundCap);
    pen.setJoinStyle(Qt::RoundJoin);
    this->port = port;
    QPolygon polygon;
    polygon << QPoint(0, 25) << QPoint(25, 0) << QPoint(50, 25) << QPoint(25, 50) ;
    this->setPolygon(polygon);
    this->setFlags(/*QGraphicsItem::ItemIsMovable |*/ QGraphicsItem::ItemIsSelectable);
    this->setPos(x, y);
    this->setPen(pen);
    this->setZValue(5);

    this->setBrush(QColor(rand()%2+252,rand()%153+102,0, 200));

    scene->addItem(this);

}
ClientOSC* Diamond::getPort()
{
    return this->port;
}
