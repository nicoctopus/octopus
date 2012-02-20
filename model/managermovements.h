#ifndef MANAGERMOVEMENTS_H
#define MANAGERMOVEMENTS_H

#include "manager.h"
#include "movement.h"
#include "managerjointmvt.h"

class ManagerMovements : public Manager
{
private:
    QList<Movement*> *listMovements;
    ManagerJointMvt *managerJointMvt;

public:
    ManagerMovements();
    ~ManagerMovements();
    void initSystem();

    void saveAll();
    void loadAll();
    void save(Movement *movement, QSettings &fichierMovement);
    void remove(Movement *movement);
    void dispacher();

    QList<Movement*>* getListMovements();
    QList<Movement*>* getListMovementsActive();
    ManagerJointMvt* getManagerJointMvt();

    //Methodes
    void sortMovements();
    void sortMovementsById();
    void addMovement(Movement *movement);
};

#endif // MANAGERMOVEMENTS_H
