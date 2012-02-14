#include "managerelements.h"

ManagerElements::ManagerElements() : Manager()
{
    //Initialisation de la serialisation des joints
    this->initSystem();
    this->listMovements = new QList<Movement*>();
    this->listSamplesVideos = new QList<SampleVideo*>();
    this->managerClientOSC = new ManagerClientOSC();
    this->managerSampleAudio = new ManagerSampleAudio();
    this->managerJointMvt = new ManagerJointMvt();

    /**
      *
      **/
    this->managerSampleAudio->getListSamplesAudios()->append(new SampleAudio("01 By the Way", "../../../01 By the Way.mp3", 0, false));
    this->managerSampleAudio->getListSamplesAudios()->append(new SampleAudio("03 Scar Tissue", "../../../03 Scar Tissue.mp3", 0, false));
    this->managerClientOSC->getListClientsOSC()->append(new ClientOSC(1234, "localhost", false));
    this->managerClientOSC->getListClientsOSC()->append(new ClientOSC(5678, "localhost", false));
    /**
      *
      **/
    //this->saveAll();
    this->loadAll();
    /**
      *
      **/
}

/**
  *   FONCTIONS LOAD ET SAVE GENERALES
  **/
void ManagerElements::loadAll()
{
    std::cout << "   debut loadAll()..." << std::endl;
    this->loadSamplesVideos();
    this->loadAllMovements();
    this->managerClientOSC->loadAll();
    this->managerSampleAudio->loadAll();
    this->managerJointMvt->loadAll();
    qDebug() << "   ...fin loadAll()" << endl;

}

void ManagerElements::saveAll()
{
    qDebug() << "   debut saveAll()..." << endl;
    QFile::remove("movement.ini");
    QFile::remove("position.ini");
    QFile::remove("jointmvt.ini");
    QFile::remove("sampleaudio.ini");
    QFile::remove("samplevideo.ini");
    QFile::remove("clientOSC.ini");
    this->managerClientOSC->saveAll();
    this->managerSampleAudio->saveAll();
    this->managerJointMvt->saveAll();
    this->saveAllMovements();
    this->saveAllSamplesVideos();
    qDebug() << "   ...fin saveAll()" << endl;

}

/**
  *  FONCTIONS LOAD ET SAVE DE TOUT LES MOVEMENTS
  **/
void ManagerElements::saveAllMovements()
{
    qDebug() << "      debut saveAllMovements()..." << endl;
    QSettings fichierMovement("movement.ini", QSettings::IniFormat);

    for(int i = 0 ; i < this->listMovements->size() ; i++)
        this->saveMovement(this->listMovements->at(i), fichierMovement);
    fichierMovement.sync();
    qDebug() << "      ...fin saveAllMovements()" << endl;
}

/**
  *  FONCTIONS LOAD ET SAVE ET DELETE D'UN MOVEMENT
  **/
void ManagerElements::loadAllMovements()
{
    //qDebug() << "      debut loadMovements()..." << endl;
    //CHARGEMENT LISTE MOVEMENTS
    this->listMovements->clear();
    QSettings fichierMovement("movement.ini", QSettings::IniFormat);
    for(int i = 0 ; i < fichierMovement.allKeys().size() ; i++)
        this->listMovements->append(new Movement(fichierMovement.value(fichierMovement.allKeys().at(i), qVariantFromValue(Movement())).value<Movement>()));

    //CHARGEMENT DE LA LISTE TEMPORAIRE DES JOINTS MOUVEMENTS ET ASSIGNEMENT DE CHACUN DANS LEUR MOUVEMENT RESPECTIF
    // CHARGEMENT DE LA LISTE TEMPORAIRE DES POSITIONS ET ASSIGNEMENT DE CHACUN DANS LEUR JOINT MOUVEMENT RESPECTIF
    this->managerJointMvt->loadAll();

    for(int i = 0 ; i < this->listMovements->size() ; i++)
    {
	for(int j = 0 ; j < this->managerJointMvt->getListJointsMvts()->size() ; j++)
	{
	    JointMvt *jointMvtTemp = this->managerJointMvt->getListJointsMvts()->at(j);
	    if(this->listMovements->at(i)->getId() == jointMvtTemp->getIdMovement())
	    {
		for(int k = 0 ; k < this->managerJointMvt->getManagerPosition()->getListPositionTemp()->size() ; k++)
		{
		    if(jointMvtTemp->getIdJointMvt() == this->managerJointMvt->getManagerPosition()->getListPositionTemp()->at(k)->getIdJointMvt())
			jointMvtTemp->addPosition(this->managerJointMvt->getManagerPosition()->getListPositionTemp()->at(k));
		}
		this->listMovements->at(i)->addJointMvt(this->managerJointMvt->getListJointsMvts()->at(j));
	    }
	}
	for(int j = 0 ; j < this->managerSampleAudio->getListSamplesAudios()->size() ; j++)
	    if(this->listMovements->at(i)->getId() == this->managerSampleAudio->getListSamplesAudios()->at(j)->getIdMovement())
	    {
		this->listMovements->at(i)->setSampleAudio(this->managerSampleAudio->getListSamplesAudios()->at(j));
		this->listMovements->at(i)->setActive(true);
		this->managerSampleAudio->getListSamplesAudios()->at(j)->setActive(true);
		qDebug() << "link movment sample" << this->listMovements->at(i)->getName() << this->managerSampleAudio->getListSamplesAudios()->at(j)->getName() << endl;
	    }
	for(int j = 0 ; j < this->listSamplesVideos->size() ; j++)
	    if(this->listMovements->at(i)->getId() == this->listSamplesVideos->at(j)->getIdMovement())
		this->listMovements->at(i)->setSampleVideo(this->listSamplesVideos->at(j));
    }
    if(this->managerJointMvt->getManagerPosition()->getListPositionTemp()->isEmpty())
        Position::idPositionsStatic = 0;
    else{
        int tmp = 0;
	for(int i=0; i<managerJointMvt->getManagerPosition()->getListPositionTemp()->size();i++)
	    if (tmp < managerJointMvt->getManagerPosition()->getListPositionTemp()->at(i)->getId()) tmp = managerJointMvt->getManagerPosition()->getListPositionTemp()->at(i)->getId();
        Position::idPositionsStatic = tmp;
    }
    if(this->managerJointMvt->getListJointsMvts()->isEmpty())
        JointMvt::idJointMvtStatic = 0;
    else{
        int tmp = 0;
	for(int i=0; i<managerJointMvt->getListJointsMvts()->size();i++)
	    if(tmp < managerJointMvt->getListJointsMvts()->at(i)->getIdJointMvt()) tmp = managerJointMvt->getListJointsMvts()->at(i)->getIdJointMvt();
        JointMvt::idJointMvtStatic = tmp;

    }if(this->listMovements->isEmpty())
        Movement::idMovementStatic = 0;
    else {
        int tmp = 0;
        for(int i=0; i<listMovements->size(); i++)
            if(tmp < listMovements->at(i)->getId()) tmp = listMovements->at(i)->getId();
        Movement::idMovementStatic = tmp;
    }if(this->managerSampleAudio->getListSamplesAudios()->isEmpty())
        SampleAudio::idSampleAudioStatic = 0;
    else {
        int tmp = 0;
	for(int i=0; i<this->managerSampleAudio->getListSamplesAudios()->size(); i++)
	    if(tmp < this->managerSampleAudio->getListSamplesAudios()->at(i)->getId())
		tmp = this->managerSampleAudio->getListSamplesAudios()->at(i)->getId();
        SampleAudio::idSampleAudioStatic = tmp;
    }if(this->listSamplesVideos->isEmpty())
        SampleVideo::idSampleVideoStatic = 0;
    else {
        int tmp = 0;
        for(int i=0; i<listSamplesVideos->size(); i++)
            if(tmp < listSamplesVideos->at(i)->getId()) tmp = listSamplesVideos->at(i)->getId();
        SampleVideo::idSampleVideoStatic = tmp;
    }
}

void ManagerElements::saveMovement(Movement *movement, QSettings &fichierMovement)
{

    QSettings fichierSampleAudio("sampleaudio.ini", QSettings::IniFormat);
    QSettings fichierSampleVideo("samplevideo.ini", QSettings::IniFormat);

    this->managerJointMvt->save(movement->getListJointsMvt());

    if(movement->getSampleAudio() != NULL)
	this->managerSampleAudio->save(movement->getSampleAudio(), fichierSampleAudio);
    if(movement->getSampleVideo() != NULL)
	this->saveSampleVideo(movement->getSampleVideo(), fichierSampleVideo);
    QString key = QString::number(movement->getId());
    fichierMovement.setValue(key, qVariantFromValue(*movement));
    fichierSampleAudio.sync();
    fichierSampleVideo.sync();
}

void ManagerElements::removeMovement(Movement *movement)
{
    qDebug() << "         debut removeMovement..." << endl;
    quint32 idTemp = movement->getId();
    this->managerJointMvt->remove(movement->getListJointsMvt());
    for(int i = 0 ;  i < movement->getListJointsMvt()->size() ; i++)
        movement->getListJointsMvt()->removeAt(i);

    QSettings fichierMovement("movement.ini", QSettings::IniFormat);

    //On supprime le mouvement du fichier de serialisation
    fichierMovement.remove(QString::number(movement->getId()));
    //On supprime le dernier mouvement de la liste de mouvement du fichier de serialisation
    fichierMovement.remove(QString::number(this->getListMovements()->last()->getId()));
    //ON SUPPRIME LE MOUVEMENT ET SES COMPOSANTES de la liste
    for(int i = 0 ; i < this->listMovements->size() ; i++)
        if(this->listMovements->at(i)->getId() == movement->getId())
        {
            delete(this->listMovements->at(i));
            this->listMovements->removeAt(i);
        }
    //On met a jour l'id du denier mouvement de la liste
    this->getListMovements()->last()->updateId(idTemp);
    //On le sauvegarde update
    this->saveMovement(this->listMovements->last(), fichierMovement);
    fichierMovement.sync();

    Movement::idMovementStatic--;
    qDebug() << "         ...fin removeMovement" << endl;
}

/**
  *   FONCTIONS LOAD ET DELETE ET SAVE SAMPLEVIDEO
  **/

void ManagerElements::saveAllSamplesVideos()
{
    QSettings fichierSampleVideo("samplevideo.ini", QSettings::IniFormat);
    for (int i = 0 ; i < this->listSamplesVideos->size() ; i++)
	this->saveSampleVideo(this->listSamplesVideos->at(i), fichierSampleVideo);
    fichierSampleVideo.sync();
}

void ManagerElements::saveSampleVideo(SampleVideo *sampleVideo, QSettings &fichierSampleVideo)
{
    QString key = QString::number(sampleVideo->getId());
    fichierSampleVideo.setValue(key, qVariantFromValue(*sampleVideo));
}

void ManagerElements::loadSamplesVideos()
{
    this->listSamplesVideos->clear();

    QSettings fichierSampleVideo("samplevideo.ini", QSettings::IniFormat);
    for(int i = 0 ; i < fichierSampleVideo.allKeys().length() ; i++)
    {
        SampleVideo *temp = new SampleVideo(fichierSampleVideo.value(fichierSampleVideo.allKeys().at(i), qVariantFromValue(SampleVideo())).value<SampleVideo>());
        this->listSamplesVideos->append(temp);
    }
}

void ManagerElements::removeSampleVideo(SampleVideo *sampleVideo)
{
    QSettings fichierSampleVideo("samplevideo.ini", QSettings::IniFormat);
    fichierSampleVideo.remove(QString::number(sampleVideo->getId()));
    fichierSampleVideo.remove(QString::number(this->listSamplesVideos->last()->getId()));
    this->listSamplesVideos->last()->updateId(sampleVideo->getId());
    this->saveSampleVideo(this->listSamplesVideos->last(), fichierSampleVideo);
    SampleVideo::idSampleVideoStatic--;
    fichierSampleVideo.sync();
}


//Initialisation du systeme pour la serialisation
void ManagerElements::initSystem()
{
    qRegisterMetaTypeStreamOperators<Movement>("Movement");
    qMetaTypeId<Movement>();
    qRegisterMetaTypeStreamOperators<SampleVideo>("SampleVideo");
    qMetaTypeId<SampleVideo>();
}

/**
  *    GETTERS
  **/

QList<Movement*>* ManagerElements::getListMovements()
{
    return this->listMovements;
}

QList<Movement*>* ManagerElements::getListMovementsActive()
{
    QList<Movement*>*listMovementActive = new QList<Movement*>();
    for(int i = 0 ; i < this->listMovements->size() ; i++)
	if(this->listMovements->at(i)->isActive() == true)
	    listMovementActive->append(this->listMovements->at(i));
    return listMovementActive;
}

QList<SampleVideo*>* ManagerElements::getListSamplesVideos()
{
    return this->listSamplesVideos;
}

ManagerClientOSC* ManagerElements::getManagerClientOSC()
{
    return this->managerClientOSC;
}

ManagerSampleAudio* ManagerElements::getManagetSampleAudio()
{
    return this->managerSampleAudio;
}

ManagerJointMvt* ManagerElements::getManagerJointMvt()
{
    return this->managerJointMvt;
}


void ManagerElements::sortMovements()
{
    for(int i = 1 ; i < this->listMovements->size() ; i++)
    if(this->listMovements->at(i - 1)->getListJointsMvt()->at(0)->getListPositions()->size() >
       this->listMovements->at(i)->getListJointsMvt()->at(0)->getListPositions()->size())
        this->listMovements->swap(i, i - 1);
}


void ManagerElements::addMovement(Movement *movement)
{
    QSettings fichierMovement("movement.ini", QSettings::IniFormat);
    this->listMovements->append(movement);
    this->saveMovement(movement, fichierMovement);
}

/**
  *    DESTRUCTEURS
  **/
ManagerElements::~ManagerElements()
{
    for(int i = 0 ; i < this->listMovements->size() ; i++)
        delete(this->listMovements->at(i));
    delete(this->listMovements);
    for(int i = 0 ; i < this->listSamplesVideos->size() ; i++)
        delete(this->listSamplesVideos->at(i));
    delete(this->listSamplesVideos);
    delete(this->managerClientOSC);
    delete(this->managerSampleAudio);
    delete(this->managerJointMvt);
}
