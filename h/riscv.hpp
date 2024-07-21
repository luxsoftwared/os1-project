//
// Created by marko on 20.4.22..
//

#ifndef OS1_VEZBE07_RISCV_CONTEXT_SWITCH_2_INTERRUPT_RISCV_HPP
#define OS1_VEZBE07_RISCV_CONTEXT_SWITCH_2_INTERRUPT_RISCV_HPP

#include "../lib/hw.h"

//extern "C" void supervisorTrap();
//extern "C" void C_call_handleSupervisorTrap();


/*
Reg     ABIName Description         Saver
x0      zero    Hard-wired zero     —
x1      ra      Return address      Caller
x2      sp      Stack pointer       Callee
x3      gp      Global pointer      —
x4      tp      Thread pointer      —
x5–7    t0–2    Temporaries         Caller
x8      s0/fp   Saved register/frame pointer    Callee
x9      s1      Saved register      Callee
x10–11  a0–1    Function arguments/return values    Caller
x12–17  a2–7    Function arguments  Caller
x18–27  s2–11   Saved registers     Callee
x28–31  t3–6    Temporaries         Caller
f0–7    ft0–7   FP temporaries      Caller
f8–9    fs0–1   FP saved registers  Callee
f10–11  fa0–1   FP arguments/return values  Caller
f12–17  fa2–7   FP arguments        Caller
f18–27  fs2–11  FP saved registers  Callee
f28–31  ft8–11  FP temporaries      Caller
 */

class Riscv
{
public:

    // read register a0
    static uint64 r_a0();

    // write register a0
    static void w_a0(uint64 a0);

    // read register a1
    static uint64 r_a1();

    // write register a1
    static void w_a1(uint64 a1);

    static uint64 r_a2();

    static uint64 r_a3();

    static uint64 r_a4();

    // pop sstatus.spp and sstatus.spie bits (has to be a non inline function)
    static void popSppSpie();

    // read register scause
    static uint64 r_scause();

    // write register scause
    static void w_scause(uint64 scause);

    // read register sepc
    static uint64 r_sepc();

    // write register sepc
    static void w_sepc(uint64 sepc);

    // read register stvec
    static uint64 r_stvec();

    // write register stvec
    static void w_stvec(uint64 stvec);

    // read register stval
    static uint64 r_stval();

    // write register stval
    static void w_stval(uint64 stval);

    enum BitMaskSip
    {
        SIP_SSIP = (1 << 1),// supervisor lvl software interrupt
        SIP_STIP = (1 << 5),// supervisor lvl timer interrupt
        SIP_SEIP = (1 << 9),// supervisor lvl external interrupt
    };

    // mask set register sip
    static void ms_sip(uint64 mask);

    // mask clear register sip
    static void mc_sip(uint64 mask);

    // read register sip
    static uint64 r_sip();

    // write register sip
    static void w_sip(uint64 sip);

    enum BitMaskSstatus
    {
        SSTATUS_SIE = (1 << 1), // supervisor interrupt enable
        SSTATUS_SPIE = (1 << 5),// previous supervisor interrupt enable
        SSTATUS_SPP = (1 << 8),// previous privilege mode
    };

    // mask set register sstatus
    static void ms_sstatus(uint64 mask);

    // mask clear register sstatus
    static void mc_sstatus(uint64 mask);

    // read register sstatus
    static uint64 r_sstatus();

    // write register sstatus
    static void w_sstatus(uint64 sstatus);

    // supervisor trap
    static void supervisorTrap();

private:

    // supervisor trap handler
    static void handleSupervisorTrap();

};

inline uint64 Riscv::r_a0()
{
    uint64 volatile a0;
    __asm__ volatile ("mv %[a0], a0" : [a0] "=r"(a0));
    return a0;
}

inline void Riscv::w_a0(uint64 a0)
{
    __asm__ volatile ("mv a0, %[a0]" : : [a0] "r"(a0));
}

inline uint64 Riscv::r_a1()
{
    uint64 volatile a1;
    __asm__ volatile ("mv %[a1], a1" : [a1] "=r"(a1));
    return a1;
}

inline void Riscv::w_a1(uint64 a1)
{
    __asm__ volatile ("mv a1, %[a1]" : : [a1] "r"(a1));
}

inline uint64 Riscv::r_a2()
{
    uint64 volatile a2;
    __asm__ volatile ("mv %[a2], a2" : [a2] "=r"(a2));
    return a2;
}
inline uint64 Riscv::r_a3()
{
    uint64 volatile a3;
    __asm__ volatile ("mv %[a3], a3" : [a3] "=r"(a3));
    return a3;
}
inline uint64 Riscv::r_a4()
{
    uint64 volatile a4;
    __asm__ volatile ("mv %[a4], a4" : [a4] "=r"(a4));
    return a4;
}

inline uint64 Riscv::r_scause()
{
    uint64 volatile scause;
    __asm__ volatile ("csrr %[scause], scause" : [scause] "=r"(scause));
    return scause;
}

inline void Riscv::w_scause(uint64 scause)
{
    __asm__ volatile ("csrw scause, %[scause]" : : [scause] "r"(scause));
}

inline uint64 Riscv::r_sepc()
{
    uint64 volatile sepc;
    __asm__ volatile ("csrr %[sepc], sepc" : [sepc] "=r"(sepc));
    return sepc;
}

inline void Riscv::w_sepc(uint64 sepc)
{
    __asm__ volatile ("csrw sepc, %[sepc]" : : [sepc] "r"(sepc));
}

inline uint64 Riscv::r_stvec()
{
    uint64 volatile stvec;
    __asm__ volatile ("csrr %[stvec], stvec" : [stvec] "=r"(stvec));
    return stvec;
}

inline void Riscv::w_stvec(uint64 stvec)
{
    __asm__ volatile ("csrw stvec, %[stvec]" : : [stvec] "r"(stvec));
}

inline uint64 Riscv::r_stval()
{
    uint64 volatile stval;
    __asm__ volatile ("csrr %[stval], stval" : [stval] "=r"(stval));
    return stval;
}

inline void Riscv::w_stval(uint64 stval)
{
    __asm__ volatile ("csrw stval, %[stval]" : : [stval] "r"(stval));
}

inline void Riscv::ms_sip(uint64 mask)
{
    __asm__ volatile ("csrs sip, %[mask]" : : [mask] "r"(mask));
}

inline void Riscv::mc_sip(uint64 mask)
{
    __asm__ volatile ("csrc sip, %[mask]" : : [mask] "r"(mask));
}

inline uint64 Riscv::r_sip()
{
    uint64 volatile sip;
    __asm__ volatile ("csrr %[sip], sip" : [sip] "=r"(sip));
    return sip;
}

inline void Riscv::w_sip(uint64 sip)
{
    __asm__ volatile ("csrw sip, %[sip]" : : [sip] "r"(sip));
}

inline void Riscv::ms_sstatus(uint64 mask)
{
    __asm__ volatile ("csrs sstatus, %[mask]" : : [mask] "r"(mask));
}

inline void Riscv::mc_sstatus(uint64 mask)
{
    __asm__ volatile ("csrc sstatus, %[mask]" : : [mask] "r"(mask));
}

inline uint64 Riscv::r_sstatus()
{
    uint64 volatile sstatus;
    __asm__ volatile ("csrr %[sstatus], sstatus" : [sstatus] "=r"(sstatus));
    return sstatus;
}

inline void Riscv::w_sstatus(uint64 sstatus)
{
    __asm__ volatile ("csrw sstatus, %[sstatus]" : : [sstatus] "r"(sstatus));
}

#endif //OS1_VEZBE07_RISCV_CONTEXT_SWITCH_2_INTERRUPT_RISCV_HPP
