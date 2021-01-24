/*------------------------------------------------------------------------------
									Calib_Func.c
------------------------------------------------------------------------------*/

#include "Calib_declarations.h"


void Initial_Settings ()
{
	int i;
	unsigned char supply_adtl_addr_wr = 68, Vref_adtl_addr_wr = 66, Vagnd_adtl_addr_wr = 70;
	unsigned char ADC_addr_wr = 144;	   			/* A/D device address + write */
	unsigned char ADC_ctrlByte = 1;			   	/* A/D device control byte (single-ended channel '1' selection) */
	char DAC_byte[7];

	put2lines("Ini-Sett", "StrtXcel");
	PAZC_Low = Read_Byte_EEPROM (&PAZC_Low);
	PAZC_High = Read_Byte_EEPROM (&PAZC_High);

	for (i = 0; i <= 6; i++)
		DAC_byte[i] = getChar();		  			/* Receive the DAC data bytes */

	sensor_cnt = DAC_byte[6];
//	readwriteA2D(DigiIO_addr_wr, ctrlByte_supply);	   		/* Set the output of 8-bit I/O expander to 
//																'64'(0x40) (Supply Sensors are enabled) */
//	wait(20000);
//	
//	writeReg(supply_adtl_addr_wr, set_DAC, DAC_byte[0], DAC_byte[1]);	/* Set supply to the sensors with the specified value */
//	wait(20000);
	
//	writeReg(Vref_adtl_addr_wr, set_DAC, DAC_byte[2], DAC_byte[3]);		/* Set Vref of ADC with the specified value */
//	wait(20000);
//	
//	writeReg(Vagnd_adtl_addr_wr, set_DAC, DAC_byte[4], DAC_byte[5]);		/* Set Vagnd of ADC with the specified value */
//	wait(20000);
//	
//	readwriteA2D(ADC_addr_wr, ADC_ctrlByte);	   			/* Read the Sensor Supply Voltage */
//	wait(20000);

	ctrlByte = 0;
	readwriteA2D(DigiIO_addr_wr, ctrlByte);		 /* Set the output of 8-bit I/O expander to 
														'0' (sensor 1 is selected through relay) */
	
//	putcharLCD("Flashing\0");
	wait(20000);

	for (i = 1; i <= sensor_cnt; i=i+1){
			putcharLCD(" RD/WR");
		i2cCommRead(sensor_addr_wr);	   
		i2cCommWrite(sensor_addr_wr);	

			putcharLCD(" FrntEnd");
		analogFntEnd(sensor_addr_wr);
		i2cCommRead(sensor_addr_wr);

		++ctrlByte;									/* Increment the ctrlByte by 1 */
		readwriteA2D(DigiIO_addr_wr, ctrlByte);	   /* Set the output of 8-bit I/O expander equal to the 
														ctrlByte value (next sensor relay is selected) */
		wait(10000);		//6000				/* wait state to stabilize relay after switching */ 
	}
	
	putcharLCD("Rd PAZC");
	while(P3_5);
	Read_PAZC(sensor_addr_wr, P_AZC);	
	
	putcharLCD("PAZChigh");
	while(P3_5);
	Read_PAZC(sensor_addr_wr, P_AZC);	

}




void Raw_Data_Collection ()
{
	unsigned char display = 0, select = 1;
	int i;

	put2lines("Raw Data\0", " 3-Temp");

	sensor_cnt = getChar();		
		
	/* Selection Menu */

	put2lines("T(L/H) <", "T_mid");
	while(select){
		if(!P3_2){
			wait(30000);
			put2lines("T(L/H) <", "T_mid");
			display = 0;			
		}

		if(!P3_3){
			wait(30000);
			put2lines("T(L/H)", "T_mid  <");
			display = 1;			
		}


		if(!P3_5){
			wait(20000);
			switch(display)
			{
				case(0):
					putcharLCD("Read T");
					while(P3_5);
					i2cCommReadPT(sensor_addr_wr, T_AZC);	   
					for (i = 1; i <= 2; i++){
						putcharLCD("Read P");
						while(P3_5);
						i2cCommReadPT(sensor_addr_wr, P_AZC);	   
					}
					break;			
		
				case(1):
					putcharLCD("Read T");
					while(P3_5);
					i2cCommReadPT(sensor_addr_wr, T_AZC);
					for (i = 1; i <= 4; i++){
						putcharLCD("Read P");
						while(P3_5);
						i2cCommReadPT(sensor_addr_wr, P_AZC);	   
					}
			}
			select = 0;
		}

	}
	
}


void Final_Calibration ()
{
	int i, j;
	unsigned char Vref_adtl_addr_wr = 66, Vagnd_adtl_addr_wr = 64;
	unsigned char ADC_addr_wr = 144;	   			/* A/D device address + write */
	unsigned char ADC_ctrlByte = 0;			   	/* A/D device control byte (single-ended channel '0' selection) */
	char DAC_byte[9];

	put2lines("FinalCal", "StrtXcel");

	for (i = 0; i <= 8; i++)
		DAC_byte[i] = getChar();		  			/* Receive the DAC data bytes */

	sensor_cnt = DAC_byte[8];


 /*----------------------------------Coefficients Flashing--------------------------------------------------*/

			ctrlByte = 0;
			readwriteA2D(DigiIO_addr_wr, ctrlByte);	   /* Set the output of 8-bit I/O expander to 
														'0' (sensor 1 is selected through relay) */
			wait(10000);
			for (i = 1; i <= sensor_cnt; i=i+1){
				put2lines("Process", "ing.....");
				i2cCommWriteCoeffts(sensor_addr_wr);
				i2cCommReadCoeffts(sensor_addr_wr);
				++ctrlByte;								  /* Increment the ctrlByte by 1 */
				readwriteA2D(DigiIO_addr_wr, ctrlByte);	  /* Set the output of 8-bit I/O expander equal to the 
														ctrlByte value (next sensor relay is selected) */
				wait(5000);		//3000		/* wait state to stabilize relay after switching */
			}	
			
 /*-----------------------------------Pressure Analog Output Read-Out-------------------------------------------------*/


//			for (j = 1; j <= 5; j=j+4){
//				if(j == 1)
//					put2lines("Analog\0", "Offset\0");
//				else
//					put2lines("Analog\0", "FSO\0");
//				while(P3_5);
//	
//				readwriteA2D(DigiIO_addr_wr, ctrlByte_supply);	   				/* Set the output of 8-bit I/O expander to 
//																			'64'(0x40) (Supply Sensors are enabled) */
//				writeReg(Vref_adtl_addr_wr, set_DAC, DAC_byte[j-1], DAC_byte[j]);		/* Set Vref of ADC with the specified value */
//				writeReg(Vagnd_adtl_addr_wr, set_DAC, DAC_byte[j+1], DAC_byte[j+2]);		/* Set Vagnd of ADC with the specified value */
//				ctrlByte = 0;
//				readwriteA2D(DigiIO_addr_wr, ctrlByte);	   				/* Set the output of 8-bit I/O expander to 
//																			'0' (sensor 1 is selected through relay) */
//				wait(10000);											/* wait state to stabilize relay after switching */
//	
//				for (i = 1; i <= sensor_cnt; i=i+1){
//					readwriteA2D(ADC_addr_wr, ADC_ctrlByte);	   			/* Read the Analog Output */
//					++ctrlByte;								 			/* Increment the ctrlByte by 1 */
//					readwriteA2D(DigiIO_addr_wr, ctrlByte);	  			/* Set the output of 8-bit I/O expander equal to the 
//																			ctrlByte value (next sensor relay is selected) */
//					wait(10000);										/* wait state to stabilize relay after switching */
//				}	
//			}
}


void Final_ReadOut ()
{
	put2lines("FinalP/T", "Read-Out");

	sensor_cnt = getChar();

	put2lines("Logging", "P/T.....");
	i2cComm(sensor_addr_wr);
	
}


void Flash_Settings ()
{
	int i;
	put2lines("Flsh-Set", "StrtXcel");

	sensor_cnt = getChar();		

	ctrlByte = 0;
	readwriteA2D(DigiIO_addr_wr, ctrlByte);		 /* Set the output of 8-bit I/O expander to 
														'0' (sensor 1 is selected through relay) */
	wait(20000);
	for (i = 1; i <= sensor_cnt; i=i+1){
		put2lines("Flashing", "Data....");
		IniSettWrite(sensor_addr_wr);
		IniSettRead(sensor_addr_wr);
		++ctrlByte;								  /* Increment the ctrlByte by 1 */
		readwriteA2D(DigiIO_addr_wr, ctrlByte);	  	/* Set the output of 8-bit I/O expander equal to the 
														ctrlByte value (next sensor relay is selected) */
		wait(5000);			//3000				/* wait state to stabilize relay after switching */
	}	

}

void RawData_Logging ()
{
	put2lines("Raw Data", "Read-Out");

	sensor_cnt = getChar();

	put2lines("Logging", "Data....");
	RawDataLog(sensor_addr_wr);
	
}
