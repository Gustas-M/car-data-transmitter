#ifndef INC_MODEM_CMD_HANDLER_H_
#define INC_MODEM_CMD_HANDLER_H_

typedef enum {
	eModemCmd_Ok,
	eModemCmd_ConnectOk,
	eModemCmd_SendOk,
	eModemCmd_Error,
	eModemCmd_WriteOk,
	eModemCmd_Last,
} eModemCmd_t;

bool MODEM_CMD_OkHandler (sCommandParams_t *cmd_params);
bool MODEM_CMD_ErrorHandler (sCommandParams_t *cmd_params);

#endif /* INC_MODEM_CMD_HANDLER_H_ */
