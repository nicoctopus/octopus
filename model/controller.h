#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "managerjoints.h"
#include "managerelements.h"
#include "serverosc.h"
#include "soundplayer.h"
#include "clientosc.h"
#include "analyse.h"

class Controller
{
private:
    ManagerJoints *managerJoints;
    SoundPlayer *playerlive;
    SoundPlayer *playerdemo;
    ManagerElements *movement;
    ServerOSC *serveurOSC;

public:
    Controller();
    ManagerJoints* getManagerJoints();
    ManagerElements* getManagerElements();
    void linkJointToJointMvt();
    SoundPlayer *getPlayerDemo();
    SoundPlayer *getPlayerLive();
    Movement * recordMovement(Movement *movement);
    void stopRecord();
    void analizeRecord();

    void bubble(QList<Movement *> *moves);
    void troncage(Movement* move);

};

#endif // CONTROLLER_H
