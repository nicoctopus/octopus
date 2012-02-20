#ifndef MANAGERCLIENTOSC_H
#define MANAGERCLIENTOSC_H

#include "clientosc.h"
#include "manager.h"

class ManagerClientOSC : public Manager
{
private:
    QList<ClientOSC*> *listClientOSC;

public:
    ManagerClientOSC();
    ~ManagerClientOSC();

    QList<ClientOSC*>* getListClientsOSC();
    QList<ClientOSC*>* getListClientsOSCActive();

    void initSystem();
    void loadAll();
    void saveAll();
    void save(QList<ClientOSC*> *listClientOSC);
    void save(ClientOSC *clientOSC, QSettings &fichierClientOSC);
    void remove(QList<ClientOSC*> *listClientsOSC);
    void remove(ClientOSC *clientOSC);

};

#endif // MANAGERCLIENTOSC_H
