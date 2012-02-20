#ifndef THREADMOVESTICKMAN_H
#define THREADMOVESTICKMAN_H

#include <QThread>
#include "jointgraphic.h"
#include "model/position.h"
#include "model/movement.h"
#include "model/position.h"

class ThreadMoveStickman : public QThread
{
    Q_OBJECT

public:
    ThreadMoveStickman();
    ThreadMoveStickman(Movement* movement);
    void run();

signals:
    void sigMoveNode(QString nameOfNodeToMove, int x, int y, int z);
    void sigEndOfMoveStickman();

private:
    Movement* movement;




};

#endif // THREADMOVESTICKMAN_H
