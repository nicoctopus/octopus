#include "ellipseduprojet.h"

#include <QWidget>
#include <QtGui>
#include <QTreeWidget>

#include <QMenu>
#include <QAction>
#include <QGraphicsSceneContextMenuEvent>

EllipseDuProjet::EllipseDuProjet(qreal x, qreal y, qreal width, qreal height, QColor* color, QGraphicsScene *scene, Movement *movement, BlackBoard* blackboard) : QGraphicsEllipseItem ()
{
    this->blackboard = blackboard;


    QPen pen;
    pen.setBrush(Qt::white);
    pen.setWidth(6);
    pen.setStyle(Qt::SolidLine);
    pen.setCapStyle(Qt::RoundCap);
    pen.setJoinStyle(Qt::RoundJoin);
    this->movement = movement;
    this->setRect(x,y,width,height);
    this->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);

    this->setPen(pen);

    this->setBrush(QColor(rand()%4,rand()%137,255, 255));
    this->setZValue(5);
    scene->addItem(this);
    this->contextMenu = false;
}

Movement* EllipseDuProjet::getMovement()
{
    return this->movement;
}

bool EllipseDuProjet::getContextMenu()
{
    return this->contextMenu;
}

void EllipseDuProjet::setContextMenu(bool contextMenu)
{
    this->contextMenu = contextMenu;
}

//-------- Pour bouger sur le blackboard ------------
QVariant EllipseDuProjet::itemChange(GraphicsItemChange change, const QVariant &value)
 {
    if(change == ItemPositionHasChanged ) {
        blackboard->itemMoved(this);
    }
     return QGraphicsItem::itemChange(change, value);
 }

 void EllipseDuProjet::mousePressEvent(QGraphicsSceneMouseEvent *event)
 {
     update();
     QGraphicsItem::mousePressEvent(event);
 }

 void EllipseDuProjet::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
 {
     update();
     QGraphicsItem::mouseReleaseEvent(event);
 }

 void EllipseDuProjet::setListLines(QGraphicsLineItem* line){
     listLines.append(line);
 }

 QList<QGraphicsLineItem*> EllipseDuProjet::getListLines(){
     return listLines;
 }
