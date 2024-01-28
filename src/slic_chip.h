#ifndef	SLIC_CHIP_H
#define SLIC_CHIP_H

class SlicChip {

    public:
     virtual ~SlicChip() {};
     virtual void initialize() = 0;
     virtual void run() = 0;
};

#endif