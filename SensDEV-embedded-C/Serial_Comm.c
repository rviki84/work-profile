/*------------------------------------------------------------------------------
							Serial_Comm.C
------------------------------------------------------------------------------*/
//#include <stdio.h>							/* Standard I/O header file */


/* Wait for end of Tx over the Uart */
//#define WAIT_EO_TX {while (TI==0); TI=0;}


void init_serial(void)
{
	SCON = 0x50;
	TMOD |= 0x20;
	TH1 = 0xF3;			   	/* BRL = 243; Baudrate = 9600 */
//	TL1 = 0xB2;
 	PCON |= 128; 			/* set SMOD1 */
//	TCON |= 0x40;
	TR1 = 1;
	TI = 1;
	RI = 0;
	SBUF = 0;
}									  



//void serial_comm_isr(void) interrupt 4
//{ 
//	if(RI == 1)
//	{
//		RI = 0;
//	}
//
//	if(TI == 1)
//	{
//		TI = 0;
//	}
//

			/* Transmit data from microcontroller via serial buffer */
void sendChar(unsigned char writeBuffer)
{
  	EA = 1;
   	while(!TI);
  	TI = 0;
	SBUF = writeBuffer;
	EA = 0;
	
}

	   		/* Receive data from external device (e.g. PC) via serial buffer */
unsigned char getChar(void)			  
{
	unsigned char readBuffer;
	while(!RI);
	readBuffer = SBUF;
	RI = 0;
	return readBuffer;
}