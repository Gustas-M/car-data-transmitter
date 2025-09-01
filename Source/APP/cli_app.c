#include <string.h>
#include "cmsis_os.h"
#include "cli_handler.h"
#include "uart_api.h"
#include "heap_api.h"
#include "cmd_api.h"
#include "cli_app.h"

#define CLI_APP_THREAD_STACK_SIZE 512
#define CLI_RESPONSE_BUFFER_SIZE 512

#define CLI_UART_PORT eUartApiPort_Usart2

#define CLI_APP_THREAD_NO_ARGS NULL

#define DEFINE_CMD(cmd, cmd_handler, sep) { \
	.command_name = (uint8_t*)#cmd, \
	.command_name_size = sizeof(#cmd) - 1, \
	.function = &cmd_handler, \
	.separator = (uint8_t*)sep, \
} \

typedef enum {
    eCliCmd_StartTCP,
    eCliCmd_StopTCP,
    eCliCmd_Last,
} eCliCmd_t;

static const sCommandDesc_t command_table[eCliCmd_Last] = {
    DEFINE_CMD(start_tcp, CLI_StartTCPHandler, ":"),
    DEFINE_CMD(stop_tcp, CLI_StopTCPHandler, ""),
};

static const osThreadAttr_t cli_app_thread_attr = {
    .name = "CLI Task",
    .stack_size = CLI_APP_THREAD_STACK_SIZE,
    .priority = osPriorityAboveNormal,
};

static osThreadId_t cli_app_thread = NULL;
static uint8_t cli_response_buffer[CLI_RESPONSE_BUFFER_SIZE] = {0};
static sMessage_t received_message = {0};
static sCmdLauncherParams_t command_parser = {
    .command_table = command_table,
    .command_table_size = eCliCmd_Last,
    .reply = cli_response_buffer,
    .reply_size = CLI_RESPONSE_BUFFER_SIZE
};

static void CLI_APP_Task (void *arguments);

bool CLI_APP_Init (void) {
    if (cli_app_thread == NULL) {
    	cli_app_thread = osThreadNew(CLI_APP_Task, CLI_APP_THREAD_NO_ARGS, &cli_app_thread_attr);
    	if (cli_app_thread == NULL) {
			return false;
		}
    }

    return true;
}


static void CLI_APP_Task (void *arguments) {
    while (1) {
        if (UART_API_Receive(CLI_UART_PORT, &received_message, osWaitForever) == false) {
            continue;
        }
        if (CMD_API_Process(&received_message, &command_parser) == false) {
            //debug_err
        }

        HEAP_API_Free(received_message.message);
    }
}
