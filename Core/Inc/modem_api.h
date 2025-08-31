#ifndef INC_MODEM_API_H_
#define INC_MODEM_API_H_

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

typedef enum {
	eModemStatus_Ok,
	eModemStatus_Error,
	eModemStatus_TimeOut,
	eModemStatus_Busy,
	eModemStatus_Unknown
} eModemStatus_t;

bool MODEM_API_Init (void);
eModemStatus_t MODEM_API_SendAndWait (uint8_t *cmd, uint16_t wait_time);
eModemStatus_t MODEM_API_RepeatedSendAndWait (uint8_t *cmd, uint16_t wait_time, size_t repeat, uint16_t delay);
bool MODEM_API_Lock (void);
bool MODEM_API_Unlock (void);
void MODEM_API_SetModemStatus (eModemStatus_t status);
bool MODEM_API_ReceiveFromQueue (void *message, uint8_t wait_time);
bool MODEM_API_PutToQueue (void *message, uint8_t wait_time);

#endif /* INC_MODEM_API_H_ */
