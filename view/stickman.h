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

    public slots:
        void slotMoveStickMan(Movement* movement);
        void slotMoveNode(QString nameOfNodeToMove, int x, int y, int z);
        void slotEndOfMoveStickman();

    signals:
        void clearStickMan();




    private:
        QMap<QString, QPoint> coord;
        QList<MyQLine*> lines;
        QList<JointGraphic*> nodes;
        QList<JointGraphic*> nodesSelected;

        QMap<QString, QList<MyQLine*> > linesLinkedAtThisNode;



        float RESIZE;


};

#endif // STICKMAN_H
