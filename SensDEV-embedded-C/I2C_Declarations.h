/*------------------------------------------------------------------------------
I2C_Declarations.h
------------------------------------------------------------------------------*/

extern unsigned char DigiIO_addr_wr;
extern unsigned char ctrlByte;
extern unsigned char sensor_cnt;
extern unsigned char P_AZC;


/*-------I/O Port Definitions---------*/
#define  	SDA 	P2_7		/* I²C Serial DAta line - port */
#define  	SCL		P2_6	    /* I²C Serial CLock line - port */

/*-------SCPB SENSOR - I2C BUS Communication ADDRESS Definitions---------*/

	 /********************************************************
	 NOTE: All the following numerical values are "DECIMAL" values 
	 	   corresponding to the Hexadecimal values used for the
		   communication with the SCPB Sensor
	 *********************************************************/
#define   SCPB_ADDRESS_WRITE 240    	/* hard coded address of the 31020/50 = 1111000x 
									 		= 0x78 Seven Bit I²C bus definition
											with "WRITE" option */

#define   SCPB_ADDRESS_READ 241      	/* hard coded address of the 31020/50 = 1111000x 
									 		= 0x78 Seven Bit I²C bus definition
											with "READ" option */

#define   SAVE_NEW_SIGN 201           	/* Save NEW signature in the EEPROM's address
											0x1D for SCPB-Sensor's internal automatic
											failure checking at start up */

#define   NORMAL_OP_MODE 113        	/* Sets SCPB-Mode to "Normal Operation Mode" */

#define   COMMAND_MODE 114            	/* Sets SCPB-Mode to "Command Mode" */

#define   START_CYC_EEP 1            	/* Start measurement cycle including initialization from EEPROM */



/*-------I2C Functions - Bit Banged---------*/
void i2cStart (void);						/*	Sends I2C Start Trasfer */
void i2cStop (void);						/*	Sends I2C Stop Trasfer */
void i2cCmdMode (unsigned char);			/* Command Mode operation */
void i2cNormOpMode (unsigned char);			/* Normal Mode operation */
void initialize (unsigned char);			/* Start measurement cycle including initialization from EEPROM */

void readOutReg (unsigned char, unsigned char);	  /* Reads data from the specific register of the I2C bus device SCPB-Sensor
													 and sends it to the display unit and the serial port */

void readOutRawAvg (unsigned char, unsigned char);	  /* Reads data from the specific register(specified no.of times) of the I2C bus device SCPB-Sensor
													 and sends it to the display unit and the serial port */

void PAZC_Log (unsigned char, unsigned char);	  /* Reads data from the specific register of the I2C bus device SCPB-Sensor
													 and sends it to the display unit and the serial port */

void readReg(UCHAR , UCHAR , int* , UINT* , UINT*);  /*	Reads data from the specific register of the I2C bus device SCPB-Sensor
														and uses it for calibration  */

void readPresTemp (unsigned char);			    	/*	Reads Pressure & Temperature  */

void readRegSelect (unsigned char, unsigned char);	/*	Selects the specific register of the I2C bus device SCPB-Sensor
														for reading purpose */

void readwriteA2D(unsigned char, unsigned char);		   /* Reads & writes data, from & to the A/D device respectively 
																and also sends the read data to serial port */

void writeReg (unsigned char, unsigned char, unsigned char, unsigned char);		/*	Writes data into the specific register
																					of the I2C bus device SCPB-Sensor	*/
void i2cSendByte(unsigned char);		  /* Sends a byte to the sensor via I2C */
unsigned char i2cGetByte(void);			  /* Receives a byte from the sensor via I2C */
void saveNewSignature(unsigned char);	  /* Saves the new signature of the sensor */
void getAck(void);						  /* Get Acknowledgement */
void giveAck(void);						  /* Give Acknowledgement */
unsigned int combineData(unsigned int, unsigned int);	/* Combines the 2 bytes received from the device(sensor) */
void process(UCHAR, char);					   /* Processes the data read out from the register (reads, calculates and writes)*/
char compare(int);	   /* Compares the data read out from the register with range values */



