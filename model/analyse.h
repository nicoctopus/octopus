#ifndef ANALYSE_H
#define ANALYSE_H

#include "math.h"
#include <QtGlobal>
#include <QList>
#include <time.h>
#include "qmath.h"
#include "movement.h"
#include "databitg.h"
#include "soundplayer.h"


class Analyse
{

private:
    float seuilFrequence;
    float seuilAmplitude;
    SoundPlayer *playerlive;

    QList<Movement*>* listMovementToAnalyze;
    QList<Position*>* ListEcartEnr;
    QList<Position*>* ListEcartBuf;
    //void modifFreq(QList<Position*>* listPositionsJointMvt, int sizeMvt);
    //Position* moyenne(Position const *p1, Position const *p2);
   // qint8 NbPivots(QList<Position*> *);
    float ecartRelatif();

public:
    Analyse();
    ~Analyse();
    Analyse(float,float, QList<Movement*>*);
    void calculBITG();

};

#endif // ANALYSE_H
