#include "diamond.h"

Diamond::Diamond(quint16 x, quint16 y, QColor* color, QGraphicsScene *scene, ClientOSC* port, BlackBoard* blackboard) : QGraphicsEllipseItem ()
{
    this->blackboard = blackboard;

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
    this->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
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

bool Diamond::getContextMenu()
{
    return this->contextMenu;
}

void Diamond::setContextMenu(bool contextMenu)
{
    this->contextMenu = contextMenu;
}

//-------- Pour bouger sur le blackboard ------------
QVariant Diamond::itemChange(GraphicsItemChange change, const QVariant &value)
 {
    if(change == ItemPositionHasChanged ) {
        blackboard->itemMoved(this);
    }
     return QGraphicsItem::itemChange(change, value);
 }

 void Diamond::mousePressEvent(QGraphicsSceneMouseEvent *event)
 {
     update();
     QGraphicsItem::mousePressEvent(event);
 }

 void Diamond::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
 {
     update();
     QGraphicsItem::mouseReleaseEvent(event);
 }

 void Diamond::setListLines(QGraphicsLineItem* line){
     listLines.append(line);
 }

 QList<QGraphicsLineItem*> Diamond::getListLines(){
     return listLines;
 }
