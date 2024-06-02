
#include "instructionSet.hpp"
#include "cpu/translator.hpp"
#include "thread.hpp"
#include <cstring>
class PPUInterpreter : public PPUInstructions, public CellTranslator
{
    PPUThread &m_thread;

public:
    PPUInterpreter(PPUThread &thr);

    void step();
    bool isCarry(u64 a, u64 b);

    void handel_overflow_XO_form(u32 rt, u32 ra, u32 rb, bool oe, bool rc);
    void add(u32 rt, u32 ra, u32 rb, bool oe, bool rc);
    void addc(u32 rt, u32 ra, u32 rb, bool oe, bool rc);
    void adde(u32 rt, u32 ra, u32 rb, bool oe, bool rc);
    void addi(u32 rt, u32 ra, s32 si);
    void addic(u32 rt, u32 ra, s32 si);
    void addic_(u32 rt, u32 ra, s32 si);
    void addis(u32 rt, u32 ra, s32 si);
    void addme(u32 rt, u32 ra, u32 rb, bool oe, bool rc);
    void addze(u32 rt, u32 ra, u32 rb, bool oe, bool rc);
    void And(u32 rs, u32 ra, u32 rb, bool rc);
    void andc(u32 rs, u32 ra, u32 rb, bool rc);
    void andi(u32 rs, u32 ra, u32 ui);
    void andis(u32 rs, u32 ra, u32 ui);
    void b(u32 li, u32 aa, u32 lk);
    void bc(u32 bo, u32 bi, u32 bd, u32 aa, u32 lk);
    void bcctr(u32 bo, u32 bi, u32 bh, u32 lk);
    void bclr(u32 bo, u32 bi, u32 bh, u32 lk);
    void cmp(u32 bf, u32 l, u32 ra, u32 rb);
    void cmpi(u32 bf, u32 l, u32 ra, s32 si);
    void cmpl(u32 bf, u32 l, u32 ra, u32 rb);
    void cmpli(u32 bf, u32 l, u32 ra, u32 ui);
    void cntlzd(u32 rs, u32 ra, bool rc);
    void cntlzw(u32 rs, u32 ra, bool rc);
    void crand(u32 bt, u32 ba, u32 bb);
    void crandc(u32 bt, u32 ba, u32 bb);
    void creqv(u32 bt, u32 ba, u32 bb);
    void crnand(u32 bt, u32 ba, u32 bb);
    void crnor(u32 bt, u32 ba, u32 bb);
    void cror(u32 bt, u32 ba, u32 bb);
    void crorc(u32 bt, u32 ba, u32 bb);
    void crxor(u32 bt, u32 ba, u32 bb);
    void dcbf(u32 ra, u32 rb);
    void dcbst(u32 ra, u32 rb);
    void dcbt(u32 ra, u32 rb, u32 th);
    void dcbtst(u32 ra, u32 rb, u32 th);
    void dcbz(u32 ra, u32 rb);
    void divd(u32 rt, u32 ra, u32 rb, bool oe, bool rc);
    void divw(u32 rt, u32 ra, u32 rb, bool oe, bool rc);
    void divwu(u32 rt, u32 ra, u32 rb, bool oe, bool rc);
    void dss(u32 strm, u32 a);
    void dst(u32 ra, u32 rb, u32 strm, u32 t);
    void dstst(u32 ra, u32 rb, u32 strm, u32 t);
    void eciwx(u32 rt, u32 ra, u32 rb);
    void ecowx(u32 rt, u32 ra, u32 rb);
    void eqv(u32 rs, u32 ra, u32 rb, bool rc);
    void extsb(u32 rs, u32 ra, bool rc);
    void extsh(u32 rs, u32 ra, bool rc);
    void extsw(u32 rs, u32 ra, bool rc);
    void fabs(u32 frd, u32 frb, bool rc);
    void fadd(u32 frd, u32 fra, u32 frb, bool rc);
    void fadds(u32 frd, u32 fra, u32 frb, bool rc);
    void fcfid(u32 frd, u32 frb, bool rc);
    void fcmpo(u32 crfd, u32 fra, u32 frb);
    void fcmpu(u32 crfd, u32 fra, u32 frb);
    void fctid(u32 frd, u32 frb, bool rc);
    void fctidz(u32 frd, u32 frb, bool rc);
    void fctiw(u32 frd, u32 frb, bool rc);
    void fctiwz(u32 frd, u32 frb, bool rc);
    void fdiv(u32 frd, u32 fra, u32 frb, bool rc);
    void fdivs(u32 frd, u32 fra, u32 frb, bool rc);
    void fmadd(u32 frd, u32 fra, u32 frc, u32 frb, bool rc);
    void fmadds(u32 frd, u32 fra, u32 frc, u32 frb, bool rc);
    void fmr(u32 frd, u32 frb, bool rc);
    void fmsub(u32 frd, u32 fra, u32 frc, u32 frb, bool rc);
    void fmsubs(u32 frd, u32 fra, u32 frc, u32 frb, bool rc);
    void fmul(u32 frd, u32 fra, u32 frc, bool rc);
    void fmuls(u32 frd, u32 fra, u32 frc, bool rc);
    void fnabs(u32 frd, u32 frb, bool rc);
    void fneg(u32 frd, u32 frb, bool rc);
    void fnmadd(u32 frd, u32 fra, u32 frc, u32 frb, bool rc);
    void fnmadds(u32 frd, u32 fra, u32 frc, u32 frb, bool rc);
    void fnmsub(u32 frd, u32 fra, u32 frc, u32 frb, bool rc);
    void fnmsubs(u32 frd, u32 fra, u32 frc, u32 frb, bool rc);
    void fres(u32 frd, u32 frb, bool rc);
    void frsp(u32 frd, u32 frb, bool rc);
    void frsqrte(u32 frd, u32 frb, bool rc);
    void fsel(u32 frd, u32 fra, u32 frc, u32 frb, bool rc);
    void fsqrt(u32 frd, u32 frb, bool rc);
    void fsqrts(u32 frd, u32 frb, bool rc);
    void fsub(u32 frd, u32 fra, u32 frb, bool rc);
    void fsubs(u32 frd, u32 fra, u32 frb, bool rc);
    void isync();
    void lbz(u32 rt, u32 ra, s32 d);
    void lbzx(u32 rt, u32 ra, u32 rb);
    void lbzu(u32 rt, u32 ra, s32 d);
    void lbzux(u32 rt, u32 ra, u32 rb);
    void ld(u32 rt, u32 ra, s32 ds);
    void ldarx(u32 rt, u32 ra, u32 rb);
    void ldu(u32 rt, u32 ra, s32 ds);
    void ldux(u32 rt, u32 ra, u32 rb);
    void ldx(u32 rt, u32 ra, u32 rb);
    void lfd(u32 frt, u32 ra, s32 d);
    void lfdu(u32 frt, u32 ra, s32 d);
    void lfdux(u32 frt, u32 ra, u32 rb);
    void lfdx(u32 frt, u32 ra, u32 rb);
    void lfs(u32 frt, u32 ra, s32 d);
    void lfsx(u32 frt, u32 ra, u32 rb);
    void lfsu(u32 frt, u32 ra, s32 d);
    void lfsux(u32 frt, u32 ra, u32 rb);
    void lha(u32 rt, u32 ra, s32 d);
    void lhax(u32 rt, u32 ra, u32 rb);
    void lhau(u32 rt, u32 ra, s32 d);
    void lhaux(u32 rt, u32 ra, u32 rb);
    void lhbrx(u32 rt, u32 ra, u32 rb);
    void lhz(u32 rt, u32 ra, s32 d);
    void lhzx(u32 rt, u32 ra, u32 rb);
    void lhzu(u32 rt, u32 ra, s32 d);
    void lhzux(u32 rt, u32 ra, u32 rb);
    void lmw(u32 rt, u32 ra, s32 d);
    void lswi(u32 rt, u32 ra, u32 nb);
    //
    void lswx(u32 rt, u32 ra, u32 rb);
    void lwa(u32 rt, u32 ra, s32 ds);
    void lwarx(u32 rt, u32 ra, u32 rb);
    void lwaux(u32 rt, u32 ra, u32 rb);
    void lwax(u32 rt, u32 ra, u32 rb);

};
