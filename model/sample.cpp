#include "sample.h"

Sample::Sample() {

}

Sample::Sample(const quint32 &id, const QString &name, const QString &fileURL, const quint32 &idMovement, const bool &active) : Element(id, name, active)
{
    this->fileURL = fileURL;
    this->idMovement = idMovement;
}

void Sample::setFileURL(const QString &file) {
    this->fileURL = file;
}

void Sample::updateIdMovement(const quint32 &idMovement)
{
    this->idMovement = idMovement;
}

QString Sample::getFileURL() {
    return this->fileURL;
}

quint32 Sample::getIdMovement()
{
    return this->idMovement;
}

const char* Sample::className(){
    return "Sample";
}

