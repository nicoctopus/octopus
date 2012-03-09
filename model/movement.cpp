#include "movement.h"
quint32 Movement::idMovementStatic = 0;


/**
  *   CONSTRUCTEURS
  **/
Movement::Movement() : Element()
{
    this->sampleAudio = NULL;
    this->sampleVideo = NULL;
    this->listClients = new QList<ClientOSC*>();
    this->listJointsMvt = new QList<JointMvt*>();
    this->active = false;
    this->idElement = ++idMovementStatic;

}
Movement::Movement(const Movement &movement) :Element(movement.idElement, movement.nameElement, movement.active, movement.posXBlackBoard, movement.posYBlackBoard)
{    
    this->sampleAudio = NULL;
    this->sampleVideo = NULL;
    this->listClients = movement.listClients;
    this->listJointsMvt = movement.listJointsMvt;

}

Movement::Movement(Movement *movement) :Element(movement->idElement, movement->nameElement, movement->active, movement->posXBlackBoard, movement->posYBlackBoard)
{
    qDebug() << "debut copie constructeur" << endl;
    this->sampleAudio = NULL;
    this->sampleVideo = NULL;
    this->listClients = movement->listClients;
    this->listJointsMvt = movement->listJointsMvt;
    qDebug() << "fin copie constructeur" << endl;
}

Movement::Movement(const QString &name) : Element(++idMovementStatic, name, false, posXBlackBoard, posYBlackBoard)
{
    this->sampleAudio = NULL;
    this->sampleVideo = NULL;
    this->listClients = new QList<ClientOSC*>();
    this->listJointsMvt = new QList<JointMvt*>();

}

Movement::Movement(const QString &name, const bool &active, const QList<JointMvt*> *listJointsMvt
           , SampleAudio *sa, SampleVideo *sv, QList<ClientOSC*> *lc) : Element(++idMovementStatic, name, active, posXBlackBoard, posYBlackBoard)
{
    this->active = active;
    this->sampleAudio = sa;
    this->sampleVideo = sv;
    this->listClients = lc;
    this->listJointsMvt = new QList<JointMvt*>(*listJointsMvt);

}


/**
  *    DESTRUCTEURS
  **/
Movement::~Movement(){
    /*for(int i = 0 ; i < this->listClients->size() ; i++)
        delete this->listClients->at(i);
    delete this->listClients;
    delete this->sampleAudio;
    delete this->sampleVideo;
    for(int i = 0 ; i < this->listJointsMvt->size() ; i++)
	delete this->listJointsMvt->at(i);
    delete(this->listJointsMvt);*/
}

/**
  *    GETTERS
  **/
QList<ClientOSC*>* Movement::getListClients()
{
    return this->listClients;
}

SampleAudio* Movement::getSampleAudio()
{
    return this->sampleAudio;
}

SampleVideo* Movement::getSampleVideo()
{
    return this->sampleVideo;
}

QList<JointMvt*>* Movement::getListJointsMvt()
{
    return this->listJointsMvt;
}

void Movement::setSampleAudio(SampleAudio *sampleAudio)
{
    this->sampleAudio = sampleAudio;
}

void Movement::setSampleVideo(SampleVideo *sampleVideo)
{
    this->sampleVideo = sampleVideo;
}

void Movement::addJointMvt(Joint *ref) {
    listJointsMvt->append(new JointMvt(ref->getId(), ref));
}

void Movement::addClientOSC(ClientOSC *clientOSC)
{
    this->listClients->append(clientOSC);
}

void Movement::addJointMvt(JointMvt *jointMvt) {
    listJointsMvt->append(jointMvt);
}

void Movement::updateId(const quint32 &id)
{
    int idTemp = this->idElement;
    this->idElement = id;
    if(this->sampleAudio != NULL)
    {
	this->sampleAudio->removeId(idTemp);
	this->sampleAudio->updateIdMovement(id);
    }
    if(this->sampleVideo != NULL)
    {
	this->sampleVideo->removeId(idTemp);
	this->sampleVideo->updateIdMovement(id);
    }
    if(this->listClients != NULL)
	for(int i = 0 ; i < this->listClients->size() ; i++)
	{
	    this->listClients->at(i)->removeIdMovement(idTemp);
	    this->listClients->at(i)->updateIdMovement(id);
	}
}

/**
  *    METHODES
  **/

/* Enregistrement du Movement */
void Movement::save()
{
    /* Pour chaque JointMvt, on recupere la derniere Position enregistree du Joint pere */
    foreach (JointMvt *jointMvt, *listJointsMvt) {
	jointMvt->savePositionFixe();
    }
}

const char* Movement::className()
{
    return "Movement";
}

/**
  *   FRIEND SERIALISATION
  **/
//Save
QDataStream & operator << (QDataStream &out, const Movement &valeur)
{
    //qDebug() << "Entree operator save Movement" << valeur.idElement << endl;
    out << valeur.idElement;
    out << valeur.posXBlackBoard;
    out << valeur.posYBlackBoard;
    out << valeur.nameElement;
    out << valeur.active;
    out << valeur.listJointsMvt->size();
    //qDebug() << "listpositionsize" << valeur.listJointsMvt.at(0)->getListPositions().size() << endl;
    for(int i = 0 ; i < valeur.listJointsMvt->size() ; i++)
	out << *(valeur.listJointsMvt->at(i));
    return out;
}

//Load
QDataStream & operator >> (QDataStream & in, Movement &valeur)
{
    int sizeTemp;
    in >> valeur.idElement;
    in >> valeur.posXBlackBoard;
    in >> valeur.posYBlackBoard;
    in >> valeur.nameElement;
    in >> valeur.active;
    in >> sizeTemp;
    //qDebug() << sizeTemp << endl;
    for(int i = 0 ; i < sizeTemp ; i++)
    {
	JointMvt temp;
	in >> temp;
	valeur.listJointsMvt->append(new JointMvt(temp));
    }
    //qDebug() << "fin operator load Movement" << valeur.idElement << endl;
    return in;
}
