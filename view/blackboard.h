#ifndef BLACKBOARD_H
#define BLACKBOARD_H

#include <QWidget>
#include <QtGui>

#include "square.h"
#include "ellipseduprojet.h".h"
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
    void setListMovements(QList<Movement*> *listMovements);
    void setListPorts(QList<ClientOSC*> *listPorts);
    void setListSamplesAudio(QList<SampleAudio*> *listSamplesAudio);
    void selectedItems();
   // void creerForme();
    
signals:
    void save(Movement *movement);
    void save(SampleAudio *sampleAudio);
    void save(ClientOSC *port);
    void refreshSignal();
    void decocherCheckBoxLink();

public slots:
    void refresh();
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
};

#endif // BLACKBOARD_H
