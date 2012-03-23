#include "stickman.h"
#include <QDebug>
#include <QTimer>



qint32 WIDTH_JOINTS = 10;
qint32 HEIGHT_JOINTS = 10;
//-----------------------------------------------------------
//                     CONSTRUCTEUR
//-----------------------------------------------------------

StickMan::StickMan(QWidget *parent) : QGraphicsView (parent)
{
    RESIZE = 2.5;

    QGraphicsScene *scene = new QGraphicsScene();
    this->setScene(scene);
    this->createStickMan();
    this->isStickManLive = false;
    QRectF sizeScene = this->scene()->sceneRect();
    this->ensureVisible(sizeScene,500,500);

    this->timer = new QTimer();
    connect(timer, SIGNAL(timeout()), this, SLOT(slotTimeOutTimer()));

    connect(this, SIGNAL(clearStickMan()), this->scene(), SLOT(clear()));
}

void StickMan::setStickManLive(bool isStickManLive){
    this->isStickManLive = isStickManLive;
}

void StickMan::launchTimerForDetection(){
    this->timer->start(3000);
    this->textDetected->setText("ARE YOU DETECTED?");
}

void StickMan::stopTimer(){
    this->timer->stop();
    this->textDetected->setText("");
    this->reCreateStickMan();
}

void StickMan::slotTimeOutTimer(){
    this->reCreateStickMan();
    this->textDetected->setText("NOT DETECTED!");
}

//-----------------------------------------------------------
void StickMan::slotMoveStickMan(Movement* movement){
    //Thread pour faire le faire bouger

    ThreadMoveStickman* threadForStickManMoving = new ThreadMoveStickman(movement);
    connect(threadForStickManMoving, SIGNAL(sigMoveNode(QString,int,int,int)), this, SLOT(slotMoveNode(QString,int,int,int)));
    connect(threadForStickManMoving, SIGNAL(sigEndOfMoveStickman()), this, SLOT(slotEndOfMoveStickman()));
    threadForStickManMoving->start();
}


void StickMan::slotMoveNode(QString nameOfNodeToMove, int x, int y, int z){

    if(this->isStickManLive==true){
	this->timer->start(1000);
	this->textDetected->setText("DETECTED");
        RESIZE = 3;
    }

    for(int i=0;i<nodes.size();++i){
	//qDebug() << nodes.size() << endl;
	//qDebug() << nodes.at(i)->getName() << endl;
	if(nodes.at(i)->getName()==nameOfNodeToMove){

	    // ==== DEPLACMENTS JOINTS =====

	    //On déplace le noeud
	    //qreal newX = (x-(nodes.at(i)->getXOrigin()))/RESIZE;
	    //qreal newY = (y-(nodes.at(i)->getYOrigin()))/RESIZE;
	    //nodes.at(i)->setPos(newX, newY);
	    int modifSize = 0;
	    if(z<0) modifSize = -(z/10);
	    else if(z>0) modifSize = -(z/20);

	    //offset pour la tete
	    if(nodes.at(i)->getName() == "head"){
		x=x-50;
		this->headLastX = x;
		this->headLastY = y+175;
		this->headLastZwithModifSize = modifSize;
	    }
	    if(nodes.at(i)->getName() == "torso"){
		x=headLastX;
		y=headLastY;
	    }
	    //qDebug(QString(nameOfNodeToMove + " z:"+QString::number(z) + "  modifSize= " + QString::number(modifSize)).toAscii());
	    //nodes.at(i)->setRect(x/RESIZE-((WIDTH_JOINTS+modifSize)/2), y/RESIZE-((HEIGHT_JOINTS+modifSize)/2) , WIDTH_JOINTS+modifSize, HEIGHT_JOINTS+modifSize);

	    // ==== DEPLACMENTS LIGNES =====

	    //On récupere la liste des lines liée au noeud que l'on déplace
	    QList<MyQLine*> listOfLinesLinkedToThisNode = linesLinkedAtThisNode.value(nodes.at(i)->getName());
	    //On parcours cette liste pour déplacer les lignes
	    for(int j=0;j<listOfLinesLinkedToThisNode.size();++j)
	    {

		MyQLine* temp = listOfLinesLinkedToThisNode.at(j);
		//Si le nom la ligne a une extremité qui a bougé on déplace le point A ou B en fct
		if((temp->getNamePointA()) == (nodes.at(i)->getName()))
		{
		    //qDebug("TEST A");
		    if(temp->getNamePointB()=="epauleGauche"){
			temp->setLastXb(epauleGaucheX);
			temp->setLastYb(epauleGaucheY);
		    }
		    else if(temp->getNamePointB()=="epauleDroite"){
			temp->setLastXb(epauleDroiteX);
			temp->setLastYb(epauleDroiteY);
		    }
		    else if(temp->getNamePointB()=="hancheGauche"){
			temp->setLastXb(hancheGaucheX);
			temp->setLastYb(hancheGaucheY);
		    }
		    else if(temp->getNamePointB()=="hancheDroite"){
			temp->setLastXb(hancheDroiteX);
			temp->setLastYb(hancheDroiteY);
		    }
                    else if(temp->getNamePointB()=="basCou"){
                        temp->setLastXb(epauleDroiteX+50);
                        temp->setLastYb(epauleGaucheY);
                    }

                    QLineF line(x/RESIZE,y/RESIZE,temp->getLastXb()/RESIZE,temp->getLastYb()/RESIZE);
		    //delete temp->getGraphicsLine()->setLine();
		    //temp->getGraphicsLine()->setLine(line);
		   // temp->getGraphicsLine()->line().translate((temp->getLastXb()-x)/RESIZE,(temp->getLastYb()-y)/RESIZE);
		    temp->setLastXa(x);
		    temp->setLastYa(y);

		}
		else if((temp->getNamePointB()) == (nodes.at(i)->getName()))
		{
		    // qDebug("TEST B");
		    if(temp->getNamePointA()=="epauleGauche"){
			temp->setLastXa(epauleGaucheX);
			temp->setLastYa(epauleGaucheY);
		    }
		    else if(temp->getNamePointA()=="epauleDroite"){
			temp->setLastXa(epauleDroiteX);
			temp->setLastYa(epauleDroiteY);
		    }
		    else if(temp->getNamePointA()=="hancheGauche"){
			temp->setLastXa(hancheGaucheX);
			temp->setLastYa(hancheGaucheY);
		    }
		    else if(temp->getNamePointA()=="hancheDroite"){
			temp->setLastXa(hancheDroiteX);
			temp->setLastYa(hancheDroiteY);
		    }
		   // temp->getGraphicsLine()->setLine(temp->getLastXa()/RESIZE,temp->getLastYa()/RESIZE,x/RESIZE,y/RESIZE);
		    //temp->getGraphicsLine()->line().translate((x-temp->getLastXa())/RESIZE,(y-temp->getLastYa())/RESIZE);
		    temp->setLastXb(x);
		    temp->setLastYb(y);
		}
		if(temp->getNamePointA()=="epauleGauche" && temp->getNamePointB() =="epauleDroite"){
		    //temp->getGraphicsLine()->setLine((x+50)/RESIZE,(y-100)/RESIZE,(x-50)/RESIZE,(y-100)/RESIZE);
		    this->epauleGaucheX=x+50;
		    this->epauleGaucheY=y-100;
		    this->epauleDroiteX=x-50;
		    this->epauleDroiteY=y-100;
		}
		if(temp->getNamePointA()=="hancheGauche" && temp->getNamePointB() =="hancheDroite"){
		    //temp->getGraphicsLine()->setLine((x+50)/RESIZE,(y+100)/RESIZE,(x-50)/RESIZE,(y+100)/RESIZE);
		    this->hancheGaucheX=x+50;
		    this->hancheGaucheY=y+100;
		    this->hancheDroiteX=x-50;
		    this->hancheDroiteY=y+100;
                }
	    }
	}
    }
}

void StickMan::slotEndOfMoveStickman(){
    this->reCreateStickMan();
}
//-----------------------------------------------------------

//-----------------------------------------------------------
//                     METHODES
//-----------------------------------------------------------

//Creer le stickMan
void StickMan::createStickMan(){

    textDetected = new QGraphicsSimpleTextItem();
    textDetected->setPos(-100,(mapToScene(0,this->height()).y())-50);
    this->scene()->addItem(textDetected);

    QPoint head(0/RESIZE, -175/RESIZE);
    QPoint epauleDroite(-50/RESIZE, -100/RESIZE);
    QPoint basCou(0/RESIZE, -100/RESIZE);
    QPoint epauleGauche(50/RESIZE, -100/RESIZE);
    QPoint righthand(-175/RESIZE, -170/RESIZE);
    QPoint lefthand(175/RESIZE, -170/RESIZE);
    QPoint rightelbow(-150/RESIZE, -80/RESIZE);
    QPoint torso(0, 0);
    QPoint leftelbow(150/RESIZE, -80/RESIZE);
    QPoint hancheDroite(-50/RESIZE, 100/RESIZE);
    QPoint hancheGauche(50/RESIZE, 100/RESIZE);
    QPoint rightknee(-75/RESIZE, 200/RESIZE);
    QPoint leftknee(75/RESIZE, 200/RESIZE);
    QPoint rightfoot(-75/RESIZE, 300/RESIZE);
    QPoint leftfoot(75/RESIZE, 300/RESIZE);

    coord.insert("head",head);
    coord.insert("torso",torso);
    coord.insert("righthand",righthand);
    coord.insert("lefthand",lefthand);
    coord.insert("rightelbow",rightelbow);
    coord.insert("leftelbow",leftelbow);
    coord.insert("rightknee",rightknee);
    coord.insert("leftknee",leftknee);
    coord.insert("rightfoot",rightfoot);
    coord.insert("leftfoot",leftfoot);

    this->epauleDroiteX = epauleDroite.x()*RESIZE;
    this->epauleDroiteY = epauleDroite.y()*RESIZE;
    this->epauleGaucheX = epauleGauche.x()*RESIZE;
    this->epauleGaucheY = epauleGauche.y()*RESIZE;
    this->hancheDroiteX = hancheDroite.x()*RESIZE;
    this->hancheDroiteY = hancheDroite.y()*RESIZE;
    this->hancheGaucheX = hancheGauche.x()*RESIZE;
    this->hancheGaucheY = hancheGauche.y()*RESIZE;

    MyQLine* cou = new MyQLine(head, basCou);
    cou->setNamePointA("head");
    cou->setXAOrigin(head.x()*RESIZE);
    cou->setYAOrigin(head.y()*RESIZE);
    cou->setNamePointB("basCou");
    cou->setXBOrigin(basCou.x()*RESIZE);
    cou->setYBOrigin(basCou.y()*RESIZE);
    cou->setLastPositionsToOrigins();
    MyQLine* hautCorps = new MyQLine(epauleGauche, epauleDroite);
    hautCorps->setNamePointA("epauleGauche");
    hautCorps->setXAOrigin(epauleGauche.x()*RESIZE);
    hautCorps->setYAOrigin(epauleGauche.y()*RESIZE);
    hautCorps->setNamePointB("epauleDroite");
    hautCorps->setXBOrigin(epauleDroite.x()*RESIZE);
    hautCorps->setYBOrigin(epauleDroite.y()*RESIZE);
    hautCorps->setLastPositionsToOrigins();
    MyQLine* brasDroit = new MyQLine(epauleDroite, rightelbow);
    brasDroit->setNamePointA("rightelbow");
    brasDroit->setXAOrigin(rightelbow.x()*RESIZE);
    brasDroit->setYAOrigin(rightelbow.y()*RESIZE);
    brasDroit->setNamePointB("epauleDroite");
    brasDroit->setXBOrigin(epauleDroite.x()*RESIZE);
    brasDroit->setYBOrigin(epauleDroite.y()*RESIZE);
    brasDroit->setLastPositionsToOrigins();
    MyQLine* avantBrasDroit = new MyQLine(rightelbow, righthand);
    avantBrasDroit->setNamePointA("rightelbow");
    avantBrasDroit->setXAOrigin(rightelbow.x()*RESIZE);
    avantBrasDroit->setYAOrigin(rightelbow.y()*RESIZE);
    avantBrasDroit->setNamePointB("righthand");
    avantBrasDroit->setXBOrigin(righthand.x()*RESIZE);
    avantBrasDroit->setYBOrigin(righthand.y()*RESIZE);
    avantBrasDroit->setLastPositionsToOrigins();
    MyQLine* brasGauche = new MyQLine(epauleGauche,leftelbow);
    brasGauche->setNamePointA("leftelbow");
    brasGauche->setXAOrigin(leftelbow.x()*RESIZE);
    brasGauche->setYAOrigin(leftelbow.y()*RESIZE);
    brasGauche->setNamePointB("epauleGauche");
    brasGauche->setXBOrigin(epauleGauche.x()*RESIZE);
    brasGauche->setYBOrigin(epauleGauche.y()*RESIZE);
    brasGauche->setLastPositionsToOrigins();
    MyQLine* avantBrasGauche = new MyQLine(leftelbow, lefthand);
    avantBrasGauche->setNamePointA("leftelbow");
    avantBrasGauche->setXAOrigin(leftelbow.x()*RESIZE);
    avantBrasGauche->setYAOrigin(leftelbow.y()*RESIZE);
    avantBrasGauche->setNamePointB("lefthand");
    avantBrasGauche->setXBOrigin(lefthand.x()*RESIZE);
    avantBrasGauche->setYBOrigin(lefthand.y()*RESIZE);
    avantBrasGauche->setLastPositionsToOrigins();
    MyQLine* coteGauche = new MyQLine(epauleGauche,torso);
    coteGauche->setNamePointA("torso");
    coteGauche->setXAOrigin(torso.x()*RESIZE);
    coteGauche->setYAOrigin(torso.y()*RESIZE);
    coteGauche->setNamePointB("epauleGauche");
    coteGauche->setXBOrigin(epauleGauche.x()*RESIZE);
    coteGauche->setYBOrigin(epauleGauche.y()*RESIZE);
    coteGauche->setLastPositionsToOrigins();
    MyQLine* coteDroite = new MyQLine(epauleDroite, torso);
    coteDroite->setNamePointA("torso");
    coteDroite->setXAOrigin(torso.x()*RESIZE);
    coteDroite->setYAOrigin(torso.y()*RESIZE);
    coteDroite->setNamePointB("epauleDroite");
    coteDroite->setXBOrigin(epauleDroite.x()*RESIZE);
    coteDroite->setYBOrigin(epauleDroite.y()*RESIZE);
    coteDroite->setLastPositionsToOrigins();
    MyQLine* ventreGauche = new MyQLine(torso, hancheGauche);
    ventreGauche->setNamePointA("torso");
    ventreGauche->setXAOrigin(torso.x()*RESIZE);
    ventreGauche->setYAOrigin(torso.y()*RESIZE);
    ventreGauche->setNamePointB("hancheGauche");
    ventreGauche->setXBOrigin(hancheGauche.x()*RESIZE);
    ventreGauche->setYBOrigin(hancheGauche.y()*RESIZE);
    ventreGauche->setLastPositionsToOrigins();
    MyQLine* ventreDroit = new MyQLine(torso, hancheDroite);
    ventreDroit->setNamePointA("torso");
    ventreDroit->setXAOrigin(torso.x()*RESIZE);
    ventreDroit->setYAOrigin(torso.y()*RESIZE);
    ventreDroit->setNamePointB("hancheDroite");
    ventreDroit->setXBOrigin(hancheDroite.x()*RESIZE);
    ventreDroit->setYBOrigin(hancheDroite.y()*RESIZE);
    ventreDroit->setLastPositionsToOrigins();
    MyQLine* basCorps = new MyQLine(hancheGauche, hancheDroite);
    basCorps->setNamePointA("hancheGauche");
    basCorps->setXAOrigin(hancheGauche.x()*RESIZE);
    basCorps->setYAOrigin(hancheGauche.y()*RESIZE);
    basCorps->setNamePointB("hancheDroite");
    basCorps->setXBOrigin(hancheDroite.x()*RESIZE);
    basCorps->setYBOrigin(hancheDroite.y()*RESIZE);
    basCorps->setLastPositionsToOrigins();
    MyQLine* cuisseGauche = new MyQLine(hancheGauche, leftknee);
    cuisseGauche->setNamePointA("leftknee");
    cuisseGauche->setXAOrigin(leftknee.x()*RESIZE);
    cuisseGauche->setYAOrigin(leftknee.y()*RESIZE);
    cuisseGauche->setNamePointB("hancheGauche");
    cuisseGauche->setXBOrigin(hancheGauche.x()*RESIZE);
    cuisseGauche->setYBOrigin(hancheGauche.y()*RESIZE);
    cuisseGauche->setLastPositionsToOrigins();
    MyQLine* jambeGauche = new MyQLine(leftknee, leftfoot);
    jambeGauche->setNamePointA("leftknee");
    jambeGauche->setXAOrigin(leftknee.x()*RESIZE);
    jambeGauche->setYAOrigin(leftknee.y()*RESIZE);
    jambeGauche->setNamePointB("leftfoot");
    jambeGauche->setXBOrigin(leftfoot.x()*RESIZE);
    jambeGauche->setYBOrigin(leftfoot.y()*RESIZE);
    jambeGauche->setLastPositionsToOrigins();
    MyQLine* cuisseDroite = new MyQLine(hancheDroite, rightknee);
    cuisseDroite->setNamePointA("rightknee");
    cuisseDroite->setXAOrigin(rightknee.x()*RESIZE);
    cuisseDroite->setYAOrigin(rightknee.y()*RESIZE);
    cuisseDroite->setNamePointB("hancheDroite");
    cuisseDroite->setXBOrigin(hancheDroite.x()*RESIZE);
    cuisseDroite->setYBOrigin(hancheDroite.y()*RESIZE);
    cuisseDroite->setLastPositionsToOrigins();
    MyQLine* jambeDroite = new MyQLine(rightknee, rightfoot);
    jambeDroite->setNamePointA("rightknee");
    jambeDroite->setXAOrigin(rightknee.x()*RESIZE);
    jambeDroite->setYAOrigin(rightknee.y()*RESIZE);
    jambeDroite->setNamePointB("rightfoot");
    jambeDroite->setXBOrigin(rightfoot.x()*RESIZE);
    jambeDroite->setYBOrigin(rightfoot.y()*RESIZE);
    jambeDroite->setLastPositionsToOrigins();

    QList<MyQLine*> listLineshead;
    listLineshead.append(cou);
    linesLinkedAtThisNode.insert("head",listLineshead);
    QList<MyQLine*> listLinesrighthand;
    listLinesrighthand.append(avantBrasDroit);
    linesLinkedAtThisNode.insert("righthand",listLinesrighthand);
    QList<MyQLine*> listLinesrightelbow;
    listLinesrightelbow.append(avantBrasDroit);
    listLinesrightelbow.append(brasDroit);
    linesLinkedAtThisNode.insert("rightelbow",listLinesrightelbow);
    QList<MyQLine*> listLineslefthand;
    listLineslefthand.append(avantBrasGauche);
    linesLinkedAtThisNode.insert("lefthand",listLineslefthand);
    QList<MyQLine*> listLinesleftelbow ;
    listLinesleftelbow.append(avantBrasGauche);
    listLinesleftelbow.append(brasGauche);
    linesLinkedAtThisNode.insert("leftelbow",listLinesleftelbow);
    QList<MyQLine*> listLinestorso ;
    listLinestorso.append(coteGauche);
    listLinestorso.append(coteDroite);
    listLinestorso.append(ventreGauche);
    listLinestorso.append(ventreDroit);
    listLinestorso.append(hautCorps);
    listLinestorso.append(basCorps);
    linesLinkedAtThisNode.insert("torso",listLinestorso);
    QList<MyQLine*> listLinesrightfoot;
    listLinesrightfoot.append(jambeDroite);
    linesLinkedAtThisNode.insert("rightfoot",listLinesrightfoot);
    QList<MyQLine*> listLinesrightknee;
    listLinesrightknee.append(cuisseDroite);
    listLinesrightknee.append(jambeDroite);
    linesLinkedAtThisNode.insert("rightknee",listLinesrightknee);
    QList<MyQLine*> listLinesleftfoot;
    listLinesleftfoot.append(jambeGauche);
    linesLinkedAtThisNode.insert("leftfoot",listLinesleftfoot);
    QList<MyQLine*> listLinesleftknee;
    listLinesleftknee.append(cuisseGauche);
    listLinesleftknee.append(jambeGauche);
    linesLinkedAtThisNode.insert("leftknee",listLinesleftknee);

    lines.push_back(cou);
    lines.push_back(hautCorps);
    lines.push_back(brasDroit);
    lines.push_back(avantBrasDroit);
    lines.push_back(brasGauche);
    lines.push_back(avantBrasGauche);
    lines.push_back(coteGauche);
    lines.push_back(coteDroite);
    lines.push_back(ventreGauche);
    lines.push_back(ventreDroit);
    lines.push_back(basCorps);
    lines.push_back(cuisseGauche);
    lines.push_back(jambeGauche);
    lines.push_back(cuisseDroite);
    lines.push_back(jambeDroite);

    QPen pen;
    pen.setBrush(Qt::white);
    pen.setWidth(2);

    //Creer les lines
    for (int j = 0; j < lines.size(); ++j) {
	QGraphicsLineItem *line = new QGraphicsLineItem(*(lines.at(j)));
	lines.at(j)->setGraphicsLine(line);
	line->setPen(pen);
        this->scene()->addItem(line);

    }

    //Creer les noeuds
    QMap<QString, QPoint>::const_iterator it = coord.constBegin();
    while (it != coord.constEnd()) {
	JointGraphic *node = new JointGraphic();
	node->setName(it.key());
	node->setXOrigin((it.value().x())*RESIZE);
	node->setYOrigin((it.value().y())*RESIZE);
	//node->setFlag(QGraphicsItem::ItemIsSelectable);
	node->setRect((it.value().x())-(WIDTH_JOINTS/2), (it.value().y())-(HEIGHT_JOINTS/2) , WIDTH_JOINTS, HEIGHT_JOINTS);
	node->setBrush(QColor(4,138,191,255));
	nodes.push_back(node);
	this->scene()->addItem(node);
	++it;
    }



}

//-----------------------------------------------------------
void StickMan::reCreateStickMan(){
    emit clearStickMan();
    this->nodes.clear();
    this->lines.clear();
    this->createStickMan();
    nodesSelected.clear();
}


//-----------------------------------------------------------
//Méthode appellée par la mainWindow a chaque fois qu'on selectionne qqch sur le stickman, elle mets à jour les joints selectionnés
void StickMan::selectedNodes(){
    //Récuperer les joints qu'on vient de selectionner
    QList<QGraphicsItem*> itemsSelected = this->scene()->selectedItems();

    for (int i = 0; i < itemsSelected.size(); ++i) {
	//Si le joint est deja selectionne on le deselectionne (liste des joints actuelement selectionne dans la QList nodesSelected
	if(nodesSelected.contains(qgraphicsitem_cast<JointGraphic*>(itemsSelected.at(i))) == true){
	    qgraphicsitem_cast<JointGraphic*>(itemsSelected.at(i))->setBrush(QColor(4,138,191,255));
	    nodesSelected.removeOne(qgraphicsitem_cast<JointGraphic*>(itemsSelected.at(i)));
	}
	else{
	    qgraphicsitem_cast<JointGraphic*>(itemsSelected.at(i))->setBrush(QColor(242,93,0,255));
	    nodesSelected.push_back(qgraphicsitem_cast<JointGraphic*>(itemsSelected.at(i)));
	}

    }

}

//-----------------------------------------------------------
QList<JointGraphic*> StickMan::getNodesSelected(){
    return nodesSelected;
}

//-----------------------------------------------------------
void StickMan::setResize(float rate){
    RESIZE = rate;
}


