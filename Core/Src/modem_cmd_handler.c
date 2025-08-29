#include <stdio.h>
#include "cmd_api.h"
#include "modem_api.h"
#include "modem_cmd_handler.h"

bool MODEM_CMD_OkHandler (sCommandParams_t *cmd_params) {
	if (CMD_API_CheckParams(cmd_params)) {
		return false;
	}

	snprintf((char*)cmd_params->reply, cmd_params->reply_length, "MODEM RESPONDED WITH: %s\r\n", cmd_params->params);
	MODEM_API_SetModemStatus(eModemStatus_Ok);

	return true;
}

bool MODEM_CMD_ErrorHandler (sCommandParams_t *cmd_params) {
	if (CMD_API_CheckParams(cmd_params)) {
		return false;
	}

	snprintf((char*)cmd_params->reply, cmd_params->reply_length, "MODEM RESPONDED WITH: %s\r\n", cmd_params->params);
	MODEM_API_SetModemStatus(eModemStatus_Error);

	return true;
}
