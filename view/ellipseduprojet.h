#ifndef ELLIPSEDUPROJET_H
#define ELLIPSEDUPROJET_H

#include "blackboard.h"
class BlackBoard;
#include<QGraphicsEllipseItem>
#include <QGraphicsScene>
#include "model/movement.h"

class EllipseDuProjet : public QGraphicsEllipseItem
{
   enum { Type = UserType + 1 };

public:
    EllipseDuProjet(qreal x, qreal y, qreal width, qreal height, QColor* color, QGraphicsScene *scene, Movement *movement, BlackBoard* blackboard);
    ~EllipseDuProjet();
    Movement* getMovement();
   int type() const
    {
        return Type;
    }
   bool getContextMenu();
   void setContextMenu(bool contextMenu);
   void setListLines(QGraphicsLineItem* line);
   QList<QGraphicsLineItem*> getListLines();


protected:
   void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
   void mousePressEvent(QGraphicsSceneMouseEvent *event);
   QVariant itemChange(GraphicsItemChange change, const QVariant &value);


private :
    Movement *movement;
    bool contextMenu;
    BlackBoard* blackboard;
    QList<QGraphicsLineItem*> listLines;
};

#endif // ELLIPSEDUPROJET_H
