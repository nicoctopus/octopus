#ifndef MESSAGESYNAPSE_H
#define MESSAGESYNAPSE_H

#include <QtGlobal>
#include <QString>

class MessageSynapse
{
public:
    MessageSynapse();
    MessageSynapse(const QString &q, const quint32 &p);
    ~MessageSynapse();
    void setStart(const QString &s);
    void setParams(const quint32 &p);
    QString getStart();
    quint32 getParams();

private:
    QString start;
    quint32 params;
};

#endif // MESSAGESYNAPSE_H
