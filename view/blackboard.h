#ifndef BLACKBOARD_H
#define BLACKBOARD_H

#include <QWidget>
#include <QtGui>

#include "square.h"
#include "ellipseduprojet.h"
#include "triangle.h"
#include "diamond.h"

#define DEPLACEMENT_HORIZONTAL 75
#define DEPLACEMENT_VERTICAL 75

class BlackBoard : public QGraphicsView
{
    Q_OBJECT
public:
    explicit BlackBoard(QWidget * parent);
    void dropEvent(QDropEvent *event);
    void dragEnterEvent(QDragEnterEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
    void addLineItem(const quint16 x1, const quint16 y1, const quint16 x2, const quint16 y2);
    void updateSampleAudioOfMovement(Movement *movement, SampleAudio *newSampleAudio);
    void updateClientOSCOfMovement(Movement *movement, ClientOSC *clientOSC);
    void setListMovements(QList<Movement*> *listMovements);
    void setListPorts(QList<ClientOSC*> *listPorts);
    void setListSamplesAudio(QList<SampleAudio*> *listSamplesAudio);
    void selectedItems();
    
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
    void slotLier();
    void slotDelier();

protected :
    void contextMenuEvent(QContextMenuEvent  *event);

private:
    QList<Movement*> *listMovements;
    QList<SampleAudio*> *listSamplesAudio;
    QList<ClientOSC*> *listPorts;
    QList<EllipseDuProjet*> listEllipse;
    QList<Triangle*> listTriangle;
    QList<Diamond*> listDiamond;
    QList<QGraphicsItem*> itemsToLink;
    QList<QGraphicsLineItem*> listLines;
    quint16 xEllipse, yEllipse, xDiamond, yDiamond, xTriangle, yTriangle;

    //Menu clique droit
    QAction *actionRemove;
    QAction *actionVisualisation;
    QAction *actionLier;
    QAction *actionDelier;
    void createActions();

};

#endif // BLACKBOARD_H
