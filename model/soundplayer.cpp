#include "soundplayer.h"

SoundPlayer::SoundPlayer(int nbr)
{
    this->result = System_Create(&system);
    if(result != FMOD_OK){
	qDebug()<< "PBLM create system !" << endl;

    }

    this->result = system->init(nbr, FMOD_INIT_NORMAL, NULL);
    if(result != FMOD_OK){
	qDebug()<< "PBLM init system !" << endl;

    }



}

void SoundPlayer::Stop(){

    int nbr;
    this->system->getChannelsPlaying(&nbr);

    if(nbr>0){

	this->result=this->system->getChannel(0,&channel);
	if(result != FMOD_OK)
	    qDebug()<< "PBLM recup index" << endl;

	channel->stop();

    }




}


bool SoundPlayer::playDemo(SampleAudio*aud){
    // FMOD::Sound *sound;
    bool playing=0;
    bool paused=0;
    int nbr;

    //soundlist.append(sound);


    this->system->getChannelsPlaying(&nbr);


    if(nbr>0){
	this->result=this->system->getChannel(0,&channel);
	if(result != FMOD_OK)
	    qDebug()<< "PBLM recup index" << endl;

	this->result = channel->isPlaying(&playing);
	if(result != FMOD_OK)
	    qDebug()<< "PBLM test si joué ?" << endl;

	this->result = channel->getPaused(&paused);
	if(result != FMOD_OK)
	    qDebug()<< "PBLM test si pause ?" << endl;
    }

    qDebug()<< "bide : "<< paused << endl;
    qDebug()<< "bidepl : "<< playing << endl;
    if(playing){


	if(paused){
	    channel->setPaused(false);
	    return true;
	}else{
	    channel->setPaused(true);
	    return false;
	}


    }else{

	this->result = system->createSound(aud->getFileURL().toStdString().c_str(),FMOD_DEFAULT,0,&soundDemo);
	if(result != FMOD_OK)
	    qDebug()<< "PBLM creation stream" << endl;


	this->result = system->playSound((FMOD_CHANNELINDEX) 0, soundDemo, FALSE,&channel);
	if(result != FMOD_OK)
	    qDebug()<< "PBLM play stream arreté" << endl;


	return true;
    }

    return true;

}

quint32 SoundPlayer::currentTime(){


    quint32 pos=0;
    int nbr;
    this->system->getChannelsPlaying(&nbr);

    if(nbr>0){

	this->result=this->system->getChannel(0,&channel);
	if(result != FMOD_OK)
	    qDebug()<< "PBLM recup index" << endl;

	channel->getPosition(&pos, FMOD_TIMEUNIT_MS);

    }
    return pos;




}

quint32 SoundPlayer::TimeEnd(SampleAudio* aud){

    //FMOD::Sound *sound;
    quint32 time;

    this->result = system->createStream(aud->getFileURL().toStdString().c_str(),FMOD_DEFAULT,0,&soundDemo);
    if(result != FMOD_OK)
	qDebug()<< "PBLM creation stream" << endl;

    soundDemo->getLength(&time,FMOD_TIMEUNIT_MS);

    return time;

}


quint32 SoundPlayer::Position(SampleAudio* aud){

    quint32 pos;
    int nbr;
    this->system->getChannelsPlaying(&nbr);

    if(nbr>0){

	this->result=this->system->getChannel(0,&channel);
	if(result != FMOD_OK)
	    qDebug()<< "PBLM recup index" << endl;

	channel->getPosition(&pos, FMOD_TIMEUNIT_MS);

    }
    return pos;

}
/*
void ERRCHECK(FMOD_RESULT result)
{
    if (result != FMOD_OK)
    {
	printf("FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
	exit(-1);
    }
}
*/


SoundPlayer::~SoundPlayer(){

    /*
    qDebug()<<soundlist.size()<<endl;
    for(int i=1;i<soundlist.size();i++){
	this->result = soundlist.at(i)->release();
	if(result != FMOD_OK)
	    qDebug()<< "PBLM RELEASE SOUND" << endl;
    }*/

    this->result = system->close();
    if(result != FMOD_OK){
	qDebug()<< "PBLM CLOSE SYSTEM" << endl;

    }

    this->result = system->release();
    if(result != FMOD_OK){
	qDebug()<< "PBLM RELEASE SYSTEM" << endl;

    }



}

void SoundPlayer::play(SampleAudio* aud){


    if(map.contains(aud->getId())){
	FMOD::Channel *chan;

	bool playing=0;


	this->result=this->system->getChannel(map.value(aud->getId()),&chan);
	if(result != FMOD_OK)
	    qDebug()<< "PBLM recup index" << endl;


	this->result = chan->isPlaying(&playing);
	if(result != FMOD_OK)
	    qDebug()<< "PBLM test si joué ?" << endl;

	if(playing){

	    chan->setPaused(true);
	    chan->setPosition(500,FMOD_TIMEUNIT_MS);
	    chan->setPaused(false);


	}else{
	    FMOD::Sound *sound;
	    soundlist.append(sound);

	    this->result = system->createStream(aud->getFileURL().toStdString().c_str(),FMOD_DEFAULT,0,&sound);
	    if(result != FMOD_OK)
		qDebug()<< "PBLM creation stream" << endl;


	    this->result = system->playSound((FMOD_CHANNELINDEX) map.value(aud->getId()) , sound, FALSE,&chan);
	    if(result != FMOD_OK)
		qDebug()<< "PBLM play stream arreté" << endl;

	}


    }else{
	FMOD::Sound *sound;
	soundlist.append(sound);
	int x=0;


	this->result = system->createStream(aud->getFileURL().toStdString().c_str(),FMOD_DEFAULT,0,&sound);
	if(result != FMOD_OK)
	    qDebug()<< "PBLM Creation Nouveau Stream" << endl;

	this->result = system->playSound(FMOD_CHANNEL_FREE, sound, FALSE,&this->channel);
	if(result != FMOD_OK)
	    qDebug()<< "PBLM play nouveau stream" << endl;

	this->result = channel->getIndex(&x);
	if(result != FMOD_OK)
	    qDebug()<< "PBLM Index" << endl;


	map.insert(aud->getId(),x);


    }



}

void SoundPlayer::loop(quint32 deb, quint32 end, quint32 compt, SampleAudio *aud){


    if(map.contains(aud->getId())){
	FMOD::Channel *chan;
	FMOD::Sound *sound;

	bool playing=0;

	this->result=this->system->getChannel(map.value(aud->getId()),&chan);
	if(result != FMOD_OK)
	    qDebug()<< "PBLM recup index" << endl;


	this->result = chan->isPlaying(&playing);
	if(result != FMOD_OK)
	    qDebug()<< "PBLM test si joué ?" << endl;

	if(playing){
	    soundlist.append(sound);
	    chan->stop();
	    this->result = system->createStream(aud->getFileURL().toStdString().c_str(),FMOD_DEFAULT,0,&sound);
	    if(result != FMOD_OK)
		qDebug()<< "PBLM creation stream" << endl;

	    if(deb!=0 && end!=0){
		sound->setLoopPoints(deb,FMOD_TIMEUNIT_MS,end,FMOD_TIMEUNIT_MS);
		sound->setLoopCount(compt);
		sound->setMode(FMOD_LOOP_NORMAL);
	    }else{
		sound->getLength(&end,FMOD_TIMEUNIT_MS);
		sound->setLoopPoints(0,FMOD_TIMEUNIT_MS,end,FMOD_TIMEUNIT_MS);
		sound->setLoopCount(compt);

	    }

	    this->result = system->playSound((FMOD_CHANNELINDEX) map.value(aud->getId()) , sound, FALSE,&chan);
	    if(result != FMOD_OK)
		qDebug()<< "PBLM play stream arreté" << endl;

	}else{

	    soundlist.append(sound);

	    this->result = system->createStream(aud->getFileURL().toStdString().c_str(),FMOD_DEFAULT,0,&sound);
	    if(result != FMOD_OK)
		qDebug()<< "PBLM creation stream" << endl;


	    if(deb!=0 && end!=0){
		sound->setLoopPoints(deb,FMOD_TIMEUNIT_MS,end,FMOD_TIMEUNIT_MS);
		sound->setLoopCount(compt);
		sound->setMode(FMOD_LOOP_NORMAL);
	    }else{
		sound->getLength(&end,FMOD_TIMEUNIT_MS);
		sound->setLoopPoints(0,FMOD_TIMEUNIT_MS,end,FMOD_TIMEUNIT_MS);
		sound->setLoopCount(compt);

	    }

	    this->result = system->playSound((FMOD_CHANNELINDEX) map.value(aud->getId()) , sound, FALSE,&chan);
	    if(result != FMOD_OK)
		qDebug()<< "PBLM play stream arreté" << endl;

	}


    }else{
	FMOD::Sound *sound;
	soundlist.append(sound);
	int x=0;


	this->result = system->createStream(aud->getFileURL().toStdString().c_str(),FMOD_DEFAULT,0,&sound);
	if(result != FMOD_OK)
	    qDebug()<< "PBLM Creation Nouveau Stream" << endl;

	if(deb!=0 && end!=0){
	    sound->setLoopPoints(deb,FMOD_TIMEUNIT_MS,end,FMOD_TIMEUNIT_MS);
	    sound->setLoopCount(compt);
	    sound->setMode(FMOD_LOOP_NORMAL);
	}else{
	    sound->getLength(&end,FMOD_TIMEUNIT_MS);
	    sound->setLoopPoints(0,FMOD_TIMEUNIT_MS,end,FMOD_TIMEUNIT_MS);
	    sound->setLoopCount(compt);

	}


	this->result = system->playSound(FMOD_CHANNEL_FREE, sound, FALSE,&this->channel);
	if(result != FMOD_OK)
	    qDebug()<< "PBLM play nouveau stream" << endl;

	this->result = channel->getIndex(&x);
	if(result != FMOD_OK)
	    qDebug()<< "PBLM Index" << endl;


	map.insert(aud->getId(),x);


    }


}


