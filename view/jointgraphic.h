#ifndef JOINTGRAPHIC_H
#define JOINTGRAPHIC_H

#include <QGraphicsRectItem>

class JointGraphic : public QGraphicsRectItem
{

public:
    JointGraphic();
    QString getName();
    void setName(QString name);
    

private:
    QString name;
    
};

#endif // JOINTGRAPHIC_H
