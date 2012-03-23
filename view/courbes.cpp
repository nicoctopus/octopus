#include "courbes.h"

Courbes::Courbes(QWidget * parent) : QWidget(parent)
{
    startTimer(40);
    this->listPositionBuffer = NULL;
    this->listPositionMouvement = new QList<qint32>();
}
void Courbes::setPositions(QList<Position *> *listPositionMouvement, QList<Position *> *listPositionBuffer, int val)
{
    this->listPositionMouvement->clear();
    this->valeur = val;
    for( int i = 0 ;  i < listPositionMouvement->size() ; i++)
    {
	if(valeur == 1)
	    this->listPositionMouvement->append(listPositionMouvement->at(i)->getX());
	if(valeur == 2)
	    this->listPositionMouvement->append(listPositionMouvement->at(i)->getY());
	if(valeur == 3)
	    this->listPositionMouvement->append(listPositionMouvement->at(i)->getZ());
    }
    this->listPositionBuffer = listPositionBuffer;
}

void Courbes::timerEvent(QTimerEvent *) {
    update();
}
void Courbes::paintEvent(QPaintEvent *e) {
    QPainter painter(this);
    painter.scale(1.5, 0.5);
    QPointF oldPoint, newPoint;
    quint16 yOffset = 0;
    quint16 xOffset = 0;

    yOffset = 100;
    if(this->listPositionBuffer && this->listPositionMouvement)
    {
	QPen pen;
	pen.setColor(Qt::blue);
	pen.setWidth(1);
	painter.setPen(pen);
	xOffset = SIZE_MAX_BUFFERS / 2;
	for(int i = 0 ; i < this->listPositionMouvement->size() ; i++)
	{
	    qreal val = (qreal)this->listPositionMouvement->at(i);
	    newPoint = QPointF(i + xOffset, val + yOffset);
	    if(i == 0) oldPoint = newPoint;
	    painter.drawLine(oldPoint, newPoint);
	    oldPoint = newPoint;
	}
	pen.setColor(Qt::red);
	painter.setPen(pen);
	for(int i = this->listPositionBuffer->size() / 2  ; i < this->listPositionBuffer->size() ; i++)
	{
	    qreal val;
	    if(valeur == 1)
		val = (qreal)this->listPositionBuffer->at(i)->getX();
	    if(valeur == 2)
		val = (qreal)this->listPositionBuffer->at(i)->getY();
	    if(valeur == 3)
		val = (qreal)this->listPositionBuffer->at(i)->getZ();
	    newPoint = QPointF(i - this->listPositionBuffer->size() / 2, val + yOffset);
	    if(i == this->listPositionBuffer->size() / 2) oldPoint = newPoint;
	    painter.drawLine(oldPoint, newPoint);
	    oldPoint = newPoint;
	}
    }
    /*yOffset = 150;
 pen.setColor(Qt::blue);
 painter.setPen(pen);
 for(int i = 0 ; i < this->listPositionMouvement->size() ; i++)
 {
     qreal val = (qreal)this->listPositionMouvement->at(i);
     newPoint = QPointF(i + xOffset, val + yOffset);
     if(i == 0) oldPoint = newPoint;
     painter.drawLine(oldPoint, newPoint);
     oldPoint = newPoint;
 }
 pen.setColor(Qt::red);
 painter.setPen(pen);
 for(int i = this->listPositionBuffer->size() / 2 ; i < this->listPositionBuffer->size() ; i++)
 {
     qreal val = (qreal)this->listPositionBuffer->at(i);
     newPoint = QPointF(i - this->listPositionBuffer->size() / 2, val + yOffset);
     if(i == this->listPositionBuffer->size() / 2) oldPoint = newPoint;
     painter.drawLine(oldPoint, newPoint);
     oldPoint = newPoint;
 }
 yOffset = 250;
 pen.setColor(Qt::blue);
 painter.setPen(pen);
 for(int i = 0 ; i < this->listPositionMouvement->size() ; i++)
 {
     qreal val = (qreal)this->listPositionMouvement->at(i);
     newPoint = QPointF(i + xOffset, val + yOffset);
     if(i == 0) oldPoint = newPoint;
     painter.drawLine(oldPoint, newPoint);
     oldPoint = newPoint;
 }
 pen.setColor(Qt::red);
 painter.setPen(pen);
 for(int i = this->listPositionBuffer->size() / 2 ; i < this->listPositionBuffer->size() ; i++)
 {
     qreal val = (qreal)this->listPositionBuffer->at(i);
     newPoint = QPointF(i - this->listPositionBuffer->size() / 2, val + yOffset);
     if(i == this->listPositionBuffer->size() / 2) oldPoint = newPoint;
     painter.drawLine(oldPoint, newPoint);
     oldPoint = newPoint;
 }
    }*/
}
