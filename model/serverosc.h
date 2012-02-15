#ifndef SERVEROSC_H
#define SERVEROSC_H

#include "port.h"
#include "inputable.h"

#include "movement.h"
#include "joint.h"
#include "analyse.h"

class ServerOSC : public Inputable, public Port
{
    Q_OBJECT
signals:
    void jointMvtTooBig();

public:
    ServerOSC();
    ServerOSC(const quint16 &portNb, const bool &active);
    ~ServerOSC();
    virtual const char* className();
    virtual void run();

    /**
      * Synapse
      */
    void fillBuffers(); // rempli les buffers de tous les joints
    void record(); // rempli les jointsMvt actifs pour enregistrer un mouvement
    void analyze(); // analyser en temps reel tous les mouvements (positions buffers vs positions fixes de chaque mouvement)
    void setListJoints(QList<Joint*> *list);
    void setListMovements(QList<Movement*> *list);
    void setRecording(const bool &b);
    bool isRecording();
    void setAnalyse();
    bool allBuffersSameSize();
    void setMovement(Movement *movement);
    void deleteAnalyse();

private :
    QList<Joint*> *listJoints; //liste de tous les joints possibles
    QList<Movement*> *listMovements; //liste des mouvements enregistres
    Movement *movement; //movement pout le record
    bool recording;
    Analyse *analyse; //objet Analyse pour analyser en mode record
};

#endif // SERVEROSC_H
