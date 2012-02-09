#include "stickman.h"



// -----------------------------------------------------------
//                     CONSTRUCTEUR
//-----------------------------------------------------------

StickMan::StickMan(QWidget *parent) : QGraphicsView (parent)
{
    RESIZE = 2.5;

    QGraphicsScene *scene = new QGraphicsScene();
    this->setScene(scene);

    this->createStickMan();

    QRectF sizeScene = this->scene()->sceneRect();
    this->ensureVisible(sizeScene,500,500);

    connect(this, SIGNAL(clearStickMan()), this->scene(), SLOT(clear()));
}


//-----------------------------------------------------------
//                     METHODES
//-----------------------------------------------------------

//Creer le stickMan
void StickMan::createStickMan(){

    QPoint head(0/RESIZE, -175/RESIZE);
    QPoint epauleGauche(-50/RESIZE, -100/RESIZE);
    QPoint basCou(0/RESIZE, -100/RESIZE);
    QPoint epauleDroite(50/RESIZE, -100/RESIZE);
    QPoint righthand(-175/RESIZE, -170/RESIZE);
    QPoint lefthand(175/RESIZE, -170/RESIZE);
    QPoint rightelbow(-150/RESIZE, -80/RESIZE);
    QPoint torso(0, 0);
    QPoint leftelbow(150/RESIZE, -80/RESIZE);
    QPoint hancheGauche(-50/RESIZE, 100/RESIZE);
    QPoint hancheDroite(50/RESIZE, 100/RESIZE);
    QPoint rightknee(-75/RESIZE, 200/RESIZE);
    QPoint leftknee(75/RESIZE, 200/RESIZE);
    QPoint rightfoot(-75/RESIZE, 300/RESIZE);
    QPoint leftfoot(75/RESIZE, 300/RESIZE);

    qint32 WIDTH_JOINTS = 10;
    qint32 HEIGHT_JOINTS = 10;

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

    QLine cou(head, basCou);
    QLine hautCorps(epauleGauche, epauleDroite);
    QLine brasDroit(epauleDroite, leftelbow);
    QLine avantBrasDroit(leftelbow, lefthand);
    QLine brasGauche(epauleGauche, rightelbow);
    QLine avantBrasGauche(rightelbow, righthand);
    QLine coteGauche(epauleGauche,torso);
    QLine coteDroite(epauleDroite, torso);
    QLine ventreGauche(torso, hancheGauche);
    QLine ventreDroit(torso, hancheDroite);
    QLine basCorps(hancheGauche, hancheDroite);
    QLine cuisseGauche(hancheGauche, rightknee);
    QLine jambeGauche(rightknee, rightfoot);
    QLine cuisseDroite(hancheDroite, leftknee);
    QLine jambeDroite(leftknee, leftfoot);

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
        QGraphicsLineItem *line = new QGraphicsLineItem(lines.at(j));
        line->setPen(pen);
        this->scene()->addItem(line);
    }

    //Creer les noeuds
    QMap<QString, QPoint>::const_iterator it = coord.constBegin();
    while (it != coord.constEnd()) {
        JointGraphic *node = new JointGraphic();
        node->setName(it.key());
        //node->setFlag(QGraphicsItem::ItemIsSelectable);
        node->setRect((it.value().x())-(WIDTH_JOINTS/2), (it.value().y())-(HEIGHT_JOINTS/2) , WIDTH_JOINTS, HEIGHT_JOINTS);
        node->setBrush(QColor(4,138,191,255));
        this->scene()->addItem(node);
        ++it;
    }
}

//-----------------------------------------------------------
void StickMan::reCreateStickMan(){
    emit clearStickMan();
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


