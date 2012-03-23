#include "widgetcourbes.h"
#include "ui_widgetcourbes.h"

WidgetCourbes::WidgetCourbes(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WidgetCourbes)
{
    ui->setupUi(this);
}

void WidgetCourbes::setJointMvt(JointMvt *jointMouvement)
{
    this->joint = jointMouvement;
    ui->widgetX->setPositions(joint->getListPositions(), joint->getJointRef()->getBufferPositions(), 1);
    ui->widgetY->setPositions(joint->getListPositions(), joint->getJointRef()->getBufferPositions(), 2);
    ui->widgetZ->setPositions(joint->getListPositions(), joint->getJointRef()->getBufferPositions(), 3);
}

WidgetCourbes::~WidgetCourbes()
{
    delete ui;
}
