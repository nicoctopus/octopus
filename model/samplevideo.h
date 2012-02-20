#ifndef SAMPLEVIDEO_H
#define SAMPLEVIDEO_H

#include "sample.h"
#include <QMetaType>

class SampleVideo : public Sample
{
private:
    friend QDataStream & operator << (QDataStream &, const SampleVideo &);
    friend QDataStream & operator >> (QDataStream &, SampleVideo &);
public:
    SampleVideo();
    SampleVideo(const SampleVideo &copie);
    SampleVideo(const QString &name, const QString &url, const quint32 &listIdMovement, const bool &active);

    static quint32 idSampleVideoStatic;
};

Q_DECLARE_METATYPE(SampleVideo)
QDataStream & operator << (QDataStream & out, const SampleVideo &valeur);
QDataStream & operator >> (QDataStream & in, SampleVideo &valeur);

#endif // SAMPLEVIDEO_H
