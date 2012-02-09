#ifndef DATABITG_H
#define DATABITG_H

#include <QtGlobal>

/**
  * Donnees en format BITG
  * B le barycentre entre 2 Joints
  * I l'id du Mouvement en cours
  * T le pas de temps
  * G l'ecart entre deux mesures
  */

class DataBITG
{
public:
    DataBITG();
    DataBITG(const quint32 &bary, const quint32 &idmove, const quint32 &t, const quint32 &g);
    void setBarycenter(const quint32 &b);
    quint32 getBarycenter();
    void setIdMovement(const quint32 &id);
    quint32 getIdMovement();
    void setTime(const quint32 &t);
    quint32 getTime();
    void setGap(const quint32 &g);
    quint32 getGap();

private:
    quint32  barycenter;
    quint32  idMovement;
    quint32  time;
    quint32  gap;
};

#endif // DATABITG_H
