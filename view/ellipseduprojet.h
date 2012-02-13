#ifndef ELLIPSEDUPROJET_H
#define ELLIPSEDUPROJET_H

#include<QGraphicsEllipseItem>
#include <QGraphicsScene>
#include "model/movement.h"

class EllipseDuProjet : public QGraphicsEllipseItem
{
   enum { Type = UserType + 1 };

public:
    EllipseDuProjet(qreal x, qreal y, qreal width, qreal height, QColor* color, QGraphicsScene *scene, Movement *movement);
    Movement* getMovement();
   int type() const
    {
        return Type;
    }

private :
    Movement *movement;
};

#endif // ELLIPSEDUPROJET_H
