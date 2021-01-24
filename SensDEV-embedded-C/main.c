/*-------------------------------------------------------- 
*********   MULTI-SENSOR CALIBRATION MODULE  ************
---------------------------------------------------------*/

#include "reg_C51.h"			 /* include AT89C51XD2 header file */ 
#include "LCD.c"
#include "LCD_Declarations.h"
#include "Serial_Comm.c"
#include "Menu.h"
#include "I2C_Comm.c"
#include "Calib_Func.c"
#include "Menu.c"
#define PRESS_P3_5	putcharLCD("PressSel\0");while(P3_5);wait(40000)
void subMenu(unsigned char *);

void main ()
{
	/* Initialization of LCD and Serial Communication */
	init_lcd();
	init_serial();

	while(1){
				/* Selection Menu */
		mainloop = 1;
		limit = 1;
		index = 0;
		while(P3_5){
			put2lines("Calibrtn", "Mod V1.0");
			wait(65000);
			wait(30000);
			put2lines("*Click**", "For Menu");
			wait(65000);
			wait(30000);
		} 	

		put2lines(main_menu[index], main_menu[index+1]);
		while(!P3_5);

		while(mainloop){
			wait(1000);
			WriteInstr4bit (0x87);
			wait(1000);
			WriteData ('<');
	
			if(!P3_2){
				while(!P3_2);
				if(index > 0)
					index--;
				put2lines(main_menu[index], main_menu[index+1]);
			}
	
			if(!P3_3){
				while(!P3_3);
				if(index < limit)
					index++;
				put2lines(main_menu[index], main_menu[index+1]);
			}
	
	
			if(!P3_5){
				while(!P3_5);
				switch(index)
				{
					case(0):
						limit = 1;
						subMenu(&(*PAZC_range));
						break;
			
					case(1):
						limit = 6;
						subMenu(&(*calibration_menu));
						break;			
				}
				mainloop = 0;
			}
		}
	}	
}



void subMenu(UCHAR *sub_menu[])
{
	index = 0;
	subloop = 1;
	put2lines(sub_menu[index], sub_menu[index+1]);	

	while(subloop){
		wait(1000);
		WriteInstr4bit (0x87);
		wait(1000);
		WriteData ('<');

		if(!P3_2){
			while(!P3_2);
			if(index > 0)
				index--;
			put2lines(sub_menu[index], sub_menu[index+1]);
		}

		if(!P3_3){
			while(!P3_3);
			if(index < limit)
				index++;
			put2lines(sub_menu[index], sub_menu[index+1]);
		}


		if(!P3_5){
			while(!P3_5);
			switch(index)
			{
				case(0):
					if(sub_menu[index] == "Minus"){
						valIncDec(sub_menu[index], &PAZC_Low);
						subloop = 0;
					}
					else if(sub_menu[index] == "InitSet"){
						Initial_Settings();
					}
					break;			
		
				case(1):
					if(sub_menu[index] == "Plus"){
						valIncDec(sub_menu[index], &PAZC_High);
						subloop = 0;
					}
					else if(sub_menu[index] == "3TempRaw"){
						Raw_Data_Collection();
					}
					break;

				case(2):
					Final_Calibration();
					break;

				case(3):
					Final_ReadOut();
					break;

				case(4):
					Flash_Settings();
					break;

				case(5):
					RawData_Logging();
					break;

				case(6):
					subloop = 0;
					break;
		
			}				
			index = 0;
			put2lines(sub_menu[index], sub_menu[index+1]);

		}
	}
}	