/*------------------------------------------------------------------------------
LCD.C
------------------------------------------------------------------------------*/

#include "LCD_Declarations.h"
#include "string.h"							/* Header file for 'strlen' function */


#define high_to_low(x) ((x & 0xF0) >> 4)		  /* Eg. (1011 1001) to (0000 1011) */
#define low(x) (x & 0x0F)						  /* Eg. (1011 1001) to (0000 1001) */
#define low_to_high(x) ((x & 0x0F) << 4)		  /* Eg. (1011 1001) to (1001 0000) */
#define CR       0x0D		   /* Hyperterminal Command - Carriage Return */
#define LF       0x0A		   /* Hyperterminal Command - Line Feed */
#define TAB      0x09		   /* Hyperterminal Command - Tabular Column */

/* Function declaration */
void sendChar(unsigned char);  /* Transmit data from microcontroller via serial buffer */

/* Delay loop */
void wait(unsigned int time) {
  unsigned int i;
  for (i = 0; i < time ;i++);
}

/* ReadInstr4: Read from INSTRUCTION REGISTER (4-bit mode) of LCD Device */
static unsigned char ReadInstr(void){
  unsigned char val;

#ifdef __4BIT__
  RS = 0;          /* select instruction register */ 
  RW = 1;          /* read operation */ 
  E = 1;           /* give operation start signal */ 
  _nop_ (); _nop_ (); 
  val = low_to_high(P1);           /* read Instruction */ 
  E = 0;
    _nop_ (); _nop_ (); 
  E = 1;           /* give operation start signal */ 
  _nop_ (); _nop_ (); 
  val |= low(P1);           /* read Instruction */ 
  E = 0;
#endif

  return(val);
}

/* ReadData4: Read from DATA REGISTER (4-bit mode) of LCD Device */
static unsigned char ReadData(void){
  unsigned char val;

#ifdef __4BIT__ 
  RS = 1;         /* select data register  */
  RW = 1;         /* read-operation  */
  E = 1;          /* give operation start signal  */
  _nop_ (); _nop_ (); 
  val = low_to_high(P1);            /* read value */ 
  E = 0;
  _nop_ (); _nop_ (); 
  E = 1;
  _nop_ (); _nop_ (); 
  val |= low(P1);					/* read value */
  E = 0;
#endif
  return (val);
}


/* WriteInstr4bit: Write to Instruction Register (4-bit mode) of LCD Device */
void WriteInstr4bit(unsigned char val){
  LCD_busy4bit();
  P1 &= 0xF0;
  RS = 0;          /* select instruction register */
  RW = 0;          /* write operation  */
  E = 1;          /* give operation start signal */
  _nop_ (); _nop_ (); 
  P1 |= high_to_low(val);           /* write instruction */ 
      _nop_ (); _nop_ (); 
  E = 0;
    _nop_ (); _nop_ (); 
  P1 &= 0xF0;
  	
	E = 1;          /* give operation start signal */
  _nop_ (); _nop_ (); 
  P1 |= low(val);				   /* write instruction */
  E = 0;
}

/* WriteInstr8bit: Write to Instruction Register (8-bit mode) of LCD Device  */
void WriteInstr8bit(unsigned char val){
  LCD_busy8bit();
  P1 &= 0xF0;

  RS = 0;          /* select instruction register  */
  RW = 0;          /* write operation  */
  E = 1;          /* give operation start signal */
  _nop_ (); _nop_ (); 
  P1 |= high_to_low(val);        /* write instruction */ 
      _nop_ (); _nop_ (); 
  E = 0;

}

/* WriteData4: Write to Data Register (4-bit mode) of LCD Device */
void WriteData(unsigned char val){
  LCD_busy4bit();
#ifdef __4BIT__
  P1 &= 0xF0;
  RS = 1;          /* select data register  */
  RW = 0;          /* write operation  */

  E = 1;          /* give operation start signal */
  _nop_ (); _nop_ (); 
  P1 |= high_to_low(val);           /* write value */
  E = 0;
  _nop_ (); _nop_ (); 
    P1 &= 0xF0;

  E = 1;          /* give operation start signal */
  _nop_ (); _nop_ (); 
  P1 |= low(val);					/* write value */
  E = 0;
#endif
}

/* Wait for the LCD busy pin (4-bit mode) to clear */
void LCD_busy4bit(){
  unsigned char busy;

#ifdef __4BIT__
	do {
	  RW = 1;
	  RS = 0;
	  E = 1;           /* give operation start signal  */
	  _nop_ (); _nop_ ();
    if(DB7 & 1) {      /* MSB is the busy bit */
      busy = 1;
    }
    else busy = 0;
	  E = 0;
	} while (busy==1);
#endif
}

/* Wait for the LCD busy pin (8-bit mode) to clear */
void LCD_busy8bit(){
  unsigned char busy;

#ifdef __8BIT__
	do {
	  RW = 1;
	  RS = 0;
	  E = 1;           /* give operation start signal  */
	  _nop_ (); _nop_ ();
    if(DB7 & 1) {     /* MSB is the busy bit */
      busy = 1;
    }
    else busy = 0;
	  E = 0;
	} while (busy==1);
#endif

}


/* --------------------------------------------------------
	Sends string characters to be displayed via Serial Port 
	Argument:
		string[] - String to be displayed
---------------------------------------------------------*/
void putcharSerial (char string[]){
  int i = 0;

    while(string[i]!= '\0')
	{
	  sendChar(string[i]);		 /*  Transmit each character via Serial Port */
	  ++i;
	}

	if (string[i]== '\0')
	{
	  sendChar(' ');			 /*  End of a string in Serial Communication (for excel VBA) */
	}
	
}  


/* -------------------------------------------------------
	Display a string in one line of the 2-line LCD 
	Argument:
		string[] - String to be displayed
---------------------------------------------------------*/
void putcharLCD (char string[]){
//  unsigned char linechk;	 /* active line check variable */
  int i = 0;

	ClearDisplay();
	wait(500);

    while(string[i]!= '\0')
	{
	  wait(500);
      WriteData (string[i]);	 /*  Write each character to LCD */
	  ++i;
	}


//    linechk = ReadInstr ();		/* Read Instruction Register for current active line in LCD */
//	if (linechk & LINE2_ADDR)   /* IF Cursor is already in line 2 */
//	{
//	   wait(500);
//       WriteInstr4bit (set_line1);   /* Set cursor in line 1 */  
//    }
//	else
//    {   
//	   wait(500);
//	   WriteInstr4bit (set_line2);   /*  Set cursor in line 2 */  
//	}
	
}  

/* -------------------------------------------------------
 	Display a string in each line of the 2-line LCD & via Serial Port
	Argument:
		line1[] - string 1 to be dispalyed
		line2[] - string 2 to be displayed
---------------------------------------------------------*/
//void put2lines (char line1[], char line2[]){
// 	
//	int i = 0;
//	ClearDisplay();					/*  Clear whole display and set cursor in line 1 */
//    
//	while(line1[i]!= '\0')
//	{
//	  wait(500);
//      WriteData (line1[i]);		   	/*  Write each character to line1 of LCD */
//	  wait(500);					
//	  sendChar(line1[i]);			/*  Transmit each character via Serial Communication */
//	  ++i;
//	}
//
//	if (line1[i]== '\0')
//	{
//	  wait(500);
//	  sendChar(' ');		   		/*  End of a string for Serial Communication (for excel VBA) */
//	}
//	
//	WriteInstr4bit (set_line2);   /*  Set cursor in line 2 */  
//	i = 0;
//
//	while(line2[i]!= '\0')
//	{
//	  wait(500);
//      WriteData (line2[i]);			/*  Write each character to line2 of LCD */
//	  wait(500);
//	  sendChar(line2[i]);			/*  Transmit each character via Serial Communication */
//	  ++i;
//	}
//
//	if (line2[i]== '\0')
//	{
//	  wait(500);
//	  sendChar(' ');			   /*  End of a string for Serial Communication (for excel VBA) */
//	}
//
//    WriteInstr4bit (set_line1);   /* Set cursor in line 1 */  
//
//}  



void put2lines (char line1[], char line2[]){
 	
	int i = 0;

	ClearDisplay();

	while(line1[i]!= '\0')
	{
	  wait(500);
      WriteData (line1[i]);		   	/*  Write each character to line1 of LCD */
	  ++i;
	}

	
	WriteInstr4bit (set_line2);   /*  Set cursor in line 2 */  
	i = 0;

	while(line2[i]!= '\0')
	{
	  wait(500);
      WriteData (line2[i]);			/*  Write each character to line2 of LCD */
	  ++i;
	}


    WriteInstr4bit (set_line1);   /* Set cursor in line 1 */  

}  



/* reverse:  reverse string s in place */
void reverse(char s[])
{
    int c, i, j;

    for (i = 0, j = strlen(s)-1; i<j; i++, j--) {
        c = s[i];
        s[i] = s[j];
        s[j] = c;
    }
}

/* itoa:  convert integer 'n' to string 's' */
void itoa(int n, char s[])
{
    int i, sign;

    if ((sign = n) < 0)  /* record sign */
        n = -n;          /* make n positive */
    i = 0;
    do {       /* generate digits in reverse order */
        s[i++] = n % 10 + '0';   /* get next digit */
    } while ((n /= 10) > 0);     /* delete it */
    if (sign < 0)
        s[i++] = '-';
    s[i] = '\0';
    reverse(s);
} 



/* ClearDisplay: Clears the whole display in LCD and sets the cursor in home position */
void ClearDisplay (void)  {
  wait(500);
  WriteInstr4bit (clear_lcd);
  wait(500);
  WriteInstr4bit (return_home);

}

/* Initialization of LCD to 4-bit mode Operation */
void init_lcd(void){
  			    
#ifdef __8BIT__
  wait(500);
  WriteInstr8bit(set_8bit);		/* Set 8-bit mode 3 times */
  wait(500);
  WriteInstr8bit(set_8bit);
  wait(500);
  WriteInstr8bit(set_8bit);
  wait(500);
  WriteInstr8bit(set_4bit);		/* Sets 4-bit mode (bit) alone because of current 8-bit
  							       mode. After this instruction 4-bit mode is achieved */
#endif

#ifdef __4BIT__
  wait(500);
  WriteInstr4bit(clear_lcd);	
  wait(500);
  WriteInstr4bit(set_4bit);		/* Set 4-bit mode, 2 lines display and 5x7 dots */
  wait(500);
  WriteInstr4bit(cursor5);
  wait(500);
  WriteInstr4bit(return_home);
  wait(500);
  WriteInstr4bit(cursor4);
  wait(500);
  WriteInstr4bit(entry_mode);
#endif
}
