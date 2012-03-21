#include "sampleaudio.h"
#include <QDebug>

quint32 SampleAudio::idSampleAudioStatic = 0;

SampleAudio::SampleAudio() : Sample()
{
    this->r = rand()%167;
    this->g = 198;
    this->b = rand()%26+67;


    this->resetAcvtive = false;
    this->nbLoop = 0;
}

SampleAudio::SampleAudio(const QString &name, const QString &url, const quint32 &idMovement, const bool &active, const qint16 &posXBlackBoard, const qint16 &posYBlackBoard)
    : Sample(++idSampleAudioStatic, name, url, idMovement, active, posXBlackBoard, posYBlackBoard)
{
    this->r = rand()%167;
    this->g = 198;
    this->b = rand()%26+67;


    this->resetAcvtive = false;
    this->nbLoop = 0;
}

SampleAudio::SampleAudio(const SampleAudio &copie) : Sample(copie.idElement, copie.nameElement, copie.fileURL, *(copie.listIdMovement), copie.active, copie.posXBlackBoard, copie.posYBlackBoard)
{
    this->r = copie.r;
    this->g = copie.g;
    this->b = copie.b;


    this->resetAcvtive = copie.resetAcvtive;
    this->nbLoop = copie.nbLoop;
}

SampleAudio::SampleAudio(const QString &name, const QString &url, const bool &active, const qint16 &posXBlackBoard, const qint16 &posYBlackBoard)
    :Sample(++idSampleAudioStatic, name, url, active, posXBlackBoard, posYBlackBoard)
{
    this->r = rand()%167;
    this->g = 198;
    this->b = rand()%26+67;

    this->resetAcvtive = false;
    this->nbLoop = 0;
}


bool SampleAudio::getResetActive(){
    return this->resetAcvtive;
}

int SampleAudio::getNbLoop(){
    return this->nbLoop;
}

void SampleAudio::setResetActive(bool r){
    this->resetAcvtive = r;
}

void SampleAudio::setNbLoop(int n){
   this->nbLoop = n;
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
    out << valeur.resetAcvtive;
    out << valeur.nbLoop;
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
    in >> valeur.nbLoop;
    in >> valeur.resetAcvtive;
    return in;
}
