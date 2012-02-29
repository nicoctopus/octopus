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
Movement::Movement(const Movement &movement) :Element(movement.idElement, movement.nameElement, movement.active)
{
    this->listJointsMvt = new QList<JointMvt*>();
    for(int i = 0 ; i < movement.listJointsMvt->size() ; i++)
	this->listJointsMvt->append(new JointMvt(*(movement.listJointsMvt->at(i))));
    //this->sampleAudio = new SampleAudio(*(movement.sampleAudio));
    //this->sampleVideo = new SampleVideo(*(movement.sampleVideo));
    this->sampleAudio = NULL;
    this->sampleVideo = NULL;
    this->listClients = new QList<ClientOSC*>();
    this->listClients->append(*(movement.listClients));
}

Movement::Movement(const QString &name) : Element(++idMovementStatic, name, false)
{
    this->sampleAudio = NULL;
    this->sampleVideo = NULL;
    this->listJointsMvt = new QList<JointMvt*>();
    this->listClients = new QList<ClientOSC*>();
}

Movement::Movement(const QString &name, const bool &active, QList<JointMvt*> *listJointsMvt
	   , SampleAudio *sa, SampleVideo *sv, QList<ClientOSC*> *lc) : Element(++idMovementStatic, name, active)
{
    this->active = active;
    this->sampleAudio = sa;
    this->sampleVideo = sv;
    this->listClients = lc;
    this->listJointsMvt = listJointsMvt;
}


/**
  *    DESTRUCTEURS
  **/
Movement::~Movement(){
    for(int i = 0 ; i < this->listClients->size() ; i++)
        delete this->listClients->at(i);

    for(int j = 0 ; j < this->listJointsMvt->size() ; j++)
        delete this->listJointsMvt->at(j);

    delete this->listClients;
    delete this->listJointsMvt;
    delete this->sampleAudio;
    delete this->sampleVideo;

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

QList<JointMvt*>* Movement::getListJointsMvt() {
    return listJointsMvt;
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
    listJointsMvt->append(new JointMvt(ref->getId(), this->idElement, ref));
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
    for(int i = 0 ; i < this->listJointsMvt->size() ; i++)
	this->listJointsMvt->at(i)->updateIdMovement(id);
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
    //std::cout << "Entree operator << Movement" << std::endl;
    out << valeur.idElement << valeur.nameElement << valeur.active /*<< static_cast<QList <ClientOSC*> >(*(valeur.listClients))*/;
;
    return out;
}

//Save
QDataStream & operator << (QDataStream &out, Movement *valeur)
{
    out.writeRawData((char*)&valeur, sizeof(valeur));
    //std::cout << "Entree operator << Movement" << std::endl;
    return out;
}

//Load
QDataStream & operator >> (QDataStream &in, Movement *&valeur)
{
    in.readRawData((char*)&valeur, sizeof(valeur));
    //std::cout << "Entree operator << Movement" << std::endl;
    return in;
}

//Load
QDataStream & operator >> (QDataStream & in, Movement &valeur)
{
    //std::cout << "Entree operator >> Movement" << std::endl;
    in >> valeur.idElement;
    in >> valeur.nameElement;
    in >> valeur.active;
    /*QList<ClientOSC> *list = new QList<ClientOSC>();
    in >> *list;
    valeur.getListClients()->append(*list);*/
    return in;
}
