/*------------------------------------------------------------------------------
LCD_Declarations.h
------------------------------------------------------------------------------*/

#ifndef __LCD_DECLARATIONS_H__
#define __LCD_DECLARATIONS_H__
#define FACTORY
#define HOME
#define Sbit(x, y, z)   sbit x = y^z


#define __8BIT__  /* 8-bit mode needs 11 Port lines */
#define __4BIT__  /* 4-bit mode needs 7 Port lines */


Sbit (E , 0x90, 6);			 /* I/O Pin  Enable	bit	of LCD */
Sbit (RW , 0x90, 5);		 /* I/O Pin  Read/Write	bit	of LCD */
Sbit (RS , 0x90, 4);		 /* I/O Pin  Register Select of LCD	*/

      		/* ---------Four bits (Data Port) for 4-bit data communications ------------*/

Sbit (DB7 , 0x90, 3);	 
Sbit (DB6 , 0x90, 2);	 			  
Sbit (DB5 , 0x90, 1);	
Sbit (DB4 , 0x90, 0);	


#define LINE1_ADDR 0x00      /* Start of line 1 in the DD-Ram */
#define LINE2_ADDR 0x40      /* Start of line 2 in the DD-Ram */
#define DD_RAM_PTR  0x80   /* Address Display Data RAM pointer */

			/* ---------Display-Commands------------ */

#define clear_lcd         0x01 /* Clear Display */
#define return_home       0x02 /* Cursor to Home position */
#define entry_mode        0x06 /* Normal (increment) entry mode */
#define entry_mode_shift  0x07 /* - with shift */
#define set_4bit  0x28 /* 4 bit data mode 2 line (5x7 font) */
#define set_8bit  0x38 /* 8 bit data mode 2 line (5x7 font)	*/
#define set_line1      0x80 /* Line 1 position 1 */
#define set_line2      0xC0 /* Line 2 position 1 */
#define set_address    0x80 /* set display address */
#define cursor1 0x0D  /* display-on, blink-on, cursor-off */
#define cursor2 0x0F  /* display-on, blink-on, cursor-on */ 
#define cursor3 0x0E  /* display-on, blink-off, cursor-on */ 
#define cursor4 0x0C  /* display-on, blink-off, cursor-off */ 
#define cursor5 0x08  /* display-off, blink-off, cursor-off */ 


			/*----------Functions Declaration-----------*/

void init_lcd(void);		   /* LCD Initialization */
void LCD_busy4bit(void);	  /* 4-bit mode busy check */
void LCD_busy8bit(void);	  /* 8-bit mode busy check */
extern void _nop_ (void);	  /* NO Operation */
void WriteInstr4bit(unsigned char);		/* Write to Instruction Register (4-bit mode) of LCD Device  */
void WriteInstr8bit(unsigned char);		/* Write to Instruction Register (8-bit mode) of LCD Device  */
void WriteData(unsigned char);			/* Write to Data Register of LCD Device  */
void ClearDisplay (void);				/* Dispaly Clear */

#endif