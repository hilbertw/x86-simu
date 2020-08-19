
#ifndef __iso9660_h__
#define __iso9660_h__

#pragma pack(push,1)


//date and time field is of the following form: 
//
//     length
//     in bytes  contents
//     --------  ---------------------------------------------------------
//        4      year, as four ASCII digits
//        2      month, as two ASCII digits, where
//                 01=January, 02=February, etc.
//        2      day of month, as two ASCII digits, in the range
//                 from 01 to 31
//        2      hour, as two ASCII digits, in the range from 00 to 23
//        2      minute, as two ASCII digits, in the range from 00 to 59
//        2      second, as two ASCII digits, in the range from 00 to 59
//        2      hundredths of a second, as two ASCII digits, in the range
//                 from 00 to 99
//        1      offset from Greenwich Mean Time, in 15-minute intervals,
//                 as a twos complement signed number, positive for time
//                 zones east of Greenwich, and negative for time zones
//                 west of Greenwich

typedef struct iso9660_time 
{
unsigned char year[4];
unsigned char month[2];
unsigned char day[2];
unsigned char hour[2];
unsigned char minute[2];
unsigned char second[2];
unsigned char ten_ms[2];
unsigned char gmt;	
}ISO9660_TIME;

     
//The contents of a Primary Volume Descriptor are as follows: 

//length
 //in bytes  contents
 //--------  ---------------------------------------------------------
 //   1      1
 //   6      67, 68, 48, 48, 49 and 1, respectively (same as Volume
 //            Descriptor Set Terminator)
 //   1      0
 //  32      system identifier
 //  32      volume identifier
 //   8      zeros
 //   8      total number of sectors, as a both endian double word
 //  32      zeros
 //   4      1, as a both endian word [volume set size]
 //   4      1, as a both endian word [volume sequence number]
 //   4      2048 (the sector size), as a both endian word
 //   8      path table length in bytes, as a both endian double word
 //   4      number of first sector in first little endian path table,
 //            as a little endian double word
 //   4      number of first sector in second little endian path table,
 //            as a little endian double word, or zero if there is no
 //            second little endian path table
 //   4      number of first sector in first big endian path table,
 //            as a big endian double word
 //   4      number of first sector in second big endian path table,
 //            as a big endian double word, or zero if there is no
 //            second big endian path table
 //  34      root directory record, as described below
 // 128      volume set identifier
 // 128      publisher identifier
 // 128      data preparer identifier
 // 128      application identifier
 //  37      copyright file identifier
 //  37      abstract file identifier
 //  37      bibliographical file identifier
 //  17      date and time of volume creation
 //  17      date and time of most recent modification
 //  17      date and time when volume expires
 //  17      date and time when volume is effective
 //   1      1
 //   1      0
 // 512      reserved for application use (usually zeros)
 // 653      zeros
typedef struct Primary_Volume_Descriptor
{
unsigned char one1;
unsigned char as_terminator[6];
unsigned char zero1;
unsigned char sys_id[32];
unsigned char vol_id[32];
unsigned char zeros2[8];
unsigned long num_sectors_le;
unsigned long num_sectors_be;
unsigned char zeros3[32];
unsigned short vol_set_size_le;
unsigned short vol_set_size_be;
unsigned short vol_set_seq_num_le;
unsigned short vol_set_seq_num_be;
unsigned short sector_size_le;
unsigned short sector_size_be;
unsigned long path_tbl_len_le;
unsigned long path_tbl_len_be;

unsigned long first_sector_1st_path_tbl_le;
unsigned long first_sector_2st_path_tbl_le;
unsigned long first_sector_1st_path_tbl_be;
unsigned long first_sector_2st_path_tbl_be;
unsigned char root_dir_record[34];
unsigned char vol_set_id[128];
unsigned char publisher_id[128];
unsigned char data_preparer_id[128];
unsigned char application_id[128];

unsigned char copyright_file[37];
unsigned char abstract_file[37];
unsigned char biblio_file[37];
ISO9660_TIME t_creation;
ISO9660_TIME t_modify;
ISO9660_TIME t_expire;
ISO9660_TIME t_effective;
unsigned char one2;
unsigned char zero4;
unsigned char reserved[512];
unsigned char zeros5[653];
}PrimaryVolumeDescriptor;





#pragma pack(pop)

#endif