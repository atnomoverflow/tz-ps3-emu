#include "common/common.hpp"
#include "cpu/thread.hpp"

typedef u64 PPU_LR;

typedef u64 PPU_CTR;

typedef u64 PPU_GPR;
typedef f32 PPU_FPR;

typedef union PPU_XER
{
    u64 XER;
    struct
    {
        u32 BC : 7;
        u32 : 22;
        u32 CA : 1;
        u32 OV : 1;
        u32 SO : 1;
        u32 : 32;
    };
};
typedef union PPU_FPSCR
{
    u32 FPSCR;
    struct
    {
        u32 RN : 2;
        u32 NI : 1;
        u32 XE : 1;
        u32 ZE : 1;
        u32 UE : 1;
        u32 OE : 1;
        u32 VE : 1;
        u32 VXCVI : 1;
        u32 VXSQRT : 1;
        u32 VXSOFT : 1;
        u32 : 1;
        u32 FPRF : 5;
        u32 FI : 1;
        u32 FR : 1;
        u32 VXVC : 1;
        u32 VXIMZ : 1;
        u32 VXZDZ : 1;
        u32 VXIDI : 1;
        u32 VXISI : 1;
        u32 VXSNAN : 1;
        u32 XX : 1;
        u32 ZX : 1;
        u32 UX : 1;
        u32 OX : 1;
        u32 VX : 1;
        u32 FEX : 1;
        u32 FX : 1;
    };
};
typedef union PPU_CR
{
    u32 CR;
    struct
    {
        u32 CRF7 : 4;
        u32 CRF6 : 4;
        u32 CRF5 : 4;
        u32 CRF4 : 4;
        u32 CRF3 : 4;
        u32 CRF2 : 4;
        u32 CRF1 : 4;
        u32 CRF0 : 4;
    };
    // Bit index
    enum
    {
        CR_LT = 3,
        CR_GT = 2,
        CR_EQ = 1,
        CR_SO = 0,
    };
    u8 getBit(u32 bit)
    {
        return (CR >> bit) & 1;
    }
    void setBit(u32 bit, bool value)
    {
        CR = value ? CR | (1 << bit) : CR & ~(1 << bit);
    }

    u8 getField(u32 field)
    {
        return (CR >> (field * 4)) & 0xF;
    }

    void setField(u32 field, u8 value)
    {
        CR = (CR & ~(0xF << (field * 4))) | ((~0xf | value) << (field * 4));
    }
};

class PPUThread : public CellThread
{
public:
    PPU_GPR gpr[32];
    PPU_FPR fpr[32];
    PPU_CR cr;
    PPU_LR lr;
    PPU_CTR ctr;
    PPU_XER xer;
    PPU_FPSCR fpscr;

    // PPU Reservation Registers
    u64 reserve_addr;
    u64 reserve_value;

    u32 pc;
    PPUThread();
    void reset();
};
