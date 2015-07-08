
#ifndef MMU_H
#define MMU_H

#include <iostream>
#include <cstring>
#include "PCE.h"

class MMU {
public:

	MMU();
   ~MMU();

    unsigned char readMemory(unsigned short addr);
    unsigned char readMPRi(unsigned char n);
    unsigned char readIO(unsigned short addr);
    unsigned char getTimerStart();
    unsigned char readStack(unsigned short addr);

    void writeStack(unsigned short addr, unsigned char data);
    void writeMemory(unsigned short addr, unsigned char data);
    void writeIO(unsigned short addr, unsigned char data);
    void setMPRi(unsigned char n, unsigned char data);
    void clearMPR();
    void writeLog(std::string text);


    bool startMemory();
    bool isTimerEnable();



private:
	PCE pceLoader;
	bool timerEnable;
	unsigned char timerStart;
	unsigned char mpr        [0x8];
	unsigned char wram       [0x8000];
	unsigned char HuCardROM  [0x100000];


	// Only for debug purpose, log system

	FILE *log;
	
};



#endif