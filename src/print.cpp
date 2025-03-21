//
// Created by marko on 20.4.22..
//

#include "../h/print.hpp"
#include "../h/riscv.hpp"
//#include "../lib/console.h"
#include "../h/syscall_c.h"
/*
void printString(char const *string)
{
    uint64 sstatus = Riscv::r_sstatus();
    Riscv::mc_sstatus(Riscv::SSTATUS_SIE);
    if(string)
        while (*string != '\0')
        {
            putc(*string);
            string++;
        }
    else
        putc('Q');
    Riscv::ms_sstatus(sstatus & Riscv::SSTATUS_SIE ? Riscv::SSTATUS_SIE : 0);
}
*/


void printInteger(uint64 integer)
{
    uint64 sstatus = Riscv::r_sstatus();
    Riscv::mc_sstatus(Riscv::SSTATUS_SIE);
    static char digits[] = "0123456789";
    char buf[16];
    int i, neg;
    uint x;

    neg = 0;
    if ((long)integer < 0)
    {
        neg = 1;
        x = -integer;
    } else
    {
        x = integer;
    }

    i = 0;
    do
    {
        buf[i++] = digits[x % 10];
    } while ((x /= 10) != 0);
    if (neg)
        buf[i++] = '-';

    while (--i >= 0) { putc(buf[i]); }
    Riscv::ms_sstatus(sstatus & Riscv::SSTATUS_SIE ? Riscv::SSTATUS_SIE : 0);
}

void printAddress(void* address){
    uint64 sstatus = Riscv::r_sstatus();
    Riscv::mc_sstatus(Riscv::SSTATUS_SIE);

    uint64 addr = (uint64)address;
    static char digits[] = "0123456789ABCDEF";
    char buf[24];
    int i;

    i = 0;
    do
    {
        buf[i++] = digits[addr % 16];
    } while ((addr /= 16) != 0);
    buf[i++] = 'x';
    buf[i++] = '0';

    while (--i >= 0) { putc(buf[i]); }
    Riscv::ms_sstatus(sstatus & Riscv::SSTATUS_SIE ? Riscv::SSTATUS_SIE : 0);
}