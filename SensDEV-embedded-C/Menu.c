/*------------------------------------------------------------------------------
									Menu.c
------------------------------------------------------------------------------*/


//void subMenu1(UCHAR *sub_menu1[])
//{
//	index = 0;
//	loop = 1;
//	put2lines(sub_menu1[index], sub_menu1[index+1]);	
//	
//	while(loop){
//		wait(1000);
//		WriteInstr4bit (0x87);
//		wait(1000);
//		WriteData ('<');
//
//		if(!P3_2){
//			while(!P3_2);
//			if(index > 0)
//				index--;
//			put2lines(sub_menu1[index], sub_menu1[index+1]);
//		}
//
//		if(!P3_3){
//			while(!P3_3);
//			if(index < limit)
//				index++;
//			put2lines(sub_menu1[index], sub_menu1[index+1]);
//		}
//
//
//		if(!P3_5){
//			while(!P3_5);
//			loop = 0;
//			if(sub_menu1[index] == "Offset"){
//				menuSelect(index);
//			}
//			else if(sub_menu1[index] == "FSO"){
//				valIncDec(sub_menu1[index], &source_FSO);
//			}
//			else if(sub_menu1[index] == "P:Low"){
//				valIncDec(sub_menu1[index], &P_Low);
//			}
//			else if(sub_menu1[index] == "P:High"){
//				valIncDec(sub_menu1[index], &P_High);
//			}
//			else if(sub_menu1[index] == "SrcUnit"){
//				menuSelect(1);
//			}
//			else if(sub_menu1[index] == "DstUnit"){
//				menuSelect(1);
//			}
//		}
//	}
//}



//void subMenu2(UCHAR *sub_menu2[])
//{
//	index = 0;
//	loop = 1;
//	put2lines(sub_menu2[index], sub_menu2[index+1]);	
//	
//	while(loop){
//		wait(1000);
//		WriteInstr4bit (0x87);
//		wait(1000);
//		WriteData ('<');
//
//		if(!P3_2){
//			while(!P3_2);
//			if(index > 0)
//				index--;
//			put2lines(sub_menu2[index], sub_menu2[index+1]);
//		}
//
//		if(!P3_3){
//			while(!P3_3);
//			if(index < limit)
//				index++;
//			put2lines(sub_menu2[index], sub_menu2[index+1]);
//		}
//
//
//		if(!P3_5){
//			while(!P3_5);
//			loop = 0;
//			if(sub_menu2[index] == "Auto #0"){
//				auto_zero = 1;
//				Write_Byte_EEPROM (&auto_zero, auto_zero);
//				readPresCnt(sensor_addr_write, read_times_offset, &autoZero_offset);
//				Write_Byte_EEPROM (&autoZero_offset, autoZero_offset);
//			}
//			else if(sub_menu2[index] == "Manual"){
//				auto_zero = 0;
//				Write_Byte_EEPROM (&auto_zero, auto_zero);
//				valIncDec(sub_menu2[index], &manual_offset);
//			}
//			else if(sub_menu2[0] == "bar"){
//				if(subMenu_index == 0)
//					Write_Byte_EEPROM (&src_unit_index, index);
//				else
//					Write_Byte_EEPROM (&dst_unit_index, index);
//			}
//		}
//	}
//}
//
//
//
//
//void menuSelect(UCHAR select)
//{
//	unsigned int value;
//
//	switch(select)
//	{
//		case(0):
//			value = Read_Byte_EEPROM (&auto_zero);
//			if(value == 1)
//				put2lines("Curr.Set", "->Auto#0");
//			else if(value == 0)
//				put2lines("Curr.Set", "->Manual");
//			else
//				put2lines("Curr.Set", "NOT SET!");
//			while(P3_5);
//			while(!P3_5);
//			limit = 1;
//			subMenu2(&(*source_offset));
//			break;			
//
//		case(1):
//			if(subMenu_index == 0)
//				value = Read_Byte_EEPROM (&src_unit_index);
//			else
//				value = Read_Byte_EEPROM (&dst_unit_index);
//
//			if(0 <= value <= 8)
//				put2lines("Curr.Set", unit[value]);
//			else
//				put2lines("Curr.Set", "NOT SET!");
//			while(P3_5);
//			while(!P3_5);
//			limit = 8;
//			subMenu2(&(*unit));
//			break;
//
////		case(2):
////			limit = 1;
////			subMenu2(&(*I2C_menu));
////			break;
////
////		case(3):
////			limit = 3;
////			subMenu2(&(*optoswitch_menu));
////			break;			
////
////		case(4):
////			limit = 2;
////			subMenu2(&(*source_menu));
////			break;
////
////		case(5):
////			limit = 2;
////			subMenu2(&(*source_menu));
////			break;
//	
//	}
//}
//
//
//


void valIncDec(char string[], UINT* param)
{
	char str[10];
	unsigned int value;
	unsigned char count;
	loop = 1;
	
	value = Read_Byte_EEPROM (param);
	itoa(value, str);
	put2lines(string, str);

  	while(loop)
  	{
		count = 0;
   		while(!P3_3){
			wait(30000);
			count++;
			if(count <= 10) 			
				value = value - 1;	   /* Value Decrement */
			else if(count <= 20) 			
				value = value - 10;	   /* Value Decrement */
			else if(count <= 30) 			
				value = value - 100;	   /* Value Decrement */
			else 			
				value = value - 1000;	   /* Value Decrement */
			itoa(value, str);
			put2lines(string, str);
   		}
			
   		while(!P3_2){
			wait(30000);
			count++;
			if(count <= 10) 			
				value = value + 1;	   /* Value Decrement */
			else if(count <= 20) 			
				value = value + 10;	   /* Value Decrement */
			else if(count <= 30) 			
				value = value + 100;	   /* Value Decrement */
			else 			
				value = value + 1000;	   /* Value Decrement */
			itoa(value, str);
			put2lines(string, str);
   		}
   			
	
		if(!P3_5){
			while(!P3_5);
			Write_Byte_EEPROM (param, value);
			loop = 0;
 		}
			
 	}
}



// Return EEPROM 8-bit value at address 'adr'
UINT Read_Byte_EEPROM (UINT adr) small{

  unsigned char high_byte, low_byte;
  unsigned int value;
  while(EECON&1);
  EA = 0;
  EECON = 0x02;    						/*  enable EEPROM */
  high_byte = *((unsigned char xdata *)adr++);    /*  read value	*/
  low_byte = *((unsigned char xdata *)adr);    /*  read value	*/
  EECON = 0x00;    						/*  disable EEPROM	*/
  EA = 1;
  value = high_byte*256+low_byte;
  return(value);
}

// Write EEPROM 8-bit 'val' at address 'adr'
void Write_Byte_EEPROM (UINT adr, UINT val) small{

  unsigned char byte_val;
  unsigned char n = 2;
  while(n--){
  	  while(EECON&1);
	  EA = 0;
	  EECON = 0x02;							 /* enable EEPROM */
	  if(n==1){
	  	byte_val = val/256;
	  *((unsigned char xdata *)adr++) = byte_val;  /* write value */
	  }
	  else{
	  	byte_val = val%256;
	  *((unsigned char xdata *)adr) = byte_val;  /* write value */
	  }
	  EECON = 0x00;      					/*  disable EEPROM */
	  EA = 1;
  }
}
