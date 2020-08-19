void F2XM1_();

void FABS_();

void FADD_STi0(int i);

void FADD_ST0i(int i);

void FADD_mem4r(char * buffer);

void FADD_mem8r(char * buffer);

void FADDP_STi0(int i);

void FBLD_mem10d(char * buffer);

void FBSTP_mem10d(char * buffer);

void FCHS_();

void FCLEX_();

void FCMOVB_ST0i(int i);

void FCMOVBE_ST0i(int i);

void FCMOVE_ST0i(int i);

void FCMOVNB_ST0i(int i);

void FCMOVNBE_ST0i(int i);

void FCMOVNE_ST0i(int i);

void FCMOVNU_ST0i(int i);

void FCMOVU_ST0i(int i);

void FCOMI_ST0i(unsigned short &cflags,int i);

void FCOMIP_ST0i(unsigned short &cflags,int i);

void FCOS_();

void FDECSTP_();

void FDIV_mem8r(char * buffer);

void FDIV_ST0i(int i);

void FDIV_STi0(int i);

void FDIV_mem4r(char * buffer);

void FDIVP_STi0(int i);

void FDIVR_mem4r(char * buffer);

void FDIVR_mem8r(char * buffer);

void FDIVR_ST0i(int i);

void FDIVR_STi0(int i);

void FDIVRP_STi0(int i);

void FFREE_STi(int i);

void FIADD_mem4i( unsigned long  data);
void FIADD_mem2i(unsigned short data);
void FICOM_mem2i(unsigned short data);
void FICOM_mem4i( unsigned long  data);
void FICOMP_mem4i( unsigned long  data);
void FICOMP_mem2i(unsigned short data);
void FIDIV_mem2i(unsigned short data);
void FIDIV_mem4i( unsigned long  data);
void FIDIVR_mem2i(unsigned short data);
void FIDIVR_mem4i( unsigned long  data);
void FILD_mem2i(unsigned short data);
void FILD_mem4i( unsigned long  data);
void FILD_mem8i( unsigned long * data);
void FIMUL_mem2i(unsigned short data);
void FIMUL_mem4i( unsigned long  data);
void FINCSTP_();

unsigned short FIST_mem2i();
unsigned long FIST_mem4i();

unsigned long FISTP_mem4i();

void FISTP_mem8i( unsigned long * data);
unsigned short FISTP_mem2i();
unsigned short FISTTP_mem2i();
unsigned long FISTTP_mem4i();

void FISTTP_mem8i( unsigned long * data);
void FISUB_mem2i(unsigned short data);
void FISUB_mem4i( unsigned long  data);
void FISUBR_mem2i(unsigned short data);
void FISUBR_mem4i( unsigned long  data);
void FLD_mem8r(char * buffer);

void FLD_STi(int i);

void FLD_mem10r(char * buffer);

void FLD_mem4r(char * buffer);

void FLD1_();

void FLDCW_mem2i(unsigned short data);
void FLDENV_mem14(char * buffer);

void FLDL2E_();

void FLDL2T_();

void FLDLG2_();

void FLDLN2_();

void FLDPI_();

void FLDZ_();

void FMUL_mem4r(char * buffer);

void FMUL_mem8r(char * buffer);

void FMUL_ST0i(int i);

void FMUL_STi0(int i);

void FMULP_STi0(int i);

void FNINIT_();

void FNOP_();

unsigned short FNSTCW_mem2i();
void FNSTENV_mem14(char * buffer);

unsigned short FNSTSW_fmt_AX();
unsigned short FNSTSW_mem2i();
void FPATAN_();

void FPREM_();

void FPREM1_();

void FPTAN_();

void FRNDINT_();

void FRSTOR_mem108(int OperandSize,char * buffer);

void FSAVE_mem108(int OperandSize,char * buffer);

void FSCALE_();

void FSIN_();

void FSINCOS_();

void FSQRT_();

void FST_mem8r(char * buffer);

void FST_STi(int i);

void FST_mem4r(char * buffer);

void FSTP_mem4r(char * buffer);

void FSTP_mem10r(char * buffer);

void FSTP_STi(int i);

void FSTP_mem8r(char * buffer);

void FSUB_STi0(int i);

void FSUB_ST0i(int i);

void FSUB_mem4r(char * buffer);

void FSUB_mem8r(char * buffer);

void FSUBP_STi0(int i);

void FSUBR_ST0i(int i);

void FSUBR_mem8r(char * buffer);

void FSUBR_STi0(int i);

void FSUBR_mem4r(char * buffer);

void FSUBRP_STi0(int i);

void FTST_();

void FUCOM_STi(int i);

void FUCOMI_ST0i(unsigned short &cflags,int i);

void FUCOMIP_ST0i(unsigned short &cflags,int i);

void FUCOMP_();

void FUCOMP_STi(int i);

void FUCOMPP_();

void FXAM_();

void FXCH_STi(int i);

void FXTRACT_();

void FYL2X_();

void FYL2XP1_();

