#include "sampleaudio.h"
#include <QDebug>

quint32 SampleAudio::idSampleAudioStatic = 0;

SampleAudio::SampleAudio() : Sample()
{
    this->r = rand()%167;
    this->g = 198;
    this->b = rand()%26+67;
}

SampleAudio::SampleAudio(const QString &name, const QString &url, const quint32 &idMovement, const bool &active) : Sample(++idSampleAudioStatic, name, url, idMovement, active)
{
    this->r = rand()%167;
    this->g = 198;
    this->b = rand()%26+67;
}

SampleAudio::SampleAudio(const SampleAudio &copie) : Sample(copie.idElement, copie.nameElement, copie.fileURL, *(copie.listIdMovement), copie.active)
{
    this->r = copie.r;
    this->g = copie.g;
    this->b = copie.b;
}

SampleAudio::SampleAudio(const QString &name, const QString &url, const bool &active) :Sample(++idSampleAudioStatic, name, url, active)
{
    this->r = rand()%167;
    this->g = 198;
    this->b = rand()%26+67;
}

//save
QDataStream & operator << (QDataStream & out, const SampleAudio &valeur)
{
    //qDebug() << "Entree operator << SampleAudio" << endl;
    out << valeur.idElement;
    out << static_cast<QList <quint32> >(*(valeur.listIdMovement));
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
    QList<quint32> *list = new QList<quint32>();
    in >> *list;
    valeur.getListIdMovement()->append(*list);
    in >> valeur.nameElement;
    in >> valeur.fileURL;
    in >> valeur.active;
    return in;
}
