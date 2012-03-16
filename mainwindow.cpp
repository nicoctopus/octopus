#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtGui>

bool isRecording = false;
bool isLive = false;
//------------------------------------------------
//              Constructeur Main Window
//------------------------------------------------
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setCorner(Qt::TopLeftCorner, Qt::LeftDockWidgetArea);
    this->setCorner(Qt::BottomLeftCorner, Qt::LeftDockWidgetArea);
    this->setCorner(Qt::TopRightCorner, Qt::RightDockWidgetArea);
    setCorner(Qt::BottomRightCorner, Qt::RightDockWidgetArea);
    this->controller = new Controller();
    this->timer = new QTimer(this);
    connect(this->timer, SIGNAL(timeout()), this, SLOT(updateLCDTimer()));
    connect(this, SIGNAL(emitTime(QString)), ui->timerMusic, SLOT(display(QString)));
    this->timer->start(1000);
    ui->stickManLive->setStickManLive(true);
    //ui->stickManLive->launchTimerForDetection();

    this->setWindowTitle("Octopus");
    ui->nommouvement->setVisible(false);

    ui->pushButton_enregistrermouvement->setEnabled(false);
    ui->pushButton_playmouvement->setEnabled(false);
    ui->pushButton_recordmouvement->setEnabled(false);
    ui->pushButton_supprimermouvement->setEnabled(false);
    ui->pushButton_verrouiller->setEnabled(false);

    ui->pushButton_creermouvement->setStyleSheet("background:url(:/new/prefix1/images_boutons/creermouvement.png)");
    ui->pushButton_enregistrermouvement->setStyleSheet("background:url(:/new/prefix1/images_boutons/savegris.png)");
    ui->pushButton_playmouvement->setStyleSheet("background:url(:/new/prefix1/images_boutons/playgris.png)");
    ui->pushButton_recordmouvement->setStyleSheet("background:url(:/new/prefix1/images_boutons/recordgris.png)");
    ui->pushButton_supprimermouvement->setStyleSheet("background:url(:/new/prefix1/images_boutons/deletegris.png)");
    ui->pushButton_verrouiller->setStyleSheet("background:url(:/new/prefix1/images_boutons/cadenasgris.png)");
    ui->pushButton_playlecteur->setStyleSheet("background:url(:/new/prefix1/images_boutons/play.png)");
    ui->pushButton_stoplecteur->setStyleSheet("background:url(:/new/prefix1/images_boutons/stop.png)");

    connect(ui->pushButton_playlecteur,SIGNAL(pressed()),this,SLOT(slotPlayPause()));

    connect(ui->pushButton_stoplecteur,SIGNAL(pressed()),this,SLOT(slotStop()));

    //-------------------------------------------
    connect(ui->stickMan->scene(), SIGNAL(selectionChanged()), this, SLOT(slotNewSelectionOnStickMan()));
    connect(ui->pushButton_creermouvement, SIGNAL(clicked()), this, SLOT(slotUnlockStickMan()));
    connect(ui->pushButton_verrouiller, SIGNAL(clicked()), this, SLOT(slotLockNodesForNewMouvement()));
    connect(ui->pushButton_recordmouvement, SIGNAL(clicked()), this, SLOT(slotRecordNewMovement()));
    connect(ui->pushButton_enregistrermouvement, SIGNAL(clicked()), this, SLOT(slotValidNewMovement()));
    connect(ui->pushButton_supprimermouvement, SIGNAL(clicked()), this, SLOT(slotEscNewMovement()));
    connect(ui->leftTree, SIGNAL(itemClicked(QTreeWidgetItem*,int)), this, SLOT(slotDisplayInfos(QTreeWidgetItem*,int)));
    connect(ui->ButtonAdd, SIGNAL(clicked()), this, SLOT(boutonAddSample()));

    connect(this->controller->getServerOsc(), SIGNAL(jointMvtTooBig()), this, SLOT(slotTimeOutRecord()));

    connect(ui->start, SIGNAL(clicked()), this,SLOT(slotStartLivePerformance()));
    connect(this, SIGNAL(sigMoveStickman(Movement*)), ui->stickMan, SLOT(slotMoveStickMan(Movement*)));
    connect(ui->blackboard, SIGNAL(sigDisplayInfos(QGraphicsItem*)), this, SLOT(slotDisplayInfos(QGraphicsItem*)));

    this->initBlackBoard();
    this->initTreeView();

    //CONNECTS POUR STICKMAN LIVE
    for(int i=0;i<this->controller->getManagerJoints()->getListJoints()->size();i++){
        connect(this->controller->getManagerJoints()->getListJoints()->at(i),SIGNAL(sigNewPosAddedToBuffer(QString,int,int,int)),ui->stickManLive,SLOT(slotMoveNode(QString,int,int,int)));
    }


}

void MainWindow::initTreeView()
{
    //Initialisation du TreeView
    connect(this, SIGNAL(refreshLeftTree()), ui->leftTree, SLOT(refresh()));
    ui->leftTree->setListMovements(this->controller->getManagerElements()->getManagerMovements()->getListMovementsByName());
    ui->leftTree->setListPorts(this->controller->getManagerElements()->getManagerClientOSC()->getListClientsOSC());
    ui->leftTree->setListSamplesAudio(this->controller->getManagerElements()->getManagetSampleAudio()->getListSamplesAudios());
    emit refreshLeftTree();
    connect(ui->leftTree, SIGNAL(remove(Movement*)), this, SLOT(remove(Movement*)));
    connect(ui->leftTree, SIGNAL(remove(SampleAudio*)), this, SLOT(remove(SampleAudio*)));
    connect(ui->leftTree, SIGNAL(remove(ClientOSC*)), this, SLOT(remove(ClientOSC*)));
}

void MainWindow::initBlackBoard()
{
    ui->blackboard->setListMovements(controller->getManagerElements()->getManagerMovements()->getListMovementsActive());
    ui->blackboard->setListPorts(controller->getManagerElements()->getManagerClientOSC()->getListClientsOSCActive());
    ui->blackboard->setListSamplesAudio(controller->getManagerElements()->getManagetSampleAudio()->getListSamplesAudiosActive());
    connect(ui->blackboard, SIGNAL(save(Movement*)), this, SLOT(save(Movement*)));
    connect(ui->blackboard, SIGNAL(save(ClientOSC*)), this, SLOT(save(ClientOSC*)));
    connect(ui->blackboard, SIGNAL(save(SampleAudio*)), this, SLOT(save(SampleAudio*)));
    connect(ui->blackboard, SIGNAL(remove(Movement*)), this, SLOT(remove(Movement*)));
    connect(ui->blackboard, SIGNAL(remove(SampleAudio*)), this, SLOT(remove(SampleAudio*)));
    connect(ui->blackboard, SIGNAL(remove(ClientOSC*)), this, SLOT(remove(ClientOSC*)));
    connect(ui->blackboard, SIGNAL(visualisation(Movement*)), ui->stickMan, SLOT(slotMoveStickMan(Movement*)));

    connect(this, SIGNAL(refreshBlackBoard()), ui->blackboard, SLOT(refresh()));
    //connect(ui->blackboard->scene(), SIGNAL(selectionChanged()), this, SLOT(slotDisplayInfos()));
    connect(ui->leftTree, SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)), this, SLOT(slotLeftTreeDoubleClicked(QTreeWidgetItem*,int)));
    connect(this, SIGNAL(sigPlaySample()), this, SLOT(slotStop()));
    connect(this, SIGNAL(sigPlaySample()), this, SLOT(slotPlayPause()));
    emit refreshBlackBoard();
}

Controller* MainWindow::getController()
{
    return this->controller;
}

//------------------------------------------------
//                  Destructeur
//------------------------------------------------

MainWindow::~MainWindow()
{
    delete ui;
}

//------------------------------------------------
//                   METHODES
//------------------------------------------------


void MainWindow::slotNewSelectionOnStickMan(){
    ui->stickMan->selectedNodes();
}

//------------------------------------------------
//                   SLOT
//------------------------------------------------
void MainWindow::save(Movement *movement)
{
    controller->getManagerElements()->saveMovement(movement);
    //ui->blackboard->setListSamplesAudio(controller->getManagerElements()->getManagetSampleAudio()->getListSamplesAudiosActive());
    //emit refreshBlackBoard();
}

void MainWindow::save(ClientOSC *clientOSC)
{
    QSettings fichierClientOSC("clientOSC.ini", QSettings::IniFormat);
    controller->getManagerElements()->getManagerClientOSC()->save(clientOSC, fichierClientOSC);
    fichierClientOSC.sync();
}

void MainWindow::save(SampleAudio *sampleAudio)
{
    QSettings fichierSampleAudio("sampleaudio.ini", QSettings::IniFormat);
    //qDebug() << "test" << endl;
    controller->getManagerElements()->getManagetSampleAudio()->save(sampleAudio, fichierSampleAudio);
    fichierSampleAudio.sync();
    //emit refreshBlackBoard();
}

void MainWindow::remove(Movement *movement)
{
    this->controller->getManagerElements()->remove(movement);
    ui->blackboard->setListMovements(controller->getManagerElements()->getManagerMovements()->getListMovementsActive());
    this->ui->leftTree->setListMovements(this->controller->getManagerElements()->getManagerMovements()->getListMovementsByName());
    emit refreshLeftTree();
    emit refreshBlackBoard();
}

void MainWindow::remove(SampleAudio *sampleAudio)
{
    this->controller->getManagerElements()->remove(sampleAudio);
    ui->blackboard->setListSamplesAudio(controller->getManagerElements()->getManagetSampleAudio()->getListSamplesAudiosActive());
    emit refreshLeftTree();
    emit refreshBlackBoard();
}

void MainWindow::remove(ClientOSC *clientOSC)
{
    this->controller->getManagerElements()->remove(clientOSC);
    ui->blackboard->setListPorts(controller->getManagerElements()->getManagerClientOSC()->getListClientsOSCActive());
    emit refreshLeftTree();
    emit refreshBlackBoard();
}

void MainWindow::slotLeftTreeDoubleClicked(QTreeWidgetItem* item, int){
    if(item->text(0) == "Movements" || item->text(0) == "Samples" || item->text(0) == "Videos" || item->text(0) == "Ports"){
        return;
    }
    if(item->parent()->text(0) == "Samples"){
        emit sigPlaySample();
    }
    else if(item->parent()->text(0) == "Movements"){
        movingStickMan();
    }
}

//------------- Pour stickMan -------
void MainWindow::movingStickMan(){
    Movement *movement = NULL;
    if(!ui->leftTree->selectedItems().isEmpty())
    {
        if(ui->leftTree->selectedItems().at(0)->parent()->text(0) == "Movements")
        {
	    movement = (Movement*)(ui->leftTree->getMapTreeItemsMovement().value(ui->leftTree->selectedItems().at(0)));
	    emit sigMoveStickman(movement);
        }
    }

}

void MainWindow::slotPlayPause(){

    SampleAudio *sampleAudio = NULL;
    bool ok = false;
    if(!ui->blackboard->scene()->selectedItems().isEmpty()){
        if(ui->blackboard->scene()->selectedItems().at(0)->type() == 65539)
        {
            Triangle *triangle = (Triangle*)(ui->blackboard->scene()->selectedItems().at(0));
            sampleAudio = triangle->getSampleAudio();
            ok = true;
        }
    }
    else if(!ui->leftTree->selectedItems().isEmpty())
    {
        if(ui->leftTree->selectedItems().at(0)->parent()->text(0) == "Samples")
        {
            sampleAudio = (SampleAudio*)(ui->leftTree->getMapTreeItemsSample().value(ui->leftTree->selectedItems().at(0)));
            ok = true;
        }
    }
    if(ok == true)
    {
        bool temp = false;
	temp = controller->getPlayerDemo()->playDemo(sampleAudio);
        ui->labelTitleSong->setText(sampleAudio->getName());
        if(temp==true){
            //ui->pushButton_pause->setVisible(true);
            //ui->pushButton_playlecteur->setVisible(false);

            ui->pushButton_playlecteur->setStyleSheet("background:url(:/new/prefix1/images_boutons/pause.png)");
        }else{

            // ui->pushButton_pause->setVisible(false);
            // ui->pushButton_playlecteur->setVisible(true);
            ui->pushButton_playlecteur->setStyleSheet("background:url(:/new/prefix1/images_boutons/play.png)");
        }
    }

}

void MainWindow::slotStop(){
    controller->getPlayerDemo()->Stop();
    ui->labelTitleSong->setText("");
    ui->pushButton_playlecteur->setStyleSheet("background:url(:/new/prefix1/images_boutons/play.png)");
    //ui->pushButton_pause->setVisible(false);
    //ui->pushButton_playlecteur->setVisible(true);

}

void MainWindow::slotDisplayInfos(QTreeWidgetItem* item,int column){
    //Deselctionner les elements du blackboard pour le lecteur
    QList<QGraphicsItem*> itemsSelectedOnBlackboard = ui->blackboard->scene()->selectedItems();
    for(int i=0; i<itemsSelectedOnBlackboard.size();i++){
        itemsSelectedOnBlackboard.at(i)->setSelected(false);
    }


    //Blindage
    if(item->text(0) == "Movements" || item->text(0) == "Samples" || item->text(0) == "Ports" || item->text(0) == "Videos" ){
        ui->textBrowser->setText("");
        return;
    }

    QString text;
    if(item->parent()->text(0) == "Movements")
    {
        Movement *temp = ui->leftTree->getMapTreeItemsMovement().value(item);
        text = this->textDisplay(temp);
    }
    else if(item->parent()->text(0) == "Samples")
    {
        SampleAudio *temp = ui->leftTree->getMapTreeItemsSample().value(item);
        text = this->textDisplay(temp);
    }
    else if(item->parent()->text(0) == "Ports")
    {
        ClientOSC *temp = ui->leftTree->getMapTreeItemsPort().value(item);
        text = textDisplay(temp);
    }

    ui->textBrowser->setText(text);
}

//void MainWindow::slotDisplayInfos()
//{
//    QList<QGraphicsItem*> itemsSelected = ui->blackboard->scene()->selectedItems();
//    QString text;
//    for( int i=0; i<itemsSelected.size();i++)
//    {
//        if(itemsSelected.at(i)->type() == 65537)
//        {
//            EllipseDuProjet *ellipse = (EllipseDuProjet*)itemsSelected.at(i);
//            text = this->textDisplay(ellipse->getMovement());
//        }
//        if(itemsSelected.at(i)->type() == 65539)
//        {
//            Triangle *triangle = (Triangle*)itemsSelected.at(i);
//            text = this->textDisplay(triangle->getSampleAudio());
//        }
//        if(itemsSelected.at(i)->type() == 65538)
//        {
//            Diamond *diamond = (Diamond*)itemsSelected.at(i);
//	    text = this->textDisplay(diamond->getPort());
//        }
//        ui->blackboard->setLastX(itemsSelected.at(0)->pos().x());
//        ui->blackboard->setLastY(itemsSelected.at(0)->pos().y());
//        qDebug() << "mainWindow" << endl;

//        ui->textBrowser->setText(text);
//    }
//}
void MainWindow::slotDisplayInfos(QGraphicsItem* item)
{
    QString text;
        if(item->type() == 65537)
        {
            EllipseDuProjet *ellipse = (EllipseDuProjet*)item;
            text = this->textDisplay(ellipse->getMovement());
        }
        if(item->type() == 65539)
        {
            Triangle *triangle = (Triangle*)item;
            text = this->textDisplay(triangle->getSampleAudio());
        }
        if(item->type() == 65538)
        {
            Diamond *diamond = (Diamond*)item;
            text = this->textDisplay(diamond->getPort());
        }

        ui->textBrowser->setText(text);
}

QString MainWindow::textDisplay(Movement *movement)
{
    QString text;
    text.append("<b>Name : </b>");
    text.append(movement->getName());
    text.append("<br/>");
    text.append("<b>Nombre de joints mouvements : </b>");
    text.append(QString::number(movement->getListJointsMvt()->size()));
    text.append("<br/>");
    text.append("<b>Nombre de positions : </b>");
    if(!movement->getListJointsMvt()->isEmpty())
	text.append(QString::number(movement->getListJointsMvt()->at(0)->getListPositions()->size()));
    else text.append("0 position");
    text.append("<br/>");
    text.append("<b>Temps du mouvement : </b>");
    if(!movement->getListJointsMvt()->isEmpty())
    {
	text.append(QString::number(movement->getListJointsMvt()->at(0)->getListPositions()->size() * INTERVAL_TIME));
	text.append(" ms ");
    }
    else text.append("0 ms");
    text.append("<br/>");
    for(int i = 0 ; i < movement->getListJointsMvt()->size() ; i++)
    {
        text.append("- ");
	text.append(movement->getListJointsMvt()->at(i)->getJointRef()->getNom());
        text.append("<br/>");
    }
    text.append("<br/><b>SampleAudio : </b><br/>");
    if(movement->getSampleAudio())
	text.append(movement->getSampleAudio()->getName());
    else ("Aucun");
    text.append("<br/><b>Clients OSC : </b><br/>");
    if(!movement->getListClients()->isEmpty())
	for(int i = 0 ; i < movement->getListClients()->size() ; i++)
	{
	    text.append(movement->getListClients()->at(i)->getName());
	    text.append(("<br/>"));
	}
    else text.append("Aucun");
    return text;
}

QString MainWindow::textDisplay(SampleAudio *sampleAudio)
{
    QString text;
    text.append("<b>Name : </b>");
    text.append(sampleAudio->getName());
    text.append("<br/>");
    text.append("<b>URL : </b>");
    text.append(sampleAudio->getFileURL());
    text.append("<br/>");
    text.append("<b>Mouvements liés : </b>");
    for(int i = 0 ; i < sampleAudio->getListIdMovement()->size() ; i ++)
    {
        text.append(QString::number(sampleAudio->getListIdMovement()->at(i)));
        text.append("<br/>");
    }
    return text;
}

QString MainWindow::textDisplay(ClientOSC *port)
{
    QString text;
    text.append("<b>Name : </b>");
    text.append(port->getName());
    text.append("<br/>");
    text.append("<b>Serveur : </b>");
    text.append(port->getHost());
    text.append("<br/>");
    text.append("<b>Mouvements liés : </b>");
    for(int i = 0 ; i < port->getListIdMovement()->size() ; i ++)
    {
        text.append(QString::number(port->getListIdMovement()->at(i)));
        text.append("<br/>");
    }
    return text;
}

//Permet de selectionner les joints pour un nouveau mouvement
void MainWindow::slotUnlockStickMan(){
    //QMessageBox msgBox;
    //msgBox.setText("Veuillez selectionner les joints impliquÃˆs dans votre mouvement. Cliquez ensuite sur verrouiller");
    //msgBox.exec();
    QList<QGraphicsItem *> items = ui->stickMan->scene()->items();
    for (int i = 0; i < items.size(); ++i) {
        if((items.at(i)->type()) == 3){ //3 est le type de QGraphicsRectItems
            items.at(i)->setFlags(QGraphicsItem::ItemIsSelectable);
        }
    }
    //ui->pushButton_creermouvement->setVisible(false);
    // ui->pushButton_verrouiller->setVisible(true);
    //ui->pushButton_supprimermouvement->setVisible(true);

    ui->pushButton_creermouvement->setStyleSheet("background:url(:/new/prefix1/images_boutons/creermouvementgris.png)");
    ui->pushButton_verrouiller->setStyleSheet("background:url(:/new/prefix1/images_boutons/cadenas.png)");
    ui->pushButton_supprimermouvement->setStyleSheet("background:url(:/new/prefix1/images_boutons/delete.png)");

    ui->pushButton_creermouvement->setEnabled(false);
    ui->pushButton_verrouiller->setEnabled(true);
    ui->pushButton_supprimermouvement->setEnabled(true);


}

//Enregistre les joints du nouveau mouvement
int MainWindow::slotLockNodesForNewMouvement(){

    QList<JointGraphic*> nodesSelected = ui->stickMan->getNodesSelected();

    if(nodesSelected.size() == 0){
        QMessageBox msgBox;
	msgBox.setText("Aucuns joints selectionnés. Veuillez en selectionner au moins un.");
        msgBox.exec();
        return 0;
    }
    else
    {
        QString nomsDesJoints;
        movement = new Movement();
        for (int i = 0; i < nodesSelected.size(); ++i) {
            movement->addJointMvt(controller->getManagerJoints()->getJoint(nodesSelected.at(i)->getName()));
            nomsDesJoints = nomsDesJoints + nodesSelected.at(i)->getName() + "\n";
        }
        //QMessageBox msgBox;
        //msgBox.setText("Vous avez verrouillé " + QString::number(nodesSelected.size()) + " joints:\n" + nomsDesJoints);
        //msgBox.exec();
	ui->stickMan->reCreateStickMan();

	//ui->stickMan->getNodesSelected().clear();



    }
    // ui->pushButton_verrouiller->setVisible(false);
    // ui->pushButton_recordmouvement->setVisible(true);

    ui->pushButton_recordmouvement->setStyleSheet("background:url(:/new/prefix1/images_boutons/record.png)");
    ui->pushButton_verrouiller->setStyleSheet("background:url(:/new/prefix1/images_boutons/cadenasgris.png)");

    ui->pushButton_recordmouvement->setEnabled(true);
    ui->pushButton_verrouiller->setEnabled(false);

    return 1;

}

void MainWindow::slotRecordNewMovement(){

    if(isRecording == false){
        ui->pushButton_recordmouvement->setStyleSheet("background:url(:/new/prefix1/images_boutons/stop.png)");
        //RECORD UN MOVEMENT
        this->controller->recordMovement(this->movement);
        //START RECORD
        isRecording = true;
    }
    else if(isRecording == true){
        ui->pushButton_recordmouvement->setStyleSheet("background:url(:/new/prefix1/images_boutons/recordgris.png)");
        ui->pushButton_recordmouvement->setEnabled(false);
        ui->nommouvement->setVisible(true);
	ui->nommouvement->setText("");
        ui->pushButton_enregistrermouvement->setStyleSheet("background:url(:/new/prefix1/images_boutons/save.png)");
        ui->pushButton_enregistrermouvement->setEnabled(true);

        //ON STOP LE RECORD
        this->controller->stopRecord(this->movement);
        isRecording = false;
    }
}

void MainWindow::slotTimeOutRecord()
{
    //qDebug() << "test"<< endl;
    //ON STOP LE RECORD
    this->controller->stopRecord(this->movement);
    //ON AFFICHE LES BOUTONS
    ui->pushButton_recordmouvement->setStyleSheet("background:url(:/new/prefix1/images_boutons/recordgris.png)");
    ui->pushButton_recordmouvement->setEnabled(false);
    ui->nommouvement->setVisible(true);
    ui->nommouvement->setText("");
    ui->pushButton_enregistrermouvement->setStyleSheet("background:url(:/new/prefix1/images_boutons/save.png)");
    ui->pushButton_enregistrermouvement->setEnabled(true);

    isRecording = false;
}

void MainWindow::slotValidNewMovement(){
    if(ui->nommouvement->text().isEmpty() == true){
	QMessageBox msg;
	msg.setText("Veuillez entrer un nom au mouvement");
	msg.exec();
    }
    else{
	this->movement->setName(ui->nommouvement->text());
	this->controller->getManagerElements()->addMovement(movement);
	this->controller->linkJointToJointMvt();
	this->ui->leftTree->setListMovements(this->controller->getManagerElements()->getManagerMovements()->getListMovementsByName());
	this->refreshLeftTree();
	//qDebug() << movement->getName() << endl;

	//ui->pushButton_enregistrermouvement->setVisible(false);
	//ui->pushButton_supprimermouvement->setVisible(false);
	ui->nommouvement->setVisible(false);
	//ui->pushButton_creermouvement->setVisible(true);
	ui->pushButton_enregistrermouvement->setStyleSheet("background:url(:/new/prefix1/images_boutons/savegris.png)");
	ui->pushButton_enregistrermouvement->setEnabled(false);
	ui->pushButton_supprimermouvement->setStyleSheet("background:url(:/new/prefix1/images_boutons/deletegris.png)");
	ui->pushButton_enregistrermouvement->setEnabled(false);
	ui->pushButton_creermouvement->setStyleSheet("background:url(:/new/prefix1/images_boutons/creermouvement.png)");
	ui->pushButton_creermouvement->setEnabled(true);

	//ui->stickMan->reCreateStickMan();

    }

}

void MainWindow::slotEscNewMovement(){
    ui->pushButton_creermouvement->setEnabled(true);
    ui->pushButton_enregistrermouvement->setEnabled(false);
    ui->pushButton_playmouvement->setEnabled(false);
    ui->pushButton_recordmouvement->setEnabled(false);
    ui->pushButton_supprimermouvement->setEnabled(false);
    ui->pushButton_verrouiller->setEnabled(false);

    ui->nommouvement->setVisible(false);

    ui->pushButton_creermouvement->setStyleSheet("background:url(:/new/prefix1/images_boutons/creermouvement.png)");
    ui->pushButton_enregistrermouvement->setStyleSheet("background:url(:/new/prefix1/images_boutons/savegris.png)");
    ui->pushButton_playmouvement->setStyleSheet("background:url(:/new/prefix1/images_boutons/playgris.png)");
    ui->pushButton_recordmouvement->setStyleSheet("background:url(:/new/prefix1/images_boutons/recordgris.png)");
    ui->pushButton_supprimermouvement->setStyleSheet("background:url(:/new/prefix1/images_boutons/deletegris.png)");
    ui->pushButton_verrouiller->setStyleSheet("background:url(:/new/prefix1/images_boutons/cadenasgris.png)");
    ui->stickMan->reCreateStickMan();

}


void MainWindow::slotStartLivePerformance(){

    if(isLive==false){
        this->controller->analizeRecord();
        ui->stickManLive->launchTimerForDetection();
        isLive=true;
    }else if(isLive==true){
        ui->stickManLive->stopTimer();
        this->controller->stopAnalize();
        isLive=false;
    }




}

void MainWindow::boutonAddSample()
{
    QStringList files = QFileDialog::getOpenFileNames(this, tr("Select Music Files"),
                                                      QDesktopServices::storageLocation(QDesktopServices::MusicLocation));

    if (files.isEmpty())
        return;
    else
    {
        for(int i = 0 ; i < files.size() ; i++)
        {
            this->controller->getManagerElements()->getManagetSampleAudio()->addSample(files.at(i).split("/").last(), files.at(i));
        }
    }
    emit refreshLeftTree();
}

void MainWindow::updateLCDTimer()
{
    int m, s;
    s = controller->getPlayerDemo()->currentTime() / 1000;
    m = s/60;
    s = s%60;
    QString time = QString::number(m) + ":" + QString::number(s);
    emit emitTime(time);
}
