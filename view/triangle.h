#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <QPolygon>
#include <QGraphicsPolygonItem>
#include<QGraphicsScene>
#include "../model/sampleaudio.h"


class Triangle : public QGraphicsPolygonItem
{
    enum { Type = UserType + 3 };

public:
    Triangle(quint16 x, quint16 y, QColor* color, QGraphicsScene *scene, SampleAudio *sampleAudio);
    SampleAudio* getSampleAudio();
    int type() const
    {
	return Type;
    }
    bool getContextMenu();
    void setContextMenu(bool contextMenu);

private:
    SampleAudio *sampleAudio;
    bool contextMenu;
    
};

#endif // TRIANGLE_H
