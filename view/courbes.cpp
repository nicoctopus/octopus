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
    QPointF oldPoint, newPoint;
    quint16 yOffset = 0;
    quint16 xOffset = 0;
    qreal freq = 5* (qreal)rand()/RAND_MAX;

    yOffset = 50;
    freq = 5* (qreal)rand()/RAND_MAX;
    if(this->joint)
    {
	QPen pen;
	pen.setColor(Qt::blue);
	pen.setWidth(5);
	painter.setPen(pen);
	xOffset = SIZE_MAX_BUFFERS;
	for(int i = 0 ; i < this->joint->getListPositions()->size() ; i++)
	{
	    qreal val = (qreal)this->joint->getListPositions()->at(i)->getX();
	    newPoint = QPointF(i + xOffset, val + yOffset);
	    if(i == 0) oldPoint = newPoint;
	    painter.drawLine(oldPoint, newPoint);
	    oldPoint = newPoint;
	}
	pen.setColor(Qt::red);
	pen.setWidth(2);
	painter.setPen(pen);
	for(int i = 0  ; i < this->joint->getJointRef()->getBufferPositions()->size() ; i++)
	{
	    qreal val = (qreal)this->joint->getJointRef()->getBufferPositions()->at(i)->getX();
	    newPoint = QPointF(i, val + yOffset);
	    if(i == 0) oldPoint = newPoint;
	    painter.drawLine(oldPoint, newPoint);
	    oldPoint = newPoint;
	}
	yOffset = 150;
	pen.setColor(Qt::blue);
	pen.setWidth(5);
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
	pen.setWidth(2);
	painter.setPen(pen);
	for(int i = 0 ; i < this->joint->getJointRef()->getBufferPositions()->size() ; i++)
	{
	    qreal val = (qreal)this->joint->getJointRef()->getBufferPositions()->at(i)->getY();
	    newPoint = QPointF(i, val + yOffset);
	    if(i == 0) oldPoint = newPoint;
	    painter.drawLine(oldPoint, newPoint);
	    oldPoint = newPoint;
	}
	yOffset = 250;
	pen.setColor(Qt::blue);
	pen.setWidth(5);
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
	pen.setWidth(2);
	painter.setPen(pen);
	for(int i = 0 ; i < this->joint->getJointRef()->getBufferPositions()->size() ; i++)
	{
	    qreal val = (qreal)this->joint->getJointRef()->getBufferPositions()->at(i)->getZ();
	    newPoint = QPointF(i, val + yOffset);
	    if(i == 0) oldPoint = newPoint;
	    painter.drawLine(oldPoint, newPoint);
	    oldPoint = newPoint;
	}


	/* QPainter painter(this);
    QPointF oldPoint, newPoint;
    quint16 yOffset = 0;
    qreal freq = 5* (qreal)rand()/RAND_MAX;

    yOffset = 50;
    freq = 5* (qreal)rand()/RAND_MAX;
    painter.setPen(Qt::blue);
    if(this->movement)
    {
 for(int i = 0 ; i < this->movement->getListJointsMvt()->at(this->indexJointMvtDuMouvement)->getListPositions()->size() ; i++)
 {
     qreal val = (qreal)this->movement->getListJointsMvt()->at(this->indexJointMvtDuMouvement)->getListPositions()->at(i)->getY();
     newPoint = QPointF(i, val + yOffset);
     if(i == 0) oldPoint = newPoint;
     painter.drawLine(oldPoint, newPoint);
     oldPoint = newPoint;
 }

 yOffset = 150;

 for(int i = 0 ; i < this->movement->getListJointsMvt()->at(this->indexJointMvtDuMouvement)->getJointRef()->getBufferPositions()->size() ; i++)
 {
     qreal val = (qreal)this->movement->getListJointsMvt()->at(this->indexJointMvtDuMouvement)->getJointRef()->getBufferPositions()->at(i)->getY();
     newPoint = QPointF(i, val + yOffset);
     if(i == 0) oldPoint = newPoint;
     painter.drawLine(oldPoint, newPoint);
     oldPoint = newPoint;
 }

 yOffset = 250;
 /*for(int i = 0 ; i < this->listMovementToAnalyze->at(1)->getListJointsMvt()->at(0)->getListPositions()->size() ; i++)
 {
     qreal val = (qreal)this->listMovementToAnalyze->at(1)->getListJointsMvt()->at(0)->getListPositions()->at(i)->getY();
     newPoint = QPointF(i, val + yOffset);
     if(i == 0) oldPoint = newPoint;
     painter.drawLine(oldPoint, newPoint);
     oldPoint = newPoint;
 }
 yOffset = 350;

 for(int i = 0 ; i < this->listMovementToAnalyze->at(1)->getListJointsMvt()->at(0)->getJointRef()->getBufferPositions()->size() ; i++)
 {
     qreal val = (qreal)this->listMovementToAnalyze->at(1)->getListJointsMvt()->at(0)->getJointRef()->getBufferPositions()->at(i)->getY();
     newPoint = QPointF(i, val + yOffset);
     if(i == 0) oldPoint = newPoint;
     painter.drawLine(oldPoint, newPoint);
     oldPoint = newPoint;
 }*/

	/*yOffset = 450;

 if(this->EnregistrementVite)
     for(int i = 0 ; i < this->EnregistrementVite->size() ; i++)
     {
  qreal val = (qreal)this->EnregistrementVite->at(i)->getY();
  newPoint = QPointF(i, val + yOffset);
  if(i == 0) oldPoint = newPoint;
  painter.drawLine(oldPoint, newPoint);
  oldPoint = newPoint;
     }*/
    }

    /*  freq = 5* (qreal)rand()/RAND_MAX;
    painter.setPen(Qt::blue);
    for(int i = 0 ; i < 450 ; i++) {
 qreal val = qCos((qreal)i/20 + freq) * 10;
 newPoint = QPointF(i, val + yOffset);
 if(i == 0) oldPoint = newPoint;
 painter.drawLine(oldPoint, newPoint);
 oldPoint = newPoint;
    }
    yOffset = 250;
    freq = 5* (qreal)rand()/RAND_MAX;
    painter.setPen(Qt::blue);*/

}
