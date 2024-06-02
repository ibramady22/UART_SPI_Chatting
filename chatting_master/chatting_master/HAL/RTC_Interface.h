/*
 * RTC_Interface.h
 *
 * Created: 9/26/2023 12:19:54 AM
 *  Author: Ahmed Nour
 */ 


#ifndef RTC_INTERFACE_H_
#define RTC_INTERFACE_H_

#include "StdTypes.h"
#include "i2c.h"

typedef struct
{
	u8	sec;
	u8	min;
	u8	hour;
}date;

void RTC_init();
void RTC_write(date*x);

date RTC_read(date*x);

#endif /* RTC_INTERFACE_H_ */