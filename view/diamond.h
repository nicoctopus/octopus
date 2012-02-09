#ifndef DIAMOND_H
#define DIAMOND_H

#include <QPolygon>
#include <QGraphicsPolygonItem>
#include<QGraphicsScene>
#include "../model/clientosc.h"

class Diamond : public QGraphicsPolygonItem
{

    enum { Type = UserType + 2 };
public:
    Diamond(quint16 x, quint16 y, QColor* color, QGraphicsScene *scene, ClientOSC *port);
    ClientOSC* getPort();
    int type() const
    {
	return Type;
    }

private :
    ClientOSC *port;
    
};

#endif // DIAMOND_H
