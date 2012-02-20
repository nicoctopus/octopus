#ifndef JOINTGRAPHIC_H
#define JOINTGRAPHIC_H

#include <QGraphicsRectItem>

class JointGraphic : public QGraphicsRectItem
{

public:
    JointGraphic();
    QString getName();
    void setName(QString name);
    int getXOrigin();
    int getYOrigin();
    void setXOrigin(int xOrigin);
    void setYOrigin(int yOrigin);
    

private:
    QString name;
    int xOrigin;
    int yOrigin;
    
};

#endif // JOINTGRAPHIC_H
