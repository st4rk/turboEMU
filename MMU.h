
#ifndef MMU_H
#define MMU_H

#include <iostream>
#include <cstring>
 
class MMU {
public:

	MMU();
   ~MMU();

    unsigned char readMemory(unsigned short addr);
    unsigned char readMPRi(unsigned char n);
    unsigned char readIO(unsigned short addr);
    unsigned char getTimerStart();

    void writeMemory(unsigned short addr, unsigned char data);
    void writeIO(unsigned short addr, unsigned char data);
    void setMPRi(unsigned char n, unsigned char data);
    void clearMPR();


    bool isTimerEnable();
private:
	bool timerEnable;
	unsigned char timerStart;
	unsigned char mpr        [0x8];
	unsigned char wram       [0x8000];
	unsigned char HuCardROM  [0x10000];
};



#endif