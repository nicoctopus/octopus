#ifndef MYGRAPHICSLINE_H
#define MYGRAPHICSLINE_H

#include<QGraphicsLineItem>
#include<QLine>

class MyQLine : public QLine
{
public:
    MyQLine();
    MyQLine(const QPoint &pt1, const QPoint &pt2);
    QString getNamePointA();
    int getXAOrigin();
    int getYAOrigin();
    void setNamePointA(QString namePointA);
    void setXAOrigin(int xAOrigin);
    void setYAOrigin(int yAOrigin);
    QString getNamePointB();
    int getXBOrigin();
    int getYBOrigin();
    void setNamePointB(QString namePointB);
    void setXBOrigin(int xBOrigin);
    void setYBOrigin(int yBOrigin);
    void setGraphicsLine(QGraphicsLineItem* graphicsLine);
    QGraphicsLineItem* getGraphicsLine();
    void setLastXa(int lastXa);
    void setLastYa(int lastYa);
    void setLastXb(int lastXb);
    void setLastYb(int lastYb);
    int getLastXa();
    int getLastYa();
    int getLastXb();
    int getLastYb();
    void setLastPositionsToOrigins();

private:
    QString namePointA;
    QString namePointB;
    int xAOrigin;
    int xBOrigin;
    int yAOrigin;
    int yBOrigin;
    int lastXa;
    int lastYa;
    int lastXb;
    int lastYb;
    QGraphicsLineItem* graphicsLine;



};

#endif // MYGRAPHICSLINE_H
