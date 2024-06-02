/**********************************************************************************************************************
 *  MCU as Master, receive data from TTL(UART) then send it to another MCU(Slave) by SPI
 *  -----------------------------------------------------------------------------------------------------------------*/
/**       file:main.c
 *        created: 21/09/2023  
 *
 *       Author : IBRAHIM MADY
 *
 *
 *********************************************************************************************************************/

#define F_CPU 8000000UL
#include <util/delay.h>
#include "app.h"
#include "LCD_interface.h"
#include "SPI.h"
#include "EXINT.h"
#include "UART.h"
#include "StdTypes.h"


/******* MAIN.H  *****************************************************************************************************/
typedef enum
{
	stop,
	start
}operation_type;
#define  ENTER_ASCII 13

void UART_RX_CALL(void);
void UART_TX_CALL(void);
void SPI_CALL(void);
void EX_INT_CALL(void);


/*****************************************   GLOBAL DATA  ***********************************************************/
/*******   BUFFERS  *********/
u8 UART_TO_SPI[50]={0};
u8 SPI_TO_UART[50]={0};
/*******   INDEXES  *********/
volatile u8 UART_TO_SPI_ITER=0;
volatile u8 SPI_TO_UART_ITER=0;
/*******   FLAGS  *********/		
operation_type MASTER_SEND_FLAG=stop;
operation_type MASTER_RECEIVE_FLAG=stop;

volatile u8 temp=0;
volatile u8 size_receive=0;







/*****************************************   MAIN FUNC  ***********************************************************/
int main(void)
{
	/****** INITIATION ****/
	DIO_Init();
	LCD_Init();
	UART_Init();

	
	UART_RX_InterruptEnable();	
	UART_RX_SetCallBack(UART_RX_CALL);
	UART_TX_InterruptEnable();
	UART_TX_SetCallBack(UART_TX_CALL);
	SPI_Init(MASTER);
	EX_INT_SetSenseControl(EX_INT1,FALLING_EDGE);
	EX_INT_Enable(EX_INT1);
	EX_INT_SetCall_Back(EX_INT1,EX_INT_CALL);
	LCD_WriteString("   HELLO PC1  ");
	
	sei();
	
	
	
	while(1)
	{
		while(MASTER_RECEIVE_FLAG == start)
		{
			SPI_TO_UART[SPI_TO_UART_ITER]=SPI_SendReceive(0);
			_delay_us(1000);
			if(SPI_TO_UART[SPI_TO_UART_ITER] == ENTER_ASCII)
			{
				MASTER_RECEIVE_FLAG=stop;
				SPI_TO_UART_ITER=0;
				UART_Send_noBlock(SPI_TO_UART[SPI_TO_UART_ITER]);
				SPI_TO_UART_ITER++;
			}
			else
			{
				SPI_TO_UART_ITER++;
			}
		}
		if(MASTER_SEND_FLAG == start)
		{
			while(UART_TO_SPI[UART_TO_SPI_ITER]!= 0)
			{
				SPI_SendReceive(UART_TO_SPI[UART_TO_SPI_ITER]);
				UART_TO_SPI_ITER++;
				_delay_us(10000);
			}
			UART_TO_SPI_ITER=0;
			SPI_SendReceive(0);
			MASTER_SEND_FLAG=stop;
		}
	}
	return 0;
}
void UART_RX_CALL(void)
{
	static u8 RXvar;
	RXvar=UART_Receive_noBlock();
	UART_TO_SPI[UART_TO_SPI_ITER]=RXvar;
	UART_TO_SPI_ITER++;
	if(RXvar==ENTER_ASCII)
	{
		UART_TO_SPI[UART_TO_SPI_ITER]=0;
		MASTER_SEND_FLAG=start;
		UART_TO_SPI_ITER=0;
	}
}
void EX_INT_CALL(void)
{
	MASTER_RECEIVE_FLAG=start;
	SPI_TO_UART_ITER=0;
	MASTER_SEND_FLAG=stop;
	_delay_us(1000);
}
void UART_TX_CALL(void)
{
	if(SPI_TO_UART[SPI_TO_UART_ITER-1] != ENTER_ASCII )
	{
		UART_Send_noBlock(SPI_TO_UART[SPI_TO_UART_ITER]);
		SPI_TO_UART_ITER++;
	}
	else
	{
		SPI_TO_UART_ITER=0;
	}
}