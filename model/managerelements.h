#ifndef MANAGERELEMENTS_H
#define MANAGERELEMENTS_H

#include "QWidget"
#include "manager.h"
#include "jointmvt.h"
#include "sampleaudio.h"
#include "samplevideo.h"
#include "port.h"

class ManagerElements : public Manager
{

private :
    QList<Movement*> *listMovements;
    QList<SampleAudio*> *listSamplesAudios;
    QList<SampleVideo*> *listSamplesVideos;
    QList<ClientOSC*> *listPorts;
    QList<Position*> *listPositionsTemp;
    QList<JointMvt*> *listJointsMvts;

public:
    ManagerElements();
    ~ManagerElements();

    //Methodes de serialisation
    void loadAll();
    void saveAll();
    void initSystem();
    //Movements
    void saveAllMovements();
    void loadAllMovements();
    void saveMovement(Movement *movement, QSettings &fichierMovement);
    void removeMovement(Movement *movement);

    //Joint movement
    void loadJointMvt();
    void saveJointMvt(JointMvt *jointMvt, QSettings &fichierJointMvt);
    void removeJointsMvts(QList<JointMvt*> *listJointsMvts);

    //Positions
    void savePosition(Position *position, QSettings &fichierPosition);
    void loadPositions();
    void removePositions(QList<Position*> *listPositions);

    //Samples Audio
    void saveAllSamplesAudios();
    void saveSampleAudio(SampleAudio *sampleAudio, QSettings &fichierSampleAudio);
    void loadSamplesAudios();
    void removeSampleAudio(SampleAudio *sampleAudio);

    //Samples Video
    void saveAllSamplesVideos();
    void saveSampleVideo(SampleVideo *sampleVideo, QSettings &fichierSampleVideo);
    void loadSamplesVideos();
    void removeSampleVideo(SampleVideo *sampleVideo);

    //Getters
    QList<Movement*>* getListMovements();
    QList<Movement*>* getListMovementsActive();
    QList<SampleAudio*>* getListSamplesAudios();
    QList<SampleAudio*>* getListSamplesAudiosActive();
    QList<SampleVideo*>* getListSamplesVideos();
    QList<ClientOSC*>* getListPorts();
    QList<ClientOSC*>* getListPortsActive();
    QList<Position*> *getListPositionTemp(){return this->listPositionsTemp;}
    QList<JointMvt*>* getListJointsMvts();

    //Methodes
    void sortMovements();
    void addMovement(Movement *movement);
    void addSample(QString name, QString Url);
};

#endif // MANAGERELEMENTS_H
