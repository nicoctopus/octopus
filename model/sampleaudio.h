#ifndef SAMPLEAUDIO_H
#define SAMPLEAUDIO_H

#include "sample.h"
#include <QMetaType>

class SampleAudio : public Sample
{
private:
    friend QDataStream & operator << (QDataStream &, const SampleAudio &);
    friend QDataStream & operator >> (QDataStream &, SampleAudio &);
public:
    SampleAudio();
    SampleAudio(const SampleAudio &copie);
    SampleAudio(const QString &name, const QString &url, const quint32 &idMovement, const bool &active);

    static quint32 idSampleAudioStatic;
};

Q_DECLARE_METATYPE(SampleAudio)
QDataStream & operator << (QDataStream & out, const SampleAudio &valeur);
QDataStream & operator >> (QDataStream & in, SampleAudio &valeur);
#endif // SAMPLEAUDIO_H
