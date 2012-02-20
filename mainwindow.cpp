#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtGui>

bool isRecording = false;
//------------------------------------------------
//              Constructeur Main Window
//------------------------------------------------
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->controller = new Controller();
    this->timer = new QTimer(this);
    connect(this->timer, SIGNAL(timeout()), this, SLOT(updateLCDTimer()));
    connect(this, SIGNAL(emitTime(QString)), ui->timerMusic, SLOT(display(QString)));
    this->timer->start(1000);

    this->setWindowTitle("Octopus");
    //ui->pushButton_enregistrermouvement->setVisible(false);
    //ui->pushButton_playmouvement->setVisible(false);
    //ui->pushButton_recordmouvement->setVisible(false);
    //ui->pushButton_supprimermouvement->setVisible(false);
    //ui->pushButton_verrouiller->setVisible(false);
    //ui->pushButton_stoprecord->setVisible(false);
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

    //this->controller->getPlayerDemo()->playDemo(this->controller->getManagerElements()->getListSamplesAudios()->at(0));

    connect(ui->pushButton_playlecteur,SIGNAL(pressed()),this,SLOT(slotPlayPause()));
    //connect(ui->pushButton_pause,SIGNAL(pressed()),this,SLOT(slotPlayPause()));

    connect(ui->pushButton_stoplecteur,SIGNAL(pressed()),this,SLOT(slotStop()));

    //ui->pushButton_pause->setVisible(false);

    //sleep(10);
    //this->controller->getPlayerDemo()->Stop();
    //-------------------------------------------
    connect(ui->stickMan->scene(), SIGNAL(selectionChanged()), this, SLOT(slotNewSelectionOnStickMan()));
    connect(ui->pushButton_creermouvement, SIGNAL(clicked()), this, SLOT(slotUnlockStickMan()));
    connect(ui->pushButton_verrouiller, SIGNAL(clicked()), this, SLOT(slotLockNodesForNewMouvement()));
    connect(ui->pushButton_recordmouvement, SIGNAL(clicked()), this, SLOT(slotRecordNewMovement()));
    //connect(ui->pushButton_recordmouvement, SIGNAL(clicked()), this, SLOT(slotStopRecordNewMovement()));
    connect(ui->pushButton_enregistrermouvement, SIGNAL(clicked()), this, SLOT(slotValidNewMovement()));
    connect(ui->pushButton_supprimermouvement, SIGNAL(clicked()), this, SLOT(slotEscNewMovement()));
    connect(ui->leftTree, SIGNAL(itemClicked(QTreeWidgetItem*,int)), this, SLOT(slotDisplayInfos(QTreeWidgetItem*,int)));
    connect(ui->blackboard->scene(), SIGNAL(selectionChanged()), this, SLOT(slotDisplayInfos()));
    connect(ui->ButtonAdd, SIGNAL(clicked()), this, SLOT(boutonAddSample()));


    this->initBlackBoard();
    this->initTreeView();

    //QPushButton *start = new QPushButton();
    //ui->layoutDockPerformance->addWidget(start);



}

void MainWindow::initTreeView()
{
    //Initialisation du TreeView
    connect(this, SIGNAL(refreshLeftTree()), ui->leftTree, SLOT(refresh()));
    ui->leftTree->setListMovements(this->controller->getManagerElements()->getListMovements());
    ui->leftTree->setListPorts(this->controller->getManagerElements()->getListPorts());
    ui->leftTree->setListSamplesAudio(this->controller->getManagerElements()->getListSamplesAudios());
    emit refreshLeftTree();
}

void MainWindow::initBlackBoard()
{
    ui->blackboard->setListMovements(controller->getManagerElements()->getListMovementsActive());
    ui->blackboard->setListPorts(controller->getManagerElements()->getListPortsActive());
    ui->blackboard->setListSamplesAudio(controller->getManagerElements()->getListSamplesAudiosActive());
    connect(ui->blackboard, SIGNAL(save(Movement*)), this, SLOT(save(Movement*)));
    connect(ui->blackboard, SIGNAL(save(ClientOSC*)), this, SLOT(save(ClientOSC*)));
    connect(ui->blackboard, SIGNAL(save(SampleAudio*)), this, SLOT(save(SampleAudio*)));
    connect(ui->blackboard->scene(), SIGNAL(selectionChanged()), this, SLOT(slotNewSelectionOnBlackBoard()));

    connect(this, SIGNAL(refreshBlackBoard()), ui->blackboard, SLOT(refresh()));

    connect(ui->leftTree, SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)), this, SLOT(slotLeftTreeDoubleClicked(QTreeWidgetItem*,int)));
    connect(this, SIGNAL(sigPlaySample()), this, SLOT(slotStop()));
    connect(this, SIGNAL(sigPlaySample()), this, SLOT(slotPlayPause()));
    connect(ui->blackboard, SIGNAL(decocherCheckBoxLink()), this, SLOT(decocherCheckBoxLink()));
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
    QSettings fichierMovement("movement.ini", QSettings::IniFormat);
    controller->getManagerElements()->saveMovement(movement, fichierMovement);
    fichierMovement.sync();
}

void MainWindow::save(ClientOSC *port)
{
    // controller->getManagerElements()
}

void MainWindow::save(SampleAudio *sampleAudio)
{
    QSettings fichierSampleAudio("sampleaudio.ini", QSettings::IniFormat);
    controller->getManagerElements()->saveSampleAudio(sampleAudio, fichierSampleAudio);
    fichierSampleAudio.sync();
}

void MainWindow::slotNewSelectionOnBlackBoard(){
    if((ui->checkBox->checkState() == 2)&&(ui->blackboard->scene()->selectedItems().isEmpty() == false)){
	ui->blackboard->selectedItems();
    }
}

void MainWindow::slotLeftTreeDoubleClicked(QTreeWidgetItem* item, int){
    if(item->text(0) == "Movements" || item->text(0) == "Samples" || item->text(0) == "Videos" || item->text(0) == "Ports"){
	//rien
    }
    else{
	emit sigPlaySample();
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
//	temp = controller->getPlayerDemo()->playDemo(sampleAudio);
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
    //controller->getPlayerDemo()->Stop();
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

void MainWindow::slotDisplayInfos()
{
    QList<QGraphicsItem*> itemsSelected = ui->blackboard->scene()->selectedItems();
    QString text;
    for( int i=0; i<itemsSelected.size();i++)
    {
	if(itemsSelected.at(i)->type() == 65537)
	{
	    EllipseDuProjet *ellipse = (EllipseDuProjet*)itemsSelected.at(i);
	    text = this->textDisplay(ellipse->getMovement());
	}
	if(itemsSelected.at(i)->type() == 65539)
	{
	    Triangle *triangle = (Triangle*)itemsSelected.at(i);
	    text = this->textDisplay(triangle->getSampleAudio());
	}
	if(itemsSelected.at(i)->type() == 65538)
	{
	    Diamond *diamond = (Diamond*)itemsSelected.at(i);
	    text = this->textDisplay(diamond->getPort());
	}
	ui->textBrowser->setText(text);
    }
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
    for(int i = 0 ; i < movement->getListJointsMvt()->size() ; i++)
    {
	text.append("- ");
	text.append(movement->getListJointsMvt()->at(i)->getJointRef()->getNom());
	text.append("<br/>");
    }
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
    return text;
}

//Permet de selectionner les joints pour un nouveau mouvement
void MainWindow::slotUnlockStickMan(){
    //QMessageBox msgBox;
    //msgBox.setText("Veuillez selectionner les joints impliquÈs dans votre mouvement. Cliquez ensuite sur verrouiller");
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
	msgBox.setText("Aucuns joints selectionn�s \n Veuillez en selectionner au moins un.");
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
	//msgBox.setText("Vous avez verrouill� " + QString::number(nodesSelected.size()) + " joints:\n" + nomsDesJoints);
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
    //ui->pushButton_recordmouvement->setVisible(false);
    //ui->pushButton_stoprecord->setVisible(true);

    if(isRecording == false){
	ui->pushButton_recordmouvement->setStyleSheet("background:url(:/new/prefix1/images_boutons/stop.png)");
	//START RECORD
	isRecording = true;
    }
    else if(isRecording == true){
	ui->pushButton_recordmouvement->setStyleSheet("background:url(:/new/prefix1/images_boutons/recordgris.png)");
	ui->pushButton_recordmouvement->setEnabled(false);
	ui->nommouvement->setVisible(true);
	ui->pushButton_enregistrermouvement->setStyleSheet("background:url(:/new/prefix1/images_boutons/save.png)");
	ui->pushButton_enregistrermouvement->setEnabled(true);
	isRecording = false;
    }


}

void MainWindow::slotStopRecordNewMovement(){
    //ui->pushButton_stoprecord->setVisible(false);
    //ui->pushButton_enregistrermouvement->setVisible(true);
    //ui->nommouvement->setVisible(true);
}
void MainWindow::slotValidNewMovement(){
    movement->setName(ui->nommouvement->text());
    controller->getManagerElements()->addMovement(movement);
    this->refreshLeftTree();
    qDebug() << movement->getName() << endl;


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

void MainWindow::decocherCheckBoxLink()
{
    ui->checkBox->setCheckState(Qt::Unchecked);
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
	    this->controller->getManagerElements()->addSample(files.at(i).split("/").last(), files.at(i));
	}
    }
    emit refreshLeftTree();
}

void MainWindow::updateLCDTimer()
{
    int m, s;
    //s = controller->getPlayerDemo()->currentTime() / 1000;
    m = s/60;
    s = s%60;
    QString time = QString::number(m) + ":" + QString::number(s);
    emit emitTime(time);
}
