#ifndef STICKMAN_H
#define STICKMAN_H

#include<QGraphicsScene>
#include<QGraphicsView>
#include<QGraphicsRectItem>
#include<QLine>
#include<QMessageBox>
#include"jointgraphic.h"

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

    public slots:

    signals:
        void clearStickMan();


    private:
        QMap<QString, QPoint> coord;
        QList<QLine> lines;
        QList<JointGraphic*> nodesSelected;
        float RESIZE;


};

#endif // STICKMAN_H
