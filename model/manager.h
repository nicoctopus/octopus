#ifndef MANAGER_H
#define MANAGER_H

#include <QWidget>
#include <QFile>
#include <QSettings>
#include "joint.h"
#include "movement.h"

class Manager
{
public:
    Manager();
    virtual void loadAll() = 0;
    virtual void saveAll() = 0;
    virtual void initSystem() = 0;
};

#endif // MANAGER_H
