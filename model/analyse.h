#ifndef ANALYSE_H
#define ANALYSE_H

#include "math.h"
#include <QtGlobal>
#include <QList>
#include <time.h>

#include "movement.h"
#include "databitg.h"
#include "soundplayer.h"


class Analyse
{
private:
    float seuilFrequence;
    float seuilAmplitude;
    SoundPlayer *playerlive;


    void modifFreq(QList<Position*>* listPositionsJointMvt, int sizeMvt, QList<Position*>* newList);
    Position* moyenne(Position* p1, Position *p2);

    float ecartRelatif(QList<Position*> *,QList<Position*> *);

public:
    Analyse();
    ~Analyse();
    Analyse(float,float);
    void calculBITG(QList<Movement *> *);

};

#endif // ANALYSE_H
