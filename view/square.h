#ifndef CARRE_H
#define CARRE_H


#include <time.h>
#include <QGraphicsRectItem>
#include <QGraphicsScene>

class Square : public QGraphicsRectItem
{
public:
    Square(qreal x, qreal y, qreal width, qreal height, QColor* color, QGraphicsScene *scene);
};

#endif // CARRE_H
