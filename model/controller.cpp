#include "controller.h"

Controller::Controller()
{
    this->managerJoints = new ManagerJoints();
    this->managerElements = new ManagerElements();
    this->playerlive = new SoundPlayer(32);
    this->playerdemo = new SoundPlayer(1);
    this->managerElements->sortMovements();
    //this->recordMovement();
    //this->analizeRecord();
}

void Controller::recordMovement(Movement *movement)
{
    // SERVEUR OSC
    Movement* move = new Movement("test"); // mouvement a enregistrer
    this->managerElements->getListMovements()->append(move); // ajouter le mouvement a la liste generale des mouvements
    JointMvt* j1 = new JointMvt(1,move->getId()); // jointmvt 1 du mouvement (joint ref : 1)
    JointMvt* j2 = new JointMvt(2,move->getId()); // jointmvt 2 du mouvement (joint ref : 1)
    JointMvt* j3 = new JointMvt(3,move->getId()); // jointmvt 3 du mouvement (joint ref : 1)
    JointMvt* j4 = new JointMvt(4,move->getId()); // jointmvt 4 du mouvement (joint ref : 1)
    j1->setJointRef(managerJoints->getListJoints()->at(0)); //lefthand
    //j2->setJointRef(managerJoints->getListJoints()->at(1)); //leftelbow
    //j3->setJointRef(managerJoints->getListJoints()->at(4)); //righthand
    //j4->setJointRef(managerJoints->getListJoints()->at(5)); //rightelbow
    move->addJointMvt(j1);

    //move->addJointMvt(j3);
    //move->addJointMvt(j4);

    ServerOSC *createMove = new ServerOSC(12345, false); //Serveur "ecouteur" de synapse
    createMove->setRunnable(true);
    createMove->setRecording(true); //mode record
    createMove->setListJoints(managerJoints->getListJoints());
    createMove->setListMovements(managerElements->getListMovements());
    createMove->start();
    //CLIENT OSC
    ClientOSC* client = new ClientOSC(12346, QString("localhost"), false);
    QList<MessageSynapse*>* msg = new QList<MessageSynapse*>();
    msg->append(new MessageSynapse(QString("/lefthand_trackjointpos"), 1));
    //msg->append(new MessageSynapse(QString("/leftelbow_trackjointpos"), 1));

    //msg->append(new MessageSynapse(QString("/righthand_trackjointpos"), 1));
    //msg->append(new MessageSynapse(QString("/rightelbow_trackjointpos"), 1));

    client->setMsgSynapse(msg);
    client->setRunnable(true);
    client->start();

    sleep(5);
    //Sleep(3000);
    createMove->setRunnable(false);

    troncage(move);



    for(int i=0;i<managerElements->getListMovements()->size();i++) {
        qDebug() << managerElements->getListMovements()->at(i)->getName() << endl;
        for(int j=0;j<managerElements->getListMovements()->at(i)->getListJointsMvt()->size();j++) {
            qDebug() << "Taille du joint mouvement "
                     << managerElements->getListMovements()->at(i)->getListJointsMvt()->at(j)->getJointRef()->getNom() << " :"
                     << managerElements->getListMovements()->at(i)->getListJointsMvt()->at(j)->getListPositions()->size()
                     << endl;
        }
    }


    managerElements->saveAll();
    managerElements->loadAll();
    this->linkJointToJointMvt();

    /**
      *AFFICHAGE
      **/
    for(int i=0;i<managerElements->getListMovements()->size();i++) {
        qDebug() << managerElements->getListMovements()->at(i)->getName() << endl;
        for(int j=0;j<managerElements->getListMovements()->at(i)->getListJointsMvt()->size();j++) {
            qDebug() << "Taille du joint mouvement "
                     << managerElements->getListMovements()->at(i)->getListJointsMvt()->at(j)->getJointRef()->getNom() << " :"
                     << managerElements->getListMovements()->at(i)->getListJointsMvt()->at(j)->getListPositions()->size()
                     << endl;
        }
    }

    for(int i=0;i<managerElements->getListMovements()->size();i++) {
        qDebug() << managerElements->getListMovements()->at(i)->getName() << endl;
        for(int j=0;j<managerElements->getListMovements()->at(i)->getListJointsMvt()->size();j++) {
            qDebug() << managerElements->getListMovements()->at(i)->getListJointsMvt()->at(j)->getJointRef()->getNom() << endl;
            for(int k=0; k<managerElements->getListMovements()->at(i)->getListJointsMvt()->at(j)->getListPositions()->size();k++){
                qDebug() << "Position : " << k << endl
                         << "X : " << managerElements->getListMovements()->at(i)->getListJointsMvt()->at(j)->getListPositions()->at(k)->getX() << endl
                         << "DX : " << managerElements->getListMovements()->at(i)->getListJointsMvt()->at(j)->getListPositions()->at(k)->getDx() << endl
                         << "Y : " << managerElements->getListMovements()->at(i)->getListJointsMvt()->at(j)->getListPositions()->at(k)->getY() << endl
                         << "DY : " << managerElements->getListMovements()->at(i)->getListJointsMvt()->at(j)->getListPositions()->at(k)->getDy() << endl
                         << "Z : " << managerElements->getListMovements()->at(i)->getListJointsMvt()->at(j)->getListPositions()->at(k)->getZ() << endl
                         << "DZ : " << managerElements->getListMovements()->at(i)->getListJointsMvt()->at(j)->getListPositions()->at(k)->getDz() << endl
                         << endl<<endl;
            }
        }
    }
}

void Controller::analizeRecord()
{
    //TEMPORAIRE **********////
    this->managerElements->loadAll();
    this->linkJointToJointMvt();


    // SERVEUR OSC
    ServerOSC *createMove = new ServerOSC(12345, false); //Serveur "ecouteur" de synapse
    createMove->setRunnable(true);
    createMove->setRecording(false); //mode analyze
    createMove->setAnalyse();
    createMove->setListJoints(managerJoints->getListJoints());
    createMove->setListMovements(managerElements->getListMovements());
    createMove->start();

    //CLIENT OSC
    ClientOSC* client = new ClientOSC(12346, QString("localhost"), false);
    QList<MessageSynapse*>* msg = new QList<MessageSynapse*>();
    msg->append(new MessageSynapse(QString("/lefthand_trackjointpos"), 1));
    msg->append(new MessageSynapse(QString("/leftelbow_trackjointpos"), 1));
    msg->append(new MessageSynapse(QString("/leftknee_trackjointpos"), 1));
    msg->append(new MessageSynapse(QString("/leftfoot_trackjointpos"), 1));
    msg->append(new MessageSynapse(QString("/righthand_trackjointpos"), 1));
    msg->append(new MessageSynapse(QString("/rightelbow_trackjointpos"), 1));
    msg->append(new MessageSynapse(QString("/rightknee_trackjointpos"), 1));
    msg->append(new MessageSynapse(QString("/rightfoot_trackjointpos"), 1));
    msg->append(new MessageSynapse(QString("/head_trackjointpos"), 1));
    msg->append(new MessageSynapse(QString("/torso_trackjointpos"), 1));

    client->setMsgSynapse(msg);
    client->setRunnable(true);
    client->start();

    // MacOSX :
    sleep(10);
    //Windows
    //Sleep(3000);
    createMove->setRunnable(false); // arreter l'analyse
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
        for(int j = 0 ; j < this->managerElements->getListJointsMvts()->size() ; j++)
            if(this->managerJoints->getListJoints()->at(i)->getId() ==  this->managerElements->getListJointsMvts()->at(j)->getIdJointRef())
                this->managerElements->getListJointsMvts()->at(j)->setJointRef(this->managerJoints->getListJoints()->at(i));
}

void Controller::bubble(QList<Movement*>* moves) {
    bool order = false;
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
    }
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

