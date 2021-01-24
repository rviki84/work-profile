/*------------------------------------------------------------------------------
I2C_Comm.C
------------------------------------------------------------------------------*/
#include "I2C_Declarations.h"					
#include "Sensor_Config.h"


/*	Read P_AZC
	Compare with range
	IF not inside the range
		1 selection of bytes
		2 write P_AZC
	loop the process until goal	is reached  */


void analogFntEnd(UCHAR addr_val)
{
 	int p_azc = 0;
	char cmp_result = 1;

	while(cmp_result != 0)
	{
		readReg(addr_val, PAZC_RD_CMD, &p_azc);
		cmp_result = compare(p_azc);
		if(cmp_result != 0)
		{
			process(addr_val, cmp_result);
		}
	}
}



void process(UCHAR addr_val, char cmp_val)
{
	UINT highByte, lowByte;
	readReg(addr_val, CFGAFE_RD_CMD, 0, &highByte, &lowByte);

	if(cmp_val == 1)
	{
		if((highByte%4) >= 2)
			highByte = highByte - 2;	
	}		

	if(cmp_val == -1)
	{
		if((highByte%4) < 2)
			highByte = highByte + 2;
	}			
	

	if(lowByte >= 240)
	{
		lowByte = lowByte - 240;
		highByte = highByte + 1;
	}
	else
	{
		lowByte = lowByte + 16;
	}
	writeReg(addr_val, CFGAFE_WR_CMD, highByte, lowByte);

}

 
 
char compare(int reg_val)
{
	if(reg_val > PAZC_High)
		return 1;
	if(reg_val < PAZC_Low)
		return -1;
	if(PAZC_Low <= reg_val <= PAZC_High)
		return 0;
}



			/*-------------Reads Pressure and/or Temperature---------------*/ 
void i2cComm(UCHAR addr_val)
{
	
	ctrlByte = 0;
	readwriteA2D(DigiIO_addr_wr, ctrlByte);	   /* Set the output of 8-bit I/O expander to 
														'0' (sensor 1 is selected through relay) */
	wait(30000);				   
	putcharSerial("Start\0");

	while(P3_2){
		readPresTemp(addr_val);
		++ctrlByte;								  /* Increment the ctrlByte by 1 */
		if(ctrlByte == sensor_cnt)
			ctrlByte = 0;
		readwriteA2D(DigiIO_addr_wr, ctrlByte);	  /* Set the output of 8-bit I/O expander equal to the 
														ctrlByte value (next sensor relay is selected) */
		wait(12000);	//10000			/* wait state to stabilize relay after switching */
	}
}


			/*-------------Reads PAZC continuously---------------*/ 
void RawDataLog(UCHAR addr_val)
{
	
	ctrlByte = 0;
	readwriteA2D(DigiIO_addr_wr, ctrlByte);	   /* Set the output of 8-bit I/O expander to 
														'0' (sensor 1 is selected through relay) */
	wait(30000);				   
	putcharSerial("Start\0");

	while(P3_2){
		PAZC_Log(addr_val, P_AZC);
		++ctrlByte;								  /* Increment the ctrlByte by 1 */
		if(ctrlByte == sensor_cnt)
			ctrlByte = 0;
		readwriteA2D(DigiIO_addr_wr, ctrlByte);	  /* Set the output of 8-bit I/O expander equal to the 
														ctrlByte value (next sensor relay is selected) */
		wait(20000);				/* wait state to stabilize relay after switching */
	}
}


void Read_PAZC(UCHAR addr_val, UCHAR read_cmd)
{
	int i;
 
	ctrlByte = 0;
	readwriteA2D(DigiIO_addr_wr, ctrlByte);	   /* Set the output of 8-bit I/O expander to 
														'0' (sensor 1 is selected through relay) */
	wait(10000);						/* wait state to stabilize relay after switching */
	for (i = 1; i <= sensor_cnt; i++){
//		i2cCmdMode(addr_val);
//		initialize(addr_val);
		readOutReg(addr_val, read_cmd);
		++ctrlByte;								  /* Increment the ctrlByte by 1 */
		readwriteA2D(DigiIO_addr_wr, ctrlByte);	  /* Set the output of 8-bit I/O expander equal to the 
														ctrlByte value (next sensor relay is selected) */
		wait(15000); 			//10000			 /* wait state to stabilize relay after switching */
	}
}


void i2cCommReadPT(UCHAR addr_val, UCHAR read_cmd)
{
	int i;

	ctrlByte = 0;
	readwriteA2D(DigiIO_addr_wr, ctrlByte);	   /* Set the output of 8-bit I/O expander to 
														'0' (sensor 1 is selected through relay) */
	wait(20000);				/* wait state to stabilize relay after switching */
	if(read_cmd == 217)
		put2lines("Reading\0", "Temp....\0");
	else
		put2lines("Reading\0", "Pres....\0");
	for (i = 1; i <= sensor_cnt; i++){
		i2cCmdMode(addr_val);
		initialize(addr_val);
		readOutRawAvg(addr_val, read_cmd);
		++ctrlByte;								  /* Increment the ctrlByte by 1 */
		readwriteA2D(DigiIO_addr_wr, ctrlByte);	  /* Set the output of 8-bit I/O expander equal to the 
														ctrlByte value (next sensor relay is selected) */
		wait(25000); 			//20000		/* wait state to stabilize relay after switching */	
	}
}


void i2cCommRead(UCHAR addr_val)
{

	char rd_reg[20];
	int i;
	UINT rdCmd; 
	rdCmd = 70;

	for (i = 0; i <= 19; i++){
		rd_reg[i] = rdCmd;
		rdCmd = rdCmd + 1;
		if(rdCmd == 80)
			rdCmd = 208;
		if((rdCmd == 211) | (rdCmd == 215))
			rdCmd = rdCmd + 1;
//		rd_reg[i] = getChar();				
	}								


	for (i = 0; i <= 19; i++){

//		i2cCmdMode(addr_val);
//		initialize(addr_val);
		readOutReg(addr_val, rd_reg[i]);
	}

}


void i2cCommWrite(UCHAR addr_val)
{

	char wr_reg[27];
	int i; 
				   
	for (i = 0; i <= 26; i++){

		wr_reg[i] = getChar();
	}


	for (i = 0; i <= 24; i=i+3){

		writeReg(addr_val, wr_reg[i], wr_reg[i+1], wr_reg[i+2]);

	}

}

void i2cCommReadCoeffts(UCHAR addr_val)
{

	char rd_reg[13];
	int i;
	UINT rdCmd; 
	rdCmd = 48;

	for (i = 0; i <= 12; i++){
		rd_reg[i] = rdCmd;
		rdCmd = rdCmd + 1;
	}								

	for (i = 0; i <= 12; i++){

//		i2cCmdMode(addr_val);
//		initialize(addr_val);
		readOutReg(addr_val, rd_reg[i]);
	}

}



void i2cCommWriteCoeffts(UCHAR addr_val)
{

	char wr_reg[39];
	int i; 

	for (i = 0; i <= 38; i++){

		wr_reg[i] = getChar();
	}


	for (i = 0; i <= 36; i=i+3){

		writeReg(addr_val, wr_reg[i], wr_reg[i+1], wr_reg[i+2]);

	}

}




void IniSettRead(UCHAR addr_val)
{

	char rd_reg[8];
	int i;
	UINT rdCmd; 
	rdCmd = 70;

	for (i = 0; i <= 7; i++){
		rd_reg[i] = rdCmd;
		rdCmd = rdCmd + 1;
	}								

	for (i = 0; i <= 7; i++){

//		i2cCmdMode(addr_val);
//		initialize(addr_val);
		readOutReg(addr_val, rd_reg[i]);
	}

}



void IniSettWrite(UCHAR addr_val)
{

	char wr_reg[21];
	int i; 

	for (i = 0; i <= 20; i++){

		wr_reg[i] = getChar();
	}


	for (i = 0; i <= 18; i=i+3){

		writeReg(addr_val, wr_reg[i], wr_reg[i+1], wr_reg[i+2]);

	}

}

			/*-------I2C Bus Device Communication Protocols---------*/

/*------------------------------------------------------------------------------
 	Function:	i2cStart
	Purpose:	Sends I2C Start Transfer - State "S"
------------------------------------------------------------------------------*/
void i2cStart (void)
{
  	SDA = 1;							 /* Set data line high */   
  	wait(60);
	SCL = 1;							/* Set clock line high */
	wait(60);
	SDA = 0;						   /* Set data line low (START SIGNAL) */
	wait(60);						
	SCL = 0;							/* Set clock line low */
  	wait(60);
}

/*------------------------------------------------------------------------------
 	Function:	i2cStop
	Purpose:	Sends I2C Stop Transfer - State "P"
------------------------------------------------------------------------------*/
void i2cStop (void)
{

  	SDA = 0;							/* Set data line high */
  	wait(60);
  	SCL = 1;							/* Set clock line low */
  	wait(60);
	SDA = 1;							/* Set data line high (STOP SIGNAL) */
	wait(60);
	SCL = 0;							/* Set clock line low */
  	wait(60);

}


	   /* Start measurement cycle including initialization from EEPROM */
void initialize(UCHAR scpb_ad_addr_wr)
{
	i2cStart();							/* Send I2C Start Transfer */
	i2cSendByte(scpb_ad_addr_wr);  		/* Send device identifier I2C address with initialization 'Write' */              		
  	getAck();
  	i2cSendByte(START_CYC_EEP);		/* Send command to start measurement cycle */
  	getAck();  	
	i2cStop();			    /* Send I2C Stop Transfer  */
}


	   /* Command Mode operation */
void i2cCmdMode (UCHAR scpb_ad_addr_wr)
{
	i2cStart();					/* Send I2C Start Transfer */
	i2cSendByte(scpb_ad_addr_wr);  	/* Send device identifier I2C address with initialization 'Write' */              		
  	getAck();
	i2cSendByte(COMMAND_MODE);  					           		
  	getAck();  	
	i2cStop();			    	/* Send I2C Stop Transfer  */
}


	   /* Normal Mode operation */
void i2cNormOpMode (UCHAR scpb_ad_addr_wr)
{
	i2cStart();							/* Send I2C Start Transfer */
	i2cSendByte(scpb_ad_addr_wr);  		/* Send device identifier I2C address with initialization 'Write' */              		
  	getAck();
	i2cSendByte(NORMAL_OP_MODE);  					           		
  	getAck();  	
	i2cStop();			    /* Send I2C Stop Transfer  */
}


	  /* Calculates EEPROM Signature and writes it to EEPROM address 0x1D */
void saveNewSignature(UCHAR scpb_ad_addr_wr)
{
	i2cStart();							/* Send I2C Start Transfer */
	i2cSendByte(scpb_ad_addr_wr);  		/* Send device identifier I2C address with initialization 'Write' */              		
  	getAck();
	i2cSendByte(SAVE_NEW_SIGN);  		/* Send command to Calculate & Save signature */         		
	getAck();  	
  	i2cStop();			    /* Send I2C Stop Transfer  */
}
		

/*----------------------------------------------------------------------------------------------
 Procedure:		readReg
 Inputs:		sensor address for write(scpb_ad_addr_wr) & command to read register(read_cmd)
 Outputs:		value stored in the register and the arguments decimalData, highByte and lowByte 
 				are used for calibration process
 Description:	Reads the register value
-----------------------------------------------------------------------------------------------*/

void readReg(UCHAR scpb_ad_addr_wr, UCHAR read_cmd, int* decDataVal, UINT* HByteVal, UINT* LByteVal)
{
	UINT decimalData, highByte, lowByte;
	i2cCmdMode(scpb_ad_addr_wr);
	initialize(scpb_ad_addr_wr);
 	i2cCmdMode(scpb_ad_addr_wr);
	readRegSelect(scpb_ad_addr_wr, read_cmd);

	i2cStart();						/* Send I2C Start Transfer */
 	i2cSendByte(scpb_ad_addr_wr + 1);  	/* Send device identifier I2C address with initialization 'Read'(=Write+1)*/
  	getAck();		
	highByte = i2cGetByte();		/* Get high byte */
 	giveAck();
  	lowByte = i2cGetByte();			/* Get low byte */
  	giveAck();
	i2cStop();			    /* Send I2C Stop Transfer */
	*HByteVal = highByte;
	*LByteVal = lowByte;
		   /* Combine both bytes and display */
  	decimalData = combineData(highByte, lowByte);
	*decDataVal = decimalData;
}


/*-----------------------------------------------------------------------------
 Procedure:		readOutReg
 Inputs:		sensor address for write(scpb_ad_addr_wr) & command to read register(read_cmd)
 Outputs:		value stored in the register 
 Description:	Reads the register value
------------------------------------------------------------------------------*/
void readOutReg(UCHAR scpb_ad_addr_wr, UCHAR read_cmd)
{
   	unsigned int highByte, lowByte, decimalData;
	char str[10];

	i2cCmdMode(scpb_ad_addr_wr);
	readRegSelect(scpb_ad_addr_wr, read_cmd);

	i2cStart();						/* Send I2C Start Transfer */
 	i2cSendByte(scpb_ad_addr_wr + 1);  	/* Send device identifier I2C address with initialization 'Read'(=Write+1) */
  	getAck();		
	highByte = i2cGetByte();		/* Get high byte */
 	giveAck();
  	lowByte = i2cGetByte();			/* Get low byte */
//  	giveAck();
	i2cStop();			    /* Send I2C Stop Transfer */
	
			   /* Combine both bytes and display */
  	decimalData = combineData(highByte, lowByte);
	itoa(decimalData, str);                   		
	putcharSerial(str);
}


/*-----------------------------------------------------------------------------
 Procedure:		readOutRawAvg
 Inputs:		sensor address for write(scpb_ad_addr_wr) & command to read register(read_cmd)
 Outputs:		value stored in the register(reads specified no. of times and performs average) 
 Description:	Reads the register value
------------------------------------------------------------------------------*/
void readOutRawAvg(UCHAR scpb_ad_addr_wr, UCHAR read_cmd)
{
   	unsigned int highByte, lowByte, decimalData;
	signed int count;
	signed long int sum, average;
	char str[10];
	sum = 0;

	i2cCmdMode(scpb_ad_addr_wr);
	readRegSelect(scpb_ad_addr_wr, read_cmd);

	i2cStart();						/* Send I2C Start Transfer */
 	i2cSendByte(scpb_ad_addr_wr + 1);  	/* Send device identifier I2C address with initialization 'Read'(=Write+1) */
  	getAck();
	
	for (count = 1; count <= 10; count++){
		
		highByte = i2cGetByte();		/* Get high byte */
	 	giveAck();
	  	lowByte = i2cGetByte();			/* Get low byte */
		if(count < 10)	  	
			giveAck();
	  	decimalData = combineData(highByte, lowByte);		  /* Combine both bytes and display */
		sum = sum + decimalData;
		wait(200);
	}
	i2cStop();			    /* Send I2C Stop Transfer */
	
	average = sum/10;		   
	itoa((int)average, str);                   		
	putcharSerial(str);
}


/*-----------------------------------------------------------------------------
 Procedure:		PAZC_Log
 Inputs:		sensor address for write(scpb_ad_addr_wr) & command to read register(read_cmd)
 Outputs:		value stored in the register 
 Description:	Reads the register value
------------------------------------------------------------------------------*/
void PAZC_Log(UCHAR scpb_ad_addr_wr, UCHAR read_cmd)
{
   	unsigned int highByte, lowByte, decimalData;
	signed int count;
	signed long int sum, average;
	char str[10];
	sum = 0;

	i2cCmdMode(scpb_ad_addr_wr);
	readRegSelect(scpb_ad_addr_wr, read_cmd);

	i2cStart();						/* Send I2C Start Transfer */
 	i2cSendByte(scpb_ad_addr_wr + 1);  	/* Send device identifier I2C address with initialization 'Read'(=Write+1) */
  	getAck();		
	for (count = 1; count <= 10; count++){
		
		highByte = i2cGetByte();		/* Get high byte */
	 	giveAck();
	  	lowByte = i2cGetByte();			/* Get low byte */
		if(count < 10)	  	
			giveAck();
	  	decimalData = combineData(highByte, lowByte);		  /* Combine both bytes and display */
		sum = sum + decimalData;
		wait(200);
	}
	i2cStop();			    /* Send I2C Stop Transfer */
	
	average = sum/10;		   
	itoa((int)average, str);                   		
	putcharSerial(str);
	putcharSerial(str);
}



/*-----------------------------------------------------------------------------
 Procedure:		readRegPT
 Inputs:		sensor address (scpb_ad_addr_rd)
 Outputs:		Pressure & Temperature
 Description:	Reads the pressure & temperature values
------------------------------------------------------------------------------*/
void readPresTemp(UCHAR scpb_ad_addr_wr)
{
   	unsigned int Pres_highByte, Pres_lowByte, Temp_highByte, Temp_lowByte, decPres, decTemp;
	char str[10];

	i2cCmdMode(scpb_ad_addr_wr);
	initialize(scpb_ad_addr_wr);

	i2cStart();								/* Send I2C Start Transfer */
	i2cSendByte(scpb_ad_addr_wr + 1);  			/* Send device identifier I2C address with initialization 'Read'(=Write+1) */
  	getAck();
	
	Pres_highByte = i2cGetByte();			/* Get high byte */
 	giveAck();
  	Pres_lowByte = i2cGetByte();			/* Get low byte */
  	giveAck();
  			/* Combine both bytes and display */
	decPres = combineData(Pres_highByte, Pres_lowByte);		 
	itoa(decPres, str);                   		
	putcharSerial(str);

  	Temp_highByte = i2cGetByte();
 	giveAck();
  	Temp_lowByte = i2cGetByte();
//  	giveAck();
	decTemp = combineData(Temp_highByte, Temp_lowByte);
	itoa(decTemp, str);                   		
	putcharSerial(str);
	i2cStop();			    /* Send I2C Stop Transfer */

}



/*-----------------------------------------------------------------------------------
 Procedure:		readRegSelect
 Inputs:		sensor address (scpb_ad_addr_wr) & register Read command (read_cmd)
 Outputs:		None
 Description:	Selects the specified register for reading operation
--------------------------------------------------------------------------------------*/
void readRegSelect(UCHAR scpb_ad_addr_wr, UCHAR read_cmd)
{
	i2cStart();							/* Send I2C Start Transfer */
	i2cSendByte(scpb_ad_addr_wr);  		/* Send device identifier I2C address with initialization 'Write' */              		
	getAck();
	i2cSendByte(read_cmd);  			/* Send EEPROM Register Read Command */		           		
  	getAck();  	
 	i2cStop();			    /* Send I2C Stop Transfer  */
}



/*----------------------------------------------------------------------------------------------------
 Procedure:		readwriteA2D
 Inputs:		Device address for write(a2d_addr_wr) & control byte for device function(ctrl_byte)
 Outputs:		The A/D output 
 Description:	Reads the digital ouput value
-----------------------------------------------------------------------------------------------------*/
void readwriteA2D(UCHAR a2d_addr_wr, UCHAR ctrl_byte)
{
//	unsigned int dataByte;
//	char str[10];
//	int i;
//	int sum, average;
//	sum = 0;

	i2cStart();						/* Send I2C Start Transfer */
 	i2cSendByte(a2d_addr_wr);  	/* Send device identifier I2C address with initialization 'Write' */ 
  	getAck();		
 	i2cSendByte(ctrl_byte);  	/* Send device control byte*/
  	getAck();		
	i2cStop();			    /* Send I2C Stop Transfer */

//	if(a2d_addr_wr == 144){
//	 	i2cSendByte(a2d_addr_wr + 1);  	/* Send device identifier I2C address with initialization 'Read'(=Write+1) */
//	  	getAck();		
//		for (i = 1; i <= 11; i++){
//			dataByte = i2cGetByte();		/* Get data byte from the respective device */
//	 		if(i < 11)
//				giveAck();
//			if(i > 1)
//				sum = sum + dataByte;
//		}
//		i2cStop();			    /* Send I2C Stop Transfer */
		
//		average = sum/10;		   
//		itoa(ctrl_byte, str);                   		
//		putcharSerial(str);
//	}


}



/*-----------------------------------------------------------------------------------------------
 Procedure:		writeReg
 Inputs:		sensor address (scpb_ad_addr_wr), register Write command (write_cmd) & 2 bytes
 Outputs:		None
 Description:	Selects the specified register for reading operation
------------------------------------------------------------------------------------------------*/
void writeReg(UCHAR scpb_ad_addr_wr, UCHAR write_cmd, UCHAR high_byte, UCHAR low_byte)
{
	i2cCmdMode(scpb_ad_addr_wr);
	i2cStart();							/* Send I2C Start Transfer */
	i2cSendByte(scpb_ad_addr_wr);  		/* Send device identifier I2C address with initialization 'Write' */              		
  	getAck();
	i2cSendByte(write_cmd);				/* Send EEPROM Register Write Command */
	getAck();  	
  	i2cSendByte(high_byte);				/* Send high byte */
	getAck();
  	i2cSendByte(low_byte);				/* Send low byte */
	getAck();
  	i2cStop();			    			/* Send I2C Stop Transfer  */
	i2cCmdMode(scpb_ad_addr_wr);
	saveNewSignature(scpb_ad_addr_wr);

}


		
/*-----------------------------------------------------------------------------
 	Routine:	i2cSendByte
	Inputs:		output byte
	Outputs:	none
	Purpose:	Writes data via the I2C bus
------------------------------------------------------------------------------*/
void i2cSendByte(UCHAR transmit_data){

	int i;
	SCL = 0;
	wait(10);

	for(i = 0; i <= 7; i++){  		/* Send 8 bits to the I2C Bus */

		SDA = ((transmit_data & 0x80) ? 1 : 0); 	/* Transmit(from master) a data bit to the I2C Bus	 */
		wait(100);
		SCL = 1; 
		wait(10);
		SCL = 0;
		wait(10);
   		transmit_data = transmit_data << 1;            		/* Shift the byte by one bit  */
	}
 }  

/*-----------------------------------------------------------------------------
 	Routine:	i2cGetByte
	Inputs:		none
	Outputs:	input byte
	Purpose:	Reads data from the I2C bus
------------------------------------------------------------------------------*/
UCHAR i2cGetByte(void){
	
	UCHAR receive_data;
	int i;
	receive_data = 0x00;		/* Initialization */

	for (i = 0; i <= 7; i++)  	/* Send 8 bits to the I2C Bus */
	{
		receive_data = receive_data << 1;					/* Shift the byte by one bit */
		SCL = 0;
		wait(100);
  		receive_data |=	SDA; 		   		/* Receive (from slave) the data from the I2C Bus */
		   /* Clock the data into the I2C Bus with SCL switching from 1 to 0 */
		SCL = 1;           				
		wait(10);
		SCL = 0;           									
	}
  	SDA = 1;
 	return receive_data;
}

	 /* Give Acknowledgement */
void giveAck(void){
  SCL = 0;
  SDA = 0;
  wait(10);
  SCL = 1;
  wait(10);
  SCL = 0;
  SDA = 1;
}

	/* Get Acknowledgement */
void getAck(void){

 	SCL = 0;
	wait(100);
	SCL = 1;
  	wait(10);
  	SCL = 0;
}

	/* Combines the 2 bytes received from I2C device(sensor) */
unsigned int combineData(unsigned int byte1, unsigned int byte2)
{
	unsigned int quotient1, quotient2, remainder1, remainder2, combinedVal;
	quotient1 = byte1/16;
	quotient1 = quotient1*4096;
	remainder1 = byte1%16;
	remainder1 = remainder1*256;
  	quotient2 = byte2/16;
	quotient2 = quotient2*16;
	remainder2 = byte2%16;
	combinedVal = quotient1 + quotient2 + remainder1 + remainder2;
	return combinedVal;

}

