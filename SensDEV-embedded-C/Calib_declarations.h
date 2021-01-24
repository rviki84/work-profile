/*------------------------------------------------------------------------------
									Calib_declarations.h
------------------------------------------------------------------------------*/


unsigned char sensor_addr_wr = 240;
unsigned char DigiIO_addr_wr = 64;	   /* I/O Expander device address + write */
unsigned char ctrlByte = 0;			   /* I/O Expander device control byte for sensors count */
unsigned char sensor_cnt;
unsigned char P_AZC = 216;
unsigned char T_AZC = 217;
unsigned char set_DAC = 96;
unsigned char ctrlByte_supply = 64;			   /* I/O Expander device control byte for enabling supply sensors */

