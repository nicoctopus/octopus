#include "triangle.h"
#include <time.h>


Triangle::Triangle(qreal x, qreal y, QColor* color, QGraphicsScene *scene, SampleAudio *sampleAudio, BlackBoard* blackboard) : QGraphicsPolygonItem ()
{
    this->blackboard = blackboard;

    QPen pen;
    pen.setBrush(Qt::white);
    pen.setWidth(6);
    pen.setStyle(Qt::SolidLine);
    pen.setCapStyle(Qt::RoundCap);
    pen.setJoinStyle(Qt::RoundJoin);

    QPolygon polygon;
    polygon << QPoint(0, 50) << QPoint(50, 50) << QPoint(25, 0);
    this->setPolygon(polygon);
    this->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable | ItemSendsGeometryChanges  );
    this->sampleAudio = sampleAudio;
    this->setPen(pen);
    this->setBrush(QColor(sampleAudio->r,sampleAudio->g, sampleAudio->b, 255));
    this->setPos(x, y);
    this->sampleAudio->setPosXBlackBoard(x);
    this->sampleAudio->setPosYBlackBoard(y);
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

//-------- Pour bouger sur le blackboard ------------
QVariant Triangle::itemChange(GraphicsItemChange change, const QVariant &value)
 {
    if(change == ItemPositionHasChanged ) {
        this->sampleAudio->setPosXBlackBoard(this->pos().x());
        this->sampleAudio->setPosYBlackBoard(this->pos().y());
        blackboard->itemMoved(this);
       // qDebug() << this->pos().x() << " " << this->pos().y() << endl;
       // qDebug() << "itemChange" << this->pos().x() << " " << this->pos().y() << endl;

    }
     return QGraphicsItem::itemChange(change, value);
 }

 void Triangle::mousePressEvent(QGraphicsSceneMouseEvent *event)
 {
    // this->setSelected(true);
     //blackboard->liaison(this);
     //qDebug()<<"PRESS "<< this->sampleAudio->getPosXBlackBoard() << this->sampleAudio->getPosYBlackBoard() << endl;
     update();
     QGraphicsItem::mousePressEvent(event);
 }

 void Triangle::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
 {
     update();
     QGraphicsItem::mouseReleaseEvent(event);
 }

 void Triangle::setListLines(QGraphicsLineItem* line){
     listLines.append(line);
 }

 QList<QGraphicsLineItem*> Triangle::getListLines(){
     return listLines;
 }

Triangle::~Triangle()
{
    this->blackboard = NULL;
    this->sampleAudio = NULL;
  //  for(int i = 0 ;  i< this->listLines.size() ; i++)
//	this->listLines.at(i) = NULL;
}
