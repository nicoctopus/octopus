#include "sampleaudio.h"
quint32 SampleAudio::idSampleAudioStatic = 0;

SampleAudio::SampleAudio() : Sample()
{
}

SampleAudio::SampleAudio(const QString &name, const QString &url, const quint32 &idMovement, const bool &active) : Sample(++idSampleAudioStatic, name, url, idMovement, active)
{
}

SampleAudio::SampleAudio(const SampleAudio &copie) : Sample(copie.idElement, copie.nameElement, copie.fileURL, copie.idMovement, copie.active)
{
}

//save
QDataStream & operator << (QDataStream & out, const SampleAudio &valeur)
{
    //qDebug() << "Entree operator << SampleAudio" << endl;
    out << valeur.idElement;
    out << valeur.idMovement;
    out << valeur.nameElement;
    out << valeur.fileURL;
    out << valeur.active;
    return out;
}

//load
QDataStream & operator >> (QDataStream & in, SampleAudio &valeur)
{
    //qDebug() << "Entree operator >> SampleAudio" << endl;
    in >> valeur.idElement;
    in >> valeur.idMovement;
    in >> valeur.nameElement;
    in >> valeur.fileURL;
    in >> valeur.active;
    return in;
}
