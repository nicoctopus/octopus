#ifndef SAMPLEAUDIO_H
#define SAMPLEAUDIO_H

#include "sample.h"
#include <QMetaType>

class SampleAudio : public Sample
{
private:
    friend QDataStream & operator << (QDataStream &, const SampleAudio &);
    friend QDataStream & operator >> (QDataStream &, SampleAudio &);


    bool resetAcvtive;
    int  nbLoop;

    //friend QDataStream & operator << (QDataStream & out, const QList<quint32> &valeur);
    //friend QDataStream & operator >> (QDataStream & in, QList<quint32> &valeur);
public:
    SampleAudio();
    SampleAudio(const SampleAudio &copie);
    SampleAudio(const QString &name, const QString &url, const quint32 &listIdMovement, const bool &active, const qint16 &posXBlackBoard, const qint16 &posYBlackBoard);
    SampleAudio(const QString &name, const QString &url, const bool &active, const qint16 &posXBlackBoard, const qint16 &posYBlackBoard);


    bool getResetActive();
    int getNbLoop();

    void setResetActive(bool r);
    void setNbLoop(int n);

    static quint32 idSampleAudioStatic;
};

Q_DECLARE_METATYPE(SampleAudio)
QDataStream & operator << (QDataStream & out, const SampleAudio &valeur);
QDataStream & operator >> (QDataStream & in, SampleAudio &valeur);

/*Q_DECLARE_METATYPE(QList<quint32>)
QDataStream & operator << (QDataStream & out, const QList<quint32> &valeur);
QDataStream & operator >> (QDataStream & in, QList<quint32> &valeur);*/

#endif // SAMPLEAUDIO_H
