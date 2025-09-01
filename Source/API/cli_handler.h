#ifndef INC_CLI_HANDLER_H_
#define INC_CLI_HANDLER_H_

#include <stdbool.h>
#include "cli_handler.h"
#include "cmd_api.h"

bool CLI_StartTCPHandler(sCommandParams_t *cmd_params);
bool CLI_StopTCPHandler(sCommandParams_t *cmd_params);

#endif /* INC_CLI_HANDLER_H_ */
