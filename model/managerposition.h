#ifndef MANAGERPOSITION_H
#define MANAGERPOSITION_H

#include "manager.h"
#include "position.h"

class ManagerPosition : public Manager
{
private:
    QList<Position*> *listPositionsTemp;

public:
    ManagerPosition();
    ~ManagerPosition();

    void initSystem();
    void save(Position *position, QSettings &fichierPosition);
    void loadAll();
    void saveAll();
    void remove(QList<Position*> *listPositions);

    QList<Position*> *getListPositionTemp();

};

#endif // MANAGERPOSITION_H
