#include "blackboard.h"
#include "QDebug"


// -----------------------------------------------------------
//                     CONSTRUCTEUR
// -----------------------------------------------------------

BlackBoard::BlackBoard(QWidget *parent): QGraphicsView(parent), timerId(0)
{
    this->setAcceptDrops(true);
    this->movement = NULL;
    QGraphicsScene *scene = new QGraphicsScene();
    this->setScene(scene);
    connect(this, SIGNAL(refreshSignal()), this, SLOT(refresh()));
    connect(this, SIGNAL(clearScene()), this->scene(), SLOT(clear()));
    connect(this->scene(), SIGNAL(selectionChanged()), this, SLOT(liaison()));
    //Menu clique droit
    this->createActions();

}

// -----------------------------------------------------------
//                      METHODES
// -----------------------------------------------------------


void BlackBoard::updateClientOSCOfMovement(Movement *movement, ClientOSC *clientOSC)
{
    bool ok = false;
    if(!movement->getListClients()->isEmpty())
    {
	for(int i = 0 ; i  < movement->getListClients()->size() ; i++)
	{
	    if(!movement->getListClients()->isEmpty())
		if(movement->getListClients()->at(i)->getId() == clientOSC->getId())
		{
		    clientOSC->removeIdMovement(movement->getId());
		    movement->getListClients()->removeAt(i);
		    emit save(clientOSC);
		    emit save(movement);
		    ok = true;
		}
	}
	if(ok == false)
	{
	    clientOSC->updateIdMovement(movement->getId());
	    movement->addClientOSC(clientOSC);
	    emit save(movement);
	}
    }
    else
    {
	clientOSC->updateIdMovement(movement->getId());
	qDebug() << clientOSC->getName() << endl;
	movement->addClientOSC(clientOSC);
	emit save(movement);
    }
    emit refreshSignal();
}

void BlackBoard::updateSampleAudioOfMovement(Movement *movement, SampleAudio *newSampleAudio)
{
    if(movement->getSampleAudio() != newSampleAudio)
    {
	if(movement->getSampleAudio())
	    movement->getSampleAudio()->removeId(movement->getId());
	movement->setSampleAudio(newSampleAudio);
	movement->getSampleAudio()->updateIdMovement(movement->getId());
	emit save(movement->getSampleAudio());
    }
    else
    {
	movement->getSampleAudio()->removeId(movement->getId());
	emit save(movement->getSampleAudio());
	movement->setSampleAudio(NULL);
    }
    emit save(movement);
    this->refresh();
}

// -----------------------------------------------------------
// R�implementation des m�thodes de QGraphicsView pour assurer le Drag & drop
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
		msgBox.setText("L'�lement est d�j� pr�sent sur le Blackboard");
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
		msgBox.setText("L'�lement est d�j� pr�sent sur le Blackboard");
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
		msgBox.setText("L'�lement est d�j� pr�sent sur le Blackboard");
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

    /* QList<QGraphicsItem*> listItemsToDelete = this->scene()->items();
    int nbItemsToDelete = listItemsToDelete.size();
    for(int i = 0 ; i < nbItemsToDelete ; i++){
 this->scene()->removeItem(listItemsToDelete.at(i));
    }*/
    this->scene()->selectedItems().clear();
    for(int i = this->scene()->items().size() - 1 ; i >= 0 ; i--)
	this->scene()->removeItem(this->scene()->items().at(i));
    this->listEllipse.clear();
    this->listDiamond.clear();
    this->listTriangle.clear();
    this->listLines.clear();


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
	    this->listDiamond.append(new Diamond(this->xDiamond, this->yDiamond, new QColor(100,100,100,255), this->scene(), this->listPorts->last()));
	    this->xDiamond += DEPLACEMENT_HORIZONTAL;
	}
    }

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
		    //qDebug() << "id sample : " << this->listEllipse.at(i)->getMovement()->getSampleAudio()->getId() << endl;
		    //qDebug() << "id mvt : " << this->listEllipse.at(i)->getMovement()->getId() << endl;
            //this->addLineItem(this->listEllipse.at(i)->rect().x() + 25, this->listEllipse.at(i)->rect().y() + 50, this->listTriangle.at(j)->x() + 25, this->listTriangle.at(j)->y());
            this->addLineItem(this->listEllipse.at(i)->x() + 25, this->listEllipse.at(i)->y() + 25, this->listTriangle.at(j)->x() + 25, this->listTriangle.at(j)->y() + 25);

            this->scene()->addItem(this->listLines.last());
            this->listEllipse.at(i)->getListLines().append(this->listLines.last());
            this->listTriangle.at(j)->getListLines().append(this->listLines.last());
		}
	    }
	}

	// LIGNE MOVEMENT - PORTS
	if(this->listEllipse.at(i)->getMovement()->getListClients()->size() != 0)
	    for(int j = 0 ; j < this->listDiamond.size() ; j++)
		for(int k =0; k< this->listEllipse.at(i)->getMovement()->getListClients()->size();k++)
		    if(this->listEllipse.at(i)->getMovement()->getListClients()->at(k)->getId() == this->listDiamond.at(j)->getPort()->getId())
		    {
            //this->addLineItem(this->listEllipse.at(i)->rect().x() + 25, this->listEllipse.at(i)->rect().y(), this->listDiamond.at(j)->x() + 25, this->listDiamond.at(j)->y() + 50);
            this->addLineItem(this->listEllipse.at(i)->x() + 25, this->listEllipse.at(i)->y() + 25, this->listDiamond.at(j)->x() + 25, this->listDiamond.at(j)->y() + 25);
            this->scene()->addItem(this->listLines.last());
            this->listEllipse.at(i)->getListLines().append(this->listLines.last());
            this->listDiamond.at(j)->getListLines().append(this->listLines.last());
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

void BlackBoard::contextMenuEvent(QContextMenuEvent *event)
{
    QPointF qpointf = this->mapToScene((event->pos()));
    for(int i = 0 ; i < this->listEllipse.size() ; i++)
    {
	if(qpointf.x() >= this->listEllipse.at(i)->rect().x()
		&& qpointf.x() <= this->listEllipse.at(i)->rect().x() + this->listEllipse.at(i)->rect().width()
		&& qpointf.y() >= this->listEllipse.at(i)->rect().y()
		&& qpointf.y() <= this->listEllipse.at(i)->rect().y() + this->listEllipse.at(i)->rect().height())
	{
	    this->listEllipse.at(i)->setContextMenu(true);
	    QMenu menu(this);
	    menu.addAction(this->actionRemove);
	    menu.addAction(this->actionVisualisation);
	    menu.addAction(this->actionLier);
	    menu.addAction(this->actionEnleverBlackboard);
	    menu.exec(event->globalPos());
	    return;
	}
    }
    for(int i = 0 ; i < this->listDiamond.size() ; i++)
    {
	if(qpointf.x() >= this->listDiamond.at(i)->pos().x()
		&& qpointf.x() <= this->listDiamond.at(i)->pos().x() + 50
		&& qpointf.y() >= this->listDiamond.at(i)->pos().y()
		&& qpointf.y() <= this->listDiamond.at(i)->pos().y() + 50)
	{
	    this->listDiamond.at(i)->setContextMenu(true);
	    QMenu menu(this);
	    menu.addAction(this->actionRemove);
	    menu.addAction(this->actionEnleverBlackboard);
	    menu.exec(event->globalPos());
	    return;
	}
    }
    for(int i = 0 ; i < this->listTriangle.size() ; i++)
    {
	QPointF qpointf = this->mapToScene((event->pos()));
	if(qpointf.x() >= this->listTriangle.at(i)->pos().x()
		&& qpointf.x() <= this->listTriangle.at(i)->pos().x() + 50
		&& qpointf.y() >= this->listTriangle.at(i)->pos().y()
		&& qpointf.y() <= this->listTriangle.at(i)->pos().y() + 50)
	{
	    this->listTriangle.at(i)->setContextMenu(true);
	    QMenu menu(this);
	    menu.addAction(this->actionRemove);
	    menu.addAction(this->actionEnleverBlackboard);
	    menu.exec(event->globalPos());
	    return;
	}
    }

}

void BlackBoard::createActions()
{
    this->actionRemove = new QAction(tr("&Remove"), this);
    this->actionRemove->setShortcut(tr("Ctrl+R"));
    this->actionRemove->setStatusTip(tr("Remove the object"));
    connect(this->actionRemove, SIGNAL(triggered()), this, SLOT(slotRemove()));
    this->actionVisualisation = new QAction(tr("&Visualiser"), this);
    this->actionVisualisation->setStatusTip(tr("Visualiser le mouvement"));
    connect(this->actionVisualisation, SIGNAL(triggered()), this, SLOT(slotVisualisation()));
    this->actionLier = new QAction(tr("&Lier/D�lier"), this);
    this->actionLier->setStatusTip(tr("lier ou d�lier le mouvement avec un sample ou un port"));
    connect(this->actionLier, SIGNAL(triggered()), this, SLOT(slotLiaison()));
    this->actionEnleverBlackboard = new QAction(tr("&Enlever diu blackboard"), this);
    this->actionEnleverBlackboard->setStatusTip(tr("Enlever la forme du blackboard"));
    connect(this->actionEnleverBlackboard, SIGNAL(triggered()), this, SLOT(slotEnleverBlackboard()));
}

void BlackBoard::slotRemove()
{
    for(int i = 0 ; i < this->listEllipse.size() ; i++)
	if(this->listEllipse.at(i)->getContextMenu() == true)
	{
	    emit remove(this->listEllipse.at(i)->getMovement());
	    return;
	}
    for(int i = 0 ; i < this->listDiamond.size() ; i++)
	if(this->listDiamond.at(i)->getContextMenu() == true)
	{
	    emit remove(this->listDiamond.at(i)->getPort());
	    return;
	}
    for(int i = 0 ; i < this->listTriangle.size() ; i++)
	if(this->listTriangle.at(i)->getContextMenu() == true)
	{
	    emit remove(this->listTriangle.at(i)->getSampleAudio());
	    return;
	}
}

void BlackBoard::slotVisualisation()
{
    for(int i = 0 ;  i < this->listEllipse.size() ; i++)
	if(this->listEllipse.at(i)->getContextMenu() == true)
	{
	    emit visualisation(this->listEllipse.at(i)->getMovement());
	    this->listEllipse.at(i)->setContextMenu(false);
	    return;
	}
}

void BlackBoard::slotLiaison()
{
    for(int i = 0 ;  i < this->listEllipse.size() ; i++)
	if(this->listEllipse.at(i)->getContextMenu() == true)
	{
	    this->movement = this->listEllipse.at(i)->getMovement();
	}
}

void BlackBoard::slotEnleverBlackboard()
{
    for(int i = 0 ; i < this->listEllipse.size() ; i++)
	if(this->listEllipse.at(i)->getContextMenu() == true)
	{
	    Movement *movement = this->listEllipse.at(i)->getMovement();
	    if(movement->getSampleAudio())
	    {
		movement->getSampleAudio()->removeId(movement->getId());
		emit save (movement->getSampleAudio());
	    }
	    movement->setSampleAudio(NULL);
	    if(!movement->getListClients()->isEmpty())
		for(int i = 0 ; i < movement->getListClients()->size() ; i++)
		{
		    movement->getListClients()->at(i)->removeIdMovement(movement->getId());
		    emit save(movement->getListClients()->at(i));
		}
	    movement->getListClients()->clear();
	    for(int i = 0 ; i < this->listMovements->size() ; i++)
		if(this->listMovements->at(i)->getId() == movement->getId())
		    this->listMovements->removeAt(i);
	    movement->setActive(false);
	    emit save(movement);
	    emit refreshSignal();
	    return;
	}
    for(int i = 0 ; i < this->listDiamond.size() ; i++)
	if(this->listDiamond.at(i)->getContextMenu() == true)
	{
	    ClientOSC *clientOSC = listDiamond.at(i)->getPort();
	    clientOSC->getListIdMovement()->clear();
	    for(int i = 0 ; i < this->listMovements->size() ; i++)
	    {
		for(int j = 0 ; j < this->listMovements->at(i)->getListClients()->size() ; j++)
		    if(this->listMovements->at(i)->getListClients()->at(j)->getId() == clientOSC->getId())
			this->listMovements->at(i)->getListClients()->removeAt(j);
		emit save(this->listMovements->at(i));
	    }
	    clientOSC->setActive(false);
	    for(int i = 0 ; i < this->listPorts->size() ; i++)
		if(this->listPorts->at(i)->getId() == clientOSC->getId())
		    this->listPorts->removeAt(i);
	    emit save(clientOSC);
	    emit refreshSignal();
	    return;
	}
    for(int i = 0 ; i < this->listTriangle.size() ; i++)
	if(this->listTriangle.at(i)->getContextMenu() == true)
	{
	    SampleAudio *sampleAudio = this->listTriangle.at(i)->getSampleAudio();
	    sampleAudio->getListIdMovement()->clear();
	    for(int i = 0 ; i < this->listMovements->size() ; i++)
	    {
		if(this->listMovements->at(i)->getSampleAudio())
		    if(this->listMovements->at(i)->getSampleAudio()->getId() == sampleAudio->getId())
		    {
			this->listMovements->at(i)->setSampleAudio(NULL);
			emit save(this->listMovements->at(i));
		    }
	    }
	    for(int i = 0 ; i < this->listSamplesAudio->size() ; i++)
		if(this->listSamplesAudio->at(i)->getId() == sampleAudio->getId())
		    this->listSamplesAudio->removeAt(i);
	    sampleAudio->setActive(false);
	    emit save(sampleAudio);
	    emit refreshSignal();
	    return;
	}
}

void BlackBoard::liaison()
{
    if(this->movement)
    {
	Movement *movementTemp = this->movement;
	this->movement = NULL;
	if(this->scene()->selectedItems().at(0)->type() == 65539)
	{
	    Triangle *triangle = (Triangle*)(this->scene()->selectedItems().at(0));
	    this->updateSampleAudioOfMovement(movementTemp, triangle->getSampleAudio());
	}
	else if(this->scene()->selectedItems().at(0)->type() == 65538)
	{
	    Diamond *diamond = (Diamond*)(this->scene()->selectedItems().at(0));
	    this->updateClientOSCOfMovement(movementTemp, diamond->getPort());
	}
    }

}

// -----------------------------------------------------------
//               Faire Bouger les lignes/items
// -----------------------------------------------------------
// On appelle la m�thode itemMoved quand on d�place un item (voir classes des items)
// Cette m�thode lance un timer qui fait bouger les lignes entres les items
void BlackBoard::itemMoved(QGraphicsItem* pMovingItem)
 {
     if (!timerId){
         this->movingItem = pMovingItem;
         timerId = startTimer(1000 / 25);
     }
}

void BlackBoard::timerEvent(QTimerEvent *event)
{
    Q_UNUSED(event);
    QList<QGraphicsLineItem*> listLines;
    if(this->movingItem->type() == 65539)
    {
        Triangle *triangle = (Triangle*)(this->movingItem);
        listLines = triangle->getListLines();
    }
    else if(this->movingItem->type() == 65538)
    {
        Diamond *diamond = (Diamond*)(this->movingItem);
        listLines = diamond->getListLines();
    }
    else if(this->movingItem->type() == 65537)
    {
        EllipseDuProjet *ellipse = (EllipseDuProjet*)(this->movingItem);
        listLines = ellipse->getListLines();
    }

    for (int i=0; i<listLines.size(); i++){
        if((listLines.at(i)->line().x1() == movingItem->x() + 25) && (listLines.at(i)->line().y1() == movingItem->y() + 25))
        {
            listLines.at(i)->setLine(movingItem->x() + 25, movingItem->y() + 25, listLines.at(i)->line.x2(), listLines.at(i)->line.y2());
        }
        else if((listLines.at(i)->line().x2() == movingItem->x() + 25) && (listLines.at(i)->line().y2() == movingItem->y() + 25))
        {
            listLines.at(i)->setLine(listLines.at(i)->line.x1(), listLines.at(i)->line.y1(), movingItem->x() + 25, movingItem->y() + 25);
        }

    }
    //qDebug("TIMER");
    //QLine line(0, 0, (movingItem->x())+25, (movingItem->y())+25);
    //QGraphicsLineItem *lineItem = new QGraphicsLineItem(line);
    //this->scene()->addItem(lineItem);
    killTimer(timerId);
    timerId = 0;

}

