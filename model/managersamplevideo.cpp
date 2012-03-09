#include "managersamplevideo.h"

ManagerSampleVideo::ManagerSampleVideo() : Manager()
{
    this->initSystem();
    this->listSamplesVideos = new QList<SampleVideo*>();
}

ManagerSampleVideo::~ManagerSampleVideo()
{
    for(int i = 0 ; i < this->listSamplesVideos->size() ; i++)
	delete(this->listSamplesVideos->at(i));
    delete(this->listSamplesVideos);
}

void ManagerSampleVideo::saveAll()
{
    QFile::remove("samplevideo.ini");
    QSettings fichierSampleVideo("samplevideo.ini", QSettings::IniFormat);
    for (int i = 0 ; i < this->listSamplesVideos->size() ; i++)
	this->save(this->listSamplesVideos->at(i), fichierSampleVideo);
    fichierSampleVideo.sync();
}

void ManagerSampleVideo::save(SampleVideo *sampleVideo, QSettings &fichierSampleVideo)
{
    QString key = QString::number(sampleVideo->getId());
    fichierSampleVideo.setValue(key, qVariantFromValue(*sampleVideo));
}

void ManagerSampleVideo::loadAll()
{
    this->listSamplesVideos->clear();

    QSettings fichierSampleVideo("samplevideo.ini", QSettings::IniFormat);
    for(int i = 0 ; i < fichierSampleVideo.allKeys().length() ; i++)
    {
	SampleVideo *temp = new SampleVideo(fichierSampleVideo.value(fichierSampleVideo.allKeys().at(i), qVariantFromValue(SampleVideo())).value<SampleVideo>());
	this->listSamplesVideos->append(temp);
    }
}

void ManagerSampleVideo::remove(SampleVideo *sampleVideo)
{
    QSettings fichierSampleVideo("samplevideo.ini", QSettings::IniFormat);
    fichierSampleVideo.remove(QString::number(sampleVideo->getId()));
    fichierSampleVideo.remove(QString::number(this->listSamplesVideos->last()->getId()));
    this->listSamplesVideos->last()->updateId(sampleVideo->getId());
    this->save(this->listSamplesVideos->last(), fichierSampleVideo);
    SampleVideo::idSampleVideoStatic--;
    fichierSampleVideo.sync();
}

void ManagerSampleVideo::initSystem()
{
    qRegisterMetaTypeStreamOperators<SampleVideo>("SampleVideo");
    qMetaTypeId<SampleVideo>();
}

QList<SampleVideo*>* ManagerSampleVideo::getListSamplesVideos()
{
    return this->listSamplesVideos;
}

QList<SampleVideo*>* ManagerSampleVideo::getListSamplesVideosActive()
{
    QList<SampleVideo*>*listSamplesVideosActive = new QList<SampleVideo*>();
    for(int i = 0 ; i < this->listSamplesVideos->size() ; i++)
	if(this->listSamplesVideos->at(i)->isActive() == true)
	    listSamplesVideosActive->append(this->listSamplesVideos->at(i));
    return listSamplesVideosActive;
}

void ManagerSampleVideo::addSample(QString name, QString url)
{
    this->listSamplesVideos->append(new SampleVideo(name, url, 0, false,0,0));
    QSettings fichierSamplesVideos("samplevideo.ini", QSettings::IniFormat);
    this->save(this->listSamplesVideos->last(), fichierSamplesVideos);
    fichierSamplesVideos.sync();
}

