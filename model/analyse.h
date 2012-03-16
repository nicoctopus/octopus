#ifndef ANALYSE_H
#define ANALYSE_H

#include "math.h"
#include <QtGlobal>
#include <QList>
#include <time.h>
#include <QPainter>
#include "qmath.h"
#include "movement.h"
#include "databitg.h"
#include "soundplayer.h"


class Analyse : public QWidget
{
private:
    float seuilFrequence;
    float seuilAmplitude;
    SoundPlayer *playerlive;

    QList<Movement*>*listMovementToAnalyze;
    QList<Position*>* EnregistrementVite;
    QList<Position*>* ListEcartEnr;
    QList<Position*>* ListEcartBuf;
    void modifFreq(QList<Position*>* listPositionsJointMvt, int sizeMvt, QList<Position*>* newList);
    Position* moyenne(Position const *p1, Position const *p2);
   // qint8 NbPivots(QList<Position*> *);
    float ecartRelatif(QList<Position*>*, QList<Position*>*);

public:
    Analyse();
    ~Analyse();
    Analyse(float,float, QList<Movement*>*);
    void calculBITG();

protected:
    void paintEvent(QPaintEvent *);
    void timerEvent(QTimerEvent *);

};

#endif // ANALYSE_H
