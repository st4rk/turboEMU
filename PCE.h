#ifndef PCE_H
#define PCE_H


#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <cstring>

class PCE {
public:
	PCE();
   ~PCE();

    bool PCE_LoadFile(std::string fileName);

	unsigned char *buffer;
	unsigned int   size;
};


#endif