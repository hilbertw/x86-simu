

//00每01 USBCMD USB Command 0000h R/W
//02每03 USBSTS USB Status 0020h R/WC
//04每05 USBINTR USB Interrupt Enable 0000h R/W
//06每07 FRNUM Frame Number 0000h R/W (see Note 1)
//08每0B FRBASEADD Frame List Base Address Undefined R/W
//0C SOFMOD Start of Frame Modify 40h R/W
//0D每0F 〞 Reserved 〞 〞
//10每11 PORTSC0 Port 0 Status/Control 0080h R/WC, RO, R/W
//(see Note 1)
//12每13 PORTSC1 Port 1 Status/Control 0080h R/WC, RO, R/W
//(see Note 1)
//14每17 〞 Reserved 〞 〞
//18h LOOPDATA Loop Back Test Data 00h RO
