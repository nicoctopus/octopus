#include "blackboard.h"
#include "QDebug"


// -----------------------------------------------------------
//                     CONSTRUCTEUR
// -----------------------------------------------------------

BlackBoard::BlackBoard(QWidget *parent): QGraphicsView(parent)
{
    this->setAcceptDrops(true);
    QGraphicsScene *scene = new QGraphicsScene();
    this->setScene(scene);
    connect(this, SIGNAL(refreshSignal()), this, SLOT(refresh()));
}

// -----------------------------------------------------------
//                      METHODES
// -----------------------------------------------------------

void BlackBoard::selectedItems(){
    itemsToLink.append(this->scene()->selectedItems().at(0));
    qDebug() << this->itemsToLink.size() << endl;
    if(this->itemsToLink.size() == 2){
    EllipseDuProjet *ellipse;
    if(this->itemsToLink.at(0)->type() == 65537)
	{
        ellipse = (EllipseDuProjet*)this->itemsToLink.at(0);
	    if(this->itemsToLink.at(1)->type() == 65539)
	    {
		Triangle *triangle = (Triangle*)(this->itemsToLink.at(1));
		this->updateSampleAudioOfMovement(ellipse->getMovement(), triangle->getSampleAudio());
		emit decocherCheckBoxLink();
	    }
	    else if(this->itemsToLink.at(1)->type() == 65538)
	    {
		Diamond *diamond = (Diamond*)(this->itemsToLink.at(1));
		this->updateClientOSCOfMovement(ellipse->getMovement(), diamond->getPort());
		emit decocherCheckBoxLink();
	    }
	}
	else if(this->itemsToLink.at(1)->type() == 65537)
	{
        ellipse = (EllipseDuProjet*)this->itemsToLink.at(1);
	    if(this->itemsToLink.at(0)->type() == 65539)
	    {
		Triangle *triangle = (Triangle*)(this->itemsToLink.at(0));
		this->updateSampleAudioOfMovement(ellipse->getMovement(), triangle->getSampleAudio());
		emit decocherCheckBoxLink();
	    }
	    else if(this->itemsToLink.at(0)->type() == 65538)
	    {
		Diamond *diamond = (Diamond*)(this->itemsToLink.at(0));
		this->updateClientOSCOfMovement(ellipse->getMovement(), diamond->getPort());
		emit decocherCheckBoxLink();
	    }
	}
	this->itemsToLink.clear();
    }

}

void BlackBoard::updateClientOSCOfMovement(Movement *movement, ClientOSC *clientOSC)
{
    clientOSC->updateIdMovement(movement->getId());
    /**
      *    BIZARRRREEEEEEEEEEE
      **/
    movement->addClientOSC(clientOSC);
    //emit save(clientOSC);
    emit save(movement);
    emit refreshSignal();
}

void BlackBoard::updateSampleAudioOfMovement(Movement *movement, SampleAudio *newSampleAudio)
{
    if(movement->getSampleAudio() != NULL)
    {
	movement->getSampleAudio()->removeId(movement->getId());
	emit save(movement->getSampleAudio());
    }
    movement->setSampleAudio(newSampleAudio);
    movement->getSampleAudio()->updateIdMovement(movement->getId());
    emit save(movement);
    emit refreshSignal();
}

// -----------------------------------------------------------
// Réimplementation des méthodes de QGraphicsView pour assurer le Drag & drop
// -----------------------------------------------------------
void BlackBoard::dragMoveEvent(QDragMoveEvent *event){
    event->acceptProposedAction();
}
void BlackBoard::dragEnterEvent(QDragEnterEvent *event){
    event->acceptProposedAction();
}

void BlackBoard::dropEvent(QDropEvent *event){
    event->acceptProposedAction();
    QString str = event->mimeData()->text();
    QByteArray ba = event->mimeData()->data("foo/bar");

    if(str == "Movements")
    {
	bool isInTheList = false;
	for(int i = 0 ; i < this->listMovements->size() ; i++)
	    if(this->listMovements->at(i) == *(Movement**)ba.constData()){
		isInTheList = true;
		QMessageBox msgBox;
		msgBox.setText("L'élement est déjà présent sur le Blackboard");
		msgBox.exec();
	    }
	if(isInTheList == false)
	{
	    this->listMovements->append(*(Movement**)ba.constData());
	    this->listMovements->last()->setActive(true);
	    emit save(this->listMovements->last());
	    emit refreshSignal();
	}
    }

    if(str == "Samples")
    {
	bool isInTheList = false;
	for(int i = 0 ; i < this->listSamplesAudio->size() ; i++)
	    if(this->listSamplesAudio->at(i) == *(SampleAudio**)ba.constData()){
		isInTheList = true;
		QMessageBox msgBox;
		msgBox.setText("L'élement est déjà présent sur le Blackboard");
		msgBox.exec();
	    }
	if(isInTheList == false)
	{
	    this->listSamplesAudio->append(*(SampleAudio**)ba.constData());
	    this->listSamplesAudio->last()->setActive(true);
	    emit save(this->listSamplesAudio->last());
	    emit refreshSignal();
	}
    }
    if(str == "Ports")
    {
	bool isInTheList = false;
	for(int i = 0 ; i < this->listPorts->size() ; i++)
	    if(this->listPorts->at(i) == *(ClientOSC**)ba.constData()){
		isInTheList = true;
		QMessageBox msgBox;
		msgBox.setText("L'élement est déjà présent sur le Blackboard");
		msgBox.exec();
	    }
	if(isInTheList == false)
	{
	    this->listPorts->append(*(ClientOSC**)ba.constData());
	    this->listPorts->last()->setActive(true);
	    emit save(this->listPorts->last());
	    emit refreshSignal();
	}
    }
    this->setSceneRect(this->scene()->sceneRect());
}


// -----------------------------------------------------------
//    GETTERS / SETTERS
// -----------------------------------------------------------
void BlackBoard::setListMovements(QList<Movement*>* listMovements)
{
    this->listMovements = listMovements;
}

void BlackBoard::setListPorts(QList<ClientOSC*> *listPorts)
{
    this->listPorts = listPorts;
}

void BlackBoard::setListSamplesAudio(QList<SampleAudio*> *listSamplesAudio)
{
    this->listSamplesAudio = listSamplesAudio;
}

// -----------------------------------------------------------
// SLOT
// -----------------------------------------------------------
void BlackBoard::refresh()
{
    //  emit clearBlackBoard();
    //MOVEMENT/ELLIPSE
    if(this->listEllipse.isEmpty())
    {
	this->xEllipse = 0;this->yEllipse = DEPLACEMENT_VERTICAL;
	for(int i = 0 ; i < this->listMovements->size() ; i++)
	{
        this->listEllipse.append(new EllipseDuProjet(this->xEllipse, this->yEllipse,50,50, new QColor(100,100,100,255), this->scene(), this->listMovements->at(i)));
	    this->xEllipse += DEPLACEMENT_HORIZONTAL;
	}
    }
    else
    {
	bool isInTheList = false;
	for(int i = 0 ; i < this->listEllipse.size() ; i++)
	    if(this->listEllipse.at(i)->getMovement()->getId() == this->listMovements->last()->getId())
		isInTheList = true;
	if(isInTheList == false)
	{
	   // this->xEllipse = this->listEllipse.last()->pos().x() + DEPLACEMENT_HORIZONTAL;
        this->listEllipse.append(new EllipseDuProjet(this->xEllipse, this->yEllipse,50,50, new QColor(100,100,100,255), this->scene(), this->listMovements->last()));
	    this->xEllipse += DEPLACEMENT_HORIZONTAL;
	}
    }

    //TRIANGLE/SAMPLES AUDIO
    if(this->listTriangle.isEmpty())
    {
	this->xTriangle = 0;this->yTriangle = 2 * DEPLACEMENT_VERTICAL;
	for(int i = 0 ; i < this->listSamplesAudio->size() ; i++)
	{
	    this->listTriangle.append(new Triangle(this->xTriangle, this->yTriangle, new QColor(100,100,100,255), this->scene(), this->listSamplesAudio->at(i)));
	    this->xTriangle += DEPLACEMENT_HORIZONTAL;
	}
    }
    else
    {
	bool isInTheList = false;
	for(int i = 0 ; i < this->listTriangle.size() ; i++)
	    if(this->listTriangle.at(i)->getSampleAudio()->getId() == this->listSamplesAudio->last()->getId())
		isInTheList = true;
	if(isInTheList == false)
	{
	   // this->xTriangle = this->listTriangle.last()->pos().x() + DEPLACEMENT_HORIZONTAL;
	    this->listTriangle.append(new Triangle(this->xTriangle, this->yTriangle, new QColor(100,100,100,255), this->scene(), this->listSamplesAudio->last()));
	    this->xTriangle += DEPLACEMENT_HORIZONTAL;
	}
    }

    //DIAMOND/PORTS
    if(this->listDiamond.isEmpty())

    {
	this->xDiamond = 0;this->yDiamond = 0;
	for(int i = 0 ; i < this->listPorts->size() ; i++)
	{
	    this->listDiamond.append(new Diamond(this->xDiamond, this->yDiamond, new QColor(100,100,100,255), this->scene(), this->listPorts->at(i)));
	    this->xDiamond += DEPLACEMENT_HORIZONTAL;
	}
    }
    else
    {
	bool isInTheList = false;
	for(int i = 0 ; i < this->listDiamond.size() ; i++)
	    if(this->listDiamond.at(i)->getPort()->getId() == this->listPorts->last()->getId())
		isInTheList = true;
	if(isInTheList == false)
	{
	    //this->xDiamond = this->listDiamond.last()->pos().x() + DEPLACEMENT_HORIZONTAL;
	    this->listDiamond.append(new Diamond(this->xDiamond, this->yDiamond, new QColor(100,100,100,255), this->scene(), this->listPorts->last()));
	    this->xDiamond += DEPLACEMENT_HORIZONTAL;
	}
    }

    //Suppression de toutes les lignes sur le blackboard
    for(int i = 0 ; i< this->scene()->items().size() ; i++)
	for(int j = 0 ; j < this->listLines.size() ; j++)
	    if(this->scene()->items().at(i)->type() == this->listLines.at(j)->type())
		this->scene()->removeItem(this->scene()->items().at(i));
    this->listLines.clear();

    //LIGNE ENTRE LES ELEMENTS
    for(int i = 0 ; i < this->listEllipse.size() ; i++)
    {
	//LIGNE MOVMEMENT - SAMPLE AUDIO
	if(this->listEllipse.at(i)->getMovement()->getSampleAudio() != NULL)
	{
	    for(int j = 0 ; j < this->listTriangle.size() ; j++)
	    {
		if(this->listEllipse.at(i)->getMovement()->getSampleAudio()->getId() == this->listTriangle.at(j)->getSampleAudio()->getId())
		{
		    this->addLineItem(this->listEllipse.at(i)->rect().x() + 25, this->listEllipse.at(i)->rect().y() + 50, this->listTriangle.at(j)->x() + 25, this->listTriangle.at(j)->y());
		    this->scene()->addItem(this->listLines.last());
		    qDebug()<< "LALA : " << listEllipse.at(i)->getMovement()->getName() << " - " << this->listTriangle.at(j)->getSampleAudio()->getName() << endl;
		}
	    }
	}
	// LIGNE MOVEMENT - PORTS
	if(this->listEllipse.at(i)->getMovement()->getListClients()->size() != 0)
	    for(int j = 0 ; j < this->listDiamond.size() ; j++)
		for(int k =0; k< this->listEllipse.at(i)->getMovement()->getListClients()->size();k++)
		    if(this->listEllipse.at(i)->getMovement()->getListClients()->at(k)->getId() == this->listDiamond.at(j)->getPort()->getId())
		    {
			qDebug()<< "CACA"<< this->listEllipse.at(i)->getMovement()->getName() << " - " << this->listDiamond.at(j)->getPort()->getName() << endl;
			this->addLineItem(this->listEllipse.at(i)->rect().x() + 25, this->listEllipse.at(i)->rect().y(), this->listDiamond.at(j)->x() + 25, this->listDiamond.at(j)->y() + 50);
			this->scene()->addItem(this->listLines.last());

		    }
    }

}

void BlackBoard::addLineItem(const quint16 x1, const quint16 y1, const quint16 x2, const quint16 y2)
{
    QLine line(x1, y1, x2, y2);
    QPen pen;
    pen.setBrush(Qt::white);
    pen.setWidth(5);
    this->listLines.append(new QGraphicsLineItem(line));
    this->listLines.last()->setPen(pen);
}

