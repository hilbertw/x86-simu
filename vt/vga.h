#ifndef __vga__h__
#define __vga__h__ 

//
// register set  1: 6845 
//

	//3B0/3D0/3B2/3D2/3B6/3D6   port address decodes to 3B4/3D4
	//3B1/3D1/3B3/3D3/3B7/3D7  port address decodes to 3B5/3D5
	//3B4/3D4  6845 #define SEQUENCER_register, selects which register [0-11h]
	//3B5/3D5  6845 data register [0-11h] selected by port 3B4/3D4,
	//	 registers 0C-0F may be read.  If a read occurs
	//	 without the adapter installed, FFh is returned.
	//3B8/3D8  6845 mode control register
	//3B9/3D9  color select register on color adapter
	//3BA/3DA  status register (read only)
	//3BB/3DB  light pen strobe reset

#define CRT6845_INDEX  0
#define CRT6845_DATA   1
#define CRT6845_MODE   8
#define CRT6845_COLOR  9
#define CRT6845_STATUS 10
#define NUM_6845_REG     0x80

//
// register set 2: vga  crtc registers
//
//
// The CRT Controller (CRTC) Registers  
//
#define CRTC_HORZ_TOTAL           0x00  // Horizontal Total Register 
#define CRTC_HORZ_DISPLAY         0x01  // End Horizontal Display Register 
#define CRTC_START_HORZ_BLANKING  0x02  // Start Horizontal Blanking Register 
#define CRTC_END_HORZ_BLANKING    0x03  // End Horizontal Blanking Register 
#define CRTC_START_HORZ_RETRACE   0x04  // Start Horizontal Retrace Register 
#define CRTC_END_HORZ_RETRACE     0x05  // End Horizontal Retrace Register 
#define CRTC_VERTICAL_TOTAL       0x06  // Vertical Total Register 
#define CRTC_OVERFLOW             0x07  // Overflow Register 
#define CRTC_PRESET_ROW_SCAN      0x08  // Preset Row Scan Register 
#define CRTC_MAX_SCAN_LINE        0x09  // Maximum Scan Line Register 
#define CRTC_CURSOR_START         0x0A  // Cursor Start Register 
#define CRTC_CURSOR_END           0x0B  // Cursor End Register 
#define CRTC_START_ADDRESS_HIGH   0x0C  // Start Address High Register 
#define CRTC_START_ADDRESS_LOW    0x0D  // Start Address Low Register 
#define CRTC_CURSOR_LOCATION_HIGH 0x0E  // Cursor Location High Register 
#define CRTC_CURSOR_LOCATION_LOW  0x0F  // Cursor Location Low Register 
#define CRTC_VERT_RETRACE_START   0x10  // Vertical Retrace Start Register 
#define CRTC_VERT_RETRACE_END     0x11  // Vertical Retrace End Register 
#define CRTC_VERT_DISPLAY_END     0x12  // Vertical Display End Register 
#define CRTC_OFFSET               0x13  // Offset Register 
#define CRTC_UNDERLINE_LOCATION   0x14  // Underline Location Register 
#define CRTC_START                0x15  // Start Vertical Blanking Register 
#define CRTC_END_VERT_BLANKING    0x16  // End Vertical Blanking 
#define CRTC_MODE_CONTROL         0x17  // CRTC Mode Control Register 
#define CRTC_LINE_COMPARE         0x18  // Line Compare Register 
#define CRTC_36                   0x36
#define CRTC_2C                   0x2c
#define CRTC_58                   0x58
#define CRTC_38                   0x38
#define NUM_CRTC_REG              0x80  // Line Compare Register 

//
// register set 2: vga  Graphics registers
//
//
// The Graphics Registers:
// The Address Register is located at port 3CEh 
// And the Data Register is located at port 3CFh. 
//
#define GRAPHICS_SETRESET        0x00  // Set/Reset Register 
#define GRAPHICS_EN_SETRESET     0x01  // Enable Set/Reset Register 
#define GRAPHICS_COLOR_COMPARE   0x02  // Color Compare Register 
#define GRAPHICS_DATA_ROTATE     0x03  // Data Rotate Register 
#define GRAPHICS_READ_MAP_SELECT 0x04  // Read Map Select Register 
#define GRAPHICS_MODE            0x05  // Graphics Mode Register 
#define GRAPHICS_MISC            0x06 // Miscellaneous Graphics Register 
#define GRAPHICS_COLOR_DONTCARE  0x07 // Color Don't Care Register 
#define GRAPHICS_BIT_MASK        0x08 // Bit Mask Register 
#define NUM_GRAPHICS_REG         0x09 // number of graphics reg


#define LOP_NOP 0
#define LOP_AND 1
#define LOP_OR  2
#define LOP_XOR 3

//
// register set 3: vga  Attribute registers
//
#define ATTR_PALETTE_FIRST         0 //1st Palette Register
#define ATTR_PALETTE_LAST       0x0f //last Palette Register
#define ATTR_MODE_CONTROL       0x10 //Attribute Mode Control Register
#define ATTR_OVERSCAN_COLOR     0x11 //Overscan Color Register  
#define ATTR_COLOR_PLANE_EN     0x12 //Color Plane Enable Register (
#define ATTR_HORZ_PIXEL_PANNING 0x13 //Horizontal Pixel Panning Register  
#define ATTR_COLOR_SEL          0x14 //Color Select Register 
#define NUM_ATTR_REG            0x15 // number of attribute reg
//
// register set 4: vga  Sequencer registers
//

#define SEQ_RESET        0x00  // Reset Register 
#define SEQ_CLK_MODE     0x01  // Clocking Mode Register 
#define SEQ_MAP_MASK     0x02  // Map Mask Register 
#define SEQ_CHAR_MAP_SEL 0x03  // Character Map Select Register 
#define SEQ_MEM_MODE     0x04  // Sequencer Memory Mode Register 
#define NUM_SEQ_REG      0x15  // number of Sequencer reg
//
// register set 5: vga  vga interface registers
//

#define VGA_ATTR_INDEX       0  // 3C0h -- Attribute Address/Data Register 
#define VGA_ATTR_DATA        1  // 3C1h -- Attribute Data Read Register 
#define VGA_INPUT_STATUS0    2  // 3C2h Read -- Input Status #0 Register  
#define VGA_MISC_OUTPUT      2  // 3C2h Write -- Miscellaneous Output Register 
#define VGA_ENABLE           3  // 3C3h --  Video subsystem enable
#define VGA_SEQ_INDEX        4  // 3C4h -- Sequencer Address Register 
#define VGA_SEQ_DATA         5  // 3C5h -- Sequencer Data Register 
#define VGA_DAC_STATE        7  // 3C7h Read -- DAC State Register 
#define VGA_DAC_ADDR_RD      7  // 3C7h Write -- DAC Address Read Mode Register 
#define VGA_DAC_ADDR_WR      8  // 3C8h -- DAC Address Write Mode Register 
#define VGA_DAC_DATA         9  // 3C9h -- DAC Data Register 
#define VGA_FEATURE_CONTROL  0xa// 3CAh Read -- Feature Control Register 
#define VGA_MISC             0xc// 3CCh Read -- Miscellaneous Output Register 
#define VGA_GRAPHICS_INDEX   0xe// 3ce
#define VGA_GRAPHICS_DATA    0xf// 3cf
#define NUM_VGA_REG          0x10// number of vga reg

struct vga_states
{
 char input_status1;
 unsigned char  flip_flop_3c0;
 unsigned char reg_6845[NUM_6845_REG];
 unsigned char reg_crtc[NUM_CRTC_REG];
 unsigned char reg_grapgics[NUM_GRAPHICS_REG];
 unsigned char reg_attrs[NUM_ATTR_REG];
 unsigned char reg_seq[NUM_SEQ_REG];
 unsigned char reg_vga[NUM_VGA_REG];

};

extern  struct vga_states vga_state;
  
#define FF_3C0_ADDRESS 0
#define FF_3C0_DATA    1

__inline void switch_3c0_flip_flop() {vga_state.flip_flop_3c0^=1;}
__inline void reset_3c0_flip_flop() {vga_state.flip_flop_3c0= FF_3C0_ADDRESS;}
//
// crtc,reg 0c/0d is set
//
void sync_starting_address();

// interface for i/o
//
void write_6845_reg(unsigned char no, char dat);


//6845 - Port 3DA Status Register
//
//	|7|6|5|4|3|2|1|0|  3DA Status Register
//	 | | | | | | | `---- 1 = display enable, RAM access is OK
//	 | | | | | | `----- 1 = light pen trigger set
//	 | | | | | `------ 0 = light pen on, 1 = light pen off
//	 | | | | `------- 1 = vertical retrace, RAM access OK for next 1.25ms
//	 `-------------- unused

__inline char read_6845_reg(char no )
{
	char dat;
	if(no <=7)
	{
		no&=1;
		if(no)
		{
			if(vga_state.reg_6845[0]==CRTC_36) 	
			{
				dat=vga_state.reg_crtc[ CRTC_36];
				vga_state.reg_crtc[ CRTC_36]^=0xc;
			}
			else if(vga_state.reg_6845[0]==CRTC_2C) 	dat=0x20;
			else if(vga_state.reg_6845[0]==CRTC_58) 	dat=0x0;
			else if(vga_state.reg_6845[0]==CRTC_38) 	dat=0x0;
			//else if(vga_state.reg_6845[0]<(unsigned )NUM_CRTC_REG) dat=vga_state.reg_crtc[ vga_state.reg_6845[0]];
			else dat=(char)0;
		}
		else dat=vga_state.reg_6845[0];
	}
	else if(no==CRT6845_STATUS) 
	{
		reset_3c0_flip_flop();
		//
		// VRetrace   DD are always 1 
		//
		dat=vga_state.input_status1;
		vga_state.input_status1^=9;
	}
	else dat=(no<(unsigned )NUM_6845_REG)?vga_state.reg_6845[no]:0xff;

	return dat;
}



__inline  void write_6845_reg(unsigned char no, char dat)
{
	if(no <=7)
	{
		no&=1;
		if(no)
		{
			if(vga_state.reg_6845[0]<(unsigned )NUM_CRTC_REG) vga_state.reg_crtc[ vga_state.reg_6845[0]]=dat;
			if(//vga_state.reg_6845[0]==CRTC_START_ADDRESS_HIGH||
				vga_state.reg_6845[0]==CRTC_START_ADDRESS_LOW)
			sync_starting_address();
		}
		else vga_state.reg_6845[0]=dat;
	}
	else if(no<NUM_6845_REG) 
	{
		if(no==CRT6845_STATUS) vga_state.reg_vga[VGA_FEATURE_CONTROL]=dat;// 3ca
		vga_state.reg_6845[no]=dat;
	}
}


void write_vga_reg(unsigned char no, char dat);
__inline char read_vga_reg(char no )
{
	char dat;
	if(no==VGA_ATTR_DATA) 
	{
		char no=vga_state.reg_vga[VGA_ATTR_INDEX]&0xf;
		dat=vga_state.reg_attrs[no];
	}
	else 	if(no==VGA_SEQ_DATA) 
	{
		char no=vga_state.reg_vga[VGA_SEQ_INDEX];
		//if (no==SEQ_CLK_MODE ) dat=vga_state.reg_seq[SEQ_CLK_MODE]&~1;
		if(no<NUM_SEQ_REG) dat=vga_state.reg_seq[no];
	}  
	else 	if(no==VGA_GRAPHICS_DATA) 
	{
		char no=vga_state.reg_vga[VGA_GRAPHICS_INDEX];
		if(no<NUM_SEQ_REG) dat=vga_state.reg_grapgics[no];
	}
	else if(no==VGA_DAC_STATE) dat=0;         // alwats ready for read
	else if(no==VGA_INPUT_STATUS0) dat= 0x10;             // switch high.
	//else if(no==VGA_FEATURE_CONTROL) dat= vga_state.reg_6845[CRT6845_STATUS];             // 3CA, write at 3DA
	else if(no==VGA_MISC) dat=vga_state.reg_vga[VGA_MISC_OUTPUT]; // read at 3cc, write at 3c2
	else if(no<NUM_VGA_REG) dat=vga_state.reg_vga[no];

	return dat;
}

void write_vram(unsigned int offset,char * buffer, int bytes);
void read_vram(unsigned int offset,char * buffer, int bytes);

// crt 6845 claims 3b0~3bf/3d0~3df port address space
//3B0-3BB Monochrome Monitor Adapter
//3D0-3DC Color Graphics Adapter (mapped similarly)


//
// crt 6845 registers
//
//6845 - Port 3B8 (Monochrome)
//
//	|7|6|5|4|3|2|1|0|  3B8 CRT Control Port
//	 | | | | | | | `---- 1 = 80x25 text
//	 | | | | | `------- unused
//	 | | | | `-------- 1 = enable video signal
//	 | | | `--------- unused
//	 | | `---------- 1 = blinking on
//	 `------------- unused
//
//6845 - Port 3D8 (Color)
//
//	|7|6|5|4|3|2|1|0|  3D8 Mode Select Register
//	 | | | | | | | `---- 1 = 80x25 text, 0 = 40x25 text
//	 | | | | | | `----- 1 = 320x200 graphics, 0 = text
//	 | | | | | `------ 1 = B/W, 0 = color
//	 | | | | `------- 1 = enable video signal
//	 | | | `-------- 1 = 640x200 B/W graphics
//	 | | `--------- 1 = blink, 0 = no blink
//	 `------------ unused
__inline char in_graphics_mode() {return ((vga_state.reg_attrs[ATTR_MODE_CONTROL]&1)==1);}
__inline bool screen_enabled() {return ((vga_state.reg_seq[SEQ_CLK_MODE]&0x20)!=0);}
__inline char g256_mode() {return ((vga_state.reg_attrs[ATTR_MODE_CONTROL]&0x41)==0x41);}
__inline char g16_mode() {return ((vga_state.reg_attrs[ATTR_MODE_CONTROL]&0x41)==0x1);}
__inline bool video_enabled() {return ((vga_state.reg_vga[VGA_ENABLE]&0x1)!=0);}
//__inline bool in_graphics_mode(){ return (vga_state.reg_attrs[GRAPHICS_MISC]&1)==1;}
//
//6845 - Port 3D9 Color Text Modes
//
//	|7|6|5|4|3|2|1|0|  3D9 Color Select Register (3B9 not used)
//	 | | | | | `-------- screen/border RGB
//	 | | | | `--------- select intensity setting
//	 | | | `---------- background intensity
//	 `--------------- unused
//
//6845 - Port 3D9 Color Graphics Modes
//
//	|7|6|5|4|3|2|1|0|  3D9 Color Select Register (3B9 not used)
//	 | | | | | `-------- RGB for background
//	 | | | | `--------- intensity
//	 | | | `---------- unused
//	 | | `----------- 1 = palette 1, 0=palette 0 (see below)
//	 `-------------- unused
//
//	  Palette 0 = green, red, brown
//	  Palette 1 = cyan, magenta, white
__inline char bk_color() {return vga_state.reg_6845[CRT6845_COLOR]&0x7;}


__inline bool gmode_16(){ return (vga_state.reg_attrs[ATTR_MODE_CONTROL]&0x41)==1;}
__inline bool gmode_256(){ return (vga_state.reg_attrs[ATTR_MODE_CONTROL]&0x41)==0x41;}
__inline unsigned char wr_mode() {return vga_state.reg_grapgics[GRAPHICS_MODE]&3;}
__inline unsigned char rd_mode0() {return 0==(vga_state.reg_grapgics[GRAPHICS_MODE]&8);}
__inline unsigned char rd_plane() {return vga_state.reg_grapgics[GRAPHICS_READ_MAP_SELECT]&3;}
__inline unsigned char logic_op() {return 3&(vga_state.reg_grapgics[GRAPHICS_DATA_ROTATE]>>3);}
__inline unsigned char rotate_count() {return 7&vga_state.reg_grapgics[GRAPHICS_DATA_ROTATE];}
__inline unsigned char plane_mask() {return vga_state.reg_seq[SEQ_MAP_MASK];}
__inline unsigned char setreset_mask() {return vga_state.reg_grapgics[GRAPHICS_SETRESET];}
__inline unsigned char en_setreset_mask() {return vga_state.reg_grapgics[GRAPHICS_EN_SETRESET];}
__inline unsigned char color_comp() {return vga_state.reg_grapgics[GRAPHICS_COLOR_COMPARE]&0xf;}
__inline unsigned char color_dontcare() {return vga_state.reg_grapgics[GRAPHICS_COLOR_DONTCARE]&0xf;}
__inline unsigned char bit_mask() {return vga_state.reg_grapgics[GRAPHICS_BIT_MASK];}

__inline unsigned  char mmap_sel(){ return 3&(vga_state.reg_attrs[GRAPHICS_MISC]>>2);}
void vram_write_word( unsigned long offset, unsigned short dat);
void vram_write_byte( unsigned long offset, unsigned char dat);
void vram_write_dword( unsigned long offset, unsigned long dat);
unsigned long vram_read_dword( unsigned long offset);
unsigned short vram_read_word( unsigned long offset);
unsigned char vram_read_byte( unsigned long offset);

void vram_write_word1( unsigned long offset, unsigned short dat);
void vram_write_byte1( unsigned long offset, unsigned char dat);
void vram_write_dword1( unsigned long offset, unsigned long dat);
unsigned long vram_read_dword1( unsigned long offset);
unsigned short vram_read_word1( unsigned long offset);
unsigned char vram_read_byte1( unsigned long offset);


#define write_crtc_reg write_6845_reg
#define read_crtc_reg read_6845_reg
#endif // __vga__h__