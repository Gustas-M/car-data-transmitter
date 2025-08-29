#ifndef INC_UART_API_H_
#define INC_UART_API_H_

/**********************************************************************************************************************
 * Includes
 *********************************************************************************************************************/
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "cmsis_os.h"
#include "message.h"
#include "heap_api.h"
/**********************************************************************************************************************
 * Exported definitions and macros
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Exported types
 *********************************************************************************************************************/
 typedef enum {
     eUartApiPort_First = 0,
     eUartApiPort_Usart1 = eUartApiPort_First,
     eUartApiPort_Usart2,
	 eUartApiPort_Usart6,
     eUartApiPort_Last
 } eUartApiPort_t;
/**********************************************************************************************************************
 * Exported variables
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Prototypes of exported functions
 *********************************************************************************************************************/
bool UART_API_Init (eUartApiPort_t port, uint32_t baudrate, const char *delimiter, size_t delimiter_length);
bool UART_API_Send (eUartApiPort_t port, sMessage_t *message, uint32_t wait_time);
bool UART_API_Receive (eUartApiPort_t port, sMessage_t *message, uint32_t wait_time);

#endif /* INC_UART_API_H_ */
