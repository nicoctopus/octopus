#include "managersampleaudio.h"

ManagerSampleAudio::ManagerSampleAudio() : Manager()
{
    this->initSystem();
    this->listSamplesAudios = new QList<SampleAudio*>();
}

void ManagerSampleAudio::saveAll()
{
    QFile::remove("sampleaudio.ini");
    QSettings fichierSampleAudio("sampleaudio.ini", QSettings::IniFormat);
    for (int i = 0 ; i < this->listSamplesAudios->size() ; i++)
	this->save(this->listSamplesAudios->at(i), fichierSampleAudio);
    fichierSampleAudio.sync();
}

void ManagerSampleAudio::save(SampleAudio *sampleAudio, QSettings &fichierSampleAudio)
{
    QString key = QString::number(sampleAudio->getId());
    fichierSampleAudio.setValue(key, qVariantFromValue(*sampleAudio));
}

void ManagerSampleAudio::loadAll()
{
    this->listSamplesAudios->clear();

    QSettings fichierSampleAudio("sampleaudio.ini", QSettings::IniFormat);
    for(int i = 0 ; i < fichierSampleAudio.allKeys().length() ; i++)
    {
	SampleAudio *temp = new SampleAudio(fichierSampleAudio.value(fichierSampleAudio.allKeys().at(i), qVariantFromValue(SampleAudio())).value<SampleAudio>());
	this->listSamplesAudios->append(temp);
    }
}

void ManagerSampleAudio::remove(SampleAudio *sampleAudio)
{
    QSettings fichierSampleAudio("sampleaudio.ini", QSettings::IniFormat);
    fichierSampleAudio.remove(QString::number(sampleAudio->getId()));
    int idMax = this->listSamplesAudios->at(0)->getId();
    for(int i = 1 ; i < this->listSamplesAudios->size() ; i++)
	if(this->listSamplesAudios->at(i)->getId() > this->listSamplesAudios->at(i - 1)->getId())
	    idMax = this->listSamplesAudios->at(i)->getId();
    fichierSampleAudio.remove(QString::number(idMax));
    for(int i = 0 ; i < this->listSamplesAudios->size() ; i++)
	if(this->listSamplesAudios->at(i)->getId() == idMax)
	{
	    this->listSamplesAudios->at(i)->updateId(sampleAudio->getId());
	    this->save(this->listSamplesAudios->at(i), fichierSampleAudio);
	}
    SampleAudio::idSampleAudioStatic--;
    fichierSampleAudio.sync();
    for(int i = 0 ; i < this->listSamplesAudios->size() ; i++)
	if(this->listSamplesAudios->at(i)->getId() == sampleAudio->getId())
	    this->listSamplesAudios->removeAt(i);
}

QList<SampleAudio*>* ManagerSampleAudio::getListSamplesAudios()
{
    return this->listSamplesAudios;
}

QList<SampleAudio*>* ManagerSampleAudio::getListSamplesAudiosActive()
{
    QList<SampleAudio*>*listSampleAudioActive = new QList<SampleAudio*>();
    for(int i = 0 ; i < this->listSamplesAudios->size() ; i++)
	if(this->listSamplesAudios->at(i)->isActive() == true)
	    listSampleAudioActive->append(this->listSamplesAudios->at(i));
    return listSampleAudioActive;
}

void ManagerSampleAudio::addSample(QString name, QString url)
{
    this->listSamplesAudios->append(new SampleAudio(name, url, false));
    QSettings fichierSampleAudio("sampleaudio.ini", QSettings::IniFormat);
    this->save(this->listSamplesAudios->last(), fichierSampleAudio);
    fichierSampleAudio.sync();
}

void ManagerSampleAudio::initSystem()
{
    qRegisterMetaTypeStreamOperators<SampleAudio>("SampleAudio");
    qMetaTypeId<SampleAudio>();

   // qRegisterMetaTypeStreamOperators<QList<quint32> >("QList<quint32>");
    //qMetaTypeId<QList<quint32> >();
}

ManagerSampleAudio::~ManagerSampleAudio()
{
    for(int i = 0 ; i < this->listSamplesAudios->size() ; i++)
	delete(this->listSamplesAudios->at(i));
    delete(this->listSamplesAudios);
}
