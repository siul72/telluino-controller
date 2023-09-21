#ifndef	SLIC_PCM_H
#define SLIC_PCM_H

#include <Arduino.h>

#define SAMPLE_RATE 8000
#define BUFFER_SIZE 1024



class SlicPcm
{
private:
    const int speakerPin = 11;
    unsigned long sounddata_length=0;
    unsigned long sample=0;
    unsigned long BytesReceived=0;

    unsigned long Temp=0;
    unsigned long NewTemp=0;

    int ledPin = 13;
  
    int Playing = 0;
    void initPcmCLock();
    void pipeOutputStream();
    void pipeInputStream();

public:
    SlicPcm(/* args */);
    ~SlicPcm();
    void setup();
    void startPlayback();
    static void playNext();
};

#endif