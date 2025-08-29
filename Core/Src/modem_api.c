#include "cmsis_os.h"
#include "modem_api.h"
#include "gnss_api.h"
#include "cmd_api.h"
#include "uart_api.h"
#include "modem_cmd_handler.h"

#define MODEM_UART_PORT eUartApiPort_Usart6
#define MODEM_UART_BAUDRATE 9600
#define MODEM_UART_DELIMITER "\r\n"
#define MODEM_UART_DELIMITER_LENGTH (sizeof(MODEM_UART_DELIMITER) - 1)
#define MODEM_RESPONSE_LENGTH 1024
#define MODEM_API_QUEUE_MSG_SIZE 10
#define MODEM_API_THREAD_STACK_SIZE 512
#define MODEM_API_NO_ATTR NULL

#define DEFINE_CMD(cmd, cmd_handler, sep) { \
	.command_name = (uint8_t*)#cmd, \
	.command_name_size = sizeof(#cmd) - 1, \
	.function = &cmd_handler, \
	.separator = (uint8_t*)sep, \
} \

static const osThreadAttr_t modem_api_thread_attr = {
	.name = "MODEM API Thread",
	.stack_size = MODEM_API_THREAD_STACK_SIZE,
	.priority = osPriorityNormal
};

static const sCommandDesc_t static_modem_cmd_lut[eModemCmd_Last] = {
	DEFINE_CMD(OK, MODEM_CMD_OkHandler, ""),
	DEFINE_CMD(ERROR, MODEM_CMD_ErrorHandler, "")
};

static osThreadId_t modem_api_thread = NULL;
static osEventFlagsId_t modem_api_event_flag = NULL;
static osMutexId_t modem_api_mutex = NULL;
static osMessageQueueId_t modem_api_queue = NULL;
static eModemStatusEnum_t last_command_status = eModemStatus_Error;
sMessage_t modem_message = {0};
static uint8_t modem_response_buffer[MODEM_RESPONSE_LENGTH] = {0};
static sCmdLauncherParams_t command_parser = {
	.command_table = static_modem_cmd_lut,
	.command_table_size = eModemCmd_Last,
	.reply = modem_response_buffer,
	.reply_size = MODEM_RESPONSE_LENGTH,
};

static void MODEM_API_Task (void *argument);

bool MODEM_API_Init (void) {
	if (UART_API_Init(MODEM_UART_PORT, MODEM_UART_BAUDRATE, MODEM_UART_DELIMITER, MODEM_UART_DELIMITER_LENGTH) == false) {
		return false;
	}

	if (modem_api_event_flag == NULL) {
		modem_api_event_flag = osEventFlagsNew(MODEM_API_NO_ATTR);
		if (modem_api_event_flag == NULL) {
			return false;
		}
	}

	if (modem_api_mutex == NULL) {
		modem_api_mutex = osMutexNew(MODEM_API_NO_ATTR);
		if (modem_api_mutex == NULL) {
			return false;
		}
	}

	if (modem_api_queue == NULL) {
		modem_api_queue = osMessageQueueNew(MODEM_API_QUEUE_MSG_SIZE, sizeof(sMessage_t), MODEM_API_NO_ATTR);
		if (modem_api_queue == NULL) {
			return false;
		}
	}

	if (modem_api_thread == NULL) {
		modem_api_thread = osThreadNew(MODEM_API_Task, NULL, &modem_api_thread_attr);
		if (modem_api_thread == NULL) {
			return false;
		}
	}

	return true;
}

static void MODEM_API_Task (void *argument) {
	while (1) {
		if (UART_API_Receive(MODEM_UART_PORT, &modem_message, osWaitForever) == false) {
			continue;
		}

		if (CMD_API_Process(&modem_message, &command_parser) == true) {

		} else {
			//debug_err
			sMessage_t reply = {.message = command_parser.reply, .message_length = command_parser.reply_size};
			UART_API_Send(eUartApiPort_Usart2, &reply, osWaitForever);
		}

		HEAP_API_Free(modem_message.message);
	}
}

eModemStatus_t MODEM_API_SendAndWait (uint8_t *cmd, uint8_t wait_time) {
	if ((cmd == NULL) || (wait_time <= 0)) {
		return eModemStatus_Error;
	}

	UART_API_Send(MODEM_UART_PORT, cmd, wait_time);
	uint32_t flag = osEventFlagsWait(modem_api_event_flag, 0x01, osFlagsWaitAny, wait_time);
	eModemStatus_t result = (flags & 0x01) ? last_command_status : eModemStatus_TimeOut;
	return result;
}

eModemStatus_t MODEM_API_RepeatedSendAndWait (uint8_t *cmd, uint8_t wait_time, size_t repeat, uint8_t delay) {
	if ((cmd == NULL) || (wait_time <= 0) || (repeat <= 0)) {
		return eModemStatus_Error;
	}

	eModemStatus_t result;

	while (repeat > 0) {
		result = MODEM_API_SendAndWait(cmd, wait_time);
		if (result != eModemStatus_Error) {
			return result;
		}

		repeat--;
		if (repeat > 0) {
			osDelay(delay);
		}
	}

	return result;
}

bool Modem_API_Lock (void) {
    if (g_modem_mutex_id == NULL) {
        return false;
    }
    if (osMutexAcquire(g_modem_mutex_id, MODEM_MUTEX_WAIT_TIME) == osOK) {
        return true;
    } else {
        return false;
    }
}

bool Modem_API_Unlock (void) {
    if (g_modem_mutex_id == NULL) {
        return false;
    }
    if (osMutexRelease(g_modem_mutex_id) == osOK) {
        return true;
    } else {
        return false;
    }
}

void MODEM_API_SetModemStatus (eModemStatus_t status) {
	last_command_status = status;
	osEventFlagsSet(modem_api_event_flag, 0x01);
}

bool MODEM_API_ReceiveFromQueue (void *message, uint8_t wait_time) {
	if ((message == NULL) || (wait_time <= 0)) {
		return false;
	}

	return true;
}

bool MODEM_API_PutToQueue (void *message, uint8_t wait_time) {

}
