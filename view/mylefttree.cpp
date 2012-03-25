#include "mylefttree.h"


//------------------------------------------------
//                   CONSTRUCTEUR
//------------------------------------------------
MyLeftTree::MyLeftTree(QWidget *parent) : QTreeWidget(parent)
{

    movements = new QTreeWidgetItem(this);
    movements->setText(0,"Movements");
    samples = new QTreeWidgetItem(this);
    samples->setText(0,"Samples");
    ports = new QTreeWidgetItem(this);
    ports->setText(0,"Ports");
    videos = new QTreeWidgetItem(this);
    videos->setText(0,"Videos");
    this->header()->close();
    this->createActions();

    this->setHeaderLabel(" ");


    this->setDragEnabled( true );
    this->setDropIndicatorShown(true);

}


//------------------------------------------------
//                   SLOTS
//------------------------------------------------
void MyLeftTree::refresh(){

    this->movements->takeChildren();
    this->samples->takeChildren();
    this->ports->takeChildren();

    for(int i = 0 ;  i < this->listMovements->size() ; i++)
    {
	QTreeWidgetItem *treeWidgetItem = new QTreeWidgetItem(movements);
	treeWidgetItem->setText(0, this->listMovements->at(i)->getName());
	//On sauvegarde l'item dans une map en fct de son ID pour pouvoir facilement le retrouver et le modifier plus tard
	//itemsLeftMap.insert(it.value()->getId(), treeWidgetItem);
	mapTreeItemsMovement.insert(treeWidgetItem,this->listMovements->at(i));
    }
    for(int i = 0 ;  i < this->listSamplesAudio->size() ; i++)
    {
	QTreeWidgetItem *treeWidgetItem = new QTreeWidgetItem(this->samples);
	treeWidgetItem->setText(0, this->listSamplesAudio->at(i)->getName());
	//On sauvegarde l'item dans une map en fct de son ID pour pouvoir facilement le retrouver et le modifier plus tard
	//itemsLeftMap.insert(it.value()->getId(), treeWidgetItem);
	mapTreeItemsSample.insert(treeWidgetItem,this->listSamplesAudio->at(i));
    }
    for(int i = 0 ;  i < this->listPorts->size() ; i++)
    {
	QTreeWidgetItem *treeWidgetItem = new QTreeWidgetItem(this->ports);
	treeWidgetItem->setText(0, this->listPorts->at(i)->getName());
	//On sauvegarde l'item dans une map en fct de son ID pour pouvoir facilement le retrouver et le modifier plus tard
	//itemsLeftMap.insert(it.value()->getId(), treeWidgetItem);
	mapTreeItemsPort.insert(treeWidgetItem,this->listPorts->at(i));
    }
}

//Slots qui met à jour le LeftTree



//------------------------------------------------
//                   Drag/Drop
//------------------------------------------------
void MyLeftTree::dragEnterEvent(QDragEnterEvent *e) {
    MyLeftTree *source = qobject_cast<MyLeftTree  *>(e->source());
    QTreeWidgetItem *item = source->currentItem();
    //cout << "nombre de colonnes " << item->columnCount() << endl;
    if(item->columnCount() == 1){//c'est un item parent
	e->ignore();
    }
    else{//c'est un item que l'on peut dragguer
	e->accept();
    }
}

void MyLeftTree::dragMoveEvent(QDragMoveEvent *e) {
    e->accept();
}

QMimeData *MyLeftTree::mimeData(const QList<QTreeWidgetItem *> items) const
{
    QByteArray ba;
    QDataStream ds(&ba, QIODevice::WriteOnly);
    QMimeData *md = new QMimeData;


    for (int i = 0 ; i < items.size() ; i++){

	//Blindage
	if(items.at(i)->text(0) == "Movements" || items.at(i)->text(0) == "Samples" || items.at(i)->text(0) == "Ports" || items.at(i)->text(0) == "Videos"){
	    return NULL;
	}

	md->setText(items.at(i)->parent()->text(0));
	if(items.at(i)->parent()->text(0) == "Movements")
	{
	    Movement *temp = mapTreeItemsMovement.value(items.at(i));
	    QByteArray data((const char*)&temp, sizeof(temp));
	    md->setData("foo/bar", data);
	}
	else if(items.at(i)->parent()->text(0) == "Samples")
	{
	    SampleAudio *temp = mapTreeItemsSample.value(items.at(i));
	    QByteArray data((const char*)&temp, sizeof(temp));
	    md->setData("foo/bar", data);
	}
	else if(items.at(i)->parent()->text(0) == "Ports")
	{
	    ClientOSC *temp = mapTreeItemsPort.value(items.at(i));
	    QByteArray data((const char*)&temp, sizeof(temp));
	    md->setData("foo/bar", data);
	}
    }
    return md;

}

void MyLeftTree::contextMenuEvent(QContextMenuEvent *event)
{
    if(this->currentItem()->parent())
    {
	QMenu menu(this);
	menu.addAction(this->actionRemove);

	menu.exec(event->globalPos());
	this->setCurrentItem(NULL);
    }
}

void MyLeftTree::createActions()
{
    this->actionRemove = new QAction(tr("&Remove"), this);
    this->actionRemove->setShortcut(tr("Ctrl+R"));
    this->actionRemove->setStatusTip(tr("Remove the object"));
    connect(this->actionRemove, SIGNAL(triggered()), this, SLOT(slotRemove()));
}

void MyLeftTree::slotRemove()
{
    if(this->currentItem()->parent()->text(0) == "Movements")
    {
	Movement *movement = this->mapTreeItemsMovement.value(this->currentItem());
	emit remove(movement);
    }
    else if(this->currentItem()->parent()->text(0) == "Samples")
    {
	SampleAudio *sampleAudio = this->mapTreeItemsSample.value(this->currentItem());
	emit remove(sampleAudio);
    }
    else if(this->currentItem()->parent()->text(0) == "Ports")
    {
	ClientOSC *clientOSC = this->mapTreeItemsPort.value(this->currentItem());
	emit remove(clientOSC);
    }
}



void MyLeftTree::setListMovements(QList<Movement*>* listMovements)
{
    this->listMovements = listMovements;
}

void MyLeftTree::setListPorts(QList<ClientOSC*> *listPorts)
{
    this->listPorts = listPorts;
}

void MyLeftTree::setListSamplesAudio(QList<SampleAudio*> *listSamplesAudio)
{
    this->listSamplesAudio = listSamplesAudio;
}

QMap<QTreeWidgetItem*, Movement*> MyLeftTree::getMapTreeItemsMovement(){
    return mapTreeItemsMovement;
}

QMap<QTreeWidgetItem*, SampleAudio*> MyLeftTree::getMapTreeItemsSample(){
    return mapTreeItemsSample;
}

QMap<QTreeWidgetItem*, ClientOSC*> MyLeftTree::getMapTreeItemsPort(){
    return mapTreeItemsPort;
}



