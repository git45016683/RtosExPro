/*
    FreeRTOS V9.0.0 - Copyright (C) 2016 Real Time Engineers Ltd.
    All rights reserved

    VISIT http://www.FreeRTOS.org TO ENSURE YOU ARE USING THE LATEST VERSION.

    This file is part of the FreeRTOS distribution.

    FreeRTOS is free software; you can redistribute it and/or modify it under
    the terms of the GNU General Public License (version 2) as published by the
    Free Software Foundation >>>> AND MODIFIED BY <<<< the FreeRTOS exception.

    ***************************************************************************
    >>!   NOTE: The modification to the GPL is included to allow you to     !<<
    >>!   distribute a combined work that includes FreeRTOS without being   !<<
    >>!   obliged to provide the source code for proprietary components     !<<
    >>!   outside of the FreeRTOS kernel.                                   !<<
    ***************************************************************************

    FreeRTOS is distributed in the hope that it will be useful, but WITHOUT ANY
    WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
    FOR A PARTICULAR PURPOSE.  Full license text is available on the following
    link: http://www.freertos.org/a00114.html

    ***************************************************************************
     *                                                                       *
     *    FreeRTOS provides completely free yet professionally developed,    *
     *    robust, strictly quality controlled, supported, and cross          *
     *    platform software that is more than just the market leader, it     *
     *    is the industry's de facto standard.                               *
     *                                                                       *
     *    Help yourself get started quickly while simultaneously helping     *
     *    to support the FreeRTOS project by purchasing a FreeRTOS           *
     *    tutorial book, reference manual, or both:                          *
     *    http://www.FreeRTOS.org/Documentation                              *
     *                                                                       *
    ***************************************************************************

    http://www.FreeRTOS.org/FAQHelp.html - Having a problem?  Start by reading
    the FAQ page "My application does not run, what could be wrong?".  Have you
    defined configASSERT()?

    http://www.FreeRTOS.org/support - In return for receiving this top quality
    embedded software for free we request you assist our global community by
    participating in the support forum.

    http://www.FreeRTOS.org/training - Investing in training allows your team to
    be as productive as possible as early as possible.  Now you can receive
    FreeRTOS training directly from Richard Barry, CEO of Real Time Engineers
    Ltd, and the world's leading authority on the world's leading RTOS.

    http://www.FreeRTOS.org/plus - A selection of FreeRTOS ecosystem products,
    including FreeRTOS+Trace - an indispensable productivity tool, a DOS
    compatible FAT file system, and our tiny thread aware UDP/IP stack.

    http://www.FreeRTOS.org/labs - Where new FreeRTOS products go to incubate.
    Come and try FreeRTOS+TCP, our new open source TCP/IP stack for FreeRTOS.

    http://www.OpenRTOS.com - Real Time Engineers ltd. license FreeRTOS to High
    Integrity Systems ltd. to sell under the OpenRTOS brand.  Low cost OpenRTOS
    licenses offer ticketed support, indemnification and commercial middleware.

    http://www.SafeRTOS.com - High Integrity Systems also provide a safety
    engineered and independently SIL3 certified version for use in safety and
    mission critical applications that require provable dependability.

    1 tab == 4 spaces!
*/

/*
	BASIC INTERRUPT DRIVEN SERIAL PORT DRIVER FOR UART0.
*/

/* Scheduler includes. */
#include "FreeRTOS.h"
#include "queue.h"
#include "semphr.h"

/* Library includes. */
//#include "stm32f10x_lib.h"
#include "main.h"
extern UART_HandleTypeDef huart2;
extern DMA_HandleTypeDef hdma_usart2_rx;
uint8_t aRxBuffer;

/* Demo application includes. */
#include "serial.h"
/*-----------------------------------------------------------*/

/* Misc defines. */
#define serINVALID_QUEUE				( ( QueueHandle_t ) 0 )
#define serNO_BLOCK						( ( TickType_t ) 0 )
#define serTX_BLOCK_TIME				( 40 / portTICK_PERIOD_MS )

/*-----------------------------------------------------------*/

/* The queue used to hold received characters. */
static QueueHandle_t xRxedChars;
static QueueHandle_t xCharsForTx;

/*-----------------------------------------------------------*/

///* UART interrupt handler. */
//void vUARTInterruptHandler( void );

/*-----------------------------------------------------------*/

/*
 * See the serial2.h header file.
 */
xComPortHandle xSerialPortInitMinimal( unsigned long ulWantedBaud, unsigned portBASE_TYPE uxQueueLength )
{
	xComPortHandle xReturn;

	/* Create the queues used to hold Rx/Tx characters. */
	xRxedChars = xQueueCreate( uxQueueLength, ( unsigned portBASE_TYPE ) sizeof( signed char ) );
	xCharsForTx = xQueueCreate( uxQueueLength + 1, ( unsigned portBASE_TYPE ) sizeof( signed char ) );
	
	/* If the queue/semaphore was created correctly then setup the serial port
	hardware. */
	if( ( xRxedChars != serINVALID_QUEUE ) && ( xCharsForTx != serINVALID_QUEUE ) )
	{
		#if V_UART_DMA_RECEIVE  // 空闲中断+DMA接收版本
		__HAL_UART_ENABLE_IT(&huart2, UART_IT_IDLE);  // 开启串口空闲中断
		HAL_UART_Receive_DMA(&huart2, uart2_recv_buff, RECV_BUFF_MAX);  // 重启开始DMA传输 每次255字节数据
		#else  // 中断接收版本
		HAL_UART_Receive_IT(&huart2, &aRxBuffer, 1);  // 开启中断接收
		#endif
	}
	else
	{
		xReturn = ( xComPortHandle ) 0;
	}

	/* This demo file only supports a single port but we have to return
	something to comply with the standard demo header file. */
	return xReturn;
}
/*-----------------------------------------------------------*/

signed portBASE_TYPE xSerialGetChar( xComPortHandle pxPort, signed char *pcRxedChar, TickType_t xBlockTime )
{
	/* The port handle is not required as this driver only supports one port. */
	( void ) pxPort;

	/* Get the next character from the buffer.  Return false if no characters
	are available, or arrive before xBlockTime expires. */
	if( xQueueReceive( xRxedChars, pcRxedChar, xBlockTime ) )
	{
		return pdTRUE;
	}
	else
	{
		return pdFALSE;
	}
}
/*-----------------------------------------------------------*/

void vSerialPutString( xComPortHandle pxPort, const signed char * const pcString, unsigned short usStringLength )
{
signed char *pxNext;

	/* A couple of parameters that this port does not use. */
	( void ) usStringLength;
	( void ) pxPort;

	/* NOTE: This implementation does not handle the queue being full as no
	block time is used! */

	/* The port handle is not required as this driver only supports UART1. */
	( void ) pxPort;

	/* Send each character in the string, one at a time. */
	pxNext = ( signed char * ) pcString;
	while( *pxNext )
	{
		xSerialPutChar( pxPort, *pxNext, serNO_BLOCK );
		pxNext++;
	}
}
/*-----------------------------------------------------------*/

signed portBASE_TYPE xSerialPutChar( xComPortHandle pxPort, signed char cOutChar, TickType_t xBlockTime )
{
signed portBASE_TYPE xReturn;

	if( xQueueSend( xCharsForTx, &cOutChar, xBlockTime ) == pdPASS )
	{
		xReturn = pdPASS;
		uint8_t cChar;
		if (xQueueReceive(xCharsForTx, &cChar, 0) == pdTRUE)
		{
			// 两种发送方式均可
			while(HAL_UART_Transmit_IT(&huart2, &cChar, 1) != HAL_OK);
//			if ((HAL_UART_GetState(&huart2) & HAL_UART_STATE_BUSY_TX) != HAL_UART_STATE_BUSY_TX)
//			{
//				HAL_UART_Transmit(&huart2, &cChar, 1, 1000);  // 1S超时，轮询发送
//			}
		}
	}
	else
	{
		xReturn = pdFAIL;
	}

	return xReturn;
}
/*-----------------------------------------------------------*/

void vSerialClose( xComPortHandle xPort )
{
	/* Not supported as not required by the demo application. */
}
/*-----------------------------------------------------------*/

#if V_UART_DMA_RECEIVE  // 空闲中断+DMA接收版本
uint8_t data_len = 0;
void UART2_IDLECallback(UART_HandleTypeDef* huart)
{
	HAL_UART_DMAStop(&huart2);  // 停止本次DMA传输
	
	data_len = RECV_BUFF_MAX - __HAL_DMA_GET_COUNTER(&hdma_usart2_rx);  // 计算接收到的数据长度
	
	uart2_recv_buff[data_len] = '\0';
	
	portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;
	for (uint8_t i = 0; i < data_len; i++)
	{
		xQueueSendFromISR(xRxedChars, &uart2_recv_buff[i], &xHigherPriorityTaskWoken);
	}
	
	HAL_UART_Receive_DMA(&huart2, (uint8_t*)uart2_recv_buff, RECV_BUFF_MAX);  // 重启开始DMA传输 每次255字节数据
	portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}
#else  // 中断接收版本
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if (huart->Instance == USART2)
	{
		portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;
		xQueueSendFromISR(xRxedChars, &aRxBuffer, &xHigherPriorityTaskWoken);
		HAL_UART_Receive_IT(&huart2, &aRxBuffer, 1);
		portEND_SWITCHING_ISR( xHigherPriorityTaskWoken );
	}
}
#endif
//void vUARTInterruptHandler( void )
//{
//portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;
//char cChar;

//	if( USART_GetITStatus( USART1, USART_IT_TXE ) == SET )
//	{
//		/* The interrupt was caused by the THR becoming empty.  Are there any
//		more characters to transmit? */
//		if( xQueueReceiveFromISR( xCharsForTx, &cChar, &xHigherPriorityTaskWoken ) == pdTRUE )
//		{
//			/* A character was retrieved from the queue so can be sent to the
//			THR now. */
//			USART_SendData( USART1, cChar );
//		}
//		else
//		{
//			USART_ITConfig( USART1, USART_IT_TXE, DISABLE );		
//		}		
//	}
//	
//	if( USART_GetITStatus( USART1, USART_IT_RXNE ) == SET )
//	{
//		cChar = USART_ReceiveData( USART1 );
//		xQueueSendFromISR( xRxedChars, &cChar, &xHigherPriorityTaskWoken );
//	}	
//	
//	portEND_SWITCHING_ISR( xHigherPriorityTaskWoken );
//}





	
