/*------------------------------------------------------------------------------
							Opto_Switch.C
------------------------------------------------------------------------------*/

#include "I2C_Comm.c"

void optoSwitch()
{
	static unsigned int counter_val = 425;
	char str[10];
	itoa(counter_val, str);
	ClearDisplay();
	putchar(str);
	wait(30000);
  	while(1)
  	{
   		if(!P3_2){
			counter_val = counter_val - 10;	   /* Decrease Counter Value */
			itoa(counter_val, str);
			ClearDisplay();
			wait(20000);
			putchar(str);
   		}
			
   		if(!P3_3){
			counter_val = counter_val + 10;	   /*Increase Counter Value */
			itoa(counter_val, str);
			ClearDisplay();
			wait(20000);
			putchar(str);
   		}
   			
	
		if(!P3_5){
			wait(30000);
			break;
 		}
			
 	}
}