#ifndef MANAGERJOINTMVT_H
#define MANAGERJOINTMVT_H

#include "manager.h"
#include "jointmvt.h"
#include "managerposition.h"

class ManagerJointMvt : public Manager
{
private:
    QList<JointMvt*> *listJointsMvts;
    ManagerPosition *managerPosition;
public:
    ManagerJointMvt();
    ~ManagerJointMvt();

    void initSystem();
    void loadAll();
    void saveAll();
    void save(JointMvt *jointMvt, QSettings &fichierJointMvt);
    void save(QList<JointMvt*>* listJointMvt);
    void remove(QList<JointMvt*> *listJointsMvts);

    QList<JointMvt*>* getListJointsMvts();
    ManagerPosition *getManagerPosition();
};

#endif // MANAGERJOINTMVT_H
