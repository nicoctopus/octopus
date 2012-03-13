#include "blackboard.h"
#include "QDebug"


// -----------------------------------------------------------
//                     CONSTRUCTEUR
// -----------------------------------------------------------

BlackBoard::BlackBoard(QWidget *parent): QGraphicsView(parent), timerId(0)
{
    this->setAcceptDrops(true);
    this->movement = NULL;
    //isFirstPassage = true;
    QGraphicsScene *scene = new QGraphicsScene();
    this->setScene(scene);
    //this->setSceneRect(0,0,500,500);
    ctrlPressed = false;
    connect(this, SIGNAL(refreshSignal()), this, SLOT(refresh()));
    connect(this, SIGNAL(clearScene()), this->scene(), SLOT(clear()));
    connect(this->scene(), SIGNAL(selectionChanged()), this, SLOT(liaison()));
    //Menu clique droit
    this->createActions();
    //this->setSceneRect(0,0,1000,1000);
    //this->SetCenter(QPointF(250.0,250.0));//fontion modifiée de centerOn()

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
            //this->listMovements->last()->setPosXBlackBoard(mapToScene(event->pos().x(),0).x());
            //this->listMovements->last()->setPosYBlackBoard(mapToScene(0,event->pos().y()).y());
            //qDebug() << "maptoscene" << mapToScene(event->pos().x(),0).x() <<"," << mapToScene(0,event->pos().y()).y();
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
            //this->listSamplesAudio->last()->setPosXBlackBoard(mapToScene(event->pos().x(),0).x());
            //this->listSamplesAudio->last()->setPosYBlackBoard(mapToScene(0,event->pos().y()).y());
           // qDebug() << "maptoscene" << mapToScene(event->pos().x(),0).x() <<"," << mapToScene(0,event->pos().y()).y();
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
           // this->listPorts->last()->setPosXBlackBoard(mapToScene(event->pos().x(),0).x());
           // this->listPorts->last()->setPosYBlackBoard(mapToScene(0,event->pos().y()).y());
	    emit save(this->listPorts->last());
	    emit refreshSignal();
	}
    }
   // this->setSceneRect(this->scene()->sceneRect());
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
    qDebug() << "REFRESH" << endl;
    this->scene()->selectedItems().clear();
    for(int i = this->scene()->items().size() - 1 ; i >= 0 ; i--)
	this->scene()->removeItem(this->scene()->items().at(i));

    //On clear les listes et on met à NULL les pointeurs au préalable.
   // for(int i = 0 ; i < this->listEllipse.size() ; i++)
	//delete this->listEllipse.at(i);
    this->listEllipse.clear();
    //for(int i = 0 ; i < this->listDiamond.size() ; i++)
	//delete this->listDiamond.at(i);
    this->listDiamond.clear();
   // for(int i = 0 ; i < this->listTriangle.size() ; i++)
	//delete this->listTriangle.at(i);
    this->listTriangle.clear();
   // for(int i = 0 ; i < this->listLines.size() ; i++)
	//delete this->listLines.at(i);
    this->listLines.clear();


    //MOVEMENT/ELLIPSE
    if(this->listEllipse.isEmpty())
    {
	this->xEllipse = 0;this->yEllipse = DEPLACEMENT_VERTICAL;
	for(int i = 0 ; i < this->listMovements->size() ; i++)
	{
            //this->listEllipse.append(new EllipseDuProjet(this->xEllipse, this->yEllipse,50,50, new QColor(100,100,100,255), this->scene(), this->listMovements->at(i),this));
            //this->xEllipse += DEPLACEMENT_HORIZONTAL;
            //qDebug() << "Refresh Ellipse x:" << this->listMovements->at(i)->getPosXBlackBoard() << "y:" << this->listMovements->at(i)->getPosYBlackBoard() << endl;
            this->listEllipse.append(new EllipseDuProjet(listMovements->at(i)->getPosXBlackBoard(),listMovements->at(i)->getPosYBlackBoard(),50,50, new QColor(100,100,100,255), this->scene(), this->listMovements->at(i),this));
	}
    }

    //TRIANGLE/SAMPLES AUDIO
    if(this->listTriangle.isEmpty())
    {
	this->xTriangle = 0;this->yTriangle = 2 * DEPLACEMENT_VERTICAL;
	for(int i = 0 ; i < this->listSamplesAudio->size() ; i++)
	{
            this->listTriangle.append(new Triangle(this->listSamplesAudio->at(i)->getPosXBlackBoard(),this->listSamplesAudio->at(i)->getPosYBlackBoard(), new QColor(100,100,100,255), this->scene(), this->listSamplesAudio->at(i),this));
          //qDebug() << "Refresh Triangle x:" << this->listSamplesAudio->at(i)->getPosXBlackBoard() << "y:" << this->listSamplesAudio->at(i)->getPosYBlackBoard() << endl;

            // this->xTriangle += DEPLACEMENT_HORIZONTAL;
	}
    }

    //DIAMOND/PORTS
    if(this->listDiamond.isEmpty())
    {
	this->xDiamond = 0;this->yDiamond = 0;
	for(int i = 0 ; i < this->listPorts->size() ; i++)
	{
            this->listDiamond.append(new Diamond(this->listPorts->at(i)->getPosXBlackBoard(), this->listPorts->at(i)->getPosYBlackBoard(), new QColor(100,100,100,255), this->scene(), this->listPorts->at(i),this));
            //this->xDiamond += DEPLACEMENT_HORIZONTAL;
	}
    }

    //LIGNE ENTRE LES ELEMENTS
    for(int i = 0 ; i < this->listEllipse.size() ; i++)
    {
        //qDebug() << "taille listEllipse : " << this->listEllipse.size() << endl;
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
                   //qDebug() << "x ellipse: " << this->listEllipse.at(i)->x() << endl;
                   //qDebug() << "y ellipse: " << this->listEllipse.at(i)->y() << endl;
                   // qDebug() << "x triangle: " << this->listTriangle.at(j)->x() << endl;
                   // qDebug() << "y triangle: " << this->listTriangle.at(j)->y() << endl;


            this->scene()->addItem(this->listLines.last());
            this->listEllipse.at(i)->setListLines(this->listLines.last());
            this->listTriangle.at(j)->setListLines(this->listLines.last());
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
            this->listEllipse.at(i)->setListLines(this->listLines.last());
            this->listDiamond.at(j)->setListLines(this->listLines.last());
		    }
    }
}

void BlackBoard::addLineItem(const qint16 x1, const qint16 y1, const qint16 x2, const qint16 y2)
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
        /*if(qpointf.x() >= this->listEllipse.at(i)->rect().x()
		&& qpointf.x() <= this->listEllipse.at(i)->rect().x() + this->listEllipse.at(i)->rect().width()
		&& qpointf.y() >= this->listEllipse.at(i)->rect().y()
		&& qpointf.y() <= this->listEllipse.at(i)->rect().y() + this->listEllipse.at(i)->rect().height())
        {*/
        if(qpointf.x() >= this->listEllipse.at(i)->x()
                && qpointf.x() <= this->listEllipse.at(i)->x() + this->listEllipse.at(i)->rect().width()
                && qpointf.y() >= this->listEllipse.at(i)->y()
                && qpointf.y() <= this->listEllipse.at(i)->y() + this->listEllipse.at(i)->rect().height())
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
    this->actionLier = new QAction(tr("&Lier/Délier"), this);
    this->actionLier->setStatusTip(tr("lier ou délier le mouvement avec un sample ou un port"));
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
    qDebug()<< "slotLiaison" << endl;
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
qDebug()<< "laison" << endl;
    if(this->movement)
    {

        Movement *movementTemp = this->movement;
	this->movement = NULL;
        if(this->scene()->selectedItems().at(0)->type() == 65539)
        //if(itemPressed->type() == 65539)
        {
            Triangle *triangle = (Triangle*)(this->scene()->selectedItems().at(0));
	    this->updateSampleAudioOfMovement(movementTemp, triangle->getSampleAudio());
	}
        else if(this->scene()->selectedItems().at(0)->type() == 65538)
        //else if(itemPressed->type() == 65538)
        {
            Diamond *diamond = (Diamond*)(this->scene()->selectedItems().at(0));
	    this->updateClientOSCOfMovement(movementTemp, diamond->getPort());
	}
    }

}


// -----------------------------------------------------------
//               Faire Bouger les lignes/items
// -----------------------------------------------------------
// On appelle la méthode itemMoved quand on déplace un item (voir classes des items)
// Cette méthode lance un timer qui fait bouger les lignes entres les items
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
         //qDebug() << "triangle" << endl;
        Triangle *triangle = (Triangle*)(this->movingItem);
        listLines = triangle->getListLines();
    }
    else if(this->movingItem->type() == 65538)
    {
 //qDebug() << "diamond" << endl;
        Diamond *diamond = (Diamond*)(this->movingItem);
        listLines = diamond->getListLines();
    }
    else if(this->movingItem->type() == 65537)
    {
        // qDebug() << "ellipse" << endl;
        EllipseDuProjet *ellipse = (EllipseDuProjet*)(this->movingItem);
        listLines = ellipse->getListLines();

    }


     //qDebug() << "taille listLines" << listLines.size() <<endl;
    for (int i=0; i<listLines.size(); i++){
       /* qDebug() << "--------------------------" <<endl;
        qDebug() << "line x1" << listLines.at(i)->line().x1() <<endl;
        qDebug() << "line y1" << listLines.at(i)->line().y1() <<endl;
        qDebug() << "line x2" << listLines.at(i)->line().x2() <<endl;
        qDebug() << "line y2" << listLines.at(i)->line().y2() <<endl;
        qDebug() << "item x" << movingItem->pos().x() + 25 <<endl;
        qDebug() << "item y" << movingItem->pos().y() + 25 <<endl;
        qDebug() << "last X" << lastPosX <<endl;
        qDebug() << "last Y" << lastPosY <<endl;
        qDebug() << "--------------------------" <<endl;
*/


        if((listLines.at(i)->line().x1() == lastPosX ) && (listLines.at(i)->line().y1() == lastPosY))
        {
            listLines.at(i)->setLine(movingItem->pos().x() + 25, movingItem->pos().y() + 25, listLines.at(i)->line().x2(), listLines.at(i)->line().y2());
        }
        else if((listLines.at(i)->line().x2() == lastPosX) && (listLines.at(i)->line().y2() == lastPosY))
        {
            listLines.at(i)->setLine(listLines.at(i)->line().x1(), listLines.at(i)->line().y1(), movingItem->pos().x() + 25, movingItem->pos().y() + 25);
        }

    }
    //qDebug("TIMER");
    //QLine line(0, 0, (movingItem->x())+25, (movingItem->y())+25);
    //QGraphicsLineItem *lineItem = new QGraphicsLineItem(line);
    //this->scene()->addItem(lineItem);
    lastPosX = movingItem->pos().x() + 25;
    lastPosY = movingItem->pos().y() + 25;
    killTimer(timerId);
    timerId = 0;

}

void BlackBoard::setLastX(int xItem){
    this->lastPosX = xItem + 25;
}

void BlackBoard::setLastY(int yItem){
    this->lastPosY = yItem + 25;
}

//------------------------------------------------------------
//                      ZOOM / DEZOOM
//------------------------------------------------------------

/**
  * Sets the current centerpoint.  Also updates the scene's center point.
  * Unlike centerOn, which has no way of getting the floating point center
  * back, SetCenter() stores the center point.  It also handles the special
  * sidebar case.  This function will claim the centerPoint to sceneRec ie.
  * the centerPoint must be within the sceneRec.
  */
//Set the current centerpoint in the
void BlackBoard::SetCenter(const QPointF& centerPoint) {
    //Get the rectangle of the visible area in scene coords
    QRectF visibleArea = mapToScene(rect()).boundingRect();

    //Get the scene area
    QRectF sceneBounds = sceneRect();

    double boundX = visibleArea.width() / 2.0;
    double boundY = visibleArea.height() / 2.0;
    double boundWidth = sceneBounds.width() - 2.0 * boundX;
    double boundHeight = sceneBounds.height() - 2.0 * boundY;

    //The max boundary that the centerPoint can be to
    QRectF bounds(boundX, boundY, boundWidth, boundHeight);

    if(bounds.contains(centerPoint)) {
        //We are within the bounds
        CurrentCenterPoint = centerPoint;
    } else {
        //We need to clamp or use the center of the screen
        if(visibleArea.contains(sceneBounds)) {
            //Use the center of scene ie. we can see the whole scene
            CurrentCenterPoint = sceneBounds.center();
        } else {

            CurrentCenterPoint = centerPoint;

            //We need to clamp the center. The centerPoint is too large
            if(centerPoint.x() > bounds.x() + bounds.width()) {
                CurrentCenterPoint.setX(bounds.x() + bounds.width());
            } else if(centerPoint.x() < bounds.x()) {
                CurrentCenterPoint.setX(bounds.x());
            }

            if(centerPoint.y() > bounds.y() + bounds.height()) {
                CurrentCenterPoint.setY(bounds.y() + bounds.height());
            } else if(centerPoint.y() < bounds.y()) {
                CurrentCenterPoint.setY(bounds.y());
            }

        }
    }

    //Update the scrollbars
    centerOn(CurrentCenterPoint);
}

/**
  * Zoom the view in and out.
  *
void BlackBoard::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case  Qt::Key_Control:
        ctrlPressed = true;
    default:
        QGraphicsView::keyPressEvent(event);
    }
}
void BlackBoard::keyReleaseEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case  Qt::Key_Control:
        ctrlPressed = false;
    default:
        QGraphicsView::keyPressEvent(event);
    }
}

void BlackBoard::wheelEvent(QWheelEvent* event) {

    if(ctrlPressed == true){
        //Get the position of the mouse before scaling, in scene coords
    QPointF pointBeforeScale(mapToScene(event->pos()));

    //Get the original screen centerpoint
    QPointF screenCenter = GetCenter(); //CurrentCenterPoint; //(visRect.center());

    //Scale the view ie. do the zoom
    double scaleFactor = 1.15; //How fast we zoom
    if(event->delta() > 0) {
        //Zoom in
        scale(scaleFactor, scaleFactor);
    } else {
        //Zooming out
        scale(1.0 / scaleFactor, 1.0 / scaleFactor);
    }

    //Get the position after scaling, in scene coords
    QPointF pointAfterScale(mapToScene(event->pos()));

    //Get the offset of how the screen moved
    QPointF offset = pointBeforeScale - pointAfterScale;

    //Adjust to the new center for correct zooming
    QPointF newCenter = screenCenter + offset;
    SetCenter(newCenter);
    }
}

  **
  * Need to update the center so there is no jolt in the
  * interaction after resizing the widget.
  *
void BlackBoard::resizeEvent(QResizeEvent* event) {
    //Get the rectangle of the visible area in scene coords
    QRectF visibleArea = mapToScene(rect()).boundingRect();
    SetCenter(visibleArea.center());

    //Call the subclass resize so the scrollbars are updated correctly
    QGraphicsView::resizeEvent(event);
}
*/
