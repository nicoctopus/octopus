#include "ellipseduprojet.h"

#include <QWidget>
#include <QtGui>
#include <QTreeWidget>

#include <QMenu>
#include <QAction>
#include <QGraphicsSceneContextMenuEvent>

EllipseDuProjet::EllipseDuProjet(qreal x, qreal y, qreal width, qreal height, QColor* color, QGraphicsScene *scene, Movement *movement) : QGraphicsEllipseItem ()
{


    QPen pen;
    pen.setBrush(Qt::white);
    pen.setWidth(6);
    pen.setStyle(Qt::SolidLine);
    pen.setCapStyle(Qt::RoundCap);
    pen.setJoinStyle(Qt::RoundJoin);
    this->movement = movement;
    this->setRect(x,y,width,height);
    this->setFlags(/*QGraphicsItem::ItemIsMovable |*/ QGraphicsItem::ItemIsSelectable);

    this->setPen(pen);

    this->setBrush(QColor(rand()%4,rand()%137,255, 255));
    this->setZValue(5);
    scene->addItem(this);
    this->createActions();
 //   connect(scene, SIGNAL(selectionChanged()), this, SLOT(removeMovement));
}

Movement* EllipseDuProjet::getMovement()
{
    return this->movement;
}

void EllipseDuProjet::contextMenuEvent(QGraphicsSceneContextMenuEvent  *event)
{
    QMenu menu;
    //menu.addAction(this->actionRemove);
    QAction *removeAction = menu.addAction("Remove");
    menu.exec(event->screenPos());

    //menu.addAction(this->actionRemove);
    //menu.exec(event->globalPos());
}

void EllipseDuProjet::createActions()
{
   /* this->actionRemove = new QAction(tr("&Remove"), this);
    connect(this->actionRemove, SIGNAL(triggered()), this, SLOT(slotRemove()));
    this->actionRemove->setShortcut(tr("Ctrl+R"));
    this->actionRemove->setStatusTip(tr("Remove the object"));*/
}

/*void EllipseDuProjet::slotRemove()
{
    emit remove(this->movement);
}*/
