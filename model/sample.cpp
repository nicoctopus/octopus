#include "sample.h"

Sample::Sample() {
    this->listIdMovement = new QList<quint32>();
}

Sample::Sample(const quint32 &id, const QString &name, const QString &fileURL, const quint32 &idMovement, const bool &active) : Element(id, name, active)
{
    this->fileURL = fileURL;
    this->listIdMovement = new QList<quint32>();
    this->listIdMovement->append(idMovement);
}

Sample::Sample(const quint32 &id, const QString &name, const QString &fileURL, const QList<quint32> &listIdMovement, const bool &active) : Element(id, name, active)
{
    this->fileURL = fileURL;
    this->listIdMovement = new QList<quint32>();
    for(int i = 0 ; i < listIdMovement.size() ; i++)
	this->listIdMovement->append(listIdMovement.at(i));
}

Sample::Sample(const quint32 &id, const QString &name, const QString &fileURL, const bool &active) : Element(id, name, active)
{
    this->fileURL = fileURL;
    this->listIdMovement = new QList<quint32>();
}

void Sample::setFileURL(const QString &file) {
    this->fileURL = file;
}

void Sample::updateIdMovement(const quint32 &newId)
{
    this->listIdMovement->append(newId);
}

QString Sample::getFileURL() {
    return this->fileURL;
}

QList<quint32>* Sample::getListIdMovement()
{
    return this->listIdMovement;
}

void Sample::removeId(const quint32 &idMovement)
{
    for(int i = 0 ; i < this->listIdMovement->size() ; i++)
	if(this->listIdMovement->at(i) == idMovement)
	    this->listIdMovement->removeAt(i);
}


const char* Sample::className(){
    return "Sample";
}

