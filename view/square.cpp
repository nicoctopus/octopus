#include "square.h"

static int R7=242;
static int V7=76;
static int B7=39;
static int R8=232;
static int V8=40;
static int B8=41;


// -----------------------------------------------------------
//                     CONSTRUCTEUR
// -----------------------------------------------------------
Square::Square(qreal x, qreal y, qreal width, qreal height, QColor* color, QGraphicsScene *scene): QGraphicsRectItem()
{

    QPen pen;
    pen.setBrush(Qt::white);
    pen.setWidth(6);
    pen.setStyle(Qt::SolidLine);
    pen.setCapStyle(Qt::RoundCap);
    pen.setJoinStyle(Qt::RoundJoin);

    //Position et geometrie du carre
    this->setRect(x, y, width, height);

    //Rendre movable
    this->setFlags(/*QGraphicsItem::ItemIsMovable |*/ QGraphicsItem::ItemIsSelectable);

    this->setPen(pen);
    this->setZValue(5);

    this->setBrush(QColor(rand()%10+232,rand()%36+40,rand()%2+49,255));
    scene->addItem(this);
}



