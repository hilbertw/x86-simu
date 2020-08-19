#define _CRT_SECURE_NO_WARNINGS

#include "stdio.h"

#include "common.h"

#include "uop.h"
/*

Table 17-2. 16-Bit Addressing Forms with the ModR/M Byte


r8(/r)                     AL    CL    DL    BL    AH    CH    DH    BH
r16(/r)                    AX    CX    DX    BX    SP    BP    SI    DI
r32(/r)                    EAX   ECX   EDX   EBX   ESP   EBP   ESI   EDI
/digit (Opcode)            0     1     2     3     4     5     6     7
REG =                      000   001   010   011   100   101   110   111

   Effective 
+---Address--+ +Mod R/M+ +--------ModR/M Values in Hexadecimal--------+

[Bx + SI]            000   00    08    10    18    20    28    30    38
[BX + DI]            001   01    09    11    19    21    29    31    39
[BP + SI]            010   02    0A    12    1A    22    2A    32    3A
[BP + DI]            011   03    0B    13    1B    23    2B    33    3B
[SI]             00  100   04    0C    14    1C    24    2C    34    3C
[DI]                 101   05    0D    15    1D    25    2D    35    3D
disp16               110   06    0E    16    1E    26    2E    36    3E
[BX]                 111   07    0F    17    1F    27    2F    37    3F

[BX+SI]+disp8        000   40    48    50    58    60    68    70    78
[BX+DI]+disp8        001   41    49    51    59    61    69    71    79
[BP+SI]+disp8        010   42    4A    52    5A    62    6A    72    7A
[BP+DI]+disp8        011   43    4B    53    5B    63    6B    73    7B
[SI]+disp8       01  100   44    4C    54    5C    64    6C    74    7C
[DI]+disp8           101   45    4D    55    5D    65    6D    75    7D
[BP]+disp8           110   46    4E    56    5E    66    6E    76    7E
[BX]+disp8           111   47    4F    57    5F    67    6F    77    7F

[BX+SI]+disp16       000   80    88    90    98    A0    A8    B0    B8
[BX+DI]+disp16       001   81    89    91    99    A1    A9    B1    B9
[BP+SI]+disp16       010   82    8A    92    9A    A2    AA    B2    BA
[BP+DI]+disp16       011   83    8B    93    9B    A3    AB    B3    BB
[SI]+disp16      10  100   84    8C    94    9C    A4    AC    B4    BC
[DI]+disp16          101   85    8D    95    9D    A5    AD    B5    BD
[BP]+disp16          110   86    8E    96    9E    A6    AE    B6    BE
[BX]+disp16          111   87    8F    97    9F    A7    AF    B7    BF

EAX/AX/AL            000   C0    C8    D0    D8    E0    E8    F0    F8
ECX/CX/CL            001   C1    C9    D1    D9    E1    E9    F1    F9
EDX/DX/DL            010   C2    CA    D2    DA    E2    EA    F2    FA
EBX/BX/BL            011   C3    CB    D3    DB    E3    EB    F3    FB
ESP/SP/AH        11  100   C4    CC    D4    DC    E4    EC    F4    FC
EBP/BP/CH            101   C5    CD    D5    DD    E5    ED    F5    FD
ESI/SI/DH            110   C6    CE    D6    DE    E6    EE    F6    FE
EDI/DI/BH            111   C7    CF    D7    DF    E7    EF    F7    FF
*/


int  mm_len16( char* mmrm_byte)
{
     int len;
     switch(mmrm_byte[0]>>6)
     {

      case 0:
          len= ((mmrm_byte[0]&7)==6)?2:0; break;
      case 1:
           len =1; break;
      case 2:
           len =2; break;
      case 3:
           len =0; break;
     }
     return len;
}

REG16 index_reg( char* mmrm_byte)
{
     REG16 reg;

     switch(mmrm_byte[0]&7)
     {
      case 0:
        reg = ((mmrm_byte[0]>>6)==3)?R16_NONE:SI; break;
      case 1:
        reg = ((mmrm_byte[0]>>6)==3)?R16_NONE:DI; break;
      case 2:
        reg = ((mmrm_byte[0]>>6)==3)?R16_NONE:SI; break;
      case 3:
        reg = ((mmrm_byte[0]>>6)==3)?R16_NONE:DI; break;
      case 4:
        reg = R16_NONE; break;
      case 5:
        reg = R16_NONE; break;
      case 6:
        reg = R16_NONE; break;
      case 7:
        reg = R16_NONE; break;
     }
     return reg;
}

REG16 base_reg16( char* mmrm_byte)
{
     REG16 reg;

     switch(mmrm_byte[0]&7)
     {
      case 0:
        reg = ((mmrm_byte[0]>>6)==3)?R16_NONE:BX; break;
      case 1:
        reg = ((mmrm_byte[0]>>6)==3)?R16_NONE:BX; break;
      case 2:
        reg = ((mmrm_byte[0]>>6)==3)?R16_NONE:BP; break;
      case 3:
        reg = ((mmrm_byte[0]>>6)==3)?R16_NONE:BP; break;
      case 4:
        reg = ((mmrm_byte[0]>>6)==3)?R16_NONE:SI; break;
      case 5:
        reg = ((mmrm_byte[0]>>6)==3)?R16_NONE:DI; break;
      case 6:
        reg = ((mmrm_byte[0]>>6)==0)?R16_NONE:((mmrm_byte[0]>>6)==3)?R16_NONE:BP; break;
      case 7:
        reg = ((mmrm_byte[0]>>6)==3)?R16_NONE:BX; break;
	 }
  
     return reg;
}
OFFSET16 disp16( char * mmrm_byte)
{
     OFFSET16 off16;
     switch(mmrm_byte[0]>>6)
     {

      case 0:
          off16=((mmrm_byte[0]&7)==6)?*(short*)(mmrm_byte+1):0; break;
      case 1:
           off16 =*(mmrm_byte+1); break;  // disp 8 sign-extended to disp16
      case 2:
           off16 =*(short*)(mmrm_byte+1); break;
      case 3:
           off16 =0; break;
     }
     return off16;
}


/*
32-Bit Addressing Forms with the ModR/M Byte


r8(/r)                     AL    CL    DL    BL    AH    CH    DH    BH
r16(/r)                    AX    CX    DX    BX    SP    BP    SI    DI
r32(/r)                    EAX   ECX   EDX   EBX   ESP   EBP   ESI   EDI
/digit (Opcode)            0     1     2     3     4     5     6     7
REG =                      000   001   010   011   100   101   110   111

   Effective
+---Address--+ +Mod R/M+ +---------ModR/M Values in Hexadecimal-------+

[EAX]                000   00    08    10    18    20    28    30    38
[ECX]                001   01    09    11    19    21    29    31    39
[EDX]                010   02    0A    12    1A    22    2A    32    3A
[EBX]                011   03    0B    13    1B    23    2B    33    3B
[--] [--]        00  100   04    0C    14    1C    24    2C    34    3C
disp32               101   05    0D    15    1D    25    2D    35    3D
[ESI]                110   06    0E    16    1E    26    2E    36    3E
[EDI]                111   07    0F    17    1F    27    2F    37    3F

disp8[EAX]           000   40    48    50    58    60    68    70    78
disp8[ECX]           001   41    49    51    59    61    69    71    79
disp8[EDX]           010   42    4A    52    5A    62    6A    72    7A
disp8[EPX];          011   43    4B    53    5B    63    6B    73    7B
disp8[--] [--]   01  100   44    4C    54    5C    64    6C    74    7C
disp8[ebp]           101   45    4D    55    5D    65    6D    75    7D
disp8[ESI]           110   46    4E    56    5E    66    6E    76    7E
disp8[EDI]           111   47    4F    57    5F    67    6F    77    7F

disp32[EAX]          000   80    88    90    98    A0    A8    B0    B8
disp32[ECX]          001   81    89    91    99    A1    A9    B1    B9
disp32[EDX]          010   82    8A    92    9A    A2    AA    B2    BA
disp32[EBX]          011   83    8B    93    9B    A3    AB    B3    BB
disp32[--] [--]  10  100   84    8C    94    9C    A4    AC    B4    BC
disp32[EBP]          101   85    8D    95    9D    A5    AD    B5    BD
disp32[ESI]          110   86    8E    96    9E    A6    AE    B6    BE
disp32[EDI]          111   87    8F    97    9F    A7    AF    B7    BF

EAX/AX/AL            000   C0    C8    D0    D8    E0    E8    F0    F8
ECX/CX/CL            001   C1    C9    D1    D9    E1    E9    F1    F9
EDX/DX/DL            010   C2    CA    D2    DA    E2    EA    F2    FA
EBX/BX/BL            011   C3    CB    D3    DB    E3    EB    F3    FB
ESP/SP/AH        11  100   C4    CC    D4    DC    E4    EC    F4    FC
EBP/BP/CH            101   C5    CD    D5    DD    E5    ED    F5    FD
ESI/SI/DH            110   C6    CE    D6    DE    E6    EE    F6    FE
EDI/DI/BH            111   C7    CF    D7    DF    E7    EF    F7    FF
Notes
[*] means a disp32 with no base if MOD is 00, [EBP] otherwise. This provides the following addressing modes: 
      disp32[index]        (MOD=00)
      disp8[EBP][index]    (MOD=01)
      disp32[EBP][index]   (MOD=10)


[--] [--] means a SIB follows the ModR/M byte. disp8 denotes an 8-bit displacement following the SIB byte, to be sign-extended and added to the index. disp32 denotes a 32-bit displacement following the ModR/M byte, to be added to the index. 
Table 17-4. 32-Bit Addressing Forms with the SIB Byte


   r32                      EAX   ECX   EDX   EBX   ESP   [*]
   Base =                   0     1     2     3     4     5     6     7
   Base =                   000   001   010   011   100   101   110   111

+Scaled Index+ +SS Index+ +--------SIB Values in Hexadecimal--------+

[EAX]                000    00    01    02    03    04    05    06    07
[ECX]                001    08    09    0A    0B    0C    0D    0E    0F
[EDX]                010    10    11    12    13    14    15    16    17
[EBX]                011    18    19    1A    1B    1C    1D    1E    1F
none             00  100    20    21    22    23    24    25    26    27
[EBP]                101    28    29    2A    2B    2C    2D    2E    2F
[ESI]                110    30    31    32    33    34    35    36    37
[EDI]                111    38    39    3A    3B    3C    3D    3E    3F

[EAX*2]              000    40    41    42    43    44    45    46    47
[ECX*2]              001    48    49    4A    4B    4C    4D    4E    4F
[ECX*2]              010    50    51    52    53    54    55    56    57
[EBX*2]              011    58    59    5A    5B    5C    5D    5E    5F
none             01  100    60    61    62    63    64    65    66    67
[EBP*2]              101    68    69    6A    6B    6C    6D    6E    6F
[ESI*2]              110    70    71    72    73    74    75    76    77
[EDI*2]              111    78    79    7A    7B    7C    7D    7E    7F

[EAX*4]              000    80    81    82    83    84    85    86    87
[ECX*4]              001    88    89    8A    8B    8C    8D    8E    8F
[EDX*4]              010    90    91    92    93    94    95    96    97
[EBX*4]              011    98    89    9A    9B    9C    9D    9E    9F
none             10  100    A0    A1    A2    A3    A4    A5    A6    A7
[EBP*4]              101    A8    A9    AA    AB    AC    AD    AE    AF
[ESI*4]              110    B0    B1    B2    B3    B4    B5    B6    B7
[EDI*4]              111    B8    B9    BA    BB    BC    BD    BE    BF

[EAX*8]              000    C0    C1    C2    C3    C4    C5    C6    C7
[ECX*8]              001    C8    C9    CA    CB    CC    CD    CE    CF
[EDX*8]              010    D0    D1    D2    D3    D4    D5    D6    D7
[EBX*8]              011    D8    D9    DA    DB    DC    DD    DE    DF
none             11  100    E0    E1    E2    E3    E4    E5    E6    E7
[EBP*8]              101    E8    E9    EA    EB    EC    ED    EE    EF
[ESI*8]              110    F0    F1    F2    F3    F4    F5    F6    F7
[EDI*8]              111    F8    F9    FA    FB    FC    FD    FE    FF


*/

int  mm_len32( char *mmrm_byte)
{
     int len;
     switch(mmrm_byte[0]>>6)
     {

      case 0:
          len= ((mmrm_byte[0]&7)==5)?4:           // disp32
                ((mmrm_byte[0]&7)==4)?(((mmrm_byte[1]&7)==5)?5:1):           // sib+disp32
                0; 
                break;
      case 1:
           len =((mmrm_byte[0]&7)==4)?2:           // sib+disp8
                1;                              // disp8
           break;
      case 2:
           len =((mmrm_byte[0]&7)==4)?5:           // sib+disp32
                4;                              //+disp32
           break;
      case 3:
           len =0; break;
     }
     return len;
}
REG32  sib_base_reg[8]={EAX,ECX,EDX,EBX,ESP,EBP,ESI,EDI};
REG32  sib_index_reg[8]={EAX,ECX,EDX,EBX,R32_NONE,EBP,ESI,EDI};
REG32  addr32_base_reg[8]={EAX,ECX,EDX,EBX,R32_NONE,EBP,ESI,EDI};

REG32 base_reg32( char *mmrm_byte)
{
        REG32  r;

     switch(mmrm_byte[0]>>6)
     {

      case 0:
		  r= ((mmrm_byte[0]&7)==4)?
			  (((mmrm_byte[1]&7)==5)?R32_NONE:sib_base_reg[mmrm_byte[1]&7]):
		   addr32_base_reg[mmrm_byte[0]&7];
           break;
      case 1:
      case 2:
           r= ((mmrm_byte[0]&7)==4)?sib_base_reg[mmrm_byte[1]&7]:addr32_base_reg[mmrm_byte[0]&7];         
           break; 

      case 3:
           r=R32_NONE;
       }	
	

     return  r;

}


REG32 SIB_index_reg( char sib)
{
     
 
     return sib_index_reg[(sib>>3)&7];
}

char SIB_scale( char sib)
{
	return sib>>6;
}

OFFSET32 disp32( char * mmrm_byte)
{
     long off32;
     switch(mmrm_byte[0]>>6)
     {

      case 0:
          off32=(((mmrm_byte[0]&7)==4)
                 &&(mmrm_byte[1]&7)==5)?*(long*)(mmrm_byte+2):0; break;
      case 1:
           off32 =*(((mmrm_byte[0]&7)==4)?mmrm_byte+2:mmrm_byte+1); break;
      case 2:
           off32 =*(long*)(((mmrm_byte[0]&7)==4)?mmrm_byte+2:mmrm_byte+1); break;
      case 3:
           off32 =0; break;
     }
     return off32;
}


//
// 16 bit address
//
//

 int decode_len_mm_imm_16_16 (unsigned char  *code)
 {
     int len;
     switch((code[0]>>6)&3)
     {

      case 0:
          len= ((code[0]&7)==6)?6:4; break;
      case 1:
           len =5; break;
      case 2:
           len =6; break;
      case 3:
           len =4; break;
     }
     return len;
 }

 int decode_len_mm_imm_16_32 (unsigned char  *code)
 {
     int len;
     switch((code[0]>>6)&3)
     {

      case 0:
          len= ((code[0]&7)==6)?8:6; break;
      case 1:
           len =7; break;
      case 2:
           len =8; break;
      case 3:
           len =6; break;
     }
     return len;
 }

int decode_len_mm_imm_16_16x (unsigned char  *code)
 {
     int len;
     switch((code[0]>>6)&3)
     {

      case 0:
          len= ((code[0]&7)==6)?7:5; break;
      case 1:
           len =6; break;
      case 2:
           len =7; break;
      case 3:
           len =5; break;
     }
     return len;
 }

 int decode_len_mm_imm_16_32x (unsigned char  *code)
 {
     int len;
     switch((code[0]>>6)&3)
     {

      case 0:
          len= ((code[0]&7)==6)?9:7; break;
      case 1:
           len =8; break;
      case 2:
           len =9; break;
      case 3:
           len =7; break;
     }
     return len;
 }

 int decode_len_mm_noimm_16 (unsigned char  *code)
 {
     int len;
     switch((code[0]>>6)&3)
     {

      case 0:
          len= ((code[0]&7)==6)?4:2; break;
      case 1:
           len =3; break;
      case 2:
           len =4; break;
      case 3:
           len =2; break;
     }
     return len;
 }

 int decode_len_mm_noimm_16x (unsigned char  *code)
 {
     int len;
     switch((code[0]>>6)&3)
     {

      case 0:
          len= ((code[0]&7)==6)?5:3; break;
      case 1:
           len =4; break;
      case 2:
           len =5; break;
      case 3:
           len =3; break;
     }
     return len;
 }
 int decode_len_mm_imm8_16 (unsigned char  *code)
 {
     int len;
     switch((code[0]>>6)&3)
     {

      case 0:
          len= ((code[0]&7)==6)?5:3; break;
      case 1:
           len =4; break;
      case 2:
           len =5; break;
      case 3:
           len =3; break;
     }
     return len;
 }
int decode_len_mm_imm8_16x (unsigned char  *code)
 {
     int len;
     switch((code[0]>>6)&3)
     {

      case 0:
          len= ((code[0]&7)==6)?6:4; break;
      case 1:
           len =5; break;
      case 2:
           len =6; break;
      case 3:
           len =4; break;
     }
     return len;
 }
//
// 32 bit address
//
//

 int decode_len_mm_noimm_32 (unsigned char  *code)
 {
     int len;
     switch((code[0]>>6)&3)
     {

      case 0:
          len= ((code[0]&7)==5)?6:           // disp32
                ((code[0]&7)==4)?(((code[1]&7)==5)?7:3):           // sib+disp32
                2; 
                break;
      case 1:
           len =((code[0]&7)==4)?4:           // sib+disp8
                3;                              // disp8
           break;
      case 2:
           len =((code[0]&7)==4)?7:           // sib+disp32
                6;                              //+disp32
           break;
      case 3:
           len =2; break;
     }
     return len;
 }


 int decode_len_mm_noimm_32x (unsigned char  *code)
 {
     int len;
     switch((code[0]>>6)&3)
     {

      case 0:
          len= ((code[0]&7)==5)?7:           // disp32
                ((code[0]&7)==4)?(((code[1]&7)==5)?8:4):           // sib+disp32
                3; 
                break;
      case 1:
           len =((code[0]&7)==4)?5:           // sib+disp8
                4;                              // disp8
           break;
      case 2:
           len =((code[0]&7)==4)?8:           // sib+disp32
                7;                              //+disp32
           break;
      case 3:
           len =3; break;
     }
     return len;
 }

 
 int decode_len_mm_imm8_32 (unsigned char  *code)
 {
     int len;
     switch((code[0]>>6)&3)
     {

      case 0:
          len= ((code[0]&7)==5)?7:           // disp32
                ((code[0]&7)==4)?(((code[1]&7)==5)?8:4):           // sib+disp32
                3; 
                break;
      case 1:
           len =((code[0]&7)==4)?5:           // sib+disp8
                4;                              // disp8
           break;
      case 2:
           len =((code[0]&7)==4)?8:           // sib+disp32
                7;                              //+disp32
           break;
      case 3:
           len =3; break;
     }
     return len;
 }


 int decode_len_mm_imm8_32x (unsigned char  *code)
 {
     int len;
     switch((code[0]>>6)&3)
     {

      case 0:
          len= ((code[0]&7)==5)?8:           // disp32
                ((code[0]&7)==4)?(((code[1]&7)==5)?9:5):           // sib+disp32
                4; 
                break;
      case 1:
           len =((code[0]&7)==4)?6:           // sib+disp8
                5;                              // disp8
           break;
      case 2:
           len =((code[0]&7)==4)?9:           // sib+disp32
                8;                              //+disp32
           break;
      case 3:
           len =4; break;
     }
     return len;
 }
 int decode_len_mm_imm_32_16 (unsigned char  *code)
 {
     int len;
     switch((code[0]>>6)&3)
     {

      case 0:
          len= ((code[0]&7)==5)?8:           // disp32
                ((code[0]&7)==4)?(((code[1]&7)==5)?9:5):           // sib+disp32
                4; 
                break;
      case 1:
           len =((code[0]&7)==4)?6:           // sib+disp8
                5;                              // disp8
           break;
      case 2:
           len =((code[0]&7)==4)?9:           // sib+disp32
                8;                              //+disp32
           break;
      case 3:
           len =4; break;
     }
     return len;
 }


int decode_len_mm_imm_32_16x (unsigned char  *code)
 {
     int len;
     switch((code[0]>>6)&3)
     {

      case 0:
          len= ((code[0]&7)==5)?9:           // disp32
                ((code[0]&7)==4)?(((code[1]&7)==5)?10:6):           // sib+disp32
                5; 
                break;
      case 1:
           len =((code[0]&7)==4)?7:           // sib+disp8
                6;                              // disp8
           break;
      case 2:
           len =((code[0]&7)==4)?10:           // sib+disp32
                9;                              //+disp32
           break;
      case 3:
           len =5; break;
     }
     return len;
 }
int decode_len_mm_imm_32_32 (unsigned char  *code)
 {
     int len;
     switch((code[0]>>6)&3)
     {

      case 0:
          len= ((code[0]&7)==5)?10:           // disp32
                ((code[0]&7)==4)?(((code[1]&7)==5)?11:7):           // sib+disp32
                6; 
                break;
      case 1:
           len =((code[0]&7)==4)?8:           // sib+disp8
                7;                              // disp8
           break;
      case 2:
           len =((code[0]&7)==4)?11:           // sib+disp32
                10;                              //+disp32
           break;
      case 3:
           len =6; break;
     }
     return len;
 }
int decode_len_mm_imm_32_32x (unsigned char  *code)
 {
     int len;
     switch((code[0]>>6)&3)
     {

      case 0:
          len= ((code[0]&7)==5)?11:           // disp32
                ((code[0]&7)==4)?(((code[1]&7)==5)?12:8):           // sib+disp32
                7; 
                break;
      case 1:
           len =((code[0]&7)==4)?9:           // sib+disp8
                8;                              // disp8
           break;
      case 2:
           len =((code[0]&7)==4)?12:           // sib+disp32
                11;                              //+disp32
           break;
      case 3:
           len =7; break;
     }
     return len;
 }
//
// 16 bit address mode
//
// when R/M = 2/3/ 6(mod!=0),base register is BP.return true;
//
//
int is_stack16(unsigned char  *code)
{
	int mm= code[1]>>6;
	int rm= (code[1])&7;

	return (rm==2 || rm==3 ||(rm==6&& mm!=0));
}
//
// 32 bit address mode:
//     whenR/M==5 mm!=0
//         R/M==4 sib.base=4/5(mm!=0,sib.index==5)
//
//
int is_stack32(unsigned char  *code)
{
	int mm= (code[1]>>6)&3;
	int rm= code[1]&7;
	int sib_base=code[2]&7;
	int sib_index= (code[2]>>3)&7;

	return ( rm==5&&mm!=0) 
		  ||(rm==4&&(sib_base==4))
		  ||(rm==4&&(sib_base==5&& mm!=0))
		  ||(rm==4&&(sib_index==5));
}

//
// address gen uop
//
void  __cdecl  uop_addrg_addr16 (
	unsigned char * code,
	UOP &uop, // uop to gen address
	RoV & r_v,      //direct address or tmp reg number
	bool & stack,
	int & imm)
{

	//
	// if direct addressing
	//
	if( ((code[1]>>6)&3)==0&&6==(code[1]&7))
	{
		r_v.va=true;
		r_v.u.value=*(short*)(code+2);
	    imm=*(int *)(code+4);
	    uop.funit=-1;// mark invalid. no address gen uop
		stack=false;
	}
	else
	{
		r_v.va=false;
		r_v.u.reg= 0x80;// tmp reg 0
		uop.dst=0x80;
		uop.funit=1;//  address gen uop
		//
		//src1,src2
		//
		 switch(code[1]&7)
		 {
		  case 0:
			  uop.src[0].u.reg=SI;uop.src[0].va=false; 
			  uop.src[1].u.reg=BX;uop.src[1].va=false; 
			  uop.sub_op=6;
			stack=false;
			  break;
		  case 1:
			  uop.src[0].u.reg=DI;uop.src[0].va=false; 
			  uop.src[1].u.reg=BX;uop.src[1].va=false;  
			  stack=false;
	  		  uop.sub_op=6;
			  break;
		  case 2:
			  uop.src[0].u.reg=SI;uop.src[0].va=false; 
			  uop.src[1].u.reg=BP;uop.src[1].va=false;  
			  stack=true;
			  uop.sub_op=6;
			  break;
		  case 3:
			  uop.src[0].u.reg=DI;uop.src[0].va=false; 
			  uop.src[1].u.reg=BP;uop.src[1].va=false; 
				stack=true;
				stack=false;
			  uop.sub_op=6;
			  break;
		  case 4:
       		  uop.src[0].u.reg=SI;uop.src[0].va=false; uop.src[1].va=true; 
			  uop.sub_op=2;
			stack=false;
			  break;
		  case 5:
			uop.src[0].u.reg=DI;uop.src[0].va=false;  uop.src[1].va=true; 
			stack=false;
			  uop.sub_op=2;
			  break;
		  case 6:
    		  uop.src[0].u.reg=BX;uop.src[0].va=false;  uop.src[1].va=true; 
				stack=false;
			  uop.sub_op=2;
			  break;
		  case 7:
			 uop.src[0].u.reg=BP;uop.src[0].va=false; uop.src[1].va=true; 
			   stack=true;
			  uop.sub_op=2;
			  break;
		 }
		//
		//src3
		//
		switch((code[1]>>6)&3)
		 {
	
		  case 1:// disp8
			 uop.src[2].u.value=code[2];
			 uop.src[2].va=true;
			  uop.sub_op|=1;
			  imm=*(int *)(code+3);
			  break;
		  case 2 :// disp 16

			 uop.src[2].u.value=*(short*)(code+2);
			 uop.src[2].va=true;
			  uop.sub_op|=1;
			  imm=*(int *)(code+4);
			  break;
		  case 3:// 
		  case 0: // no disp
			  imm=*(int *)(code+2);
			  break;
		}
	}
}
void uop_sib (
	unsigned char * code,
	UOP &uop, // uop to gen address
	bool & stack,
	int & imm);

//
// address gen uop
//
void   __cdecl uop_addrg_addr32 (
	unsigned char * code,
	UOP &uop, // uop to gen address
	RoV & r_v,      //direct address or tmp reg number
	bool & stack,
	int & imm)
{
	//
	// if direct addressing
	//
	if( ((code[1]>>6)&3)==0&&(5==(code[1]&7))||(4==(code[1]&7))&&0x45==(code[2]&0x3f))
	{
		r_v.va=true;
		r_v.u.value=*(int*)(code+2);
	    imm=*(int *)(code+6);
	    uop.funit=-1;// mark invalid. no address gen uop
		stack=false;
	}
	else
	{
		r_v.va=false;
		r_v.u.reg= 0x80;// tmp reg 0
		uop.dst=0x80;
		uop.funit=1;//  address gen uop	
		//
		// if sib addressing
		//	
		if ((code[0]&7)==4)
		
			uop_sib (
			code,
			uop, // uop to gen address
			 stack,
			 imm);
	
		else
		{

			//
			//src1,src2
			//
			uop.src[0].u.reg=(code[1]&7)|0x20;
			uop.src[0].va=false; 
			uop.src[1].va=true;   // mark true to avoid register read
			stack=(code[1]&7)==5;
			uop.sub_op=2;
			//
			//src3
			//
			switch((code[1]>>6)&3)
			 {
			  case 1:// disp8
				 uop.src[2].u.value=code[2];
				 uop.src[2].va=true;
				  uop.sub_op|=1;
				  imm=*(int *)(code+3);
				  break;
			  case 2 :// disp 32

				 uop.src[2].u.value=*(int *)(code+2);
				 uop.src[2].va=true;
				  uop.sub_op|=1;
				  imm=*(int *)(code+6);
				  break;
			  case 3:// 
			  case 0: // no disp
				  imm=*(int *)(code+2);
				  break;
				}
		}
	}
}


//
// address gen uop
//
void uop_sib (
	unsigned char * code,
	UOP &uop, // uop to gen address
	bool & stack,
	int & imm)
{ 
	if(4!=((code[2]>>3)&7))
	{
		uop.src[1].va=false; 
		uop.src[1].u.reg=((code[2]>>3)&7)|0x20;
		uop.sub_op|=2;
	}
	//
	//
	// scale
	//
	uop.sub_op|=(code[2]&0xc0);
	//
	// disp32
	//
	if(((code[1]>>6)&3)==0&&5==(code[2]&7))
	{ 
			 uop.src[2].u.value=*(int*)(code+2);
			 uop.src[2].va=true;
			 uop.sub_op|=1;
			 imm=*(int *)(code+6);
			 stack=false;
	}
	else 
	{
		//
		//src1
		//
		uop.src[0].u.reg=(code[2]&7)|0x20;
		uop.src[0].va=false; 
		uop.sub_op|=4;
	    stack=(5==(code[2]&7))||(4==(code[2]&7))||(5==((code[2]>>3)&7));

		//
		//src3
		//
		switch((code[1]>>6)&3)
		 {
		  case 1:// disp8
			 uop.src[2].u.value=code[2];
			 uop.src[2].va=true;
			 uop.sub_op|=1;
			 imm=*(int *)(code+3);
			  break;
		  case 2 :// disp 16

			 uop.src[2].u.value=*(short*)(code+2);
			 uop.src[2].va=true;
			 uop.sub_op|=1;
			 imm=*(int *)(code+4);
			  break;
		  case 3:// 
		  case 0: // no disp
			  imm=*(int *)(code+2);
			  break;
		}
	}
}