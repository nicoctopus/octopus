#ifndef DIAMOND_H
#define DIAMOND_H

#include "blackboard.h"
class BlackBoard;
#include <QPolygon>
#include <QGraphicsPolygonItem>
#include<QGraphicsScene>
#include "../model/clientosc.h"

class Diamond : public QGraphicsPolygonItem
{

    enum { Type = UserType + 2 };
public:
    Diamond(quint16 x, quint16 y, QColor* color, QGraphicsScene *scene, ClientOSC *port, BlackBoard* blackboard);
    ~Diamond();
    ClientOSC* getPort();
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
    ClientOSC *port;
    bool contextMenu;
    BlackBoard* blackboard;
    QList<QGraphicsLineItem*> listLines;
};

#endif // DIAMOND_H
