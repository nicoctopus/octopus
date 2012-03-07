#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "blackboard.h"
class BlackBoard;
#include <QPolygon>
#include <QGraphicsPolygonItem>
#include <QGraphicsScene>
#include "../model/sampleaudio.h"


class Triangle : public QGraphicsPolygonItem
{
    enum { Type = UserType + 3 };

public:
    Triangle(quint16 x, quint16 y, QColor* color, QGraphicsScene *scene, SampleAudio *sampleAudio, BlackBoard* blackboard);
    SampleAudio* getSampleAudio();
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


private:
    SampleAudio *sampleAudio;
    bool contextMenu;
    BlackBoard* blackboard;
    QList<QGraphicsLineItem*> listLines;
    
};

#endif // TRIANGLE_H
