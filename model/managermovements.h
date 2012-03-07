#ifndef MANAGERMOVEMENTS_H
#define MANAGERMOVEMENTS_H

#include "manager.h"
#include "movement.h"

class ManagerMovements : public Manager
{
private:
    QList<Movement*> *listMovements;

public:
    ManagerMovements();
    ~ManagerMovements();
    void initSystem();

    void saveAll();
    void loadAll();
    void save(Movement *movement, QSettings &fichierMovement);
    void remove(Movement *movement);

    QList<Movement*>* getListMovements();
    QList<Movement*>* getListMovementsActive();

    //Methodes
    void sortMovementsByPositionSize();
    void sortMovementsById();
    QList<Movement*>* getListMovementsByName();
    void addMovement(Movement *movement);
};

#endif // MANAGERMOVEMENTS_H
