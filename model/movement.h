#ifndef MOVEMENT_H
#define MOVEMENT_H

#include <QList>
#include <QMetaType>
#include "element.h"
#include "jointmvt.h"
#include "sampleaudio.h"
#include "samplevideo.h"
#include "clientosc.h"

class Movement : public Element
{
public:
    Movement();
    Movement(const Movement &movement);
    //Movement(Movement *movement);
    Movement(const QString &name);
    Movement(const QString &name, const bool &active, const QList<JointMvt*> *listJointsMvt
               , SampleAudio *sa, SampleVideo *sv, QList<ClientOSC*> *listClients);
    ~Movement();
    QList<ClientOSC*>* getListClients();
    SampleAudio* getSampleAudio();
    SampleVideo* getSampleVideo();
    void setSampleAudio(SampleAudio *sampleAudio);
    void setSampleVideo(SampleVideo *sampleVideo);
    QList<JointMvt*>* getListJointsMvt();
    void addJointMvt(JointMvt *jointMvt);
    void addClientOSC(ClientOSC *clientOSC);
    void save();
    void updateId(const quint32 &id);
    const char* className();
    //VARIABLE STATIC ID
    static quint32 idMovementStatic;
    void addJointMvt(Joint*ref);

private:
    SampleAudio *sampleAudio;
    SampleVideo *sampleVideo;
    QList<ClientOSC*> *listClients;
    QList<JointMvt*> *listJointsMvt;
    friend QDataStream & operator << (QDataStream &, const Movement &);
    friend QDataStream & operator >> (QDataStream &, Movement &);
};

Q_DECLARE_METATYPE(Movement)
QDataStream & operator << (QDataStream & out, const Movement &valeur);
QDataStream & operator >> (QDataStream & in, Movement &valeur);

#endif // MOVEMENT_H
