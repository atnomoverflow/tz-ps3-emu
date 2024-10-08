

class PPUInstructions
{
public:
    virtual void add(u32 rt, u32 ra, u32 rb, bool oe, bool oc) = 0;
    virtual void addc(u32 rt, u32 ra, u32 rb, bool oe, bool oc) = 0;
    virtual void adde(u32 rt, u32 ra, u32 rb, bool oe, bool oc) = 0;
    virtual void addi(u32 rt, u32 ra, s32 si) = 0;
    virtual void addic(u32 rt, u32 ra, s32 si) = 0;
    virtual void addic_(u32 rt, u32 ra, s32 si) = 0;
    virtual void addis(u32 rt, u32 ra, s32 si) = 0;
    virtual void addme(u32 rt, u32 ra, u32 rb, bool oe, bool oc) = 0;
    virtual void addze(u32 rt, u32 ra, u32 rb, bool oe, bool oc) = 0;
    virtual void And(u32 rs, u32 ra, u32 rb, bool rc) = 0;
    virtual void andc(u32 rs, u32 ra, u32 rb, bool rc) = 0;
    virtual void andi(u32 rs, u32 ra, u32 ui) = 0;
    virtual void andis(u32 rs, u32 ra, u32 ui) = 0;
    virtual void b(u32 li, u32 aa, u32 lk) = 0;
    virtual void bc(u32 bo, u32 bi, u32 bd, u32 aa, u32 lk) = 0;
    virtual void bcctr(u32 bo, u32 bi, u32 bh, u32 lk) = 0;
    virtual void bclr(u32 bo, u32 bi, u32 bh, u32 lk) = 0;
    virtual void cmp(u32 bf, u32 l, u32 ra, u32 rb) = 0;
    virtual void cmpi(u32 bf, u32 l, u32 ra, s32 si) = 0;
    virtual void cmpl(u32 bf, u32 l, u32 ra, u32 rb) = 0;
    virtual void cmpli(u32 bf, u32 l, u32 ra, u32 ui) = 0;
    virtual void cntlzd(u32 rs, u32 ra, bool rc) = 0;
    virtual void cntlzw(u32 rs, u32 ra, bool rc) = 0;
    virtual void crand(u32 bt, u32 ba, u32 bb) = 0;
    virtual void crandc(u32 bt, u32 ba, u32 bb) = 0;
    virtual void creqv(u32 bt, u32 ba, u32 bb) = 0;
    virtual void crnand(u32 bt, u32 ba, u32 bb) = 0;
    virtual void crnor(u32 bt, u32 ba, u32 bb) = 0;
    virtual void cror(u32 bt, u32 ba, u32 bb) = 0;
    virtual void crorc(u32 bt, u32 ba, u32 bb) = 0;
    virtual void crxor(u32 bt, u32 ba, u32 bb) = 0;
    virtual void dcbf(u32 ra, u32 rb) = 0;
    virtual void dcbst(u32 ra, u32 rb) = 0;
    virtual void dcbt(u32 ra, u32 rb, u32 th) = 0;
    virtual void dcbtst(u32 ra, u32 rb, u32 th) = 0;
    virtual void dcbz(u32 ra, u32 rb) = 0;
    virtual void divd(u32 rt, u32 ra, u32 rb, bool oe, bool rc) = 0;
    virtual void divw(u32 rt, u32 ra, u32 rb, bool oe, bool rc) = 0;
    virtual void divwu(u32 rt, u32 ra, u32 rb, bool oe, bool rc) = 0;
    virtual void dss(u32 strm, u32 a) = 0;
    virtual void dst(u32 ra, u32 rb, u32 strm, u32 t) = 0;
    virtual void dstst(u32 ra, u32 rb, u32 strm, u32 t) = 0;
    virtual void eciwx(u32 rt, u32 ra, u32 rb) = 0;
    virtual void ecowx(u32 rt, u32 ra, u32 rb) = 0;
    virtual void eqv(u32 rs, u32 ra, u32 rb, bool rc) = 0;
    virtual void extsb(u32 rs, u32 ra, bool rc) = 0;
    virtual void fabs(u32 frd, u32 frb, bool rc) = 0;
    virtual void fadd(u32 frd, u32 fra, u32 frb, bool rc) = 0;
    virtual void fadds(u32 frd, u32 fra, u32 frb, bool rc) = 0;
    virtual void fcfid(u32 frd, u32 frb, bool rc) = 0;
    virtual void fcmpo(u32 crfd, u32 fra, u32 frb) = 0;
    virtual void fcmpu(u32 crfd, u32 fra, u32 frb) = 0;
    virtual void fctid(u32 frd, u32 frb, bool rc) = 0;
    virtual void fctidz(u32 frd, u32 frb, bool rc) = 0;
    virtual void fctiw(u32 frd, u32 frb, bool rc) = 0;
    virtual void fctiwz(u32 frd, u32 frb, bool rc) = 0;
    virtual void fdiv(u32 frd, u32 fra, u32 frb, bool rc) = 0;
    virtual void fdivs(u32 frd, u32 fra, u32 frb, bool rc) = 0;
    virtual void fmadd(u32 frd, u32 fra, u32 frc, u32 frb, bool rc) = 0;
    virtual void fmadds(u32 frd, u32 fra, u32 frc, u32 frb, bool rc) = 0;
    virtual void fmr(u32 frd, u32 frb, bool rc) = 0;
    virtual void fmsub(u32 frd, u32 fra, u32 frc, u32 frb, bool rc) = 0;
    virtual void fmsubs(u32 frd, u32 fra, u32 frc, u32 frb, bool rc) = 0;
    virtual void fmul(u32 frd, u32 fra, u32 frc, bool rc) = 0;
    virtual void fmuls(u32 frd, u32 fra, u32 frc, bool rc) = 0;
    virtual void fnabs(u32 frd, u32 frb, bool rc) = 0;
    virtual void fneg(u32 frd, u32 frb, bool rc) = 0;
    virtual void fnmadd(u32 frd, u32 fra, u32 frc, u32 frb, bool rc) = 0;
    virtual void fnmadds(u32 frd, u32 fra, u32 frc, u32 frb, bool rc) = 0;
    virtual void fnmsub(u32 frd, u32 fra, u32 frc, u32 frb, bool rc) = 0;
    virtual void fnmsubs(u32 frd, u32 fra, u32 frc, u32 frb, bool rc) = 0;
    virtual void fres(u32 frd, u32 frb, bool rc) = 0;
    virtual void frsp(u32 frd, u32 frb, bool rc) = 0;
    virtual void frsqrte(u32 frd, u32 frb, bool rc = 0);
    virtual void fsel(u32 frd, u32 fra, u32 frc, u32 frb, bool rc = 0);
    virtual void fsqrt(u32 frd, u32 frb, bool rc) = 0;
    virtual void fsqrts(u32 frd, u32 frb, bool rc) = 0;
    virtual void fsub(u32 frd, u32 fra, u32 frb, bool rc) = 0;
    virtual void fsubs(u32 frd, u32 fra, u32 frb, bool rc) = 0;
    virtual void isync() = 0;
    virtual void lbz(u32 rt, u32 ra, s32 d) = 0;
    virtual void lbzx(u32 rt, u32 ra, u32 rb) = 0;
    virtual void lbzu(u32 rt, u32 ra, s32 d) = 0;
    virtual void lbzux(u32 rt, u32 ra, u32 rb) = 0;
    virtual void ld(u32 rt, u32 ra, s32 ds) = 0;
    virtual void ldarx(u32 rt, u32 ra, u32 rb) = 0;
    virtual void ldu(u32 rt, u32 ra, s32 ds) = 0;
    virtual void ldux(u32 rt, u32 ra, u32 rb) = 0;
    virtual void ldx(u32 rt, u32 ra, u32 rb) = 0;
    virtual void lfd(u32 frt, u32 ra, s32 d) = 0;
    virtual void lfdu(u32 frt, u32 ra, s32 d) = 0;
    virtual void lfdux(u32 frt, u32 ra, u32 rb) = 0;
    virtual void lfdx(u32 frt, u32 ra, u32 rb) = 0;
    virtual void lfs(u32 frt, u32 ra, s32 d) = 0;
    virtual void lfsx(u32 frt, u32 ra, u32 rb) = 0;
    virtual void lfsu(u32 frt, u32 ra, s32 d) = 0;
    virtual void lfsux(u32 frt, u32 ra, u32 rb = 0);
    virtual void lha(u32 rt, u32 ra, s32 d) = 0;
    virtual void lhax(u32 rt, u32 ra, u32 rb) = 0;
    virtual void lhau(u32 rt, u32 ra, s32 d) = 0;
    virtual void lhaux(u32 rt, u32 ra, u32 rb) = 0;
    virtual void lhbrx(u32 rt, u32 ra, u32 rb) = 0;
    virtual void lhz(u32 rt, u32 ra, s32 d) = 0;
    virtual void lhzx(u32 rt, u32 ra, u32 rb) = 0;
    virtual void lhzu(u32 rt, u32 ra, s32 d) = 0;
    virtual void lhzux(u32 rt, u32 ra, u32 rb) = 0;
    virtual void lmw(u32 rt, u32 ra, s32 d) = 0;
    virtual void lswi(u32 rt, u32 ra, u32 nb) = 0;
};