#include "courbes.h"

Courbes::Courbes(QWidget * parent, JointMvt* joint) : QWidget(parent)
{
    startTimer(40);
    this->movement = NULL;
    this->joint = joint;
    this->indexJointMvtDuMouvement = 0;
    //QTabWidget *tabWidget = (QTabWidget*)this->parentWidget();
    //tabWidget->addTab(this, "test");
}

void Courbes::creationTab()
{
    int size = tabWidget->count();
    for(int i = 0 ; i < size ; i++){
	tabWidget->removeTab(0);
    }
    if(movement)
	for(int i = 0 ; i < this->movement->getListJointsMvt()->size() ; i++)
	{
	    qDebug() << "i " << i << endl;
	    // tabWidget->insertTab(0, this, this->movement->getListJointsMvt()->at(i)->getJointRef()->getNom());
	    //this->tabWidget->addTab(this,this->movement->getListJointsMvt()->at(i)->getJointRef()->getNom());
	    //this->show();
	}
}

void Courbes::setTabWidget(QTabWidget* tabWidget){
    this->tabWidget = tabWidget;
    this->creationTab();
}

void Courbes::setMovement(Movement *movement)
{
    this->movement = movement;
    this->creationTab();
}

void Courbes::setIndexJointMvtDuMouvement(quint8 index)
{
    this->indexJointMvtDuMouvement = index;
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

    yOffset = 50;
    if(this->joint)
    {
	QPen pen;
	pen.setColor(Qt::blue);
	pen.setWidth(1);
	painter.setPen(pen);
	xOffset = SIZE_MAX_BUFFERS / 2;
	for(int i = 0 ; i < this->joint->getListPositions()->size() ; i++)
	{
	    qreal val = (qreal)this->joint->getListPositions()->at(i)->getX();
	    newPoint = QPointF(i + xOffset, val + yOffset);
	    if(i == 0) oldPoint = newPoint;
	    painter.drawLine(oldPoint, newPoint);
	    oldPoint = newPoint;
	}
	pen.setColor(Qt::red);
	painter.setPen(pen);
	for(int i = this->joint->getJointRef()->getBufferPositions()->size() / 2  ; i < this->joint->getJointRef()->getBufferPositions()->size() ; i++)
	{
	    qreal val = (qreal)this->joint->getJointRef()->getBufferPositions()->at(i)->getX();
	    newPoint = QPointF(i - this->joint->getJointRef()->getBufferPositions()->size() / 2, val + yOffset);
	    if(i == this->joint->getJointRef()->getBufferPositions()->size() / 2) oldPoint = newPoint;
	    painter.drawLine(oldPoint, newPoint);
	    oldPoint = newPoint;
	}
	yOffset = 150;
	pen.setColor(Qt::blue);
	painter.setPen(pen);
	for(int i = 0 ; i < this->joint->getListPositions()->size() ; i++)
	{
	    qreal val = (qreal)this->joint->getListPositions()->at(i)->getY();
	    newPoint = QPointF(i + xOffset, val + yOffset);
	    if(i == 0) oldPoint = newPoint;
	    painter.drawLine(oldPoint, newPoint);
	    oldPoint = newPoint;
	}
	pen.setColor(Qt::red);
	painter.setPen(pen);
	for(int i = this->joint->getJointRef()->getBufferPositions()->size() / 2 ; i < this->joint->getJointRef()->getBufferPositions()->size() ; i++)
	{
	    qreal val = (qreal)this->joint->getJointRef()->getBufferPositions()->at(i)->getY();
	    newPoint = QPointF(i - this->joint->getJointRef()->getBufferPositions()->size() / 2, val + yOffset);
	    if(i == this->joint->getJointRef()->getBufferPositions()->size() / 2) oldPoint = newPoint;
	    painter.drawLine(oldPoint, newPoint);
	    oldPoint = newPoint;
	}
	yOffset = 250;
	pen.setColor(Qt::blue);
	painter.setPen(pen);
	for(int i = 0 ; i < this->joint->getListPositions()->size() ; i++)
	{
	    qreal val = (qreal)this->joint->getListPositions()->at(i)->getZ();
	    newPoint = QPointF(i + xOffset, val + yOffset);
	    if(i == 0) oldPoint = newPoint;
	    painter.drawLine(oldPoint, newPoint);
	    oldPoint = newPoint;
	}
	pen.setColor(Qt::red);
	painter.setPen(pen);
	for(int i = this->joint->getJointRef()->getBufferPositions()->size() / 2 ; i < this->joint->getJointRef()->getBufferPositions()->size() ; i++)
	{
	    qreal val = (qreal)this->joint->getJointRef()->getBufferPositions()->at(i)->getZ();
	    newPoint = QPointF(i - this->joint->getJointRef()->getBufferPositions()->size() / 2, val + yOffset);
	    if(i == this->joint->getJointRef()->getBufferPositions()->size() / 2) oldPoint = newPoint;
	    painter.drawLine(oldPoint, newPoint);
	    oldPoint = newPoint;
	}
    }
}
