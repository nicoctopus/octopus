#ifndef WIDGETCOURBES_H
#define WIDGETCOURBES_H

#include <QWidget>
#include "../model/movement.h"
#include <QTableWidget>
#include <QScrollArea>

namespace Ui {
    class WidgetCourbes;
}

class WidgetCourbes : public QWidget
{
    Q_OBJECT

public:
    explicit WidgetCourbes(QWidget *parent = 0);
    void setJointMvt(JointMvt *jointMouvement);
    ~WidgetCourbes();
private :
    JointMvt* joint;

private:
    Ui::WidgetCourbes *ui;
};

#endif // WIDGETCOURBES_H
