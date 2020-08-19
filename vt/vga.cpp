#include  <windows.h>
#include  <assert.h>
#include "vga.h"
#include "log.h"
//
// text mode: 25X80: use 16 color 640X400 dib
// text mode: 25X40: use 16 color 320X400 dib
//   graphics mode: use  dib equal to the required mode

struct vga_states vga_state={5};

//
// create bitmap for  25X80 text mode
// bitmap dim: 400x640
//

unsigned char *pchBits;
HBITMAP hbmp,hOldbmp;
HDC hScrDC;
extern HWND hWnd;
RECT rect_to_redraw;
void ErrorMsg() ;

bool  create_bitmap_16color(HDC hDC)
{
	union
	{
	BITMAPINFO i;
	char y[sizeof(BITMAPINFO)+255*sizeof(RGBQUAD)];
	};

	ZeroMemory( &i, sizeof(BITMAPINFO) );
	i.bmiHeader.biWidth=640;     // 80 cols x8
	i.bmiHeader.biHeight=-481;   //
	i.bmiHeader.biPlanes=1;      
	i.bmiHeader.biBitCount=8; 
	i.bmiHeader.biSizeImage=0;
	i.bmiHeader.biSize=sizeof(BITMAPINFOHEADER);
	i.bmiHeader.biClrUsed= 0;
	i.bmiHeader.biClrImportant= 0;
	i.bmiHeader.biCompression=BI_RGB;

	//PALETTEENTRY pal[256];
	//GetSystemPaletteEntries(hDC,0,256,pal);

	unsigned int j;
	for(j=0;j<8;j++)
	{
		unsigned char c=0xc0;
		i.bmiColors[j].rgbBlue=(j&0x1)?c:0;
		i.bmiColors[j].rgbGreen=(j&0x2)?c:0;
		i.bmiColors[j].rgbRed=(j&0x4)?c:0;
	}

	for(;j<16;j++)
	{
		unsigned char c=0xff;
		i.bmiColors[j].rgbBlue=(j&0x1)?c:0;
		i.bmiColors[j].rgbGreen=(j&0x2)?c:0;
		i.bmiColors[j].rgbRed=(j&0x4)?c:0;
	}

	for(;j<256;j++)
	{
		unsigned char c=j>>3;
		i.bmiColors[j].rgbBlue=(j&0x1)?c:0;
		i.bmiColors[j].rgbGreen=(j&0x2)?c:0;
		i.bmiColors[j].rgbRed=(j&0x4)?c:0;
	}


	hbmp= CreateDIBSection( hDC,
							&i,
							DIB_RGB_COLORS,
							(void **)&pchBits,
							NULL,
							0 );
	if(hbmp!=NULL)
	{
		hScrDC=CreateCompatibleDC(hDC);
		if(hScrDC!=NULL)
		{
			hOldbmp=(HBITMAP)SelectObject(hScrDC,hbmp);

			return TRUE;
		}
		else
			DeleteObject(hbmp);
	}
	ErrorMsg() ;
	return FALSE;
}

#define G16_MAX_ADDR (481*80)

void add_redraw_area(int left, int top,int right,int bottom)
{
	if(rect_to_redraw.left>left)    rect_to_redraw.left=left;
	if(rect_to_redraw.top>top)      rect_to_redraw.top=top;
	if(rect_to_redraw.right<right)  rect_to_redraw.right=right;
	if(rect_to_redraw.bottom<bottom)rect_to_redraw.bottom=bottom;
}

void add_redraw_rect(RECT *rect)
{
	add_redraw_area(rect->left,rect->top,rect->right,rect->bottom);
}
void do_vga()
{

	if(rect_to_redraw.right>rect_to_redraw.left
			&&rect_to_redraw.bottom>rect_to_redraw.top)
	{
	    HDC hDC=GetDC(hWnd);
		BitBlt(hDC,rect_to_redraw.left,rect_to_redraw.top,
			rect_to_redraw.right-rect_to_redraw.left,rect_to_redraw.bottom-rect_to_redraw.top,
			hScrDC,
			rect_to_redraw.left,rect_to_redraw.top,
			SRCCOPY);

		ReleaseDC(hWnd,hDC);
		rect_to_redraw.left=640;
		rect_to_redraw.top=480;
		rect_to_redraw.right=
		rect_to_redraw.bottom=0;
	}
}
unsigned long starting_address;
extern unsigned char dots[][0x10];

#define TEXT_AREA_OFFSET 40*640

void   vga_putc(char c,int row,int col,char attr)
{
	for(int i=0;i<16;i++)
	{
		char dot=dots[(unsigned char)c][i];
		char bkcolor=(((unsigned char )attr)>>4);
		char color=((unsigned char )attr&0xf);

		for(int j=0;j<8;j++)
		{
			char bit=dot&(0x80>>j);

			pchBits[row*640*16+i*640+col*8+j+TEXT_AREA_OFFSET]=bit?color:bkcolor;
		}
	}
	add_redraw_area(col*8,row*16+40,col*8+8,row*16+40+16);
}
unsigned char text[0x8000];
unsigned char latch[8];

static void g16_wr (unsigned long offset, unsigned char dat)
{
	int i,j;
	// byte offset
	if(offset>=G16_MAX_ADDR) return;
	offset<<=3;
   switch(wr_mode())
   {
   case 0:// rotate logic op,mask
	   j=0x80;
	   //the host data is first rotated 
	   dat=(dat>>rotate_count())|(dat<<(8-rotate_count()));
	   for(i=0;i<8;i++,j>>=1)
	   {   
		   char x;

	   		//latch[i]=pchBits[offset+i];
		   // the Bit Mask field is used to select and 
		   // 0 if come from the latch register
		   if((bit_mask()&j) ==0)
			   x=latch[i];

		   //1 if  bits come from the resulting data 
		   else
		   {
				x=(dat&j)?0xf:0;
			   // the Enable Set/Reset mechanism selects data from rotatedhost data or the Set/Reset field.
			   x &=~en_setreset_mask();
			   x |=(setreset_mask()&en_setreset_mask());
			   //Logical Operation is performed on the resulting data and the data in the latch register
			   if(logic_op()==LOP_AND)
				   x&=latch[i] ;
			   else if(logic_op()==LOP_OR)
				   x|=latch[i];
			   else if(logic_op()==LOP_XOR)
				   x^=latch[i];
		   }

		   //only the bit planes enabled by the Memory Plane Write Enable field are written to memory
		   pchBits[offset+i]&=~plane_mask();
		   pchBits[offset+i]|=x&plane_mask();
	   }
	   break;
   case 1:// from latch,affected only by the Memory Plane Write Enable field
	   for(i=0;i<8;i++)
	   {

		   pchBits[offset+i]&=~plane_mask();
		   pchBits[offset+i]|=(latch[i]&plane_mask());

	   }
	   break;
   case 2:// 
	   j=0x80;

	   for(i=0;i<8;i++,j>>=1)
	   {   
		   char x;

	   		//latch[i]=pchBits[offset+i];
		   // the Bit Mask field is used to select and 
		   // 0 if come from the latch register
		   if((bit_mask()&j) ==0)
			   x=latch[i];

		   //1 if  bits come from the resulting data 
		   else
		   {
			   //the bits 3-0 of the host data are replicated across all 8 bits of their respective planes
				x=dat&0xf;
		
			   //Logical Operation is performed on the resulting data and the data in the latch register
			   if(logic_op()==LOP_AND)
				   x&=latch[i] ;
			   else if(logic_op()==LOP_OR)
				   x|=latch[i];
			   else if(logic_op()==LOP_XOR)
				   x^=latch[i];
		   }

		   //only the bit planes enabled by the Memory Plane Write Enable field are written to memory
		   pchBits[offset+i]&=~plane_mask();
		   pchBits[offset+i]|=x&plane_mask();
		   //pchBits[offset+i]=x;
	   }
	   break;
   case 3://host data  rotate,and,
	   j=0x80;
	   //the host data is first rotated 
	   dat=(dat>>rotate_count())|(dat<<(8-rotate_count()));
	   //logical ANDed with the value of the Bit Mask field
	   dat &=bit_mask();
	   for(i=0;i<8;i++,j>>=1)
	   {   
		   char x;

	   		//latch[i]=pchBits[offset+i];
		   // The resulting value is used in the same way as the Bit Mask field
		   // to select bits come from the expansion of the Set/Reset field and from the latch register
		   // 0 if come from the latch register
		   if((dat&j)==0) 
			   x=latch[i];

		   //1 if  bits come from the expansion of the Set/Reset field 
		   else
		   // the Set/Reset fieldas if the Enable Set/Reset field were set to 1111b
			   x =setreset_mask();
		   

		   //only the bit planes enabled by the Memory Plane Write Enable field are written to memory
		   pchBits[offset+i]&=~plane_mask();
		   pchBits[offset+i]|=x&plane_mask();
	   }
	   break;
   }
   int y=offset/640;
   int x= offset %640;
   add_redraw_area(x,y,x+8,y+1);
}
unsigned char  g16_rd( unsigned long offset)
{
	int i,j;
	unsigned char dat=0;
	// byte offset
	if(offset >=G16_MAX_ADDR) return 0xff;
	offset<<=3;

   j=0x80;
   for(i=0;i<8;i++,j>>=1)
   {
		// latch 24 bits
	   latch[i]= pchBits[offset+i];
		if(rd_mode0())
		{

		   if(latch[i]&(1<<rd_plane())) dat |=j;
		}
		else
		{

		   if((latch[i]&color_dontcare())==(color_comp()&color_dontcare())) dat |=j;
	   }


	}
   return dat;
}
void vram_write_word( unsigned long offset, unsigned short dat)
{

	assert(offset<sizeof(text)-1 ); // must be within 32k space
	*(unsigned short *)(text+offset)=dat;
	if(!in_graphics_mode()&& offset>=starting_address )
	{
			int page,row,col;
			unsigned long off;

			offset &=0xfffe;
			off=offset-starting_address;
			page=off/(80*25*2);
			row=(off/80/2)%25;
			col=(off%160)/2;

			vga_putc((unsigned char )dat,row,col,(unsigned char )(dat>>8));
	}
}


void vram_write_byte( unsigned long offset, unsigned char dat)
{

	assert(offset<sizeof(text) ); // must be within 32k space
	*(unsigned char *)(text+offset)=dat;
	if(!in_graphics_mode()&& offset>=starting_address )
	{
		int page,row,col;
		unsigned long off;
		offset &=0xfffe;
		off=offset-starting_address;

		page=off/(80*25*2);
		row=(off/80/2)%25;
		col=(off%160)/2;

		vga_putc(*(unsigned char *)(text+off),row,col,*(unsigned char *)(text+off+1));
	}
}
 unsigned long vram_read_dword( unsigned long offset)
{
	 unsigned long dat;

	assert(offset<sizeof(text)-3 ); // must be within 32k space
	dat=*(unsigned long *)(text+offset);
	

	return dat;
}


 unsigned short vram_read_word( unsigned long offset)
{
	 unsigned short dat;

	assert(offset<sizeof(text)-1 ); // must be within 32k space
	dat=*(unsigned short *)(text+offset);


	return dat;
}

 unsigned char vram_read_byte( unsigned long offset)
{
	 unsigned char dat;

	assert(offset<sizeof(text) ); // must be within 32k space
	dat=*(unsigned char *)(text+offset);
	
	return dat;
}

void vram_write_word1( unsigned long address, unsigned short dat)
{

	if(in_graphics_mode())
	{
		g16_wr(address-0xa0000,(char)dat);
		g16_wr(address-0xa0000+1,(char)(dat>>8));
	}
	else if(address>=0xb8000)
	{
		vram_write_word(address-0xb8000,dat);
	}
	else if(address<0xa0000+G16_MAX_ADDR)
	{
		g16_wr(address-0xa0000,(char)dat);
		g16_wr(address-0xa0000+1,(char)(dat>>8));
	}
	//else
	//log("vga write:%x, %x(%c) %x\n",address,dat,(char)dat,wr_mode());
}


void vram_write_byte1( unsigned long address, unsigned char dat)
{
	//unsigned long base,size;
	//
	//switch(mmap_sel())
	//{
	//case 0:base=0xa0000;size=0x20000;break;
	//case 1:base=0xa0000;size=0x10000;break;
	//case 2:base=0xa0000;size=0x8000;break;
	//case 3:base=0xb8000;size=0x8000;break;
	//}
	//if(address<base||address-base>size) return;

	if(in_graphics_mode())
	{
		g16_wr(address-0xa0000,dat);
	}
	else if(address>=0xb8000)
	{
		vram_write_byte(address-0xb8000,dat);
	}
	else if(address<0xa0000+G16_MAX_ADDR)
	{

		g16_wr(address-0xa0000,(char)dat);
	}
	//log("vga write:%x, %x(%c) %x\n",address,dat,(char)dat,wr_mode());
}
 unsigned long vram_read_dword1( unsigned long address)
{
	union
	{
		char c[4];
		short s[2];
		int n;
	};
	if(in_graphics_mode())
	{
		for(int i=0;i<4;i++) c[i]= g16_rd(address+i-0xa0000);
	}
	else if(address>=0xb8000)
	{
		s[0]=vram_read_word(address-0xb8000);
		s[1]=vram_read_word(address-0xb8000+2);
	}
	//log("vga read:%x, %x\n",address,n);
	return n;
}


 unsigned short vram_read_word1( unsigned long address)
{
	union
	{
		char c[2];
		 unsigned short  n;
	};
	if(in_graphics_mode())
	{
		for(int i=0;i<2;i++) c[i]= g16_rd(address+i-0xa0000);
	}
	else if(address>=0xb8000)
		n=vram_read_word(address-0xb8000);
	else 
	{
		for(int i=0;i<2;i++) c[i]= g16_rd(address+i-0xa0000);
	}
	//log("vga read:%x, %x\n",address,n);
	return n;
}

 unsigned char vram_read_byte1( unsigned long address)
{
	unsigned char n= in_graphics_mode()?g16_rd(address-0xa0000):
		  (address>=0xb8000)?vram_read_byte(address-0xb8000):
		  g16_rd(address-0xa0000);

	//log("vga read:%x, %x\n",address,n);
	return n;
}



 void write_vga_reg(unsigned char reg_no, char dat)
{

	if(reg_no==VGA_ATTR_INDEX) 
	{

		//		Port 3C0h is special in that it is both address and data-write register.
		//An internal flip-flop remembers whether it is currently acting as address or
		// data register. Data reads happen from port 3C1h.
		if(vga_state.flip_flop_3c0==FF_3C0_DATA)
		{
			char no=vga_state.reg_vga[VGA_ATTR_INDEX];
			
			if(no<NUM_ATTR_REG) vga_state.reg_attrs[no]=dat;
		}
		else
			vga_state.reg_vga[VGA_ATTR_INDEX]=dat;

		//log("attr: %x,%x,%x\n",vga_state.reg_vga[VGA_ATTR_INDEX],dat,vga_state.flip_flop_3c0);
		switch_3c0_flip_flop();
	}
	else 	if(reg_no==VGA_SEQ_DATA) 
	{
		char no=vga_state.reg_vga[VGA_SEQ_INDEX];
		if(no<NUM_SEQ_REG)vga_state.reg_seq[no]=dat;
	}  
	else 	if(reg_no==VGA_GRAPHICS_DATA) 
	{
		char no=vga_state.reg_vga[VGA_GRAPHICS_INDEX];
		if(no<NUM_SEQ_REG)vga_state.reg_grapgics[no]=dat;
	}

	else 	if(reg_no<NUM_VGA_REG) vga_state.reg_vga[reg_no]=dat;

}

#define BYTES_PER_LINE 640
#define LINES_PER_ROW  16

void sync_starting_address()
{
	unsigned long new_address=((vga_state.reg_crtc[CRTC_START_ADDRESS_HIGH]<<8)|vga_state.reg_crtc[CRTC_START_ADDRESS_LOW]);
	//
	//in text mode:
	//
	log("vga offset:%x, %x,3d8:%x,3c5:%x en:%x\n",new_address,starting_address/2,
		vga_state.reg_6845[8],
		vga_state.reg_attrs[ATTR_MODE_CONTROL],
		vga_state.reg_vga[VGA_ENABLE]);
	
	if(new_address!=starting_address/2) 
	if(new_address<sizeof(text)&&!in_graphics_mode())
	{
		int rows_to_scroll=(new_address-starting_address/2)/80;///80 *LINES_PER_ROW*BYTES_PER_LINE;
		//
		// scroll up only for the time being
		//
		if(rows_to_scroll<25&&rows_to_scroll>0)
		memcpy(pchBits+TEXT_AREA_OFFSET,
			pchBits+rows_to_scroll*LINES_PER_ROW*BYTES_PER_LINE+TEXT_AREA_OFFSET,
			(25-rows_to_scroll)*LINES_PER_ROW*BYTES_PER_LINE);
		else 
			rows_to_scroll=25;


		//
		// render  newlines
		//
		for(int i=25-rows_to_scroll;i<25;i++)
		{
			for(int j=0;j<80;j++)
			{
				unsigned int offset=(new_address+i*80+j);

				if(offset<0x4000)
				{
					short c=((short *)text)[offset];// wrap around
					vga_putc((char)c,i,j,c>>8);
				}
				else
				{
					log0("crtc starting address too high:%x (%d,%d)\n",new_address,i,j);
					break;
				}
			}
		}

		//
		// sync screen,whole screen should be redrawn
		//
		add_redraw_area(0,0,640,480);
		starting_address=new_address<<1;
	}
}