#ifndef STICKMAN_H
#define STICKMAN_H

#include<QGraphicsScene>
#include<QGraphicsView>
#include<QGraphicsRectItem>
#include<QLine>
#include<QMessageBox>
#include "jointgraphic.h"
#include "model/position.h"
#include "threadmovestickman.h"
#include "myqline.h"
#include "model/movement.h"
#include <QTimer>
#include <QLabel>

class StickMan : public QGraphicsView
{

    Q_OBJECT

    public:
        StickMan(QWidget *parent);
        void setResize(float rate);
        void selectedNodes();
        QList<JointGraphic*> getNodesSelected();
        void createStickMan();
        void reCreateStickMan();
        //void moveNode(JointGraphic* nodeToMove, int x, int y, int z);
        void setStickManLive(bool isStickManLive);
        void launchTimerForDetection();
        void stopTimer();
        void setLabelDetected(QLabel* label);

    public slots:
        void slotMoveStickMan(Movement* movement);
        void slotMoveNode(QString nameOfNodeToMove, int x, int y, int z);
        void slotEndOfMoveStickman();
        void slotTimeOutTimer();

    signals:
        void clearStickMan();
        void sigMoveNode(QString,int,int,int);

    private:
        QLabel * textDetected;
        QMap<QString, QPoint> coord;
        QList<MyQLine*> lines;
        QList<JointGraphic*> nodes;
        QList<JointGraphic*> nodesSelected;
        QMap<QString, QList<MyQLine*> > linesLinkedAtThisNode;
        float RESIZE;
        bool isStickManLive;
        QTimer *timer;
        int headLastX;
        int headLastY;
        int headLastZwithModifSize;
	int epauleGaucheX;
	int epauleGaucheY;
	int epauleDroiteX;
	int epauleDroiteY;
	int hancheGaucheX;
	int hancheGaucheY;
	int hancheDroiteX;
	int hancheDroiteY;


};

#endif // STICKMAN_H
