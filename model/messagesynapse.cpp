#include "messagesynapse.h"

MessageSynapse::MessageSynapse()
{
}

MessageSynapse::MessageSynapse(const QString &s, const quint32 &p)
{
    this->start = s;
    this->params = p;
}

MessageSynapse::~MessageSynapse() {

}

void MessageSynapse::setStart(const QString &s) {
    this->start = s;
}

void MessageSynapse::setParams(const quint32 &p) {
    this->params = p;
}

QString MessageSynapse::getStart() {
    return this->start;
}

quint32 MessageSynapse::getParams() {
    return this->params;
}
