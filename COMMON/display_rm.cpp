#define _CRT_SECURE_NO_WARNINGS



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <common.h>


char * data_size_name[]={"byte","word","dword","Bword","far"};
char * segment_name[]={ "ES","CS","SS","DS","FS","GS"};

//extern char * segment_name[];
extern char *reg16_name[];
extern char *reg8_name[];
//
//
//
//  data_sise_num: -1:don't display


int display_Mv(
	unsigned char  *code, 
	char * buffer,
	int address_size,
	int data_size_num,
	int override_seg )
{
	int len=0;

	if(-1!=data_size_num)
		len=sprintf( buffer,"%s ptr ",data_size_name[data_size_num]);
	//
	// check whether segment overriden
	//
	if(override_seg!=NO_SEGOVR)
	{
	int flag=0;
	if (  (address_size==16&&is_stack16(code))
    	||(address_size==32&&is_stack32(code)))
	{
		flag= (override_seg!=SS);
	}
	else
		flag= (override_seg!=DS);

	if(flag)
		len+=sprintf( buffer+len,"%s:",segment_name[override_seg]);
	}

	len +=(address_size==16)?Mv_display16(code,buffer+len):
		  (address_size==32)?Mv_display32(code,buffer+len):0;

	return len;
}

int display_Rv(
	int reg,
	char * buffer,
	int data_size,
	bool word)
{
	int len;

	if (word) {
			if(data_size==32){*buffer++='e';len=1;}else len=0;
			len += sprintf(buffer,"%s",reg16_name[reg]);
	}
	else
			len = sprintf(buffer,"%s",reg8_name[reg]);
	return len;

}

int display_Ev(
	unsigned char  *code, 
	char * buffer,
	int address_size,
	int data_size,
	bool word,
	int override_seg,
	bool disp_dsize)
{
	int len=0;

	//
	// if is a memory
	//
	if ( ((code[1]>>6)&3)!=3)
	
		len= display_Mv(
			code, 
			buffer,
			 address_size,
			 (!disp_dsize)?-1:
			 (!word)?0:
			 (data_size==8)?0:
			 (data_size==16)?1:
			 (data_size==32)?2:
			 -1,

			 override_seg );
		
	else
	
		//
		//it is a register
		//

		len= display_Rv(
			code[1]&7, 
			buffer,
			data_size,
			 word);
			
	
	return len;
}

int display_EvGv(
	unsigned char  *code, 
	char * buffer,
	int address_size,
	int data_size,
	bool word,
	bool dir,
	int override_seg )
{
	int len;
	//
	//dir=true
	//
	if (dir)
	{
		len= display_Rv(
			(code[1]>>3)&7, 
			buffer,
			data_size,
			 word);

		buffer[len++]=',';

		len+= display_Ev(
			code, 
			buffer+len,
			 address_size,
			data_size,
			 word,
			 override_seg );
	}
	else
	{
		len= display_Ev(
			code, 
			buffer,
			 address_size,
			data_size,
			 word,
			 override_seg );

		buffer[len++]=',';

		len+= display_Rv(
			(code[1]>>3)&7, 
			buffer+len,
			data_size,
			 word);

	}

	return len;
}

char *addr16_base_index_display[]={
	"BX + SI","BX + DI","BP + SI","BP + DI",
	"SI", "DI","BP","BX" } ;  

char *reg16_name[]={	"AX",    "CX",    "DX",    "BX",  "SP",    "BP",    "SI",    "DI"};

char *reg8_name[]={	"AL",   "CL",    "DL",    "BL",    "AH",    "CH",    "DH",    "BH"};
int Mv_display16( unsigned char  *code, char* buffer)
{
     int len=0;
	 char disp8;
	 short disp16;


	 len=sprintf(buffer,"[%s",addr16_base_index_display[code[1]&7]);
     switch(code[1]>>6)
     {

      case 0:
          if((code[1]&7)==6)
	 len=sprintf(buffer,"[%04x",*(unsigned short*)(code+2));
		  break;

      case 1:
          disp8=code[2];

		  if (disp8>0)
			   len+=sprintf(buffer+len,"+%02x",disp8);
		  else
			   len+=sprintf(buffer+len,"-%02x",-disp8);
		  break;
      case 2:
          disp16=*(short*)(code+2);

		  if (disp16>0)
			   len+=sprintf(buffer+len,"+%04x",disp16);
		  else
			   len+=sprintf(buffer+len,"-%04x",-disp16);
		  break;
      case 3:

          return 0;            // reportr illegal instruction
     }
	 buffer[len++]=']';
     return len;
}

int sib_display( unsigned char  *code, char* buffer)
{
	int len;
    //
	// mm=0, sib.base==5. disp32[index]
	//
	if(0==(code[1]>>6)&&5==(code[2]&7))
	{
		unsigned int disp32=*(unsigned int*)(code+3);

		//
		// scale==0? don't display *1
		//
		len=(code[2]>>6)?sprintf(buffer,"[E%s*%d+%08X",reg16_name[(code[2]>>3)&7],1<<((code[2]>>6)&7),disp32):
			sprintf(buffer,"[E%s+%08X",reg16_name[(code[2]>>3)&7],disp32);
	}
	else if(((code[2]>>3)&7)==4)   // no index.
	{
		len=sprintf(buffer,"[E%s",reg16_name[code[2]&7]);
	}
	else
	{
		//
		// scale==0? don't display *1
		//
		len=(code[2]>>6)?sprintf(buffer,"[E%s+E%s*%d",reg16_name[code[2]&7],reg16_name[(code[2]>>3)&7],1<<((code[2]>>6)&7)):
		sprintf(buffer,"[E%s+E%s",reg16_name[code[2]&7],reg16_name[(code[2]>>3)&7]);
	}
	return len;
}
int Mv_display32( unsigned char  *code, char* buffer)
{
     int len=0;
	 char  disp8;
	 int disp32;
	int sib;
	 //
	 //sib?
	 //
	 if(sib=(4==(code[1]&7)))
		len =sib_display(code,buffer);
	 else
	    len=sprintf(buffer,"[E%s",reg16_name[code[1]&7]);


     switch((code[1]>>6)&3)
     {

      case 0:
          if((code[1]&7)==5)
		 len=sprintf(buffer,"[%08x",*(unsigned int*)(code+2));//[disp32]
		  break;

      case 1:
		  disp8=sib?code[3]:code[2];

		  if (disp8>0)
			   len+=sprintf(buffer+len,"+%02x",disp8);
		  else
			   len+=sprintf(buffer+len,"-%02x",-disp8);
		  break;
      case 2:
		  disp32=sib?*(int*)(code+3):*(int*)(code+2);

		  if (disp32>0)
			   len+=sprintf(buffer+len,"+%08x",disp32);
		  else
			   len+=sprintf(buffer+len,"-%08x",-disp32);
		  break;
      case 3:

          return 0;            // reportr illegal instruction
     }
	 buffer[len++]=']';
	 return len;
}



int display_R_sse(
	int reg,
	char * buffer,
	bool xmm)
{
	int len;

	len = sprintf(buffer,xmm?"xmm%d":"mm%d",reg);
	return len;

}




int display_R_RorM_sse(
	unsigned char  *code, 
	char * buffer,
	int address_size,
	bool dir,
	bool xmm,
	int override_seg )
{
	int len;
	//
	//dir=true
	//
	if (dir)
	{
		len= display_R_sse(
			(code[1]>>3)&7, 
			buffer,
			xmm);

		buffer[len++]=',';

		len+= display_RorM_sse(
			code, 
			buffer+len,
			address_size,
			 override_seg,
			 xmm);
	}
	else
	{
		len= display_RorM_sse(
			code, 
			buffer,
			address_size,
			override_seg,
			 xmm);

		buffer[len++]=',';

		len+= display_R_sse(
			(code[1]>>3)&7, 
			buffer+len,
			xmm);
	}

	return len;
}


int display_RorM_sse(
	unsigned char  *code, 
	char * buffer,
	int address_size,
	int override_seg,
	bool xmm)
{
	int len=0;

	//
	// if is a memory
	//
	if ( ((code[1]>>6)&3)!=3)
	
		len= display_Mv(
			code, 
			buffer,
			 address_size,
			 -1,
			 override_seg );
	else
	
		//
		//it is a register
		//

		len= display_R_sse(
			code[1]&7, 
			buffer,
			xmm);
			
	
	return len;
}