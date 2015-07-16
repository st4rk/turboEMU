
#ifndef MMU_H
#define MMU_H

#include <iostream>
#include <cstring>
#include "PCE.h"
#include "HuC6270.h"

class HuC6270;

class MMU {
public:

	MMU();
   ~MMU();

    unsigned char readMemory(unsigned short addr);
    unsigned char readMPRi(unsigned char n);
    unsigned char readIO(unsigned short addr);
    unsigned char getTimerStart();
    unsigned char readStack(unsigned short addr);

    unsigned short readVRAM(unsigned short addr);
    
    void writeStack(unsigned short addr, unsigned char data);
    void writeMemory(unsigned short addr, unsigned char data);
    void writeIO(unsigned short addr, unsigned char data);
    void writeVRAM(unsigned short addr, unsigned short data);

    void setMPRi(unsigned char n, unsigned char data);
    void clearMPR();
    void writeLog(std::string text);
    void setupVDC(HuC6270 *vdc);


    bool startMemory();
    bool isTimerEnable();

    unsigned char* getVRAM();


private:
	PCE pceLoader;
	bool timerEnable;
	unsigned char timerStart;
	unsigned char mpr        [0x8];
	unsigned char wram       [0x8000];
    unsigned char HuCardROM  [0x100000];
    unsigned char vram       [0x10000];
    unsigned char interruptMask;

    // CPU can access some registers of HuC6270(VDC) and HuC6260(VCE)
    // So we will use *ptr to solve it =)

    HuC6270* VDC;

	// Only for debug purpose, log system
	FILE *log;
	
};



#endif