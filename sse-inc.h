void PMINUB_xmm_xm128_r(int r_xmm1,int r_xmm2);
void PMINUB_xmm_xm128_m(int r_xmm,unsigned long *data);
void PREFETCHT0_fmt_none();

void PREFETCHT1_fmt_none();

void PREFETCHT2_fmt_none();

void PREFETCHTA_fmt_none();

void addpd_xmm_xm128_r(int r_xmm1,int r_xmm2);
void addpd_xmm_xm128_m(int r_xmm,unsigned long *data);
void addps_xmm_xm128_r(int r_xmm1,int r_xmm2);
void addps_xmm_xm128_m(int r_xmm,unsigned long *data);
void addsd_xmm_xm64_r(int r_xmm1,int r_xmm2);
void addsd_xmm_xm64_m(int r_xmm,unsigned long *data);
void addss_xmm_xm32_r(int r_xmm1,int r_xmm2);
void addss_xmm_xm32_m(int r_xmm,unsigned long *data);
void addsubpd_xmm_xm128_r(int r_xmm1,int r_xmm2);
void addsubpd_xmm_xm128_m(int r_xmm,unsigned long *data);
void addsubps_xmm_xm128_r(int r_xmm1,int r_xmm2);
void addsubps_xmm_xm128_m(int r_xmm,unsigned long *data);
void andnpd_xmm_xm128_r(int r_xmm1,int r_xmm2);
void andnpd_xmm_xm128_m(int r_xmm,unsigned long *data);
void andnps_xmm_xm128_r(int r_xmm1,int r_xmm2);
void andnps_xmm_xm128_m(int r_xmm,unsigned long *data);
void andpd_xmm_xm128_r(int r_xmm1,int r_xmm2);
void andpd_xmm_xm128_m(int r_xmm,unsigned long *data);
void andps_xmm_xm128_r(int r_xmm1,int r_xmm2);
void andps_xmm_xm128_m(int r_xmm,unsigned long *data);
void blendpd_xmm_xm128Ib_r(int r_xmm1,int r_xmm2, int Ib);
void blendpd_xmm_xm128Ib_m(int r_xmm,unsigned long *data, int Ib);
void blendps_xmm_xm128Ib_r(int r_xmm1,int r_xmm2, int Ib);
void blendps_xmm_xm128Ib_m(int r_xmm,unsigned long *data, int Ib);
void blendvpd_xmm_xm128_r(int r_xmm1,int r_xmm2);
void blendvpd_xmm_xm128_m(int r_xmm,unsigned long *data);
void blendvps_xmm_xm128_r(int r_xmm1,int r_xmm2);
void blendvps_xmm_xm128_m(int r_xmm,unsigned long *data);
void cmppd_xmm_xm128Ib_r(int r_xmm1,int r_xmm2, int Ib);
void cmppd_xmm_xm128Ib_m(int r_xmm,unsigned long *data, int Ib);
void cmpps_xmm_xm128Ib_r(int r_xmm1,int r_xmm2, int Ib);
void cmpps_xmm_xm128Ib_m(int r_xmm,unsigned long *data, int Ib);
void cmpsd_xmm_xm64Ib_r(int r_xmm1,int r_xmm2, int Ib);
void cmpsd_xmm_xm64Ib_m(int r_xmm,unsigned long *data, int Ib);
void cmpss_xmm_xm32Ib_r(int r_xmm1,int r_xmm2, int Ib);
void cmpss_xmm_xm32Ib_m(int r_xmm,unsigned long *data, int Ib);
void comisd_xmm_xm64_r(int r_xmm1,int r_xmm2);
void comisd_xmm_xm64_m(int r_xmm,unsigned long *data);
void comiss_xmm_xm32_r(int r_xmm1,int r_xmm2);
void comiss_xmm_xm32_m(int r_xmm,unsigned long *data);
void cvtdq2ps_xmm_xm128_r(int r_xmm1,int r_xmm2);
void cvtdq2ps_xmm_xm128_m(int r_xmm,unsigned long *data);
void cvtpd2dq_xmm_xm128_r(int r_xmm1,int r_xmm2);
void cvtpd2dq_xmm_xm128_m(int r_xmm,unsigned long *data);
void cvtpd2pi_xmm_xm128_r(int r_xmm1,int r_xmm2);
void cvtpd2pi_xmm_xm128_m(int r_xmm,unsigned long *data);
void cvtpd2ps_xmm_xm128_r(int r_xmm1,int r_xmm2);
void cvtpd2ps_xmm_xm128_m(int r_xmm,unsigned long *data);
void cvtpi2pd_xmm_mmm64(char * buffer);

void cvtpi2ps_xmm_mmm64(char * buffer);

void cvtps2dq_xmm_xm128_r(int r_xmm1,int r_xmm2);
void cvtps2dq_xmm_xm128_m(int r_xmm,unsigned long *data);
void cvtps2pd_xmm_xm64_r(int r_xmm1,int r_xmm2);
void cvtps2pd_xmm_xm64_m(int r_xmm,unsigned long *data);
void cvtps2pi_mm_xm64();

void cvtsd2si_r32_xm64();

void cvtsd2ss_xmm_xm64_r(int r_xmm1,int r_xmm2);
void cvtsd2ss_xmm_xm64_m(int r_xmm,unsigned long *data);
void cvtsi2sd_xmm_rm32();

void cvtsi2ss_xmm_rm32();

void cvtss2sd_xmm_xm32_r(int r_xmm1,int r_xmm2);
void cvtss2sd_xmm_xm32_m(int r_xmm,unsigned long *data);
void cvtss2si_r32_xm32(int i);

void cvttpd2dq_xmm_xm128_r(int r_xmm1,int r_xmm2);
void cvttpd2dq_xmm_xm128_m(int r_xmm,unsigned long *data);
void cvttpd2pi_mm_xm128();

void cvttps2dq_xmm_xm128_r(int r_xmm1,int r_xmm2);
void cvttps2dq_xmm_xm128_m(int r_xmm,unsigned long *data);
void cvttps2pi_mm_xm64();

void cvttsd2si_r32_xm64();

void cvttss2si_r32_xm32(int i);

void divpd_xmm_xm128_r(int r_xmm1,int r_xmm2);
void divpd_xmm_xm128_m(int r_xmm,unsigned long *data);
void divps_xmm_xm128_r(int r_xmm1,int r_xmm2);
void divps_xmm_xm128_m(int r_xmm,unsigned long *data);
void divsd_xmm_xm64_r(int r_xmm1,int r_xmm2);
void divsd_xmm_xm64_m(int r_xmm,unsigned long *data);
void divss_xmm_xm32_r(int r_xmm1,int r_xmm2);
void divss_xmm_xm32_m(int r_xmm,unsigned long *data);
void dppd_xmm_xm128Ib_r(int r_xmm1,int r_xmm2, int Ib);
void dppd_xmm_xm128Ib_m(int r_xmm,unsigned long *data, int Ib);
void dpps_xmm_xm128Ib_r(int r_xmm1,int r_xmm2, int Ib);
void dpps_xmm_xm128Ib_m(int r_xmm,unsigned long *data, int Ib);
void extractps_xmm_xm128Ib_r(int r_xmm1,int r_xmm2, int Ib);
void extractps_xmm_xm128Ib_m(int r_xmm,unsigned long *data, int Ib);
void haddpd_xmm_xm128_r(int r_xmm1,int r_xmm2);
void haddpd_xmm_xm128_m(int r_xmm,unsigned long *data);
void haddps_xmm_xm128_r(int r_xmm1,int r_xmm2);
void haddps_xmm_xm128_m(int r_xmm,unsigned long *data);
void hsubpd_xmm_xm128_r(int r_xmm1,int r_xmm2);
void hsubpd_xmm_xm128_m(int r_xmm,unsigned long *data);
void hsubps_xmm_xm128_r(int r_xmm1,int r_xmm2);
void hsubps_xmm_xm128_m(int r_xmm,unsigned long *data);
void insertps_xmm_xm128Ib_r(int r_xmm1,int r_xmm2, int Ib);
void insertps_xmm_xm128Ib_m(int r_xmm,unsigned long *data, int Ib);
void lddqu_xmm_xm128_r(int r_xmm1,int r_xmm2);
void lddqu_xmm_xm128_m(int r_xmm,unsigned long *data);
void maskmovdqu_xmm_xm128_r(int r_xmm1,int r_xmm2);
void maskmovdqu_xmm_xm128_m(int r_xmm,unsigned long *data);
void maskmovq_xmm_xm128_r(int r_xmm1,int r_xmm2);
void maskmovq_xmm_xm128_m(int r_xmm,unsigned long *data);
void maxpd_xmm_xm128_r(int r_xmm1,int r_xmm2);
void maxpd_xmm_xm128_m(int r_xmm,unsigned long *data);
void maxps_xmm_xm128_r(int r_xmm1,int r_xmm2);
void maxps_xmm_xm128_m(int r_xmm,unsigned long *data);
void maxsd_xmm_xm64_r(int r_xmm1,int r_xmm2);
void maxsd_xmm_xm64_m(int r_xmm,unsigned long *data);
void maxss_xmm_xm32_r(int r_xmm1,int r_xmm2);
void maxss_xmm_xm32_m(int r_xmm,unsigned long *data);
void minpd_xmm_xm128_r(int r_xmm1,int r_xmm2);
void minpd_xmm_xm128_m(int r_xmm,unsigned long *data);
void minps_xmm_xm128_r(int r_xmm1,int r_xmm2);
void minps_xmm_xm128_m(int r_xmm,unsigned long *data);
void minsd_xmm_xm64_r(int r_xmm1,int r_xmm2);
void minsd_xmm_xm64_m(int r_xmm,unsigned long *data);
void minss_xmm_xm32_r(int r_xmm1,int r_xmm2);
void minss_xmm_xm32_m(int r_xmm,unsigned long *data);
void movapd_xmm_xm128_r(int r_xmm1,int r_xmm2);
void movapd_xmm_xm128_m(int r_xmm,unsigned long *data);
void movaps_xmm_xm128_r(int r_xmm1,int r_xmm2);
void movaps_xmm_xm128_m(int r_xmm,unsigned long *data);
void movd_mm_rm32(int i);

void movd_xmm_rm32();

void movddup_xmm_xm128_r(int r_xmm1,int r_xmm2);
void movddup_xmm_xm128_m(int r_xmm,unsigned long *data);
void movdq2q_xmm_xm128_r(int r_xmm1,int r_xmm2);
void movdq2q_xmm_xm128_m(int r_xmm,unsigned long *data);
void movdqa_xmm_xm128_r(int r_xmm1,int r_xmm2);
void movdqa_xmm_xm128_m(int r_xmm,unsigned long *data);
void movdqu_xmm_xm128_r(int r_xmm1,int r_xmm2);
void movdqu_xmm_xm128_m(int r_xmm,unsigned long *data);
void movhlps_xmm_xm128_r(int r_xmm1,int r_xmm2);
void movhlps_xmm_xm128_m(int r_xmm,unsigned long *data);
void movhpd_xmm_xm128_r(int r_xmm1,int r_xmm2);
void movhpd_xmm_xm128_m(int r_xmm,unsigned long *data);
void movhps_xmm_xm128_r(int r_xmm1,int r_xmm2);
void movhps_xmm_xm128_m(int r_xmm,unsigned long *data);
void movlhpd_xmm_xm128_r(int r_xmm1,int r_xmm2);
void movlhpd_xmm_xm128_m(int r_xmm,unsigned long *data);
void movlhps_xmm_xm128_r(int r_xmm1,int r_xmm2);
void movlhps_xmm_xm128_m(int r_xmm,unsigned long *data);
void movlpd_xmm_xm128v(char * buffer);

void movlps_xmm_xm128v(char * buffer);

void movmskpd_r32_xmm();

void movmskps_r32_xmm();

void movndq_m128_xmm();

void movntdqa_xmm1_m128();

void movnti_EvGv();

void movntpd_m128_xmm();

void movntps_m128_xmm();

void movntq_m64_mm();

void movq_xm64_xmm();

void movq_xmm_xm64_r(int r_xmm1,int r_xmm2);
void movq_xmm_xm64_m(int r_xmm,unsigned long *data);
void movq_mm_rm64();

void movq_xmm_rm64();

void movq_mm_mmm64();

void movq2dq_xmm_xm128_r(int r_xmm1,int r_xmm2);
void movq2dq_xmm_xm128_m(int r_xmm,unsigned long *data);
void movsd_xmm_xm64_r(int r_xmm1,int r_xmm2);
void movsd_xmm_xm64_m(int r_xmm,unsigned long *data);
void movshdup_xmm_xm128_r(int r_xmm1,int r_xmm2);
void movshdup_xmm_xm128_m(int r_xmm,unsigned long *data);
void movsldup_xmm_xm128_r(int r_xmm1,int r_xmm2);
void movsldup_xmm_xm128_m(int r_xmm,unsigned long *data);
void movss_xmm_xm32_r(int r_xmm1,int r_xmm2);
void movss_xmm_xm32_m(int r_xmm,unsigned long *data);
void movupd_xmm_xm128_r(int r_xmm1,int r_xmm2);
void movupd_xmm_xm128_m(int r_xmm,unsigned long *data);
void movups_xmm_xm128_r(int r_xmm1,int r_xmm2);
void movups_xmm_xm128_m(int r_xmm,unsigned long *data);
void mpsadbw_xmm_xm128Ib_r(int r_xmm1,int r_xmm2, int Ib);
void mpsadbw_xmm_xm128Ib_m(int r_xmm,unsigned long *data, int Ib);
void mulpd_xmm_xm128_r(int r_xmm1,int r_xmm2);
void mulpd_xmm_xm128_m(int r_xmm,unsigned long *data);
void mulps_xmm_xm128_r(int r_xmm1,int r_xmm2);
void mulps_xmm_xm128_m(int r_xmm,unsigned long *data);
void mulsd_xmm_xm64_r(int r_xmm1,int r_xmm2);
void mulsd_xmm_xm64_m(int r_xmm,unsigned long *data);
void mulss_xmm_xm32_r(int r_xmm1,int r_xmm2);
void mulss_xmm_xm32_m(int r_xmm,unsigned long *data);
void orpd_xmm_xm128_r(int r_xmm1,int r_xmm2);
void orpd_xmm_xm128_m(int r_xmm,unsigned long *data);
void orps_xmm_xm128_r(int r_xmm1,int r_xmm2);
void orps_xmm_xm128_m(int r_xmm,unsigned long *data);
void packssdw_xmm_xm128_r(int r_xmm1,int r_xmm2);
void packssdw_xmm_xm128_m(int r_xmm,unsigned long *data);
void packssdw_xmm_xm128_r(int r_xmm1,int r_xmm2);
void packssdw_xmm_xm128_m(int r_xmm,unsigned long *data);
void packsswb_xmm_xm128_r(int r_xmm1,int r_xmm2);
void packsswb_xmm_xm128_m(int r_xmm,unsigned long *data);
void packsswb_xmm_xm128_r(int r_xmm1,int r_xmm2);
void packsswb_xmm_xm128_m(int r_xmm,unsigned long *data);
void packuswb_xmm_xm128_r(int r_xmm1,int r_xmm2);
void packuswb_xmm_xm128_m(int r_xmm,unsigned long *data);
void packuswb_xmm_xm128_r(int r_xmm1,int r_xmm2);
void packuswb_xmm_xm128_m(int r_xmm,unsigned long *data);
void paddb_xmm_xm128_r(int r_xmm1,int r_xmm2);
void paddb_xmm_xm128_m(int r_xmm,unsigned long *data);
void paddb_xmm_xm128_r(int r_xmm1,int r_xmm2);
void paddb_xmm_xm128_m(int r_xmm,unsigned long *data);
void paddd_xmm_xm128_r(int r_xmm1,int r_xmm2);
void paddd_xmm_xm128_m(int r_xmm,unsigned long *data);
void paddd_xmm_xm128_r(int r_xmm1,int r_xmm2);
void paddd_xmm_xm128_m(int r_xmm,unsigned long *data);
void paddq_xmm_xm128_r(int r_xmm1,int r_xmm2);
void paddq_xmm_xm128_m(int r_xmm,unsigned long *data);
void paddq_xmm_xm128_r(int r_xmm1,int r_xmm2);
void paddq_xmm_xm128_m(int r_xmm,unsigned long *data);
void paddsb_xmm_xm128_r(int r_xmm1,int r_xmm2);
void paddsb_xmm_xm128_m(int r_xmm,unsigned long *data);
void paddsb_xmm_xm128_r(int r_xmm1,int r_xmm2);
void paddsb_xmm_xm128_m(int r_xmm,unsigned long *data);
void paddsw_xmm_xm128_r(int r_xmm1,int r_xmm2);
void paddsw_xmm_xm128_m(int r_xmm,unsigned long *data);
void paddsw_xmm_xm128_r(int r_xmm1,int r_xmm2);
void paddsw_xmm_xm128_m(int r_xmm,unsigned long *data);
void paddusb_xmm_xm128_r(int r_xmm1,int r_xmm2);
void paddusb_xmm_xm128_m(int r_xmm,unsigned long *data);
void paddusb_xmm_xm128_r(int r_xmm1,int r_xmm2);
void paddusb_xmm_xm128_m(int r_xmm,unsigned long *data);
void paddusw_xmm_xm128_r(int r_xmm1,int r_xmm2);
void paddusw_xmm_xm128_m(int r_xmm,unsigned long *data);
void paddusw_xmm_xm128_r(int r_xmm1,int r_xmm2);
void paddusw_xmm_xm128_m(int r_xmm,unsigned long *data);
void paddw_xmm_xm128_r(int r_xmm1,int r_xmm2);
void paddw_xmm_xm128_m(int r_xmm,unsigned long *data);
void paddw_xmm_xm128_r(int r_xmm1,int r_xmm2);
void paddw_xmm_xm128_m(int r_xmm,unsigned long *data);
void pand_xmm_xm128_r(int r_xmm1,int r_xmm2);
void pand_xmm_xm128_m(int r_xmm,unsigned long *data);
void pand_mm_mm64(int i);

void pandn_xmm_xm128_r(int r_xmm1,int r_xmm2);
void pandn_xmm_xm128_m(int r_xmm,unsigned long *data);
void pandn_mm_mm64(int i);

void pavgb_xmm_xm128_r(int r_xmm1,int r_xmm2);
void pavgb_xmm_xm128_m(int r_xmm,unsigned long *data);
void pavgb_xmm_xm128_r(int r_xmm1,int r_xmm2);
void pavgb_xmm_xm128_m(int r_xmm,unsigned long *data);
void pavgw_xmm_xm128_r(int r_xmm1,int r_xmm2);
void pavgw_xmm_xm128_m(int r_xmm,unsigned long *data);
void pavgw_xmm_xm128_r(int r_xmm1,int r_xmm2);
void pavgw_xmm_xm128_m(int r_xmm,unsigned long *data);
void pcmpeqb_xmm_xm128_r(int r_xmm1,int r_xmm2);
void pcmpeqb_xmm_xm128_m(int r_xmm,unsigned long *data);
void pcmpeqb_xmm_xm128_r(int r_xmm1,int r_xmm2);
void pcmpeqb_xmm_xm128_m(int r_xmm,unsigned long *data);
void pcmpeqd_xmm_xm128_r(int r_xmm1,int r_xmm2);
void pcmpeqd_xmm_xm128_m(int r_xmm,unsigned long *data);
void pcmpeqd_xmm_xm128_r(int r_xmm1,int r_xmm2);
void pcmpeqd_xmm_xm128_m(int r_xmm,unsigned long *data);
void pcmpeqw_xmm_xm128_r(int r_xmm1,int r_xmm2);
void pcmpeqw_xmm_xm128_m(int r_xmm,unsigned long *data);
void pcmpeqw_xmm_xm128_r(int r_xmm1,int r_xmm2);
void pcmpeqw_xmm_xm128_m(int r_xmm,unsigned long *data);
void pcmpgtb_xmm_xm128_r(int r_xmm1,int r_xmm2);
void pcmpgtb_xmm_xm128_m(int r_xmm,unsigned long *data);
void pcmpgtb_xmm_xm128_r(int r_xmm1,int r_xmm2);
void pcmpgtb_xmm_xm128_m(int r_xmm,unsigned long *data);
void pcmpgtd_xmm_xm128_r(int r_xmm1,int r_xmm2);
void pcmpgtd_xmm_xm128_m(int r_xmm,unsigned long *data);
void pcmpgtd_xmm_xm128_r(int r_xmm1,int r_xmm2);
void pcmpgtd_xmm_xm128_m(int r_xmm,unsigned long *data);
void pcmpgtw_xmm_xm128_r(int r_xmm1,int r_xmm2);
void pcmpgtw_xmm_xm128_m(int r_xmm,unsigned long *data);
void pcmpgtw_xmm_xm128_r(int r_xmm1,int r_xmm2);
void pcmpgtw_xmm_xm128_m(int r_xmm,unsigned long *data);
void pextrw_xmm_xm128Ib_r(int r_xmm1,int r_xmm2, int Ib);
void pextrw_xmm_xm128Ib_m(int r_xmm,unsigned long *data, int Ib);
void pinsrw_xmm_xm128Ib_r(int r_xmm1,int r_xmm2, int Ib);
void pinsrw_xmm_xm128Ib_m(int r_xmm,unsigned long *data, int Ib);
void pmaddwd_xmm_xm128_r(int r_xmm1,int r_xmm2);
void pmaddwd_xmm_xm128_m(int r_xmm,unsigned long *data);
void pmaxsw_xmm_xm128_r(int r_xmm1,int r_xmm2);
void pmaxsw_xmm_xm128_m(int r_xmm,unsigned long *data);
void pmaxub_xmm_xm128_r(int r_xmm1,int r_xmm2);
void pmaxub_xmm_xm128_m(int r_xmm,unsigned long *data);
void pminsw_xmm_xm128_r(int r_xmm1,int r_xmm2);
void pminsw_xmm_xm128_m(int r_xmm,unsigned long *data);
void pmovmskb_xmm_xm128_r(int r_xmm1,int r_xmm2);
void pmovmskb_xmm_xm128_m(int r_xmm,unsigned long *data);
void pmulhuw_xmm_xm128_r(int r_xmm1,int r_xmm2);
void pmulhuw_xmm_xm128_m(int r_xmm,unsigned long *data);
void pmullhw_xmm_xm128_r(int r_xmm1,int r_xmm2);
void pmullhw_xmm_xm128_m(int r_xmm,unsigned long *data);
void pmulludq_xmm_xm128_r(int r_xmm1,int r_xmm2);
void pmulludq_xmm_xm128_m(int r_xmm,unsigned long *data);
void pmullw_xmm_xm128_r(int r_xmm1,int r_xmm2);
void pmullw_xmm_xm128_m(int r_xmm,unsigned long *data);
void por_xmm_xm128_r(int r_xmm1,int r_xmm2);
void por_xmm_xm128_m(int r_xmm,unsigned long *data);
void por_mm_mm64(int i);

void ppushd_mm_mm64(int i);

void ppushd_xmm_xm128_r(int r_xmm1,int r_xmm2);
void ppushd_xmm_xm128_m(int r_xmm,unsigned long *data);
void psadbw_mm_mm64(int i);

void psadbw_xmm_xm128_r(int r_xmm1,int r_xmm2);
void psadbw_xmm_xm128_m(int r_xmm,unsigned long *data);
void pshufhw_xmm_xm128Ib_r(int r_xmm1,int r_xmm2, int Ib);
void pshufhw_xmm_xm128Ib_m(int r_xmm,unsigned long *data, int Ib);
void pshuflw_xmm_xm128Ib_r(int r_xmm1,int r_xmm2, int Ib);
void pshuflw_xmm_xm128Ib_m(int r_xmm,unsigned long *data, int Ib);
void pshufw_xmm_xm128Ib_r(int r_xmm1,int r_xmm2, int Ib);
void pshufw_xmm_xm128Ib_m(int r_xmm,unsigned long *data, int Ib);
void pslld_mm_mm64(int i);

void pslld_xmm_xm128_r(int r_xmm1,int r_xmm2);
void pslld_xmm_xm128_m(int r_xmm,unsigned long *data);
void pslld_xmm_xm128Ib_r(int r_xmm1,int r_xmm2, int Ib);
void pslld_xmm_xm128Ib_m(int r_xmm,unsigned long *data, int Ib);
void pslldq_xmm_xm128Ib_r(int r_xmm1,int r_xmm2, int Ib);
void pslldq_xmm_xm128Ib_m(int r_xmm,unsigned long *data, int Ib);
void psllq_mm_mm64(int i);

void psllq_xmm_xm128_r(int r_xmm1,int r_xmm2);
void psllq_xmm_xm128_m(int r_xmm,unsigned long *data);
void psllq_xmm_xm128Ib_r(int r_xmm1,int r_xmm2, int Ib);
void psllq_xmm_xm128Ib_m(int r_xmm,unsigned long *data, int Ib);
void psllw_xmm_xm128_r(int r_xmm1,int r_xmm2);
void psllw_xmm_xm128_m(int r_xmm,unsigned long *data);
void psllw_mm_mm64(int i);

void psllw_xmm_xm128Ib_r(int r_xmm1,int r_xmm2, int Ib);
void psllw_xmm_xm128Ib_m(int r_xmm,unsigned long *data, int Ib);
void psrad_mm_mm64(int i);

void psrad_xmm_xm128_r(int r_xmm1,int r_xmm2);
void psrad_xmm_xm128_m(int r_xmm,unsigned long *data);
void psrad_xmm_xm128Ib_r(int r_xmm1,int r_xmm2, int Ib);
void psrad_xmm_xm128Ib_m(int r_xmm,unsigned long *data, int Ib);
void psraw_xmm_xm128v(char * buffer);

void psraw_xmm_xm128v(char * buffer);

void psraw_xmm_xm128Ib_r(int r_xmm1,int r_xmm2, int Ib);
void psraw_xmm_xm128Ib_m(int r_xmm,unsigned long *data, int Ib);
void psrldq_xmm_xm128Ib_r(int r_xmm1,int r_xmm2, int Ib);
void psrldq_xmm_xm128Ib_m(int r_xmm,unsigned long *data, int Ib);
void psubb_mm_mm64(int i);

void psubd_mm_mm64(int i);

void psubq_xmm_xm128_r(int r_xmm1,int r_xmm2);
void psubq_xmm_xm128_m(int r_xmm,unsigned long *data);
void psubsb_mm_mm64(int i);

void psubsw_xmm_xm128_r(int r_xmm1,int r_xmm2);
void psubsw_xmm_xm128_m(int r_xmm,unsigned long *data);
void psubusb_mm_mm64(int i);

void psubusw_xmm_xm128_r(int r_xmm1,int r_xmm2);
void psubusw_xmm_xm128_m(int r_xmm,unsigned long *data);
void psubw_xmm_xm128_r(int r_xmm1,int r_xmm2);
void psubw_xmm_xm128_m(int r_xmm,unsigned long *data);
void punpckhbw_mm_mm32(int i);

void punpckhbw_xmm_xm128_r(int r_xmm1,int r_xmm2);
void punpckhbw_xmm_xm128_m(int r_xmm,unsigned long *data);
void punpckhdq_mm_mm32(int i);

void punpckhdq_xmm_xm128_r(int r_xmm1,int r_xmm2);
void punpckhdq_xmm_xm128_m(int r_xmm,unsigned long *data);
void punpckhqdq_xmm_xm128_r(int r_xmm1,int r_xmm2);
void punpckhqdq_xmm_xm128_m(int r_xmm,unsigned long *data);
void punpckhwd_mm_mm32(int i);

void punpckhwd_xmm_xm128_r(int r_xmm1,int r_xmm2);
void punpckhwd_xmm_xm128_m(int r_xmm,unsigned long *data);
void punpcklbw_xmm_xm128_r(int r_xmm1,int r_xmm2);
void punpcklbw_xmm_xm128_m(int r_xmm,unsigned long *data);
void punpcklbw_mm_mm32(int i);

void punpckldq_mm_mm32(int i);

void punpckldq_xmm_xm128_r(int r_xmm1,int r_xmm2);
void punpckldq_xmm_xm128_m(int r_xmm,unsigned long *data);
void punpcklqdq_xmm_xm128_r(int r_xmm1,int r_xmm2);
void punpcklqdq_xmm_xm128_m(int r_xmm,unsigned long *data);
void punpcklwd_mm_mm32(int i);

void punpcklwd_xmm_xm128_r(int r_xmm1,int r_xmm2);
void punpcklwd_xmm_xm128_m(int r_xmm,unsigned long *data);
void pxor_xmm_xm128_r(int r_xmm1,int r_xmm2);
void pxor_xmm_xm128_m(int r_xmm,unsigned long *data);
void pxor_mm_mm64(int i);

void rcpps_xmm_xm128_r(int r_xmm1,int r_xmm2);
void rcpps_xmm_xm128_m(int r_xmm,unsigned long *data);
void rcpss_xmm_xm128_r(int r_xmm1,int r_xmm2);
void rcpss_xmm_xm128_m(int r_xmm,unsigned long *data);
void rsqrtps_xmm_xm128_r(int r_xmm1,int r_xmm2);
void rsqrtps_xmm_xm128_m(int r_xmm,unsigned long *data);
void rsqrtss_xmm_xm128_r(int r_xmm1,int r_xmm2);
void rsqrtss_xmm_xm128_m(int r_xmm,unsigned long *data);
void shufpd_xmm_xm128Ib_r(int r_xmm1,int r_xmm2, int Ib);
void shufpd_xmm_xm128Ib_m(int r_xmm,unsigned long *data, int Ib);
void shufps_xmm_xm128Ib_r(int r_xmm1,int r_xmm2, int Ib);
void shufps_xmm_xm128Ib_m(int r_xmm,unsigned long *data, int Ib);
void sqrtps_xmm_xm128_r(int r_xmm1,int r_xmm2);
void sqrtps_xmm_xm128_m(int r_xmm,unsigned long *data);
void sqrtsd_xmm_xm64_r(int r_xmm1,int r_xmm2);
void sqrtsd_xmm_xm64_m(int r_xmm,unsigned long *data);
void sqrtss_xmm_xm32_r(int r_xmm1,int r_xmm2);
void sqrtss_xmm_xm32_m(int r_xmm,unsigned long *data);
void subpd_xmm_xm128_r(int r_xmm1,int r_xmm2);
void subpd_xmm_xm128_m(int r_xmm,unsigned long *data);
void subps_xmm_xm128_r(int r_xmm1,int r_xmm2);
void subps_xmm_xm128_m(int r_xmm,unsigned long *data);
void subsd_xmm_xm64_r(int r_xmm1,int r_xmm2);
void subsd_xmm_xm64_m(int r_xmm,unsigned long *data);
void subss_xmm_xm32_r(int r_xmm1,int r_xmm2);
void subss_xmm_xm32_m(int r_xmm,unsigned long *data);
void ucomisd_xmm_xm64_r(int r_xmm1,int r_xmm2);
void ucomisd_xmm_xm64_m(int r_xmm,unsigned long *data);
void ucomiss_xmm_xm32_r(int r_xmm1,int r_xmm2);
void ucomiss_xmm_xm32_m(int r_xmm,unsigned long *data);
void unpckhpd_xmm_xm128_r(int r_xmm1,int r_xmm2);
void unpckhpd_xmm_xm128_m(int r_xmm,unsigned long *data);
void unpckhps_xmm_xm128_r(int r_xmm1,int r_xmm2);
void unpckhps_xmm_xm128_m(int r_xmm,unsigned long *data);
void unpcklpd_xmm_xm128_r(int r_xmm1,int r_xmm2);
void unpcklpd_xmm_xm128_m(int r_xmm,unsigned long *data);
void unpcklps_xmm_xm128_r(int r_xmm1,int r_xmm2);
void unpcklps_xmm_xm128_m(int r_xmm,unsigned long *data);
void xorpd_xmm_xm128_r(int r_xmm1,int r_xmm2);
void xorpd_xmm_xm128_m(int r_xmm,unsigned long *data);
void xorps_xmm_xm128_r(int r_xmm1,int r_xmm2);
void xorps_xmm_xm128_m(int r_xmm,unsigned long *data);
