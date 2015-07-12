
#ifndef HUC_6280_H
#define HUC_6280_H

#include <iostream>
#include "MMU.h"

#define FLAG_CARRY 0x1
#define FLAG_ZERO  0x2
#define FLAG_INT   0x4
#define FLAG_DEC   0x8
#define FLAG_BRK   0x10
#define FLAG_T     0x20
#define FLAG_OVER  0x40
#define FLAG_SIGN  0x80

#define SET_FLAG(f, n) (f = f | n)
#define CLEAR_FLAG(f, n) (f = f & ~n)


// The processor can operate in two different speed mode
// 1.79 Mhz and 7.16 Mhz
#define SPEED_MODE_HIGH 1
#define SPEED_MODE_LOW  0

// On the original hardware, the CPU/PPU are running together
// however in this case, we will call the cpu at each scanline
// to it, we will get CPU_HZ/60 (60 frames per second of PPU)
// and how we have 263 scanline, result / 263 and we will have
// ≃ 114 which is the total of cycles per scanline
#define MAX_TICKET 114


#define TIMER_CLOCK 1024

class HuC6280 {
public:
	HuC6280(MMU *ptr);
   ~HuC6280();

   // Stack
   void push(unsigned char data);
   unsigned char pop();

   // Addressing mode 
   void immediate();
   void zeroPage();
   void zeroPage_X();
   void zeroPage_Y();
   void indirect();
   void indexedIndirect_X();
   void indirectIndexed_Y();
   void absolute();
   void absolute_X();
   void absolute_Y();
   void absoluteIndirect_X();
   void relative();

   // Instruction Set
   void adc();
   void and_();
   void asl();
   void asl_a();
   void bbri(char);
   void bcc();
   void bbsi(char);
   void bcs();
   void beq();
   void bit();
   void bmi();
   void bne();
   void bpl();
   void bra();
   void brk();
   void bsr();
   void bvs();
   void bvc();
   void clc();
   void cla();
   void cld();
   void cli();
   void clv();
   void cly();
   void clx();
   void cpx();
   void csh();
   void csl();
   void cmp();
   void dex();
   void dec();
   void dec_a();
   void cpy();
   void eor();
   void inc();
   void inc_a();
   void inx();
   void dey();
   void iny();
   void jmp();
   void jsr();
   void lda();
   void ldx();
   void ldy();
   void lsr();
   void lsr_a();
   void ora();
   void nop();
   void pha();
   void php();
   void phx();
   void phy();
   void pla();
   void plp();
   void plx();
   void ply();
   void rol();
   void rol_a();
   void rmbi(char);
   void ror();
   void ror_a();
   void rti();
   void rts();
   void sax();
   void say();
   void sbc();
   void sed();
   void sec();
   void sei();
   void set();
   void st0();
   void st1();
   void st2();
   void smbi(char);
   void sta();
   void stx();
   void sty();
   void stz();
   void tai();
   void sxy();
   void tami();
   void tax();
   void tay();
   void tia();
   void tdd();
   void tin();
   void tii();
   void tmai();
   void trb();
   void tsb();
   void tst();
   void tsx();
   void txa();
   void txs();
   void tya();

   // CPU Functions
   void resetCPU();
   void executeCPU();
   void setupROM();
   
private:
	// CPU Registers
    unsigned char timer;
    unsigned short timerCycles;

	 unsigned char speedMode;
    unsigned char x;
    unsigned char y;
    unsigned char a;
    unsigned char flag;
    unsigned short sp;
    unsigned short pc;
    unsigned short t_cycles;
    short addrReg; // This register is used to store values from addr modes
    short addrReg_2; // there is some instructions(TST, BBRI[...]) which uses more than one addressing mode
    MMU *memory;
};


#endif