#include "controller.h"

Controller::Controller()
{
    this->managerJoints = new ManagerJoints();
    this->managerElements = new ManagerElements();
    this->linkJointToJointMvt();

    this->playerdemo = new SoundPlayer(1);

   // this->managerElements->getManagerMovements()->getListMovements()->swap(0,1);
   /* for(int i = 0 ; i < this->managerElements->getManagerMovements()->getListMovements()->size() ; i++)
	qDebug() << this->managerElements->getManagerMovements()->getListMovements()->at(i)->getListJointsMvt()->at(0)->getListPositions()->size() << endl;

    for(int i = 0 ; i < this->managerElements->getManagerMovements()->getListMovements()->size() ; i++)
	qDebug() << "Après Bubble : " << this->managerElements->getManagerMovements()->getListMovements()->at(i)->getListJointsMvt()->at(0)->getListPositions()->size() << endl;
*/
    /**
      *AFFICHAGE des infos sur le MVT
      **
    qDebug() <<  this->managerElements->getManagerMovements()->getListMovements()->at(0)->getName() << endl;
    for(int j=0;j<this->managerElements->getManagerMovements()->getListMovements()->at(0)->getListJointsMvt()->size();j++) {
	qDebug() << "Taille du joint mouvement "
		 << this->managerElements->getManagerMovements()->getListMovements()->at(0)->getListJointsMvt()->at(j)->getJointRef()->getNom() << " :"
		 << this->managerElements->getManagerMovements()->getListMovements()->at(0)->getListJointsMvt()->at(j)->getListPositions()->size()
		 << endl;
    }

    qDebug() << this->managerElements->getManagerMovements()->getListMovements()->at(0)->getName() << endl;
    for(int j=0;j<this->managerElements->getManagerMovements()->getListMovements()->at(0)->getListJointsMvt()->size();j++) {
	qDebug() << this->managerElements->getManagerMovements()->getListMovements()->at(0)->getListJointsMvt()->at(j)->getJointRef()->getNom() << endl;
	for(int k=0; k<this->managerElements->getManagerMovements()->getListMovements()->at(0)->getListJointsMvt()->at(j)->getListPositions()->size();k++){
	    qDebug() << "Position : " << k << endl
		     << "X : " << this->managerElements->getManagerMovements()->getListMovements()->at(0)->getListJointsMvt()->at(j)->getListPositions()->at(k)->getX() << endl
		     << "DX : " << this->managerElements->getManagerMovements()->getListMovements()->at(0)->getListJointsMvt()->at(j)->getListPositions()->at(k)->getDx() << endl
		     << "Y : " << this->managerElements->getManagerMovements()->getListMovements()->at(0)->getListJointsMvt()->at(j)->getListPositions()->at(k)->getY() << endl
		     << "DY : " << this->managerElements->getManagerMovements()->getListMovements()->at(0)->getListJointsMvt()->at(j)->getListPositions()->at(k)->getDy() << endl
		     << "Z : " << this->managerElements->getManagerMovements()->getListMovements()->at(0)->getListJointsMvt()->at(j)->getListPositions()->at(k)->getZ() << endl
		     << "DZ : " << this->managerElements->getManagerMovements()->getListMovements()->at(0)->getListJointsMvt()->at(j)->getListPositions()->at(k)->getDz() << endl
		     << endl<<endl;
	}
    }
    /**
      *
      **/

    this->serveurOSC = new ServerOSC(12345, false); //Serveur "ecouteur" de synapse

    this->client = new ClientOSC(12346, QString("localhost"), false);



}

void Controller::stopRecord(Movement *movement)
{
    this->serveurOSC->setRunnable(false);
    this->client->setRunnable(false);

    this->troncage(movement);
    /**
      *AFFICHAGE des infos sur le MVT
      **/
    qDebug() << movement->getName() << endl;
    for(int j=0;j<movement->getListJointsMvt()->size();j++) {
	qDebug() << "Taille du joint mouvement "
		 << movement->getListJointsMvt()->at(j)->getJointRef()->getNom() << " :"
		 << movement->getListJointsMvt()->at(j)->getListPositions()->size()
		 << endl;
    }

    qDebug() << movement->getName() << endl;
    for(int j=0;j<movement->getListJointsMvt()->size();j++) {
	qDebug() << movement->getListJointsMvt()->at(j)->getJointRef()->getNom() << endl;
	for(int k=0; k<movement->getListJointsMvt()->at(j)->getListPositions()->size();k++){
            qDebug() << "Position : " << k << endl;
		qDebug()     << "X : " << movement->getListJointsMvt()->at(j)->getListPositions()->at(k)->getX() << endl;
		qDebug()     << "Y : " << movement->getListJointsMvt()->at(j)->getListPositions()->at(k)->getY() << endl;
		qDebug()     << "Z : " << movement->getListJointsMvt()->at(j)->getListPositions()->at(k)->getZ() << endl;
		qDebug()     << endl<<endl;
	}
    }
}

void Controller::recordMovement(Movement *movement)
{
    /**
      *   SERVEUR OSC
      **/

    this->serveurOSC->setRunnable(true);
    this->serveurOSC->setRecording(true); //mode record
    this->serveurOSC->setListJoints(managerJoints->getListJoints());
    this->serveurOSC->setMovement(movement);
    this->serveurOSC->start();

    /**
      *  CLIENT OSC
      **/


    //Chargement de la liste des messages
    QList<MessageSynapse*>* msg = new QList<MessageSynapse*>();
    for(int i = 0 ; i < movement->getListJointsMvt()->size() ; i++)
	msg->append(new MessageSynapse(movement->getListJointsMvt()->at(i)->getJointRef()->getMessageSynapse(), 1));
    this->client->setMsgSynapse(msg);
    this->client->setRunnable(true);
    this->client->start();

}

void Controller::analizeRecord()
{

    // SERVEUR OSC

    this->serveurOSC->setRunnable(true);
    this->serveurOSC->setRecording(false); //mode analyze
    this->serveurOSC->setAnalyse();
    this->serveurOSC->setListJoints(managerJoints->getListJoints());
    this->managerElements->getManagerMovements()->sortMovementsByPositionSize();
    this->serveurOSC->setListMovements(managerElements->getManagerMovements()->getListMovementsActive());
    this->serveurOSC->start();

    //CLIENT OSC

    QList<MessageSynapse*>* msg = new QList<MessageSynapse*>();
    for(int i = 0 ; i < this->managerJoints->getListJoints()->size() ; i++)
        msg->append(new MessageSynapse(this->managerJoints->getListJoints()->at(i)->getMessageSynapse(), 1));

    this->client->setMsgSynapse(msg);
    this->client->setRunnable(true);
    this->client->start();

}

void Controller::stopAnalize(){

    qDebug() << "STOP ANALYSE !! " << endl;

    this->serveurOSC->setRunnable(false);
    this->client->setRunnable(false);

}

ManagerJoints* Controller::getManagerJoints()
{
    return this->managerJoints;
}

ManagerElements* Controller::getManagerElements()
{
    return this->managerElements;
}

SoundPlayer * Controller::getPlayerDemo() {

    return this->playerdemo;
}

void Controller::linkJointToJointMvt()
{
    for(int i = 0 ; i < this->managerJoints->getListJoints()->size() ; i++)
	for(int j = 0 ; j < this->managerElements->getManagerMovements()->getListMovements()->size() ; j++)
	    for(int k = 0 ; k < this->managerElements->getManagerMovements()->getListMovements()->at(j)->getListJointsMvt()->size() ; k++)
		if(this->managerJoints->getListJoints()->at(i)->getId() ==  this->managerElements->getManagerMovements()->getListMovements()->at(j)->getListJointsMvt()->at(k)->getIdJointRef())
		    this->managerElements->getManagerMovements()->getListMovements()->at(j)->getListJointsMvt()->at(k)->setJointRef(this->managerJoints->getListJoints()->at(i));
}

void Controller::bubble(QList<Movement*>* moves) {
    /*bool order = false;
    int size = moves->size();
    while (!order) {
	order = true;
	for (int i=0;i<size-1;i++) {
	    int s1 = moves->at(i)->getListJointsMvt()->at(0)->getListPositions()->size();
	    int s2 = moves->at(i+1)->getListJointsMvt()->at(0)->getListPositions()->size();
	    //qDebug() << s1 << s2 << endl;
	    if(s1 < s2) {
		moves->swap(i,i+1);
		order = false;
	    }
	}
	size --;
    }*/
    for(int i = 0 ; i < moves->size() - 1 ; i++)
	for(int j = 0 ; j < moves->size() ; j++)
	    if(moves->at(i)->getListJointsMvt()->at(0)->getListPositions()->size()
		    < moves->at(j)->getListJointsMvt()->at(0)->getListPositions()->size())
		moves->swap(i, j);
}

void Controller::troncage(Movement* movement) {
    // enregistrer en fonction du jointmvt ayant le moins de positions
    int size = movement->getListJointsMvt()->at(0)->getListPositions()->size();
    for(int i=0; i<movement->getListJointsMvt()->size();i++) {
	if(movement->getListJointsMvt()->at(i)->getListPositions()->size() < size)
	    size = movement->getListJointsMvt()->at(i)->getListPositions()->size();
    }
    for(int j = 0 ; j < movement->getListJointsMvt()->size() ; j++)
    {
	//parcourir de la fin jusqu'a la position SIZE correspondant a l'indice max du tableau de positions le plus petit
	// et supprimer les cases de l'intervalle
	for(int k=movement->getListJointsMvt()->at(j)->getListPositions()->size() ; k >size  ; k--){
	    movement->getListJointsMvt()->at(j)->getListPositions()->removeLast();
	}
    }
}

ServerOSC* Controller::getServerOsc()
{
    return this->serveurOSC;
}

