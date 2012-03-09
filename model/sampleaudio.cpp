#include "sampleaudio.h"
#include <QDebug>

quint32 SampleAudio::idSampleAudioStatic = 0;

SampleAudio::SampleAudio() : Sample()
{
}

SampleAudio::SampleAudio(const QString &name, const QString &url, const quint32 &idMovement, const bool &active, const qint16 &posXBlackBoard, const qint16 &posYBlackBoard)
    : Sample(++idSampleAudioStatic, name, url, idMovement, active, posXBlackBoard, posYBlackBoard)
{
}

SampleAudio::SampleAudio(const SampleAudio &copie) : Sample(copie.idElement, copie.nameElement, copie.fileURL, *(copie.listIdMovement), copie.active, copie.posXBlackBoard, copie.posYBlackBoard)
{
}

SampleAudio::SampleAudio(const QString &name, const QString &url, const bool &active, const qint16 &posXBlackBoard, const qint16 &posYBlackBoard)
    :Sample(++idSampleAudioStatic, name, url, active, posXBlackBoard, posYBlackBoard)
{

}

//save
QDataStream & operator << (QDataStream & out, const SampleAudio &valeur)
{
    //qDebug() << "Entree operator << SampleAudio" << endl;
    out << valeur.idElement;
    out << valeur.posXBlackBoard;
    out << valeur.posYBlackBoard;
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
    in >> valeur.posXBlackBoard;
    in >> valeur.posYBlackBoard;
    QList<quint32> *list = new QList<quint32>();
    in >> *list;
    valeur.getListIdMovement()->append(*list);
    in >> valeur.nameElement;
    in >> valeur.fileURL;
    in >> valeur.active;
    return in;
}
