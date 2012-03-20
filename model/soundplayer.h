#ifndef SOUNDPLAYER_H
#define SOUNDPLAYER_H

#include <fmod.hpp>
#include <fmod_errors.h>


#include <stdio.h>
#include <stdlib.h>

#include <QList>
#include <QDebug>
#include "sampleaudio.h"
#include <QMap>


class SoundPlayer
{

private:

    FMOD::System    *system;
    FMOD::Sound *soundDemo;
    FMOD::Channel    *channel;
    FMOD_RESULT      result;
    QList <FMOD::Sound*> soundlist;
    QMap <quint32,int> map;




public:
    SoundPlayer(int);
    ~SoundPlayer();
    //void loop(quint32,quint32,qint32,SampleAudio*);
    void play(SampleAudio*);
    //void readyToPlay(SampleAudio*);
    quint32 TimeEnd(SampleAudio*);
    bool playDemo(SampleAudio*);
    void Stop();
    quint32 Position(SampleAudio*);
    quint32 currentTime();

};

#endif // SOUNDPLAYER_H
