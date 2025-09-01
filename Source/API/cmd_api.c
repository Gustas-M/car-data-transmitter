/**********************************************************************************************************************
 * Includes
 *********************************************************************************************************************/
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include "cmd_api.h"
/**********************************************************************************************************************
 * Private definitions and macros
 *********************************************************************************************************************/
#define COMMAND_SEPERATOR ':'

#define NO_SEPERATOR -1
#define EMPTY_MESSAGE 0
/**********************************************************************************************************************
 * Private typedef
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Private constants
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Private variables
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Exported variables and references
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Prototypes of private functions
 *********************************************************************************************************************/
int CMD_API_FindSeperator (uint8_t *message, uint8_t seperator);
/**********************************************************************************************************************
 * Definitions of private functions
 *********************************************************************************************************************/
int CMD_API_FindSeperator (uint8_t *message, uint8_t seperator) {
    char *delimiter_pos = strchr((char*)message, seperator);

    if (delimiter_pos == NULL) {
        return NO_SEPERATOR;
    } else if ((uint8_t*)delimiter_pos == message) {
        return EMPTY_MESSAGE;
    }

    return (uint8_t*)delimiter_pos - message + 1;
}
/**********************************************************************************************************************
 * Definitions of exported functions
 *********************************************************************************************************************/
bool CMD_API_Process (sMessage_t *received_message, sCmdLauncherParams_t *cmd_params) {
    if ((received_message->message == NULL) || (received_message->message_length <= 0)) {
        return false;
    }

    for (size_t cmd = 0; cmd < cmd_params->command_table_size; cmd++) {
        const sCommandDesc_t *command = &cmd_params->command_table[cmd];
        int seperator_pos = CMD_API_FindSeperator(received_message->message, *command->separator);

		if (seperator_pos == NO_SEPERATOR) {
			snprintf((char*)cmd_params->reply, cmd_params->reply_size, "No seperator was found\r\n");
			return false;
		} else if (seperator_pos == EMPTY_MESSAGE) {
			snprintf((char*)cmd_params->reply, cmd_params->reply_size, "An empty message was received\r\n");
			return false;
		}

        if ((command->command_name_size + 1 == seperator_pos) && (strncmp((char*)command->command_name, (char*)received_message->message, command->command_name_size) == 0)) {
            if (command->function == NULL) {
                snprintf((char*)cmd_params->reply, cmd_params->reply_size, "Command does not have a function\r\n");
                return false;
            }

            sCommandParams_t params = {
                .params = received_message->message + seperator_pos,
                .params_length = received_message->message_length - seperator_pos,
            };

            command->function(&params);
            return true;
        }
    }

    snprintf((char*)cmd_params->reply, cmd_params->reply_size, "Such command does not exist\r\n");
    return false;
}

bool CMD_API_CheckParams (sCommandParams_t *cmd_params) {
	if (cmd_params == NULL) {
		return false;
	}

	if ((cmd_params->params == NULL) || (cmd_params->params_length == 0)) {
		return false;
	}

	return true;
}
