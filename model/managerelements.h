#ifndef MANAGERELEMENTS_H
#define MANAGERELEMENTS_H

#include "QWidget"
#include "manager.h"
#include "managerclientosc.h"
#include "managersampleaudio.h"
#include "managersamplevideo.h"
#include "managermovements.h"

class ManagerElements : public Manager
{

private :
    ManagerClientOSC *managerClientOSC;
    ManagerSampleAudio *managerSampleAudio;
    ManagerSampleVideo *managerSampleVideo;
    ManagerMovements *managerMovements;

public:
    ManagerElements();
    ~ManagerElements();

    //Methodes de serialisation
    void loadAll();
    void saveAll();
    void initSystem();

    void dispacher();
    void saveMovement(Movement *movement);

    //Getters
    ManagerClientOSC* getManagerClientOSC();
    ManagerSampleAudio* getManagetSampleAudio();
    ManagerSampleVideo* getManagerSampleVideo();
    ManagerMovements* getManagerMovements();

    void addMovement(Movement *movemement);
    void remove(Movement *movement);
    void remove(SampleAudio *sampleAudio);
    void remove(ClientOSC *clientOSC);
};

#endif // MANAGERELEMENTS_H
