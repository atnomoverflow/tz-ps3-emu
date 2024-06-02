#include "cpu/ppu/interpetor.hpp"
#include "interpetor.hpp"
#include <cmath> // For isnan
void PPUInterpreter::step()
{
}

bool PPUInterpreter::isCarry(u64 a, u64 b)
{
    return (a + b) < a;
}

void PPUInterpreter::handel_overflow_XO_form(u32 rt, u32 ra, u32 rb, bool oe, bool rc)
{
    if (rc)
    {
        u8 cr0 = ((m_thread.gpr[rt] < 0) << 3 | (m_thread.gpr[rt] > 0) << 2 | (m_thread.gpr[rt] == 0) << 1);
        cr0 |= ((u8)m_thread.xer.SO);
        m_thread.cr.setField(0, cr0);
    }
    if (oe)
    {

        // if they have the same sign but the sum is in diffrent sign then it's an overflow
        if ((m_thread.gpr[rb] > 0 && m_thread.gpr[ra] > 0 && m_thread.gpr[rt] < 0) ||
            (m_thread.gpr[rb] < 0 && m_thread.gpr[ra] < 0 && m_thread.gpr[rt] > 0))
        {
            m_thread.xer.OV = 1;
            m_thread.xer.SO = 1;
        }
        else
        {
            m_thread.xer.OV = 0;
            m_thread.xer.SO = 0;
        }
    }
}

void PPUInterpreter::add(u32 rt, u32 ra, u32 rb, bool oe, bool rc)
{
    m_thread.gpr[rt] = m_thread.gpr[ra] + m_thread.gpr[rb];
    handel_overflow_XO_form(rt, ra, rb, oe, rc);
}

void PPUInterpreter::addc(u32 rt, u32 ra, u32 rb, bool oe, bool rc)
{
    s64 sgrpa = (s32)m_thread.gpr[ra];
    s64 sgrpb = (s32)m_thread.gpr[rb];

    m_thread.gpr[rt] = sgrpa + sgrpb;
    m_thread.xer.CA = isCarry(sgrpa, sgrpb);
    handel_overflow_XO_form(rt, ra, rb, oe, rc);
}

void PPUInterpreter::adde(u32 rt, u32 ra, u32 rb, bool oe, bool rc)
{
    if (!m_thread.xer.CA)
    {
        addc(rt, ra, rb, oe, rc);
        return;
    }
    s64 sgrpa = (s32)m_thread.gpr[ra];
    s64 sgrpb = (s32)m_thread.gpr[rb];
    if (sgrpa == ~0UL)
    {
        m_thread.gpr[rt] = sgrpb;
        m_thread.xer.CA = 1;
        handel_overflow_XO_form(rt, ra, rb, oe, rc);
        return;
    }
    if (sgrpb == ~0UL)
    {
        m_thread.gpr[rt] = sgrpa;
        m_thread.xer.CA = 1;
        handel_overflow_XO_form(rt, ra, rb, oe, rc);
        return;
    }
    m_thread.gpr[rt] = sgrpa + sgrpb + 1;
    m_thread.xer.CA = isCarry(sgrpa, sgrpb + 1);
    handel_overflow_XO_form(rt, ra, rb, oe, rc);
}

void PPUInterpreter::addi(u32 rt, u32 ra, s32 si)
{
    m_thread.gpr[rt] = ra ? ((s64)m_thread.gpr[ra] + si) : si;
}

void PPUInterpreter::addic(u32 rt, u32 ra, s32 si)
{
    u32 grpa = m_thread.gpr[ra];
    m_thread.gpr[rt] = grpa + si;
    m_thread.xer.CA = isCarry(grpa, si);
}

void PPUInterpreter::addic_(u32 rt, u32 ra, s32 si)
{
    addic(rt, ra, si);
}

void PPUInterpreter::addis(u32 rt, u32 ra, s32 si)
{
    m_thread.gpr[rt] = ra ? ((s64)m_thread.gpr[ra] + (si << 16)) : (si << 16);
}

void PPUInterpreter::addme(u32 rt, u32 ra, u32 rb, bool oe, bool rc)
{
    m_thread.gpr[rt] = (u64)m_thread.gpr[ra] + m_thread.xer.CA - 1;

    m_thread.xer.CA = isCarry(m_thread.gpr[ra], m_thread.xer.CA - 1);
    if (rc)
    {
        u8 cr0 = ((m_thread.gpr[rt] < 0) << 3 | (m_thread.gpr[rt] > 0) << 2 | (m_thread.gpr[rt] == 0) << 1);
        cr0 |= ((u8)m_thread.xer.SO);
        m_thread.cr.setField(0, cr0);
    }
    if (oe)
    {
        // if they have the same sign but the sum is in diffrent sign then it's an overflow
        if (((m_thread.xer.CA - 1) > 0 && m_thread.gpr[ra] > 0 && m_thread.gpr[rt] < 0) ||
            ((m_thread.xer.CA - 1) < 0 && m_thread.gpr[ra] < 0 && m_thread.gpr[rt] > 0))
        {
            m_thread.xer.OV = 1;
            m_thread.xer.SO = 1;
        }
        else
        {
            m_thread.xer.OV = 0;
            m_thread.xer.SO = 0;
        }
    }
}

void PPUInterpreter::addze(u32 rt, u32 ra, u32 rb, bool oe, bool rc)
{
    m_thread.gpr[rt] = (u64)m_thread.gpr[ra] + m_thread.xer.CA;

    m_thread.xer.CA = isCarry((u64)m_thread.gpr[ra], m_thread.xer.CA);
    if (rc)
    {
        u8 cr0 = ((m_thread.gpr[rt] < 0) << 3 | (m_thread.gpr[rt] > 0) << 2 | (m_thread.gpr[rt] == 0) << 1);
        cr0 |= ((u8)m_thread.xer.SO);
        m_thread.cr.setField(0, cr0);
    }
    if (oe)
    {
        m_thread.xer.OV = ((u64)m_thread.gpr[ra] == ~0ULL);
        m_thread.xer.SO = ((u64)m_thread.gpr[ra] == ~0ULL);
    }
}

void PPUInterpreter::And(u32 rs, u32 ra, u32 rb, bool rc)
{
    m_thread.gpr[ra] = m_thread.gpr[rs] & m_thread.gpr[rb];
    if (rc)
    {
        // do smth
    }
}

void PPUInterpreter::andc(u32 rs, u32 ra, u32 rb, bool rc)
{
    m_thread.gpr[ra] = m_thread.gpr[rs] & ~(m_thread.gpr[rb]);
    if (rc)
    {
        // do smth
    }
}

void PPUInterpreter::andi(u32 rs, u32 ra, u32 ui)
{
    m_thread.gpr[ra] = m_thread.gpr[rs] & ui;
}

void PPUInterpreter::andis(u32 rs, u32 ra, u32 ui)
{
    m_thread.gpr[ra] = m_thread.gpr[rs] & (ui << 16);
}

void PPUInterpreter::b(u32 li, u32 aa, u32 lk)
{
    if (lk)
        m_thread.lr = m_thread.pc + 4;
    m_thread.pc = (aa ? li : (m_thread.pc + li)) & ~0x3ULL;
}

void PPUInterpreter::bc(u32 bo, u32 bi, u32 bd, u32 aa, u32 lk)
{
    bool bo0 = bo & 1;
    bool bo1 = (bo >> 1) & 1;
    bool bo2 = (bo >> 2) & 1;
    bool bo3 = (bo >> 3) & 1;
    bool ctr_ok = bo2 | ((m_thread.ctr != 0) ^ bo3);
    bool cond_ok = bo0 | m_thread.cr.getBit(bi) ^ ~bo1;
    if (!bo2)
        m_thread.ctr--;
    if (!(ctr_ok && cond_ok))
        return;
    if (lk)
        m_thread.lr = m_thread.pc + 4;
    m_thread.pc = (aa ? bd : ((s64)m_thread.pc + bd)) & ~0x3ULL;
}

void PPUInterpreter::bcctr(u32 bo, u32 bi, u32 bh, u32 lk)
{
    if (!((bo >> 2) & 1))
        return;
    bool bo0 = bo & 1;
    bool bo1 = (bo >> 1) & 1;
    bool cond_ok = bo0 | m_thread.cr.getBit(bi) ^ ~bo1;
    if (!cond_ok)
        return;
    if (lk)
        m_thread.lr = m_thread.pc + 4;
    m_thread.ctr = m_thread.ctr & ~0x3ULL;
}

void PPUInterpreter::bclr(u32 bo, u32 bi, u32 bh, u32 lk)
{
    bool bo0 = bo & 1;
    bool bo1 = (bo >> 1) & 1;
    bool bo2 = (bo >> 2) & 1;
    bool bo3 = (bo >> 3) & 1;
    bool ctr_ok = bo2 | ((m_thread.ctr != 0) ^ bo3);
    bool cond_ok = bo0 | m_thread.cr.getBit(bi) ^ ~bo1;
    if (!bo2)
        m_thread.ctr--;
    if (!(ctr_ok && cond_ok))
        return;
    if (lk)
        m_thread.lr = m_thread.pc + 4;
    m_thread.pc = m_thread.lr & ~0x3ULL;
}

void PPUInterpreter::cmp(u32 bf, u32 l, u32 ra, u32 rb)
{
    s32 a = l ? (s32)m_thread.gpr[ra] : (s32)(m_thread.gpr[ra] & ~((0x1ULL << 32) - 1));
    s32 b = l ? (s32)m_thread.gpr[rb] : (s32)(m_thread.gpr[rb] & ~(0x1ULL << 32) - 1);

    u8 c = 0b001;
    if (a < b)
        c = 0b100;
    if (a > b)
        c = 0b010;
    u8 result = c | (m_thread.xer.SO << 3);
    m_thread.cr.setField(bf, result);
}

void PPUInterpreter::cmpi(u32 bf, u32 l, u32 ra, s32 si)
{
    s32 a = l ? (s32)m_thread.gpr[ra] : (s32)(m_thread.gpr[ra] & ~(0x1ULL << 32));

    u8 c = 0b001;
    if (a < si)
        c = 0b100;
    if (a > si)
        c = 0b010;
    u8 result = c | (m_thread.xer.SO << 3);
    m_thread.cr.setField(bf, result);
}

void PPUInterpreter::cmpl(u32 bf, u32 l, u32 ra, u32 rb)
{
    u32 a = l ? (u32)m_thread.gpr[ra] : (u32)(m_thread.gpr[ra] & ~((0x1ULL << 32) - 1));
    u32 b = l ? (u32)m_thread.gpr[rb] : (u32)(m_thread.gpr[rb] & ~(0x1ULL << 32) - 1);

    u8 c = 0b001;
    if (a < b)
        c = 0b100;
    if (a > b)
        c = 0b010;
    u8 result = c | (m_thread.xer.SO << 3);
    m_thread.cr.setField(bf, result);
}

void PPUInterpreter::cmpli(u32 bf, u32 l, u32 ra, u32 ui)
{
    u32 a = l ? (u32)m_thread.gpr[ra] : (u32)(m_thread.gpr[ra] & ~(0x1ULL << 32));

    u8 c = 0b001;
    if (a < si)
        c = 0b100;
    if (a > si)
        c = 0b010;
    u8 result = c | (m_thread.xer.SO << 3);
    m_thread.cr.setField(bf, result);
}

void PPUInterpreter::cntlzd(u32 rs, u32 ra, bool rc)
{

    for (size_t i = 0; i < 64; i++)
    {
        if (m_thread.gpr[rs] & (1ULL << (63 - i)))
        {
            m_thread.gpr[ra] = i;
            break;
        }
    }
    if (rc)
        thread.cr.setBit(m_thread.cr.CR_LT, false);
}

void PPUInterpreter::cntlzw(u32 rs, u32 ra, bool rc)
{

    for (size_t i = 0; i < 32; i++)
    {
        if (m_thread.gpr[rs] & (1ULL << (31 - i)))
        {
            m_thread.gpr[ra] = i;
            break;
        }
    }
    if (rc)
        thread.cr.setBit(m_thread.cr.CR_LT, false);
}

void PPUInterpreter::crand(u32 bt, u32 ba, u32 bb)
{
    const u8 value = m_thread.cr.getBit(ba) & m_thread.cr.getBit(bb);
    thread.cr.setBit(bt, value);
}

void PPUInterpreter::crandc(u32 bt, u32 ba, u32 bb)
{
    const u8 value = m_thread.cr.getBit(ba) & (1 ^ m_thread.cr.getBit(bb));
    thread.cr.setBit(bt, value);
}

void PPUInterpreter::creqv(u32 bt, u32 ba, u32 bb)
{
    const u8 value = 1 ^ (m_thread.cr.getBit(ba) ^ m_thread.cr.getBit(bb));
    thread.cr.setBit(bt, value);
}
void PPUInterpreter::crnand(u32 bt, u32 ba, u32 bb)
{
    const u8 value = 1 ^ (m_thread.cr.getBit(ba) & m_thread.cr.getBit(bb));
    thread.cr.setBit(bt, value);
}

void PPUInterpreter::crnor(u32 bt, u32 ba, u32 bb)
{
    const u8 value = 1 ^ (m_thread.cr.getBit(ba) | m_thread.cr.getBit(bb));
    thread.cr.setBit(bt, value);
}

void PPUInterpreter::cror(u32 bt, u32 ba, u32 bb)
{
    const u8 value = m_thread.cr.getBit(ba) | m_thread.cr.getBit(bb);
    thread.cr.setBit(bt, value);
}

void PPUInterpreter::crorc(u32 bt, u32 ba, u32 bb)
{
    const u8 value = m_thread.cr.getBit(ba) | (1 ^ m_thread.cr.getBit(bb));
    thread.cr.setBit(bt, value);
}

void PPUInterpreter::crxor(u32 bt, u32 ba, u32 bb)
{
    const u8 value = (m_thread.cr.getBit(ba) ^ m_thread.cr.getBit(bb));
    thread.cr.setBit(bt, value);
}
void PPUInterpreter::dcbf(u32 ra, u32 rb)
{
    _mm_mfence();
}
void PPUInterpreter::dcbst(u32 ra, u32 rb)
{
    _mm_mfence();
}
void PPUInterpreter::dcbt(u32 ra, u32 rb, u32 th)
{
    _mm_mfence();
}
void PPUInterpreter::dcbtst(u32 ra, u32 rb, u32 th)
{
    _mm_mfence();
}

void PPUInterpreter::dcbz(u32 ra, u32 rb)
{
    const u32 addr = ra ? m_thread.gpr[ra] + m_thread.gpr[rb] : m_thread.gpr[rb];
    void *cache_line = tzPs3Emu.memory + (addr & ~127);

    if (cache_line)
    {
        std::memset(cache_line, 0, 128);
    }
    _mm_mfence();
}

void PPUInterpreter::divd(u32 rt, u32 ra, u32 rb, bool oe, bool rc)
{
    const s64 gpra = m_thread.gpr[ra];
    const s64 gprb = m_thread.gpr[rb];
    if (gprb == 0 || ((u64)gpra == (1ULL << 63) && gprb == -1))
    {
        thread.gpr[rt] = 0;
    }
    else
    {
        thread.gpr[rt] = gpra / gprb;
    }
    if (rc)
    {
    }
}
void PPUInterpreter::divd(u32 rt, u32 ra, u32 rb, bool oe, bool rc)
{
    const u64 gpra = m_thread.gpr[ra];
    const u64 gprb = m_thread.gpr[rb];
    if (gprb == 0 || (gpra == (1ULL << 63) && gprb == -1))
    {
        thread.gpr[rt] = 0;
    }
    else
    {
        m_thread.gpr[rt] = gpra / gprb;
    }
    if (rc)
    {
    }
}
void PPUInterpreter::divw(u32 rt, u32 ra, u32 rb, u32 oe, bool rc)
{
    const s32 gpra = m_thread.gpr[ra];
    const s32 gprb = m_thread.gpr[rb];
    if (gprb == 0 || ((u32)gpra == (1 << 31) && gprb == -1))
    {
        m_thread.gpr[rt] = 0;
    }
    else
    {
        m_thread.gpr[rt] = (u32)(gpra / gprb);
    }
    if (rc)
    {
    } // TODO: CPU.UpdateCR0<s64>(thread.gpr[rd]);
}
void PPUInterpreter::divwu(u32 rt, u32 ra, u32 rb, u32 oe, bool rc)
{
    const u32 gpra = m_thread.gpr[ra];
    const u32 gprb = m_thread.gpr[rb];
    if (gprb == 0)
    {
        // if (oe) unknown("divwuo");
        m_thread.gpr[rt] = 0;
    }
    else
    {
        m_thread.gpr[rt] = gpra / gprb;
    }
    if (rc)
    {
    } // TODO: CPU.UpdateCR0<s64>(thread.gpr[rd]);
}
void PPUInterpreter::dss(u32 strm, u32 a)
{
    _mm_mfence();
}
void PPUInterpreter::dst(u32 ra, u32 rb, u32 strm, u32 t)
{
    _mm_mfence();
}
void PPUInterpreter::dstst(u32 ra, u32 rb, u32 strm, u32 t)
{
    _mm_mfence();
}

void PPUInterpreter::eciwx(u32 rt, u32 ra, u32 rb)
{
    const u32 addr = ra ? m_thread.gpr[ra] + m_thread.gpr[rb] : m_thread.gpr[rb];
    m_thread.gpr[rt] = tzPs3Emu.memory.read32(addr);
}

void PPUInterpreter::ecowx(u32 rt, u32 ra, u32 rb)
{
    const u32 addr = ra ? m_thread.gpr[ra] + m_thread.gpr[rb] : m_thread.gpr[rb];
    m_thread.gpr[rt] = tzPs3Emu.memory.write32(addr, m_thread.gpr[rt]);
}

void PPUInterpreter::eieio()
{
    _mm_mfence();
}

void PPUInterpreter::eqv(u32 rs, u32 ra, u32 rb, bool rc)
{
    m_thread.gpr[ra] = m_thread.gpr[ra] ^ ~m_thread.gpr[rb];
}
void PPUInterpreter::extsb(u32 rs, u32 ra, bool rc)
{
    m_thread.gpr[ra] = (s64)(s8)m_thread.gpr[rs]
}

void PPUInterpreter::extsh(u32 rs, u32 ra, bool rc)
{
    m_thread.gpr[ra] = (s64)(s16)m_thread.gpr[rs]
}

void PPUInterpreter::extsw(u32 rs, u32 ra, bool rc)
{
    m_thread.gpr[ra] = (s64)(s32)m_thread.gpr[rs]
}

void PPUInterpreter::fabs(u32 frd, u32 frb, bool rc)
{
    const f32 val = m_thread.fpr[frb];
    thread.fpr[frd] = (val < 0) ? -val : val;
}

void PPUInterpreter::fadd(u32 frd, u32 fra, u32 frb, bool rc)
{
    thread.fpr[frd] = thread.fpr[fra] + thread.fpr[frb];
}

void PPUInterpreter::fadds(u32 frd, u32 fra, u32 frb, bool rc)
{
    thread.fpr[frd] = static_cast<f32>(thread.fpr[fra] + thread.fpr[frb]);
}

void PPUInterpreter::fcfid(u32 frd, u32 frb, bool rc)
{
    const s64 bi = (s64 &)m_thread.fpr[frb];
    const f64 bf = (f64)bi;
    const s64 bfi = (s64)bf;
    if (bi == bfi)
    {
        m_thread.fpscr.FI = 0;
        m_thread.fpscr.FR = 0;
    }
    else
    {
        m_thread.fpscr.FI = 1;
        m_thread.fpscr.FR = abs(bfi) > abs(bi);
    }
    m_thread.fpr[frd] = bf;
}

void PPUInterpreter::fcmpo(u32 crfd, u32 fra, u32 frb)
{
    u32 c = 0x2;
    if (std::isnan(m_thread.fpr[fra]) || std::isnan(m_thread.fpr[frb]))
        c = 0x1;
    else if (m_thread.fpr[fra] > m_thread.fpr[frb])
        c = 0x4;
    else if (m_thread.fpr[fra] > m_thread.fpr[frb])
        c = 0x8;
    m_thread.fpscr.FPSCR = c << 1;
    m_thread.cr.setField(crfd, (u8)c);
}

void PPUInterpreter::fcmpu(u32 crfd, u32 fra, u32 frb)
{
}

void PPUInterpreter::fctid(u32 frd, u32 frb, bool rc)
{
}

void PPUInterpreter::fctidz(u32 frd, u32 frb, bool rc)
{
}

void PPUInterpreter::fctiw(u32 frd, u32 frb, bool rc)
{
}

void PPUInterpreter::fctiwz(u32 frd, u32 frb, bool rc)
{
}

void PPUInterpreter::fdiv(u32 frd, u32 fra, u32 frb, bool rc)
{
}

void PPUInterpreter::fdivs(u32 frd, u32 fra, u32 frb, bool rc)
{
}

void PPUInterpreter::fmadd(u32 frd, u32 fra, u32 frc, u32 frb, bool rc)
{
}

void PPUInterpreter::fmadds(u32 frd, u32 fra, u32 frc, u32 frb, bool rc)
{
}

void PPUInterpreter::fmr(u32 frd, u32 frb, bool rc)
{
}

void PPUInterpreter::fmsub(u32 frd, u32 fra, u32 frc, u32 frb, bool rc)
{
}

void PPUInterpreter::fmsubs(u32 frd, u32 fra, u32 frc, u32 frb, bool rc)
{
}

void PPUInterpreter::fmul(u32 frd, u32 fra, u32 frc, bool rc)
{
}

void PPUInterpreter::fmuls(u32 frd, u32 fra, u32 frc, bool rc)
{
}

void PPUInterpreter::fnabs(u32 frd, u32 frb, bool rc)
{
}

void PPUInterpreter::fneg(u32 frd, u32 frb, bool rc)
{
}

void PPUInterpreter::fnmadd(u32 frd, u32 fra, u32 frc, u32 frb, bool rc)
{
}

void PPUInterpreter::fnmadds(u32 frd, u32 fra, u32 frc, u32 frb, bool rc)
{
}

void PPUInterpreter::fnmsub(u32 frd, u32 fra, u32 frc, u32 frb, bool rc)
{
}

void PPUInterpreter::fnmsubs(u32 frd, u32 fra, u32 frc, u32 frb, bool rc)
{
}

void PPUInterpreter::fres(u32 frd, u32 frb, bool rc)
{
}

void PPUInterpreter::frsp(u32 frd, u32 frb, bool rc)
{
}

void PPUInterpreter::frsqrte(u32 frd, u32 frb, bool rc)
{
}

void PPUInterpreter::fsel(u32 frd, u32 fra, u32 frc, u32 frb, bool rc)
{
}

void PPUInterpreter::fsqrt(u32 frd, u32 frb, bool rc)
{
}

void PPUInterpreter::fsqrts(u32 frd, u32 frb, bool rc)
{
}

void PPUInterpreter::fsub(u32 frd, u32 fra, u32 frb, bool rc)
{
}

void PPUInterpreter::fsubs(u32 frd, u32 fra, u32 frb, bool rc)
{
}

void PPUInterpreter::isync()
{
    _mm_mfence();
}

void PPUInterpreter::lbz(u32 rt, u32 ra, s32 d)
{
    m_thread.gpr[rt] = tzPs3Emu.memory.read8(ra ? d + m_thread.gpr[ra] : d);
}

void PPUInterpreter::lbzx(u32 rt, u32 ra, u32 rb)
{
    const u32 ea = ra ? m_thread.gpr[ra] + m_thread.gpr[rb] : m_thread.gpr[rb];
    m_thread.gpr[rt] = tzPs3Emu.memory.read8(ea);
}

void PPUInterpreter::lbzu(u32 rt, u32 ra, s32 d)
{
    const u32 ea = m_thread.gpr[ra] + d;
    m_thread.gpr[rt] = tzPs3Emu.memory.read8(ea);
    m_thread.gpr[ra] = ea;
}

void PPUInterpreter::lbzux(u32 rt, u32 ra, u32 rb)
{
    const u32 ea = m_thread.gpr[ra] + m_thread.gpr[rb];
    m_thread.gpr[rt] = tzPs3Emu.memory.read8(ea);
    m_thread.gpr[ra] = ea;
}

void PPUInterpreter::ld(u32 rt, u32 ra, s32 ds)
{
    const u32 ea = ra ? m_thread.gpr[ra] + ds : ds;
    m_thread.gpr[rt] = tzPs3Emu.memory.read64(ea);
}

void PPUInterpreter::ldarx(u32 rt, u32 ra, u32 rb)
{
    const u32 ea = ra ? m_thread.gpr[ra] + m_thread.gpr[rb] : m_thread.gpr[rb];
    m_thread.gpr[rt] = tzPs3Emu.memory.read64(ea);
    m_thread.reserve_addr = ea;
    m_thread.reserve_value = re64(m_thread.gpr[rt]);
}

void PPUInterpreter::ldu(u32 rt, u32 ra, s32 ds)
{
    const u32 ea = ra ? m_thread.gpr[ra] + ds : ds;
    m_thread.gpr[rt] = tzPs3Emu.memory.read64(ea);
    m_thread.gpr[ra] = ea;
}

void PPUInterpreter::ldux(u32 rt, u32 ra, u32 rb)
{
    const u32 ea = m_thread.gpr[ra] + m_thread.gpr[rb];
    m_thread.gpr[rt] = tzPs3Emu.memory.read64(ea);
    m_thread.gpr[ra] = ea;
}

void PPUInterpreter::ldx(u32 rt, u32 ra, u32 rb)
{
    const u32 ea = ra ? m_thread.gpr[ra] + m_thread.gpr[rb] : m_thread.gpr[rb];
    m_thread.gpr[rt] = tzPs3Emu.memory.read64(ea);
}

void PPUInterpreter::lfd(u32 frt, u32 ra, s32 d)
{
    const u32 ea = ra ? m_thread.gpr[ra] + d : d;
    (u64 &)m_thread.fpr[frt] = tzPs3Emu.memory.read64(ea);
}

void PPUInterpreter::lfdu(u32 frt, u32 ra, s32 d)
{
    const u32 ea = m_thread.gpr[ra] + d;
    (u64 &)m_thread.fpr[frt] = tzPs3Emu.memory.read64(ea);
    m_thread.gpr[ra] = ea;
}

void PPUInterpreter::lfdux(u32 frt, u32 ra, u32 rb)
{
    const u32 ea = m_thread.gpr[ra] + m_thread.gpr[rb];
    (u64 &)m_thread.fpr[frt] = tzPs3Emu.memory.read64(ea);
    m_thread.gpr[ra] = ea;
}

void PPUInterpreter::lfdx(u32 frt, u32 ra, u32 rb)
{
    const u32 ea = ra ? m_thread.gpr[ra] + m_thread.gpr[rb] : m_thread.gpr[rb];
    (u64 &)m_thread.fpr[frt] = tzPs3Emu.memory.read64(ea);
}

void PPUInterpreter::lfs(u32 frt, u32 ra, s32 d)
{
    const u32 ea = ra ? m_thread.gpr[ra] + d : d;
    m_thread.fpr[frt] = (f32 &)tzPs3Emu.memory.read32(ea);
}

void PPUInterpreter::lha(u32 rt, u32 ra, s32 d)
{
    const u32 ea = ra ? m_thread.gpr[ra] + d : d;
    m_thread.gpr[rt] = (s64)(s16)tzPs3Emu.memory.read16(ea);
}

void PPUInterpreter::lhax(u32 rt, u32 ra, u32 rb)
{
    const u32 ea = ra ? m_thread.gpr[ra] + m_thread.gpr[rb] : m_thread.gpr[rb];
    m_thread.gpr[rt] = (s64)(s16)tzPs3Emu.memory.read16(ea);
}

void PPUInterpreter::lhau(u32 rt, u32 ra, s32 d)
{
    const u32 ea = m_thread.gpr[ra] + d;
    m_thread.gpr[rt] = (s64)(s16)tzPs3Emu.memory.read16(ea);
    m_thread.gpr[ra] = ea;
}

void PPUInterpreter::lhaux(u32 rt, u32 ra, u32 rb)
{
    const u32 ea = ra ? m_thread.gpr[ra] + m_thread.gpr[rb] : m_thread.gpr[rb];
    m_thread.gpr[rt] = (s64)(s16)tzPs3Emu.memory.read16(ea);
    m_thread.gpr[ra] = ea;
}

void PPUInterpreter::lhbrx(u32 rt, u32 ra, u32 rb)
{
    const u32 ea = ra ? m_thread.gpr[ra] + m_thread.gpr[rb] : m_thread.gpr[rb];
    m_thread.gpr[rt] = re16(tzPs3Emu.memory.read16(ea));
}

void PPUInterpreter::lhz(u32 rt, u32 ra, s32 d)
{
    const u32 ea = ra ? m_thread.gpr[ra] + d : d;
    m_thread.gpr[rt] = tzPs3Emu.memory.read16(ea);
}

void PPUInterpreter::lhzx(u32 rt, u32 ra, u32 rb)
{
    const u32 ea = ra ? m_thread.gpr[ra] + m_thread.gpr[rb] : m_thread.gpr[rb];
    m_thread.gpr[rt] = tzPs3Emu.memory.read16(ea);
}

void PPUInterpreter::lhzu(u32 rt, u32 ra, s32 d)
{
    const u32 ea = m_thread.gpr[ra] + d;
    m_thread.gpr[rt] = tzPs3Emu.memory.read16(ea);
    m_thread.gpr[ra] = ea;
}

void PPUInterpreter::lhzux(u32 rt, u32 ra, u32 rb)
{
    const u32 ea = m_thread.gpr[ra] + m_thread.gpr[rb];
    m_thread.gpr[rt] = tzPs3Emu.memory.read16(ea);
    m_thread.gpr[ra] = ea;
}

void PPUInterpreter::lmw(u32 rt, u32 ra, s32 d)
{
    const u32 ea = ra ? m_thread.gpr[ra] + d : d;
    for (int i = 0; i < 31; i++)
    {
        m_thread.gpr[i] = tzPs3Emu.memory.read32(ea);
        ea += 4;
    }
}

void PPUInterpreter::lswi(u32 rt, u32 ra, u32 nb)
{
    u32 ea = ra ? m_thread.gpr[ra] : 0;
    s32 n = nb ? nb : 32;
    u32 reg = rt - 1;

    while (n > 0)
    {
        if (n > 3)
        {
            m_thread.gpr[reg] = tzPs3Emu.memory.read32(ea);
            n -= 4;
            ea += 4;
        }
        else
        {
            u32 buffer = 0;
            while (n > 0)
            {
                n--;
                buffer |= (tzPs3Emu.memory.read8(ea)) << (8 * n);
                ea++;
            }
            m_thread.gpr[reg] = buffer;
        }
        reg = (reg + 1) % 32;
    }
}

void PPUInterpreter::lswx(u32 rt, u32 ra, u32 rb)
{
    u32 ea = ra ? m_thread.gpr[ra] + m_thread.gpr[rb] : m_thread.gpr[rb];
    s32 n = m_thread.xer.BC;
    u32 reg = rt - 1;

    while (n > 0)
    {
        if (n > 3)
        {
            m_thread.gpr[reg] = tzPs3Emu.memory.read32(ea);
            n -= 4;
            ea += 4;
        }
        else
        {
            u32 buffer = 0;
            while (n > 0)
            {
                n--;
                buffer |= (tzPs3Emu.memory.read8(ea)) << (8 * n);
                ea++;
            }
            m_thread.gpr[reg] = buffer;
        }
        reg = (reg + 1) % 32;
    }
}

void PPUInterpreter::lwa(u32 rt, u32 ra, s32 ds)
{
    const u32 ea = ra ? m_thread.gpr[ra] + ds : ds;
    m_thread.gpr[rt] = (s64)(s32)tzPs3Emu.memory.read32(ea);
}

void PPUInterpreter::lwarx(u32 rt, u32 ra, u32 rb)
{
    const u32 ea = ra ? m_thread.gpr[ra] + m_thread.gpr[rb] : m_thread.gpr[rb];
    m_thread.reserve_addr = ea;
    m_thread.gpr[rt] = (u64)tzPs3Emu.memory.read32(ea);
    m_thread.reserve_value = m_thread.gpr[rt];
}

void PPUInterpreter::lwaux(u32 rt, u32 ra, u32 rb)
{
    const u32 ea = m_thread.gpr[ra] + m_thread.gpr[rb];
    m_thread.gpr[rt] = (s64)(s32)tzPs3Emu.memory.read32(ea);
    m_thread.gpr[ra] = ea;
}

void PPUInterpreter::lwax(u32 rt, u32 ra, u32 rb)
{
    const u32 ea = ra ? m_thread.gpr[ra] + m_thread.gpr[rb] : m_thread.gpr[rb];
    m_thread.gpr[rt] = (s64)(s32)tzPs3Emu.memory.read32(ea);
}

void PPUInterpreter::lwbrx(u32 rt, u32 ra, u32 rb)
{
    const u32 ea = ra ? m_thread.gpr[ra] + m_thread.gpr[rb] : m_thread.gpr[rb];
    m_thread.gpr[rt] = re32(tzPs3Emu.memory.read32(ea));
}

void PPUInterpreter::lwz(u32 rt, u32 ra, s32 d)
{
    const u32 ea = ra ? m_thread.gpr[ra] + d : d;
    m_thread.gpr[rt] = tzPs3Emu.memory.read32(ea);
}

void PPUInterpreter::lwzu(u32 rt, u32 ra, s32 d)
{
    const u32 ea = m_thread.gpr[ra] + d;
    m_thread.gpr[rt] = tzPs3Emu.memory.read32(ea);
    m_thread.gpr[ra] = ea;
}

void PPUInterpreter::lwzx(u32 rt, u32 ra, ur32 rb)
{
    const u32 ea = ra ? m_thread.gpr[ra] + m_thread.gpr[rb] : m_thread.gpr[rb];
    m_thread.gpr[rt] = tzPs3Emu.memory.read32(ea);
}

void PPUInterpreter::lwzux(u32 rt, u32 ra, ur32 rb)
{
    const u32 ea = ra ? m_thread.gpr[ra] + m_thread.gpr[rb] : m_thread.gpr[rb];
    m_thread.gpr[rt] = tzPs3Emu.memory.read32(ea);
    m_thread.gpr[ra] = ea;
}

void PPUInterpreter::mcrf(u32 bf, u32 bfa)
{
    m_thread.cr.setField(bf,m_thread.cr.getField(bfa));
}


void PPUInterpreter::lfsx(u32 frt, u32 ra, u32 rb)
{
    const u32 ea = ra ? m_thread.gpr[ra] + m_thread.gpr[rb] : m_thread.gpr[rb];
    m_thread.fpr[frt] = (f32 &)tzPs3Emu.memory.read32(ea);
}

void PPUInterpreter::lfsu(u32 frt, u32 ra, s32 d)
{
    const u32 ea = ra ? m_thread.gpr[ra] + d : d;
    m_thread.fpr[frt] = (f32 &)tzPs3Emu.memory.read32(ea);
    m_thread.gpr[ra] = ea;
}

void PPUInterpreter::lfsux(u32 frt, u32 ra, u32 rb)
{
    const u32 ea = m_thread.gpr[ra] + m_thread.gpr[rb];
    m_thread.fpr[frt] = (f32 &)tzPs3Emu.memory.read32(ea);
    m_thread.gpr[ra] = ea;
}
