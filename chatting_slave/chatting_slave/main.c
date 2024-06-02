/**********************************************************************************************************************
 *  MCU as Slave, receive data from MCU(Master) by SPI then send it to TTL(UART)
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
#include "LCD_interface.h"
#include "SPI.h"
#include "EXINT.h"
#include "UART.h"

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
void send_signal(void);
void SPI_RECEIVE_CALL (void);
void SPI_SEND_CALL (void);


/*****************************************   GLOBAL DATA  ***********************************************************/
/*******   BUFFERS  *********/
u8 UART_TO_SPI[50]={0};
u8 SPI_TO_UART[50]={0};
/*******   INDEXES  *********/
u8 UART_TO_SPI_ITER=0;
u8 SPI_TO_UART_ITER=0;
/*******   FLAGS  *********/
operation_type SLAVE_RECEIVE_FLAG=start;
operation_type SLAVE_SEND_FLAG=stop;

volatile u8 size_send=0;




/*****************************************   MAIN FUNC  ***********************************************************/
int main(void)
{
	/****** INITIATION ****/
	DIO_Init();
	LCD_Init();
	UART_Init();
	UART_RX_InterruptEnable();
	UART_RX_SetCallBack(UART_RX_CALL);
// 	UART_TX_SetCallBack(UART_TX_CALL);
// 	UART_TX_InterruptEnable();
	SPI_Init(SLAVE);
// 	SPI_InterruptEnable();
// 	SPI_SetCallback(SPI_RECEIVE_CALL);
	LCD_WriteString("     HELLO PC2");
	sei();
	
	while(1)
	{		
		while(SLAVE_SEND_FLAG==start)
		{
			SPI_SendReceive(UART_TO_SPI[UART_TO_SPI_ITER]);
			if(UART_TO_SPI[UART_TO_SPI_ITER]==ENTER_ASCII)
			{
				UART_TO_SPI_ITER=0;
				SLAVE_RECEIVE_FLAG=start;
				SLAVE_SEND_FLAG=stop;
				break;
			}
			else
			{
				UART_TO_SPI_ITER++;
			}
		}
		while(SLAVE_RECEIVE_FLAG==start)
		{
			UART_Send_noBlock(SPI_SendReceive(SPI_TO_UART[SPI_TO_UART_ITER]));
			if(SLAVE_SEND_FLAG==start)
			{
				SPI_TO_UART_ITER++;
				break;
			}
		}
	}
	return 0;
}


void UART_RX_CALL(void)
{
	
	static u8 RXvar;
	RXvar=UART_Receive_noBlock();
	UART_TO_SPI[UART_TO_SPI_ITER]=RXvar;
	if(UART_TO_SPI[UART_TO_SPI_ITER]==ENTER_ASCII)
	{
		SLAVE_SEND_FLAG=start;
		UART_TO_SPI_ITER=-1;	
		send_signal();	
	}
	else
	{
		UART_TO_SPI_ITER++;
	}
}
void send_signal(void)
{
	SLAVE_SEND_FLAG=start;
	DIO_WritePin(PINA0,HIGH);
	_delay_us(5);
	DIO_WritePin(PINA0,LOW);
}



