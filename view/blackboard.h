#ifndef BLACKBOARD_H
#define BLACKBOARD_H

#include <QWidget>
#include <QtGui>

#include "square.h"

#include "model/movement.h"
#include "ellipseduprojet.h"
class EllipseDuProjet;
#include "triangle.h"
class Triangle;
#include "diamond.h"
class Diamond;

#define DEPLACEMENT_HORIZONTAL 75
#define DEPLACEMENT_VERTICAL 75

class BlackBoard : public QGraphicsView
{
    Q_OBJECT
public:
    explicit BlackBoard(QWidget * parent);
    void addLineItem(const quint16 x1, const quint16 y1, const quint16 x2, const quint16 y2);
    void updateSampleAudioOfMovement(Movement *movement, SampleAudio *newSampleAudio);
    void updateClientOSCOfMovement(Movement *movement, ClientOSC *clientOSC);
    void setListMovements(QList<Movement*> *listMovements);
    void setListPorts(QList<ClientOSC*> *listPorts);
    void setListSamplesAudio(QList<SampleAudio*> *listSamplesAudio);
    void itemMoved(QGraphicsItem* pMovingItem);
    void setLastX(int xItem);
    void setLastY(int yItem);
    void liaison(QGraphicsItem *itemPressed);
    void SetCenter(const QPointF& centerPoint);
    QPointF GetCenter() { return CurrentCenterPoint; }
    
signals:
    void save(Movement *movement);
    void save(SampleAudio *sampleAudio);
    void save(ClientOSC *clientOSC);
    void refreshSignal();
    void decocherCheckBoxLink();
    void clearScene();
    void remove(Movement *movement);
    void remove(SampleAudio *sampleAudio);
    void remove(ClientOSC *clientOSC);
    void visualisation(Movement *movement);

public slots:
    void refresh();
    void slotRemove();
    void slotVisualisation();
    void slotLiaison();
    //void liaison();
    void slotEnleverBlackboard();

protected :
    void contextMenuEvent(QContextMenuEvent  *event);
    void dropEvent(QDropEvent *event);
    void dragEnterEvent(QDragEnterEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
    void timerEvent(QTimerEvent *event);
    virtual void wheelEvent(QWheelEvent* event);
    virtual void resizeEvent(QResizeEvent* event);


private:
    QList<Movement*> *listMovements;
    QList<SampleAudio*> *listSamplesAudio;
    QList<ClientOSC*> *listPorts;
    QList<EllipseDuProjet*> listEllipse;
    QList<Triangle*> listTriangle;
    QList<Diamond*> listDiamond;
    QList<QGraphicsLineItem*> listLines;
    quint16 xEllipse, yEllipse, xDiamond, yDiamond, xTriangle, yTriangle;
    Movement *movement;
    QGraphicsItem* movingItem;
    int timerId;
    QPointF CurrentCenterPoint;

    //Menu clique droit
    QAction *actionRemove;
    QAction *actionVisualisation;
    QAction *actionLier;
    QAction *actionDelier;
    QAction *actionEnleverBlackboard;
    void createActions();

    int lastPosX;
    int lastPosY;
    bool isFirstPassage;

};

#endif // BLACKBOARD_H
