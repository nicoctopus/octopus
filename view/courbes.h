#ifndef COURBES_H
#define COURBES_H

#include "QWidget"
#include "QTabWidget"
#include "QPainter"
#include "../model/position.h"

class Courbes : public QWidget
{
public:
    Courbes(QWidget * parent);
    void setPositions(QList<Position *> *listPositionMouvement, QList<Position *> *listPositionBuffer, int val);

private :
    QList<qint32>*listPositionMouvement;
    QList<Position*>*listPositionBuffer;
    int valeur;

protected:
    void paintEvent(QPaintEvent *);
    void timerEvent(QTimerEvent *);
};

#endif // COURBES_H
