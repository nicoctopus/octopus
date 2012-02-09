#ifndef ELLIPSE_H
#define ELLIPSE_H

#include<QGraphicsEllipseItem>
#include <QGraphicsScene>
#include "model/movement.h"
class Ellipse : public QGraphicsEllipseItem
{
    enum { Type = UserType + 1 };

public:
    Ellipse(qreal x, qreal y, qreal width, qreal height, QColor* color, QGraphicsScene *scene, Movement *movement);
    Movement* getMovement();
    int type() const
    {
	return Type;
    }

private :
    Movement *movement;
};

#endif // ELLIPSE_H
