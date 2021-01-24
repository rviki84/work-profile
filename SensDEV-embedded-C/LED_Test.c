/*--------------------------------------------------- 
*********   PRESSURE CONTROLLER PROJECT ************
-----------------------------------------------------*/

#include "reg_C51.h"			 /* include AT89C51XD2 header file */ 
#include "LCD_Freq.c"
#include "LCD_Declarations.h"
//#include "Opto_Switch.c"
#include "Serial_Comm.c"
#include "I2C_Comm.c"
//#include <stdio.h>							/* Standard I/O header file */
//#include <defines.h>
//#include <globals.h> 
//#include <sm_printf.h>
//#include <dos.h>
//#include <conio.h>





void main ()
{
	unsigned char display = 0, select = 1;

	/* Initialization of LCD and Serial Communication */
	init_lcd();
	init_serial();
	ES = 1;
	EA = 0;


	/* Reading the sensor continuously */
	put2lines("PRESS TO\0", "START-->\0");
 	while(P3_5);
	i2cComm();
	wait(20000);
		
	/* Selection Menu */

	put2lines("Read  <\0", "Write\0");
	while(select){
		if(!P3_3){
			wait(20000);
			put2lines("Read\0", "Write <\0");
			display = 1;			
		}

		if(!P3_2){
			wait(20000);
			put2lines("Read  <\0", "Write\0");
			display = 0;			
		}

		if(!P3_5){
			wait(20000);
			switch(display)
			{
				case(0):
					put2lines("I am\0", "Reading\0");
					i2cCommRead();
					break;			
		
				case(1):
					put2lines("I am\0", "Writing\0");
					i2cCommWrite();
					break;
			}
			select = 0;
		}

	}

}

	