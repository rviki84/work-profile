/*------------------------------------------------------------------------------
									Menu.h
------------------------------------------------------------------------------*/


#define UCHAR 	unsigned char
#define UINT 	unsigned int
#define	SGINT 	signed int


UCHAR limit = 1;
UCHAR index = 0;
bit mainloop = 1;
bit loop = 1;
bit subloop = 1;
int PAZC_Low = -1000;
int PAZC_High = 1000;


UCHAR *main_menu[] = {	"PAZCRng"			   /* limit 1 */
						,"ZumKalib"
						,"**END***" };

UCHAR *PAZC_range[] = {"Minus"				/* limit 1 */
       				 	  ,"Plus"
      				 	  ,"**END***"};

UCHAR *calibration_menu[] = {"InitSet"				/* limit 6 */
	       				 	  ,"3TempRaw"
	       				 	  ,"FinalKal"
	       				 	  ,"P/T Read"
	       				 	  ,"FlashSet"
	       				 	  ,"Raw Logg"
	       				 	  ,"Exit->"
					 		  ,"**END***"};




//void subMenu1(unsigned char *);
//void subMenu2(unsigned char *);
void valIncDec(char[], unsigned int*);
//void menuSelect(unsigned char);
unsigned int Read_Byte_EEPROM (unsigned int) small;
void Write_Byte_EEPROM (unsigned int, unsigned int) small;
