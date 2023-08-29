#ifndef	SLIC_PCM_H
#define SLIC_PCM_H

class SlicPcm
{
private:
    void initPcmCLock();
    void pipeOutputStream();
    void pipeInputStream();
public:
    SlicPcm(/* args */);
    ~SlicPcm();
};




#endif