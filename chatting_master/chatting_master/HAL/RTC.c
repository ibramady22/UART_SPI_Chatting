/*
 * RTC.c
 *
 * Created: 
 *  Author: 
 */ 


#include "RTC_Config.h"
#include "RTC_Private.h"
#include "RTC_Interface.h"
#include <util/delay.h>
void RTC_init()
{
	
	I2C_StartCondition();
	I2C_Send_SlaveAddressWriteOperation(RTC_Address);
	I2C_WriteByte(0x07);
	I2C_Send_SlaveAddressReadOperation(0x00);
	I2C_StopCondition();
}

void RTC_write(date*x)
{
	//TWI_voidInitMaster(0);
	//enable start condition
	I2C_StartCondition();
	// send address of Rtc with write
	I2C_Send_SlaveAddressWriteOperation(RTC_Address);
	// this is first add location for Rtc (Note that the address inc  automatically)
	I2C_Send_SlaveAddressWriteOperation(0x00); 
	// send data (sec , minuet , hour) 
	I2C_WriteByte(x->sec);
	I2C_WriteByte(x->min);
	I2C_WriteByte(x->hour);
	I2C_StopCondition();
}


date RTC_read(date*x)
{
	//TWI_voidInitMaster(0);
	I2C_StartCondition();
	I2C_Send_SlaveAddressWriteOperation(RTC_Address);
	
	I2C_WriteByte(0x00); // Set the address register to 0x00
	I2C_RepeatedStartCondition();
	I2C_Send_SlaveAddressReadOperation(RTC_Address);
	I2C_ReadByte(x->sec);

	I2C_ReadByte(x->min);
	
	I2C_ReadByte(x->hour);
	_delay_ms(100);
	I2C_StopCondition();
	
	//TWI_SendStopCondition();
}


/*
date RTC_read(date*x)
{
	TWI_voidInitMaster(0);
	TWI_SendStartCondition();
	TWI_SendSlaveAddressWithWrite(RTC_Address);
	TWI_SendRepeatedStart();
	TWI_SendSlaveAddressWithRead(0x00);
	TWI_MasterReadDataByte(x->sec);
	TWI_MasterReadDataByte(x->min);
	TWI_MasterReadDataByte(x->hour);
	TWI_SendStopCondition();
}*/
