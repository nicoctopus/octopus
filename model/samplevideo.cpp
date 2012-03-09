#include "samplevideo.h"

quint32 SampleVideo::idSampleVideoStatic = 0;

SampleVideo::SampleVideo() : Sample()
{

}

SampleVideo::SampleVideo(const QString &name, const QString &url, const quint32 &idMovement, const bool &active, const qint16 &xBlackBoard, const qint16 &yBlackBoard)
    : Sample(++idSampleVideoStatic, name, url, idMovement, active, xBlackBoard, yBlackBoard)
{
}


SampleVideo::SampleVideo(const SampleVideo &copie) : Sample(copie.idElement, copie.nameElement, copie.fileURL, *(copie.listIdMovement), copie.active, copie.posXBlackBoard, copie.posYBlackBoard)
{
}

//save
QDataStream & operator << (QDataStream & out, const SampleVideo &valeur)
{
    //std::cout << "Entree operator << SampleVideo" << std::endl;
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
QDataStream & operator >> (QDataStream & in, SampleVideo &valeur)
{
    //std::cout << "Entrée operator >> SampleVideo" << std::endl;
    in >> valeur.idElement;
    in >> valeur.posXBlackBoard;
    in >> valeur.posYBlackBoard;
    QList<quint32> *list = new QList<quint32>();
    in >> *list;
    valeur.getListIdMovement()->append(*list);    in >> valeur.nameElement;
    in >> valeur.fileURL;
    in >> valeur.active;
    return in;
}
