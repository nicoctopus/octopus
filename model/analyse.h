#ifndef ANALYSE_H
#define ANALYSE_H

#include "math.h"
#include <QtGlobal>
#include <QList>
#include <time.h>

#include "movement.h"
#include "databitg.h"
#include "soundplayer.h"
#include "sampleaudio.h"

class Analyse
{
private:
    float seuilFrequence;
    float seuilAmplitude;
    SoundPlayer * p;
    SampleAudio * music;

    QList<Position*>* modifFreq(QList<Position*>* listPositionsJointMvt, int sizeMvt);
    Position* moyenne(Position* p1, Position *p2);
    qint8 NbPivots(QList<Position*> *);
    float EcartRelatif(QList<Position*> *,QList<Position*> *);

public:
    Analyse();
    ~Analyse();
    Analyse(float,float);
    void calculBITG(QList<Movement *> *);

};

#endif // ANALYSE_H
