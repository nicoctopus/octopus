#include "ellipseduprojet.h"

static int R3=4;
static int V3=138;
static int B3=255;
static int R4=0;
static int V4=1;
static int B4=255;

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
}

Movement* EllipseDuProjet::getMovement()
{
    return this->movement;
}
