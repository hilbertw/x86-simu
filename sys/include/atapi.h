#ifndef __atapi_h__
#define __atapi_h__

#pragma pack(push,1)

typedef struct inquery_data_def
{
	unsigned char type:5;//Reserved Peripheral Device Type
	unsigned char reserved1:3;//Reserved
	unsigned char reserved2:7;//Reserved
	unsigned char RMB:1;     // RMB 
	unsigned char ansi_er:3; // ANSI Version (00h)
	unsigned char ecma_ver:3;// ECMA Version 
	unsigned char iso_ver:2; // ISO Version
	unsigned char data_fmt:4; // Response Data Format
	unsigned char reserved3:4;// Reserved 
	unsigned char extra_bytes;// Additional Length (Number of bytes following this one)
	unsigned char reserved4[3];// Reserved
	unsigned char vendor_info[8];// Vendor Information
	unsigned char product_id[16];// Product Identification
	unsigned char rev_level[4]; // Product Revision Level
}INQUIRY_DATA;

//Peripheral Device Types
//00h Direct-access device (e.g., UHD Floppy disk)
//01h Sequential-access device (e.g., magnetic tape)
//02-03h Reserved
//04h Write-once device (e.g., WORM optical disk)
//05h CD-ROM device
//06h Reserved
//07h Optical memory device (e.g., Non-CD optical disks)
//08h-1Eh Reserved
//1Fh Unknown or no device type

typedef struct cmd_packet_def
{
	BYTE op_code;
	BYTE reserved;
	DWORD lba;
	BYTE reserved1;
	WORD blk_count;
} CMD_PACKET,*PCMD_PACKET;
#pragma pack(pop)

#define PACKET_CMD_TEST      0x00 //TEST UNIT READY Command
#define PACKET_CMD_FORMAT    0x04 //FORMAT UNIT Command
#define PACKET_CMD_INQUIRY   0x12//INQUIRY Command
#define PACKET_CMD_WRITEV    0x2e //WRITE AND VERIFY Command
#define PACKET_CMD_WRITE12   0xaa //WRITE (12) Command
#define PACKET_CMD_WRITE10   0x2a //WRITE (10) Command
#define PACKET_CMD_VERIFY    0x2f //VERIFY Command
#define PACKET_CMD_STARTSTOP 0x1b //START/STOP UNIT Command
#define PACKET_CMD_SEEK      0x2b //SEEK Command
#define PACKET_CMD_REQUEST   0x03 //REQUEST SENSE Command
#define PACKET_CMD_READ_FCAP 0x23 //READ FORMAT CAPACITIES Command
#define PACKET_CMD_READ_CAP  0x25 //READ CAPACITY Command
#define PACKET_CMD_READ12    0xa8 //READ (12) Command
#define PACKET_CMD_READ10    0x28 //READ (10) Command
#define PACKET_CMD_MED_REMOVAL 0x1e //PREVENT/ALLOW MEDIUM REMOVAL Command
#define PACKET_CMD_MODE_SENSE 0x5a //MODE SENSE Command
#define PACKET_CMD_MODE_SEL   0x55 //MODE SELECT Command
#define PACKET_CMD_LOAD_UNLOAD	0xA6//LOAD/UNLOAD CD		
#define PACKET_CMD_READ_SUB_CHANNEL	0x42//READ SUB-CHANNEL		
#define PACKET_CMD_READ_TOC	        0x43//READ TOC		
#define PACKET_CMD_READ_HEADER	    0x44//READ HEADER	
#define PACKET_CMD_PLAY_AUDIO	    0x45//PLAY AUDIO (10)		
#define PACKET_CMD_PLAY_AUDIO_MSF    0x47//PLAY AUDIO MSF	
#define PACKET_CMD_PAUSE_RESUME	 0x4B//PAUSE/RESUME		

#define PACKET_CMD_READ_CD_MSF   0xB9//READ CD MSF	
#define PACKET_CMD_SCAN          0xBA//SCAN		
#define PACKET_CMD_SET_CD_SPEED  0xBB//SET CD SPEED	
#define PACKET_CMD_PLAY_CD	     0xBC	//PLAY CD	
#define PACKET_CMD_MECHANISM_STATUS	0xBD//MECHANISM STATUS	
#define PACKET_CMD_READ_CD	        0xBE//READ CD	
#define PACKET_CMD_STOP_PLAY_SCAN   0x4E//STOP PLAY / SCAN




void atapi_do_packet_command(int controller_no);


//IDENTIFY PACKET DEVICE information word 0 defines command packet size
#define COMMAND_PACKET_WORDS 6



/*
 * definition of play audio msf structure
 */
struct cdrom_msf {
	unsigned char	cdmsf_min0;	/* starting minute*/
	unsigned char	cdmsf_sec0;	/* starting second*/
	unsigned char	cdmsf_frame0;	/*starting frame*/
	unsigned char	cdmsf_min1;	/* ending minute */
	unsigned char	cdmsf_sec1;	/* ending second */
	unsigned char	cdmsf_frame1;	/* ending frame */
};

/*
 * definition of read toc entry structure
 */
struct cdrom_tocentry {
	unsigned char  cdte_track;
	unsigned char  cdte_adr   :4;
	unsigned char  cdte_ctrl   :4;
	unsigned char  cdte_format;
	union {
		struct {
			unsigned char  minute;
			unsigned char  second;
			unsigned char  frame;
		} msf;
		int	lba;
	} cdte_addr;
	unsigned char  cdte_datamode;
};


struct cdrom_subchnl {
	unsigned char	cdsc_format;
	unsigned char	cdsc_audiostatus;
	unsigned char	cdsc_adr:	4;
	unsigned char	cdsc_ctrl:	4;
	unsigned char	cdsc_trk;
	unsigned char	cdsc_ind;
	union {
		struct {
			unsigned char 	minute;
			unsigned char 	second;
			unsigned char 	frame;
		} msf;
		int	lba;
	} cdsc_absaddr;
	union {
		struct {
			unsigned char 	minute;
			unsigned char 	second;
			unsigned char 	frame;
		} msf;
		int	lba;
	} cdsc_reladdr;
};



//Table 59 - CD-ROM Media Type Codes

//0h Door closed / caddy inserted, medium type unknown
//01h 120 mm CD-ROM data only, door closed or caddy inserted
//02h 120 mm CD-DA audio only, door closed or caddy inserted
//03h 120 mm CD-ROM data and audio combined, door closed or caddy inserted
//04h 120 mm CD-ROM Hybrid disc (Photo CD), door closed or caddy inserted
//05h 80 mm CD-ROM data only, door closed or caddy inserted
//06h 80 mm CD-DA audio only, door closed or caddy inserted
//07h 80 mm CD-ROM data and audio combined, door closed or caddy inserted
//08h 80 mm CD-ROM Hybrid disc (Photo CD), door closed or caddy inserted
//09h - 0Fh Reserved
//10h Door closed / caddy inserted, medium type (CD-R) size unknown
//11h 120 mm CD-ROM (CD-R) data only, door closed or caddy inserted
//12h 120 mm CD-DA (CD-R) audio only, door closed or caddy inserted
//13h 120 mm CD-ROM (CD-R) data and audio combined, door closed or caddy inserted
//14h 120 mm CD-ROM (CD-R) Hybrid disc (Photo CD), door closed or caddy inserted
//15h 80 mm CD-ROM (CD-R) data only, door closed or caddy inserted
//16h 80 mm CD-DA (CD-R) audio only, door closed or caddy inserted
//17h 80 mm CD-ROM (CD-R) data and audio combined, door closed or caddy inserted
#endif //__atapi_h__