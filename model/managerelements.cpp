#include "managerelements.h"

ManagerElements::ManagerElements() : Manager()
{
    //Initialisation de la serialisation des joints
    this->initSystem();
    this->managerClientOSC = new ManagerClientOSC();
    this->managerSampleAudio = new ManagerSampleAudio();
    this->managerSampleVideo = new ManagerSampleVideo();
    this->managerMovements = new ManagerMovements();

    /**
      *
      **
    this->managerSampleAudio->getListSamplesAudios()->append(new SampleAudio("01 By the Way", "../../../01 By the Way.mp3", 1, false));
    this->managerSampleAudio->getListSamplesAudios()->append(new SampleAudio("03 Scar Tissue", "../../../03 Scar Tissue.mp3", 2, false));
    this->managerClientOSC->getListClientsOSC()->append(new ClientOSC(1234, "localhost", false));
    this->managerClientOSC->getListClientsOSC()->append(new ClientOSC(5678, "localhost", false));
    this->managerMovements->getListMovements()->append(new Movement("bonjour"));
    JointMvt *j = new JointMvt(1, 1);
    j->addPosition(new Position(1,3,0,5,0,0,0));
    j->addPosition(new Position(1,1,0,0,0,0,0));
    this->managerMovements->getListMovements()->at(0)->addJointMvt(j);
    this->managerMovements->getListMovements()->append(new Movement("bonjour2"));
    JointMvt *j2 = new JointMvt(1, 2);
    j2->addPosition(new Position(1,3,0,5,0,0,0));
    j2->addPosition(new Position(1,1,0,0,0,0,0));
    this->managerMovements->getListMovements()->at(1)->addJointMvt(j2);
    this->managerMovements->getListMovements()->append(new Movement("bonjour3"));
    JointMvt *j3 = new JointMvt(1, 3);
    j3->addPosition(new Position(1,3,0,5,0,0,0));
    j3->addPosition(new Position(1,1,0,0,0,0,0));
    this->managerMovements->getListMovements()->at(2)->addJointMvt(j3);
    /**
     *
    **
    this->saveAll();
    this->dispacher();
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
    this->managerMovements->loadAll();
    this->managerClientOSC->loadAll();
    this->managerSampleAudio->loadAll();
    this->managerSampleVideo->loadAll();
    this->dispacher();
    qDebug() << "   ...fin loadAll()" << endl;

}

void ManagerElements::saveAll()
{
    qDebug() << "   debut saveAll()..." << endl;
    this->managerClientOSC->saveAll();
    this->managerSampleAudio->saveAll();
    this->managerMovements->saveAll();
    this->managerSampleVideo->saveAll();
    qDebug() << "   ...fin saveAll()" << endl;

}

void ManagerElements::dispacher()
{
    for(int i = 0 ; i < this->managerMovements->getListMovements()->size() ; i++)
    {
	for(int j = 0 ; j < this->managerSampleAudio->getListSamplesAudios()->size() ; j++)
	    for(int k = 0 ; k < this->managerSampleAudio->getListSamplesAudios()->at(j)->getListIdMovement()->size() ; k++)
	    if(this->managerMovements->getListMovements()->at(i)->getId() == this->managerSampleAudio->getListSamplesAudios()->at(j)->getListIdMovement()->at(k))
	    {
		this->managerMovements->getListMovements()->at(i)->setSampleAudio(this->managerSampleAudio->getListSamplesAudios()->at(j));
		this->managerMovements->getListMovements()->at(i)->setActive(true);
		this->managerSampleAudio->getListSamplesAudios()->at(j)->setActive(true);
	    }
	for(int j = 0 ; j < this->managerSampleVideo->getListSamplesVideos()->size() ; j++)
	    for(int k = 0 ; k < this->managerSampleVideo->getListSamplesVideos()->at(j)->getListIdMovement()->size() ; k++)
	    if(this->managerMovements->getListMovements()->at(i)->getId() == this->managerSampleVideo->getListSamplesVideos()->at(j)->getListIdMovement()->at(k))
	    {
		this->managerMovements->getListMovements()->at(i)->setSampleVideo(this->managerSampleVideo->getListSamplesVideos()->at(j));
		this->managerMovements->getListMovements()->at(i)->setActive(true);
		this->managerSampleVideo->getListSamplesVideos()->at(j)->setActive(true);
	    }
	for(int j = 0 ; j < this->managerClientOSC->getListClientsOSC()->size() ; j++)
	{
	    for(int k = 0 ; k < this->managerClientOSC->getListClientsOSC()->at(j)->getListIdMovement()->size() ; k++)
		if(this->managerClientOSC->getListClientsOSC()->at(j)->getListIdMovement()->at(k) == this->managerMovements->getListMovements()->at(i)->getId())
		{
		    this->managerMovements->getListMovements()->at(i)->getListClients()->append(this->managerClientOSC->getListClientsOSC()->at(j));
		    this->managerMovements->getListMovements()->at(i)->setActive(true);
		    this->managerClientOSC->getListClientsOSC()->at(j)->setActive(true);
		}
	}
    }
    if(this->managerClientOSC->getListClientsOSC()->isEmpty())
	ClientOSC::idClientOSCStatic = 0;
    else{
	int tmp = 0;
	for(int i = 0 ; i < this->managerClientOSC->getListClientsOSC()->size() ; i++)
	    if(tmp < this->managerClientOSC->getListClientsOSC()->at(i)->getId())
		tmp = this->managerClientOSC->getListClientsOSC()->at(i)->getId();
	ClientOSC::idClientOSCStatic = tmp;
    }
    if(this->managerSampleAudio->getListSamplesAudios()->isEmpty())
        SampleAudio::idSampleAudioStatic = 0;
    else {
        int tmp = 0;
	for(int i=0; i<this->managerSampleAudio->getListSamplesAudios()->size(); i++)
	    if(tmp < this->managerSampleAudio->getListSamplesAudios()->at(i)->getId())
		tmp = this->managerSampleAudio->getListSamplesAudios()->at(i)->getId();
        SampleAudio::idSampleAudioStatic = tmp;
    }
    if(this->managerSampleVideo->getListSamplesVideos()->isEmpty())
        SampleVideo::idSampleVideoStatic = 0;
    else {
        int tmp = 0;
	for(int i=0; i< this->managerSampleVideo->getListSamplesVideos()->size(); i++)
	    if(tmp < this->managerSampleVideo->getListSamplesVideos()->at(i)->getId())
		tmp = this->managerSampleVideo->getListSamplesVideos()->at(i)->getId();
        SampleVideo::idSampleVideoStatic = tmp;
    }
}

void ManagerElements::saveMovement(Movement *movement)
{
    QSettings fichierSampleAudio("sampleaudio.ini", QSettings::IniFormat);
    QSettings fichierSampleVideo("samplevideo.ini", QSettings::IniFormat);
    QSettings fichierMovement ("movement.ini", QSettings::IniFormat);

    this->managerMovements->save(movement, fichierMovement);
    if(movement->getSampleAudio())
	this->managerSampleAudio->save(movement->getSampleAudio(), fichierSampleAudio);
    if(movement->getSampleVideo())
	this->managerSampleVideo->save(movement->getSampleVideo(), fichierSampleVideo);
    if(movement->getListClients())
	this->managerClientOSC->save(movement->getListClients());
    fichierSampleAudio.sync();
    fichierSampleVideo.sync();
    fichierMovement.sync();
}

//Initialisation du systeme pour la serialisation
void ManagerElements::initSystem()
{

}

/**
  *    GETTERS
  **/

ManagerClientOSC* ManagerElements::getManagerClientOSC()
{
    return this->managerClientOSC;
}

ManagerSampleAudio* ManagerElements::getManagetSampleAudio()
{
    return this->managerSampleAudio;
}

ManagerSampleVideo* ManagerElements::getManagerSampleVideo()
{
    return this->managerSampleVideo;
}

ManagerMovements* ManagerElements::getManagerMovements()
{
    return this->managerMovements;
}

void ManagerElements::addMovement(Movement *movement)
{
    this->managerMovements->addMovement(movement);
    this->saveMovement(movement);
}

void ManagerElements::remove(Movement *movement)
{
    if(movement->getSampleAudio())
    {
	movement->getSampleAudio()->removeId(movement->getId());
	QSettings fichierSampleAudio("sampleaudio.ini", QSettings::IniFormat);
	this->managerSampleAudio->save(movement->getSampleAudio(), fichierSampleAudio);
	fichierSampleAudio.sync();
    }
    if(movement->getSampleVideo())
	movement->getSampleVideo()->removeId(movement->getId());
    if(!movement->getListClients()->isEmpty())
    {
	QSettings fichierClientOSC("clientOSC.ini", QSettings::IniFormat);
	for(int i = 0 ; i < movement->getListClients()->size() ; i++)
	{
	    movement->getListClients()->at(i)->removeIdMovement(movement->getId());
	    this->managerClientOSC->save(movement->getListClients()->at(i), fichierClientOSC);
	}
	fichierClientOSC.sync();
    }
    this->managerMovements->remove(movement);
}

void ManagerElements::remove(SampleAudio *sampleAudio)
{
    QSettings fichierMovement ("movement.ini", QSettings::IniFormat);
    for(int i = 0 ; i < this->managerMovements->getListMovements()->size() ; i++)
	if(this->managerMovements->getListMovements()->at(i)->getSampleAudio())
	    if(this->managerMovements->getListMovements()->at(i)->getSampleAudio()->getId() == sampleAudio->getId())
	    {
		this->managerMovements->getListMovements()->at(i)->setSampleAudio(NULL);
		this->managerMovements->save(this->managerMovements->getListMovements()->at(i), fichierMovement);
	    }
    fichierMovement.sync();
    this->managerSampleAudio->remove(sampleAudio);
}

void ManagerElements::remove(ClientOSC *clientOSC)
{
    QSettings fichierMovement ("movement.ini", QSettings::IniFormat);
    for(int i = 0 ; i < this->managerMovements->getListMovements()->size() ; i++)
	if(!this->managerMovements->getListMovements()->at(i)->getListClients()->isEmpty())
	    for(int j = 0 ; j < this->managerMovements->getListMovements()->at(i)->getListClients()->size() ; j++)
		if(this->managerMovements->getListMovements()->at(i)->getListClients()->at(j)->getId() == clientOSC->getId())
		{
		    this->managerMovements->getListMovements()->at(i)->getListClients()->removeAt(j);
		    this->managerMovements->save(this->managerMovements->getListMovements()->at(i), fichierMovement);
		}
    fichierMovement.sync();
    this->managerClientOSC->remove(clientOSC);

}

/**
  *    DESTRUCTEURS
  **/
ManagerElements::~ManagerElements()
{
    delete(this->managerMovements);
    delete(this->managerSampleVideo);
    delete(this->managerClientOSC);
    delete(this->managerSampleAudio);
}
