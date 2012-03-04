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
    bool getContextMenu();
    void setContextMenu(bool contextMenu);

private :
    ClientOSC *port;
    bool contextMenu;
};

#endif // DIAMOND_H
