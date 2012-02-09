#include "managerelements.h"

ManagerElements::ManagerElements() : Manager()
{
    //Initialisation de la serialisation des joints
    this->initSystem();

    /**
   * Test enregistrement mouvement
   */
    // Creation d'un mouvement sur 2 joints
    this->listMovements = new QList<Movement*>();
    /* Movement* move = new Movement("test");
    move->setActive(true);
    JointMvt* j1 = new JointMvt(1, 1);
    JointMvt* j2= new JointMvt(2,1);
    move->addJointMvt(j1);
    move->addJointMvt(j2);
    this->listMovements->append(move);
    this->listMovements->append((new Movement("Movement 2")));
    /**
   * Fin enregistrement
   */


    this->listPorts = new QList<ClientOSC*>();
    ClientOSC *port = new ClientOSC(1234, "localhost", false);
    // port->setActive(true);
    this->listPorts->append(port);
    this->listPorts->append(new ClientOSC(5678, "localhost", false));
    /**
   *
   **/
    this->listSamplesAudios = new QList<SampleAudio*>();
    this->listSamplesAudios->append(new SampleAudio("01 By the Way", "/Users/edewilliencourt/Desktop/01 By the Way.mp3", 0, false));
    this->listSamplesAudios->append(new SampleAudio("03 Scar Tissue", "/Users/edewilliencourt/Desktop/03 Scar Tissue.mp3", 0, false));
    //this->listSamplesAudios->append(new SampleAudio("Sample 3", "URL3", 0, false));

    this->listSamplesVideos = new QList<SampleVideo*>();

    /**
   *
   **/
   // this->saveAll();
    this->loadAll();

}

/**
  *   FONCTIONS LOAD ET SAVE GENERALES
  **/
void ManagerElements::loadAll()
{
    std::cout << "   debut loadAll()..." << std::endl;
    this->loadSamplesAudios();
    this->loadSamplesVideos();
    this->loadAllMovements();
    std::cout << "   ...fin loadAll()" << std::endl;

}

void ManagerElements::saveAll()
{
    std::cout << "   debut saveAll()..." << std::endl;
    QFile::remove("movement.ini");
    QFile::remove("position.ini");
    QFile::remove("jointmvt.ini");
    QFile::remove("sampleaudio.ini");
    QFile::remove("samplevideo.ini");
    this->saveAllMovements();
    this->saveAllSamplesAudios();
    this->saveAllSamplesVideos();
    std::cout << "   ...fin saveAll()" << std::endl;

}

/**
  *  FONCTIONS LOAD ET SAVE DE TOUT LES MOVEMENTS
  **/
void ManagerElements::saveAllMovements()
{
    std::cout << "      debut saveAllMovements()..." << std::endl;

    //SAUVEGARDE DES MOUVEMENTS
    QSettings fichierMovement("movement.ini", QSettings::IniFormat);

    for(int i = 0 ; i < this->listMovements->size() ; i++)
	this->saveMovement(this->listMovements->at(i), fichierMovement);
    fichierMovement.sync();
    std::cout << "      ...fin saveAllMovements()" << std::endl;
}

/**
  *  FONCTIONS LOAD ET SAVE ET DELETE D'UN MOVEMENT
  **/
void ManagerElements::loadAllMovements()
{
    std::cout << "      debut loadMovements()..." << std::endl;
    //CHARGEMENT LISTE MOVEMENTS
    this->listMovements = new QList<Movement*>();
    QSettings fichierMovement("movement.ini", QSettings::IniFormat);
    for(int i = 0 ; i < fichierMovement.allKeys().size() ; i++)
	this->listMovements->append(new Movement(fichierMovement.value(fichierMovement.allKeys().at(i), qVariantFromValue(Movement())).value<Movement>()));

    //CHARGEMENT DE LA LISTE TEMPORAIRE DES JOINTS MOUVEMENTS ET ASSIGNEMENT DE CHACUN DANS LEUR MOUVEMENT RESPECTIF
    // CHARGEMENT DE LA LISTE TEMPORAIRE DES POSITIONS ET ASSIGNEMENT DE CHACUN DANS LEUR JOINT MOUVEMENT RESPECTIF
    this->loadJointMvt();
    this->loadPositions();

    for(int i = 0 ; i < this->listMovements->size() ; i++)
    {
	for(int j = 0 ; j < this->listJointsMvts->size() ; j++)
	{
	    JointMvt *jointMvtTemp = this->listJointsMvts->at(j);
	    if(this->listMovements->at(i)->getId() == jointMvtTemp->getIdMovement())
	    {
		for(int k = 0 ; k < this->listPositionsTemp->size() ; k++)
		{
		    if(jointMvtTemp->getIdJointMvt() == this->listPositionsTemp->at(k)->getIdJointMvt())
			jointMvtTemp->addPosition(this->listPositionsTemp->at(k));
		}
		this->listMovements->at(i)->addJointMvt(this->listJointsMvts->at(j));
	    }
	}
	for(int j = 0 ; j < this->listSamplesAudios->size() ; j++)
	    if(this->listMovements->at(i)->getId() == this->listSamplesAudios->at(j)->getIdMovement())
	    {
		this->listMovements->at(i)->setSampleAudio(this->listSamplesAudios->at(j));
		this->listMovements->at(i)->setActive(true);
		this->listSamplesAudios->at(j)->setActive(true);
		qDebug() << "link movment sample" << this->listMovements->at(i)->getName() << this->listSamplesAudios->at(j)->getName() << endl;
	    }
	for(int j = 0 ; j < this->listSamplesVideos->size() ; j++)
	    if(this->listMovements->at(i)->getId() == this->listSamplesVideos->at(j)->getIdMovement())
		this->listMovements->at(i)->setSampleVideo(this->listSamplesVideos->at(j));
    }
    if(this->listPositionsTemp->isEmpty())
	Position::idPositionsStatic = 0;
    else{
	int tmp = 0;
	for(int i=0; i<listPositionsTemp->size();i++)
	    if (tmp < listPositionsTemp->at(i)->getId()) tmp = listPositionsTemp->at(i)->getId();
	Position::idPositionsStatic = tmp;
    }
    if(this->listJointsMvts->isEmpty())
	JointMvt::idJointMvtStatic = 0;
    else{
	int tmp = 0;
	for(int i=0; i<listJointsMvts->size();i++)
	    if(tmp < listJointsMvts->at(i)->getIdJointMvt()) tmp = listJointsMvts->at(i)->getIdJointMvt();
	JointMvt::idJointMvtStatic = tmp;

    }if(this->listMovements->isEmpty())
	Movement::idMovementStatic = 0;
    else {
	int tmp = 0;
	for(int i=0; i<listMovements->size(); i++)
	    if(tmp < listMovements->at(i)->getId()) tmp = listMovements->at(i)->getId();
	Movement::idMovementStatic = tmp;
    }if(this->listSamplesAudios->isEmpty())
	SampleAudio::idSampleAudioStatic = 0;
    else {
	int tmp = 0;
	for(int i=0; i<listSamplesAudios->size(); i++)
	    if(tmp < listSamplesAudios->at(i)->getId()) tmp = listSamplesAudios->at(i)->getId();
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
    std::cout << "         debut saveMovement..." << std::endl;
    QSettings fichierJointMvt("jointmvt.ini", QSettings::IniFormat);
    QSettings fichierPosition("position.ini", QSettings::IniFormat);
    QSettings fichierSampleAudio("sampleaudio.ini", QSettings::IniFormat);
    QSettings fichierSampleVideo("samplevideo.ini", QSettings::IniFormat);
    for(int j = 0 ; j < movement->getListJointsMvt()->size() ; j++)
    {
	JointMvt *jointMvtTemp = movement->getListJointsMvt()->at(j);
	for(int k = 0 ; k < jointMvtTemp->getListPositions()->size() ; k++){
	    this->savePosition(jointMvtTemp->getListPositions()->at(k), fichierPosition);
	}
	this->saveJointMvt(jointMvtTemp, fichierJointMvt);
    }
    if(movement->getSampleAudio() != NULL)
	this->saveSampleAudio(movement->getSampleAudio(), fichierSampleAudio);
    if(movement->getSampleVideo() != NULL)
	this->saveSampleVideo(movement->getSampleVideo(), fichierSampleVideo);
    QString key = QString::number(movement->getId());
    fichierMovement.setValue(key, qVariantFromValue(*movement));
    fichierJointMvt.sync();
    fichierPosition.sync();
    fichierSampleAudio.sync();
    fichierSampleVideo.sync();
    std::cout << "         ... fin saveMovement" << std::endl;
}

void ManagerElements::removeMovement(Movement *movement)
{
    std::cout << "         debut removeMovement..." << std::endl;
    quint32 idTemp = movement->getId();
    //On remove la liste de joint mouvement du fichier de serialisation
    this->removeJointsMvts(movement->getListJointsMvt());
    for(int i = 0 ;  i < movement->getListJointsMvt()->size() ; i++)
	movement->getListJointsMvt()->removeAt(i);
    QSettings fichierMovement("movement.ini", QSettings::IniFormat);

    //On supprime le mouvement du fichier de serialisation
    fichierMovement.remove(QString(movement->getId()));
    //On supprime le dernier mouvement de la liste de mouvement du fichier de serialisation
    fichierMovement.remove(QString(this->getListMovements()->last()->getId()));
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
    std::cout << "         ...fin removeMovement" << std::endl;
}

/**
  *   FONCTIONS LOAD ET DELETE ET SAVE JOINTMVT
  **/

void ManagerElements::loadJointMvt(){
    this->listJointsMvts = new QList<JointMvt*>();
    //CHARGEMENT LISTE JOINTMOVEMENTS
    QSettings fichierJointMvt("jointmvt.ini", QSettings::IniFormat);
    for(int i = 0 ; i < fichierJointMvt.allKeys().length() ; i++)
    {
	this->listJointsMvts->append(new JointMvt(fichierJointMvt.value(fichierJointMvt.allKeys().at(i), qVariantFromValue(JointMvt())).value<JointMvt>()));
    }
}

void ManagerElements::saveJointMvt(JointMvt *jointMvt, QSettings &fichierJointMvt){
    QString key = QString::number(jointMvt->getIdJointMvt());
    fichierJointMvt.setValue(key, qVariantFromValue(*jointMvt));
}

void ManagerElements::removeJointsMvts(QList<JointMvt*> *listJointsMvtsToDelete)
{
    QSettings fichierJointMvt("jointmvt.ini", QSettings::IniFormat);
    for(int i = 0 ; i < listJointsMvtsToDelete->size() ; i++)
    {
	JointMvt *jointMvtTemp = listJointsMvtsToDelete->at(i);
	//On supprime toutes les positions du joint mouvement dans le fichier
	this->removePositions(jointMvtTemp->getListPositions());
	//on supprime chaque position dans la liste de position du joint mouvement
	for(int i = 0 ; i < jointMvtTemp->getListPositions()->size() ; i++)
	    jointMvtTemp->getListPositions()->removeAt(i);
	//on supprime le joint mouvement du fichier
	fichierJointMvt.remove(QString(jointMvtTemp->getIdJointMvt()));
	//on supprime le dernier joint du fichier
	fichierJointMvt.remove(QString(this->listJointsMvts->last()->getIdJointMvt()));
	//on update son id avec celle du joint mouvement a supprimer
	this->listJointsMvts->last()->updateIdJointMvt(jointMvtTemp->getIdJointMvt());
	//on le save a nouveau update en memoire
	this->saveJointMvt(this->listJointsMvts->last(), fichierJointMvt);
	JointMvt::idJointMvtStatic--;
    }
    fichierJointMvt.sync();
}

/**
  *   FONCTIONS LOAD ET DELETE ET SAVE POSITIONS
  **/

void ManagerElements::savePosition(Position *position, QSettings &fichierPosition)
{
    QString key = QString::number(position->getId());
    fichierPosition.setValue(key, qVariantFromValue(*position));
}

// Charger toutes les Positions
void ManagerElements::loadPositions()
{
    this->listPositionsTemp = new QList<Position*>();
    QSettings fichierPosition("position.ini", QSettings::IniFormat);
    for(int i = 0 ; i < fichierPosition.allKeys().size() ; i++)
    {
	Position *temp = new Position(fichierPosition.value(fichierPosition.allKeys().at(i), qVariantFromValue(Position())).value<Position>());
	this->listPositionsTemp->append(temp);
    }
}

void ManagerElements::removePositions(QList<Position*> *listPositionsToDelete)
{
    QSettings fichierPosition("position.ini", QSettings::IniFormat);
    for(int i = 0 ; i < listPositionsToDelete->size() ; i++)
    {
	fichierPosition.remove(QString(listPositionsToDelete->at(i)->getId()));
	fichierPosition.remove(QString(this->listPositionsTemp->last()->getId()));
	this->listPositionsTemp->last()->updateId(listPositionsToDelete->at(i)->getId());
	this->savePosition(this->listPositionsTemp->last(), fichierPosition);
	Position::idPositionsStatic--;
    }
    fichierPosition.sync();
}

/**
  *   FONCTIONS LOAD ET DELETE ET SAVE SAMPLEAUDIO
  **/

void ManagerElements::saveAllSamplesAudios()
{
    QSettings fichierSampleAudio("sampleaudio.ini", QSettings::IniFormat);
    for (int i = 0 ; i < this->listSamplesAudios->size() ; i++)
    {
	this->saveSampleAudio(this->listSamplesAudios->at(i), fichierSampleAudio);
    }
    fichierSampleAudio.sync();
}

void ManagerElements::saveSampleAudio(SampleAudio *sampleAudio, QSettings &fichierSampleAudio)
{
    QString key = QString::number(sampleAudio->getId());
    fichierSampleAudio.setValue(key, qVariantFromValue(*sampleAudio));
}

void ManagerElements::loadSamplesAudios()
{
    this->listSamplesAudios = new QList<SampleAudio*>();

    QSettings fichierSampleAudio("sampleaudio.ini", QSettings::IniFormat);
    for(int i = 0 ; i < fichierSampleAudio.allKeys().length() ; i++)
    {
	SampleAudio *temp = new SampleAudio(fichierSampleAudio.value(fichierSampleAudio.allKeys().at(i), qVariantFromValue(SampleAudio())).value<SampleAudio>());
	this->listSamplesAudios->append(temp);
    }
}

void ManagerElements::removeSampleAudio(SampleAudio *sampleAudio)
{
    QSettings fichierSampleAudio("sampleaudio.ini", QSettings::IniFormat);
    fichierSampleAudio.remove(QString(sampleAudio->getId()));
    fichierSampleAudio.remove(QString(this->listSamplesAudios->last()->getId()));
    this->listSamplesAudios->last()->updateId(sampleAudio->getId());
    this->saveSampleAudio(this->listSamplesAudios->last(), fichierSampleAudio);
    SampleAudio::idSampleAudioStatic--;
    fichierSampleAudio.sync();
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
    this->listSamplesVideos = new QList<SampleVideo*>();

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
    fichierSampleVideo.remove(QString(sampleVideo->getId()));
    fichierSampleVideo.remove(QString(this->listSamplesVideos->last()->getId()));
    this->listSamplesVideos->last()->updateId(sampleVideo->getId());
    this->saveSampleVideo(this->listSamplesVideos->last(), fichierSampleVideo);
    SampleVideo::idSampleVideoStatic--;
    fichierSampleVideo.sync();
}


//Initialisation du système pour la sérialisation
void ManagerElements::initSystem()
{
    qRegisterMetaTypeStreamOperators<Movement>("Movement");
    qMetaTypeId<Movement>();
    qRegisterMetaTypeStreamOperators<JointMvt>("JointMvt");
    qMetaTypeId<JointMvt>();
    qRegisterMetaTypeStreamOperators<Position>("Position");
    qMetaTypeId<Position>();
    qRegisterMetaTypeStreamOperators<SampleAudio>("SampleAudio");
    qMetaTypeId<SampleAudio>();
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

QList<SampleAudio*>* ManagerElements::getListSamplesAudios()
{
    return this->listSamplesAudios;
}

QList<SampleAudio*>* ManagerElements::getListSamplesAudiosActive()
{
    QList<SampleAudio*>*listSampleAudioActive = new QList<SampleAudio*>();
    for(int i = 0 ; i < this->listSamplesAudios->size() ; i++)
	if(this->listSamplesAudios->at(i)->isActive() == true)
	    listSampleAudioActive->append(this->listSamplesAudios->at(i));
    return listSampleAudioActive;
}

QList<SampleVideo*>* ManagerElements::getListSamplesVideos()
{
    return this->listSamplesVideos;
}

QList<ClientOSC*>* ManagerElements::getListPorts()
{
    return this->listPorts;
}

QList<ClientOSC*>* ManagerElements::getListPortsActive()
{
    QList<ClientOSC*>*listPortsActive = new QList<ClientOSC*>();
    for(int i = 0 ; i < this->listPorts->size() ; i++)
	if(this->listPorts->at(i)->isActive() == true)
	{
	    listPortsActive->append(this->listPorts->at(i));
	}
    return listPortsActive;

}

QList<JointMvt*>* ManagerElements::getListJointsMvts()
{
    return this->listJointsMvts;
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

void ManagerElements::addSample(QString name, QString url)
{
    this->listSamplesAudios->append(new SampleAudio(name, url, 0, false));
    QSettings fichierSampleAudio("sampleaudio.ini", QSettings::IniFormat);
    this->saveSampleAudio(this->listSamplesAudios->last(), fichierSampleAudio);
    fichierSampleAudio.sync();
    for(int i = 0 ; i < this->listSamplesAudios->size() ; i++)
	qDebug() << this->listSamplesAudios->at(i)->getName() << endl;
}

/**
  *    DESTRUCTEURS
  **/

ManagerElements::~ManagerElements()
{
    for(int i = 0 ; i < this->listMovements->size() ; i++)
	delete(this->listMovements->at(i));
    delete(this->listMovements);
    for(int i = 0 ; i < this->listSamplesAudios->size() ; i++)
	delete(this->listSamplesAudios->at(i));
    delete(this->listSamplesAudios);
    for(int i = 0 ; i < this->listSamplesVideos->size() ; i++)
	delete(this->listSamplesVideos->at(i));
    delete(this->listSamplesVideos);
    for(int i = 0 ; i < this->listPorts->size() ; i++)
	delete(this->listPorts->at(i));
    delete(this->listPorts);
}
