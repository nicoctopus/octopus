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
    this->setRect(0,0,width,height);
    this->setPos(x,y);
    this->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable | ItemSendsGeometryChanges  );
    this->movement->setPosXBlackBoard(x);
    this->movement->setPosYBlackBoard(y);

    this->setPen(pen);

    this->setBrush(QColor(movement->r,movement->g,movement->b, 255));
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
        this->movement->setPosXBlackBoard(this->pos().x());
        this->movement->setPosYBlackBoard(this->pos().y());
       // qDebug() << this->pos().x() << " " << this->pos().y() << endl;

        blackboard->itemMoved(this);
    }
     return QGraphicsItem::itemChange(change, value);
 }

 /*void EllipseDuProjet::mousePressEvent(QGraphicsSceneMouseEvent *event)
 {
     this->setSelected(false);
     update();
     QGraphicsItem::mousePressEvent(event);
     //qDebug() << "PRESS " << this->movement->getPosXBlackBoard() << ","<< this->movement->getPosYBlackBoard();
 }

 void EllipseDuProjet::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
 {
     update();
     QGraphicsItem::mouseReleaseEvent(event);
 }*/

 void EllipseDuProjet::setListLines(QGraphicsLineItem* line){
     listLines.append(line);
 }

QList<QGraphicsLineItem*> EllipseDuProjet::getListLines(){
    return listLines;
}

EllipseDuProjet::~EllipseDuProjet()
{
    this->movement = NULL;
    this->blackboard = NULL;
  //  for( int i = 0 ;  i < this->listLines.size() ; i++)
//	this->listLines.at(i) = NULL;
  //  this->listLines = NULL;
}
