#include <string.h>
#include <stdio.h>
#include "cmd_api.h"
#include "cli_handler.h"
#include "data_sending_app.h"
#include "heap_api.h"

typedef enum {
    eHandlerCode_First = 0,
    eHandlerCode_Internal = eHandlerCode_First,
    eHandlerCode_InvalidInput,
    eHandlerCode_ParameterOutOfRange,
    eHandlerCode_TaskQueueFull,
    eHandlerCode_MissingParameter,
    eHandlerCode_Last
} eHandlerCode_t;


static void CLI_HandleResponse (uint8_t *response_buffer, size_t buffer_size, eHandlerCode_t error_code) {
    if ((response_buffer == NULL) || (buffer_size == 0) || (error_code < eHandlerCode_First) || (error_code >= eHandlerCode_Last)) {
        return;
    }
    switch (error_code) {
        case eHandlerCode_Internal: {
            snprintf((char*)response_buffer, buffer_size, "Internal Error\r");
            break;
        }
        case eHandlerCode_InvalidInput: {
            snprintf((char*)response_buffer, buffer_size, "Invalid Input\r");
            break;
        }
        case eHandlerCode_ParameterOutOfRange: {
            snprintf((char*)response_buffer, buffer_size, "Invalid input: parameter out of range.\r");
            break;
        }
        case eHandlerCode_TaskQueueFull: {
            snprintf((char*)response_buffer, buffer_size, "Timeout: task queue is full.\r");
            break;
        }
        case eHandlerCode_MissingParameter: {
            snprintf((char*)response_buffer, buffer_size, "Invalid input: missing parameter\r");
            break;
        }
        default: {
        	break;
        }
    }
}


bool CLI_StartTCPHandler(sCommandParams_t *cmd_params) {
    if (CMD_API_CheckParams(cmd_params) == false) {
        return false;
    }

    char *params = (char*)cmd_params->params;

    const char *command_delimiter = ",";
	char *saveptr = NULL;
	char *token = NULL;
	token = strtok_r((char*)params, command_delimiter, &saveptr);

    if (token == NULL) {
        CLI_HandleResponse(cmd_params->reply, cmd_params->reply_length, eHandlerCode_MissingParameter);
        return false;
    }

    char ip[16] = {0};
    strncpy(ip, token, sizeof(token) - 1);

	token = strtok_r((char*)params, command_delimiter, &saveptr);

    if (token == NULL) {
        CLI_HandleResponse(cmd_params->reply, cmd_params->reply_length, eHandlerCode_MissingParameter);
        return false;
    }

    uint16_t port = atoi(token);

	token = strtok_r((char*)params, command_delimiter, &saveptr);

    if (token == NULL) {
        CLI_HandleResponse(cmd_params->reply, cmd_params->reply_length, eHandlerCode_MissingParameter);
        return false;
    }

    uint16_t socket = atoi(token);

    sNetworkAppConnParams_t *tcp_params = (sNetworkAppConnParams_t*)HEAP_API_Malloc(sizeof(sNetworkAppConnParams_t));
    if (tcp_params == NULL) {
    	CLI_HandleResponse(cmd_params->reply, cmd_params->reply_length, eHandlerCode_Internal);
        return false;
    }

    strncpy((char*)tcp_params->ip, ip, sizeof(tcp_params->ip) - 1);
    tcp_params->ip[sizeof(tcp_params->ip) - 1] = '\0';
    tcp_params->port = port;
    tcp_params->socket = socket;

    sDataSendingTask_t tcp_task;
    tcp_task.task = eDataSendingTask_Start;
    tcp_task.params = tcp_params;
    if (DataSending_APP_AddTask(&tcp_task) == false) {
    	CLI_HandleResponse(cmd_params->reply, cmd_params->reply_length, eHandlerCode_TaskQueueFull);
        HEAP_API_Free(tcp_params);
        return false;
    }

    snprintf((char*)cmd_params->reply, cmd_params->reply_length, "Starting TCP connection to %s:%u\r", ip, port);
    return true;
}

bool CLI_StopTCPHandler(sCommandParams_t *cmd_params) {
	sDataSendingTask_t params = {0};
	params.task = eDataSendingTask_Stop;
	params.params = NULL;

    if (DataSending_APP_AddTask(&params) == false) {
    	CLI_HandleResponse(cmd_params->reply, cmd_params->reply_length, eHandlerCode_TaskQueueFull);
        return false;
    }

    snprintf((char*)cmd_params->reply, cmd_params->reply_length, "Stoping TCP connection\r");
    return true;
}
