#ifndef MANAGERELEMENTS_H
#define MANAGERELEMENTS_H

#include "QWidget"
#include "manager.h"
#include "jointmvt.h"
#include "samplevideo.h"
#include "managerclientosc.h"
#include "managersampleaudio.h"
#include "managerjointmvt.h"

class ManagerElements : public Manager
{

private :
    QList<Movement*> *listMovements;
    QList<SampleVideo*> *listSamplesVideos;
    ManagerClientOSC *managerClientOSC;
    ManagerSampleAudio *managerSampleAudio;
    ManagerJointMvt *managerJointMvt;

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

    //Samples Video
    void saveAllSamplesVideos();
    void saveSampleVideo(SampleVideo *sampleVideo, QSettings &fichierSampleVideo);
    void loadSamplesVideos();
    void removeSampleVideo(SampleVideo *sampleVideo);

    //Getters
    QList<Movement*>* getListMovements();
    QList<Movement*>* getListMovementsActive();
    QList<SampleVideo*>* getListSamplesVideos();
    ManagerClientOSC* getManagerClientOSC();
    ManagerSampleAudio* getManagetSampleAudio();
    ManagerJointMvt* getManagerJointMvt();

    //Methodes
    void sortMovements();
    void addMovement(Movement *movement);
};

#endif // MANAGERELEMENTS_H
