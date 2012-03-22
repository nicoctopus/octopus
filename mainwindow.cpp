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
   // this->setCorner(Qt::TopRightCorner, Qt::RightDockWidgetArea);
    this->setCorner(Qt::BottomRightCorner, Qt::RightDockWidgetArea);
    this->controller = new Controller();
    this->timer = new QTimer(this);
    connect(this->timer, SIGNAL(timeout()), this, SLOT(updateLCDTimerLive()));
    connect(this->timer, SIGNAL(timeout()), this, SLOT(updateLabelTimeRecord()));
    connect(this, SIGNAL(emitTime(QString)), ui->timerMusic, SLOT(display(QString)));
    connect(this, SIGNAL(emitTimeLabelRecord(QString)), ui->labelTimeRecord, SLOT(setText(QString)));
    //ui->stickManLive->setStickManLive(true);
    //ui->stickManLive->launchTimerForDetection();

    this->setWindowTitle("Octopus");
    ui->nommouvement->setVisible(false);
    ui->loopSpin->setVisible(false);
    ui->loopLabel->setVisible(false);
    ui->resetBox->setVisible(false);
    ui->loopBox->setVisible(false);

    ui->loopSpin->setEnabled(false);
    ui->loopLabel->setEnabled(false);



    ui->pushButton_enregistrermouvement->setEnabled(false);
    ui->pushButton_recordmouvement->setEnabled(false);
    ui->pushButton_supprimermouvement->setEnabled(false);
    ui->pushButton_verrouiller->setEnabled(false);

    ui->pushButton_creermouvement->setStyleSheet("");
    ui->pushButton_enregistrermouvement->setStyleSheet("border-image:url(:/new/prefix1/images_boutons/savegris.png)");

    ui->pushButton_recordmouvement->setStyleSheet("border-image:url(:/new/prefix1/images_boutons/recordgris.png)");
    ui->pushButton_supprimermouvement->setStyleSheet("border-image:url(:/new/prefix1/images_boutons/deletegris.png)");
    ui->pushButton_verrouiller->setStyleSheet("border-image:url(:/new/prefix1/images_boutons/cadenasgris.png)");
    ui->pushButton_playlecteur->setStyleSheet("");
    ui->pushButton_stoplecteur->setStyleSheet("");

    connect(ui->resetBox,SIGNAL(stateChanged(int)),this,SLOT(slotSetSampleResetMode(int)));
    connect(ui->loopBox,SIGNAL(stateChanged(int)),this,SLOT(slotSetSampleLoopMode(int)));
    connect(ui->loopSpin,SIGNAL(valueChanged(QString)),this,SLOT(slotSetSampleSpinBox(QString)));

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
    this->initCourbes();

    //CONNECTS POUR STICKMAN LIVE
    for(int i=0;i<this->controller->getManagerJoints()->getListJoints()->size();i++){
	connect(this->controller->getManagerJoints()->getListJoints()->at(i),SIGNAL(sigNewPosAddedToBuffer(QString,int,int,int)),ui->stickMan,SLOT(slotMoveNode(QString,int,int,int)));
    }


    /******** RECORD MOVEMENT *******/
    this->tempLatence = 30;
    this->tempRecordMovement = 20;
    ui->labelTimeRecord->setVisible(false);
}

void MainWindow::fillComboBox()
{
    emit clearComboBox();
    QStringList stringList;
    for(int i = 0 ;  i < this->controller->getManagerElements()->getManagerMovements()->getListMovementsActive()->size() ; i++)
	stringList.append(this->controller->getManagerElements()->getManagerMovements()->getListMovementsActive()->at(i)->getName());
    ui->listMovementToShowCourbe->addItems(stringList);
}

void MainWindow::initCourbes()
{
    this->fillComboBox();
    //this->ui->widgetCourbes->setTabWidget(ui->tabWidget);
    ui->tabWidget->removeTab(0);
    connect(ui->listMovementToShowCourbe, SIGNAL(currentIndexChanged(QString)), this, SLOT(slotChangeMovementForCourbe(QString)));
    connect(this, SIGNAL(clearComboBox()), ui->listMovementToShowCourbe, SLOT(clear()));
}

void MainWindow::refreshCourbes(Movement* movement)
{
    int size = ui->tabWidget->count();
    for(int i = 0 ; i < size ; i++){
	ui->tabWidget->removeTab(0);
    }
    for(int i=0; i<movement->getListJointsMvt()->size();i++){
	ui->tabWidget->addTab(new Courbes(ui->tabWidget, movement->getListJointsMvt()->at(i)),movement->getListJointsMvt()->at(i)->getJointRef()->getNom());
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
    this->fillComboBox();
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
    this->fillComboBox();
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
    this->timer->start(1000);
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


           ui->pushButton_playlecteur->setStyleSheet("border-image:url(:/new/prefix1/images_boutons/pause.png)");
        }else{


            ui->pushButton_playlecteur->setStyleSheet("");
        }
    }

}

void MainWindow::slotStop(){
    controller->getPlayerDemo()->Stop();
    ui->labelTitleSong->setText("");
    ui->pushButton_playlecteur->setStyleSheet("");
    this->timer->stop();
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
        ui->loopSpin->setVisible(false);
        ui->resetBox->setVisible(false);
        ui->loopLabel->setVisible(false);
        ui->loopBox->setVisible(false);

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
    ui->loopSpin->setVisible(false);
    ui->resetBox->setVisible(false);
    ui->loopLabel->setVisible(false);
    ui->loopBox->setVisible(false);
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
    ui->loopSpin->setVisible(true);
    ui->resetBox->setVisible(true);
    ui->loopLabel->setVisible(true);
    ui->loopBox->setVisible(true);

    this->audioTemp = sampleAudio;

    ui->resetBox->setChecked(sampleAudio->getResetActive());
    ui->loopBox->setChecked(sampleAudio->getLoopActive());


    QString text;
    int temp;
    text.append("<b>Nom : </b>");
    text.append(sampleAudio->getName());
    text.append("<br/>");
    text.append("<b>URL : </b>");
    text.append(sampleAudio->getFileURL());
    text.append("<br/>");
    text.append("<b> Loop Mode : </b>");
    if(sampleAudio->getLoopActive())
        text.append("ON");
    else
        text.append("OFF");
    text.append("<br/>");
    if(sampleAudio->getLoopActive())
    {
     text.append(" Nombre Loop : ");
     temp= sampleAudio->getNbLoop();
    if(temp==-1){
        text.append("INFINIE");
    }else if(temp==0){
        text.append("One Shot");
    }else{

        text.append(QString::number(temp+1));
    }
    text.append("<br/>");
    }
    text.append("<b> Mode Reset actif :  </b>");
    if(sampleAudio->getResetActive())
        text.append("OUI");
    else
        text.append("NON");
    text.append("<br/>");
    if(sampleAudio->getListIdMovement()->size()>0){
    text.append("<b>Mouvements liés : </b>");
    for(int i = 0 ; i < sampleAudio->getListIdMovement()->size() ; i ++)
    {
        text.append(QString::number(sampleAudio->getListIdMovement()->at(i)));
        text.append("<br/>");
    }
    }
    return text;
}

QString MainWindow::textDisplay(ClientOSC *port)
{
    ui->loopSpin->setVisible(false);
    ui->resetBox->setVisible(false);
    ui->loopLabel->setVisible(false);
    QString text;
    text.append("<b>Nom : </b>");
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
    QList<QGraphicsItem *> items = ui->stickMan->scene()->items();
    for (int i = 0; i < items.size(); ++i) {
        if((items.at(i)->type()) == 3){ //3 est le type de QGraphicsRectItems
            items.at(i)->setFlags(QGraphicsItem::ItemIsSelectable);
        }
    }

    ui->pushButton_creermouvement->setStyleSheet("border-image:url(:/new/prefix1/images_boutons/creermouvementgris.png)");
    ui->pushButton_verrouiller->setStyleSheet("");
    ui->pushButton_supprimermouvement->setStyleSheet("");

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
	ui->stickMan->reCreateStickMan();
	this->configRecordMouvement = new ConfigRecordMouvement(0);
	this->configRecordMouvement->setDefautSettings(this->tempLatence, this->tempRecordMovement);
	connect(this->configRecordMouvement, SIGNAL(signalConfigTempsRecord(float, float)), this, SLOT(slotConfigTempsRecord(float,float)));
	this->configRecordMouvement->show();
    }

    ui->pushButton_recordmouvement->setStyleSheet("");
    ui->pushButton_verrouiller->setStyleSheet("border-image:url(:/new/prefix1/images_boutons/cadenasgris.png)");

    ui->pushButton_recordmouvement->setEnabled(true);
    ui->pushButton_verrouiller->setEnabled(false);

    return 1;

}

void MainWindow::slotRecordNewMovement()
{
    if(isRecording == false){
         ui->pushButton_recordmouvement->setStyleSheet("border-image:url(:/new/prefix1/images_boutons/stop.png)");
	ui->labelTimeRecord->setVisible(true);
	this->timer->start(100);
        isRecording = true;
    }
    else if(isRecording == true){
	ui->pushButton_recordmouvement->setStyleSheet("border-image:url(:/new/prefix1/images_boutons/recordgris.png)");
        ui->pushButton_recordmouvement->setEnabled(false);
        ui->nommouvement->setVisible(true);
	ui->nommouvement->setText("");
        ui->pushButton_enregistrermouvement->setStyleSheet("");
        ui->pushButton_enregistrermouvement->setEnabled(true);
	this->timer->stop();
        isRecording = false;
    }
}

void MainWindow::slotTimeOutRecord()
{
    //qDebug() << "test"<< endl;
    //ON STOP LE RECORD
    this->controller->stopRecord(this->movement);
    //ON AFFICHE LES BOUTONS
    ui->pushButton_recordmouvement->setStyleSheet("border-image:url(:/new/prefix1/images_boutons/recordgris.png)");
    ui->pushButton_recordmouvement->setEnabled(false);
    ui->nommouvement->setVisible(true);
    ui->nommouvement->setText("");
    ui->pushButton_enregistrermouvement->setStyleSheet("");
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
	ui->labelTimeRecord->setVisible(false);
	//qDebug() << movement->getName() << endl;


	ui->nommouvement->setVisible(false);

        ui->pushButton_enregistrermouvement->setStyleSheet("border-image:url(:/new/prefix1/images_boutons/savegris.png)");
        ui->pushButton_enregistrermouvement->setEnabled(false);
        ui->pushButton_supprimermouvement->setStyleSheet("border-image:url(:/new/prefix1/images_boutons/deletegris.png)");
	ui->pushButton_enregistrermouvement->setEnabled(false);
        ui->pushButton_creermouvement->setStyleSheet("");
	ui->pushButton_creermouvement->setEnabled(true);

	//ui->stickMan->reCreateStickMan();

    }

}

void MainWindow::slotEscNewMovement(){
    ui->pushButton_creermouvement->setEnabled(true);
    ui->pushButton_enregistrermouvement->setEnabled(false);
    ui->pushButton_recordmouvement->setEnabled(false);
    ui->pushButton_supprimermouvement->setEnabled(false);
    ui->pushButton_verrouiller->setEnabled(false);

    ui->nommouvement->setVisible(false);

    ui->pushButton_creermouvement->setStyleSheet("");
    ui->pushButton_enregistrermouvement->setStyleSheet("border-image:url(:/new/prefix1/images_boutons/savegris.png)");
    ui->pushButton_recordmouvement->setStyleSheet("border-image:url(:/new/prefix1/images_boutons/recordgris.png)");
    ui->pushButton_supprimermouvement->setStyleSheet("border-image:url(:/new/prefix1/images_boutons/deletegris.png)");
    ui->pushButton_verrouiller->setStyleSheet("border-image:url(:/new/prefix1/images_boutons/cadenasgris.png)");
    ui->stickMan->reCreateStickMan();
    ui->labelTimeRecord->setVisible(false);
}


void MainWindow::slotStartLivePerformance(){

    if(isLive==false){
        this->controller->analizeRecord();
	for(int i = 0 ; i < this->controller->getManagerElements()->getManagerMovements()->getListMovementsActive()->size() ; i++)
	    if(this->controller->getManagerElements()->getManagerMovements()->getListMovementsActive()->at(i)->getName() == ui->listMovementToShowCourbe->currentText())
		//ui->widgetCourbes->setMovement(this->controller->getManagerElements()->getManagerMovements()->getListMovementsActive()->at(i));
		this->refreshCourbes(this->controller->getManagerElements()->getManagerMovements()->getListMovementsActive()->at(i));
	ui->stickMan->setStickManLive(true);
	ui->stickMan->launchTimerForDetection();
        isLive=true;
    }else if(isLive==true){
	ui->stickMan->stopTimer();
	ui->stickMan->setStickManLive(false);
	//ui->widgetCourbes->setMovement(NULL);
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

void MainWindow::updateLCDTimerLive()
{
    int m, s;
    s = controller->getPlayerDemo()->currentTime() / 1000;
    m = s/60;
    s = s%60;
    QString time = QString::number(m) + ":" + QString::number(s);
    emit emitTime(time);
}

void MainWindow::updateLabelTimeRecord()
{
    qDebug() << "début" << this->tempLatence << endl;
    if(isRecording && this->tempLatence > 0)
    {
	qDebug() << "bouh1" << endl;
	emit emitTimeLabelRecord(QString::number(this->tempLatence / 10));
	this->tempLatence -= 1;
	this->tempRecordMovementPartantDe0 = 1;
    }
    else if(isRecording && this->tempLatence == 0 && this->tempRecordMovementPartantDe0 < this->tempRecordMovement)
    {
	qDebug() << "bouh2" << endl;
	this->controller->recordMovement(this->movement);
	emit emitTimeLabelRecord(QString::number(this->tempRecordMovementPartantDe0 / 10));
	this->tempRecordMovementPartantDe0++;
	this->tempLatence = -1;
    }
    else if(isRecording && this->tempLatence == -1 && this->tempRecordMovementPartantDe0 == this->tempRecordMovement)
    {
	qDebug() << "bouh4" << endl;
	this->timer->stop();

	//ON STOP LE RECORD
	this->controller->stopRecord(this->movement);
	emit emitTimeLabelRecord(QString::number(this->tempRecordMovementPartantDe0 / 10));
	this->tempRecordMovementPartantDe0 = 0;
	this->tempLatence = 30;
	ui->pushButton_recordmouvement->setStyleSheet("border-image:url(:/new/prefix1/images_boutons/recordgris.png)");
	ui->pushButton_recordmouvement->setEnabled(false);
	ui->nommouvement->setVisible(true);
	ui->nommouvement->setText("");
	ui->pushButton_enregistrermouvement->setStyleSheet("");
	ui->pushButton_enregistrermouvement->setEnabled(true);
	isRecording = false;
    }
    else if(isRecording && this->tempLatence == -1 && this->tempRecordMovementPartantDe0 < this->tempRecordMovement)
    {
	qDebug() << "bouh3" << endl;
	this->controller->recordMovement(this->movement);
	emit emitTimeLabelRecord(QString::number(this->tempRecordMovementPartantDe0 / 10));
	this->tempRecordMovementPartantDe0 += 1;
    }
    qDebug() << "fin" << endl;
}

void MainWindow::slotChangeMovementForCourbe(QString text)
{
    for(int i = 0 ; i < this->controller->getManagerElements()->getManagerMovements()->getListMovementsActive()->size() ; i++)
	if(this->controller->getManagerElements()->getManagerMovements()->getListMovementsActive()->at(i)->getName() == text)
	   //ui->widgetCourbes->setMovement(this->controller->getManagerElements()->getManagerMovements()->getListMovementsActive()->at(i));
	    this->refreshCourbes(this->controller->getManagerElements()->getManagerMovements()->getListMovementsActive()->at(i));

}

void MainWindow::slotSetSampleResetMode(int state){

    if(state==0){
     this->audioTemp->setResetActive(false);
    }else{
       this->audioTemp->setResetActive(true);

    }
    this->save(this->audioTemp);
 ui->textBrowser->setText(this->textDisplay(this->audioTemp));
}


/******** SLOTS DIALOG CONFIG ANALYSE *********/
void MainWindow::on_pushButton_clicked()
{
    this->configAnalyse = new ConfigAnalyse(0);
    this->configAnalyse->setParamDefaut(this->controller->getServerOsc()->getVitesse(), this->controller->getServerOsc()->getAmplitude());
    this->configAnalyse->show();
    /*********** DIALOG CONFIG ANALYSE *******/
    connect(this->configAnalyse, SIGNAL(signalConfigAnalyse(float,quint16)), this, SLOT(slotChangeConfigAnalyse(float, quint16)));
}

void MainWindow::slotChangeConfigAnalyse(float vitesse, quint16 amplitude)
{
    this->controller->getServerOsc()->setVitesse(vitesse);
    this->controller->getServerOsc()->setAmplitude(amplitude);
}

void MainWindow::slotConfigTempsRecord(float tempsLatence, float tempsRecord)
{
    this->tempLatence = tempsLatence;
    this->tempRecordMovement = tempsRecord;
}
void MainWindow::slotSetSampleLoopMode(int state){

    if(state==0){
        this->audioTemp->setLoopActive(false);
        ui->loopSpin->setEnabled(false);
        ui->loopLabel->setEnabled(false);
    }else{
        this->audioTemp->setLoopActive(true);
        ui->loopSpin->setEnabled(true);
        ui->loopLabel->setEnabled(true);
    }
     this->save(this->audioTemp);
    ui->textBrowser->setText(this->textDisplay(this->audioTemp));

}

void MainWindow::slotSetSampleSpinBox(QString str){

   if(str=="INF"){
     this->audioTemp->setNbLoop(-1);
    }else{
      int y ;
      y = str.toInt();
      this->audioTemp->setNbLoop(y-1);
    }
   this->save(this->audioTemp);
    ui->textBrowser->setText(this->textDisplay(this->audioTemp));
}
