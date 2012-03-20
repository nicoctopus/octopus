#ifndef COURBES_H
#define COURBES_H

#include "QWidget"
#include "QTabWidget"
#include "QPainter"
#include "../model/movement.h"

class Courbes : public QWidget
{
public:
    Courbes(QWidget * parent, JointMvt* joint);
    void setMovement(Movement *movement);
    void setIndexJointMvtDuMouvement(quint8 index);
    void creationTab();
    void setTabWidget(QTabWidget* tabWidget);
private :
    Movement *movement;
    quint8 indexJointMvtDuMouvement;
    QTabWidget *tabWidget;
    JointMvt* joint;

protected:
    void paintEvent(QPaintEvent *);
    void timerEvent(QTimerEvent *);
};

#endif // COURBES_H
