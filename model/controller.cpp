#include "controller.h"

Controller::Controller()
{
    this->managerJoints = new ManagerJoints();
    this->managerElements = new ManagerElements();
    this->linkJointToJointMvt();
    this->playerlive = new SoundPlayer(32);
    this->playerdemo = new SoundPlayer(1);

    //this->managerElements->sortMovements();

    /**
      * LOAD
      **
    this->managerElements->loadAll();
    this->linkJointToJointMvt();

    for(int i=0; i<managerElements->getListJointsMvts()->size();i++) {
        qDebug() << "jointMvt " << i << " - nb positions : " << managerElements->getListJointsMvts()->at(i)->getListPositions()->size() << endl;
    }

    for(int i=0;i<managerElements->getListMovements()->size();i++) {
        qDebug() << managerElements->getListMovements()->at(i)->getName() << endl;
        for(int j=0;j<managerElements->getListMovements()->at(i)->getListJointsMvt()->size();j++) {
            qDebug() << "Taille du joint mouvement "
                     << managerElements->getListMovements()->at(i)->getListJointsMvt()->at(j)->getJointRef()->getNom() << " :"
                     << managerElements->getListMovements()->at(i)->getListJointsMvt()->at(j)->getListPositions()->size()
                     << endl;
        }
    }

    /**
      * FIN LOAD
      **/

    /**
      * TEST ENREGISTREMENT D'UN MOUVEMENT
      **
    // SERVEUR OSC
    ServerOSC *createMove = new ServerOSC(12345); //Serveur "ecouteur" de synapse
    Movement* move = new Movement("test"); // mouvement a enregistrer
    JointMvt* j1 = new JointMvt(1,move->getId()); // jointmvt 1 du mouvement (joint ref : 1)
    JointMvt* j2= new JointMvt(2,move->getId()); //jointmvt 2 du mouvement (joint ref : 2)
    j1->setJointRef(managerJoints->getListJoints()->at(0));
    j2->setJointRef(managerJoints->getListJoints()->at(1));
    move->addJointMvt(j1);
    move->addJointMvt(j2);
    this->managerElements->getListMovements()->append(move); // ajouter le mouvement a la liste generale des mouvements

    createMove->setRunnable(true);
    createMove->setRecording(true); //mode record
    createMove->setListJoints(managerJoints->getListJoints());
    createMove->setListMovements(managerElements->getListMovements());
    createMove->start();

    //CLIENT OSC
    ClientOSC* client = new ClientOSC(12346, QString("localhost"));
    QList<MessageSynapse*>* msg = new QList<MessageSynapse*>();
    msg->append(new MessageSynapse(QString("/lefthand_trackjointpos"), 1));
    msg->append(new MessageSynapse(QString("/leftelbow_trackjointpos"), 1));

    client->setMsgSynapse(msg);
    client->setRunnable(true);
    client->start();
    Sleep(3000);
    createMove->setRunnable(false);
    managerElements->saveAll();
    /**
      * FIN TEST ENREGISTREMENT
      **/
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

SoundPlayer * Controller::getPlayerLive() {

    return this->playerlive;
}


void Controller::linkJointToJointMvt()
{
    for(int i = 0 ; i < this->managerJoints->getListJoints()->size() ; i++)
	if(this->managerElements->getListJointsMvts() != NULL)
	{
	    for(int j = 0 ; j < this->managerElements->getListJointsMvts()->size() ; j++)
		if(this->managerJoints->getListJoints()->at(i)->getId() ==  this->managerElements->getListJointsMvts()->at(j)->getIdJointRef())
		    this->managerElements->getListJointsMvts()->at(j)->setJointRef(this->managerJoints->getListJoints()->at(i));
	}
}

