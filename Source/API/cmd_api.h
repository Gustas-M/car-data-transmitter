#ifndef INC_CMD_API_H_
#define INC_CMD_API_H_

/**********************************************************************************************************************
 * Includes
 *********************************************************************************************************************/
#include <stdbool.h>
#include <stdint.h>
#include "message.h"
/**********************************************************************************************************************
 * Exported definitions and macros
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Exported types
 *********************************************************************************************************************/
typedef struct {
    uint8_t *params;
    size_t params_length;
    uint8_t *reply;
    size_t reply_length;
} sCommandParams_t;

typedef bool (*CommandFunction_t)(sCommandParams_t *params);

typedef struct {
    uint8_t *command_name;
    size_t command_name_size;
    CommandFunction_t function;
    uint8_t *separator;
} sCommandDesc_t;

typedef struct {
    const sCommandDesc_t *command_table;
    size_t command_table_size;
    uint8_t *reply;
    size_t reply_size;
} sCmdLauncherParams_t;
/**********************************************************************************************************************
 * Exported variables
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Prototypes of exported functions
 *********************************************************************************************************************/
bool CMD_API_Process (sMessage_t *received_message, sCmdLauncherParams_t *params);
bool CMD_API_CheckParams (sCommandParams_t *cmd_params);

#endif /* INC_CMD_API_H_ */
