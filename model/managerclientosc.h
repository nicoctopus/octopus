#ifndef MANAGERCLIENTOSC_H
#define MANAGERCLIENTOSC_H

#include <QList>
#include <QSettings>
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
    void saveClientOSC(ClientOSC *clientOSC, QSettings &fichierClientOSC);
    void removeClientOSC(QList<ClientOSC*> *listClientsOSC);

};

#endif // MANAGERCLIENTOSC_H
