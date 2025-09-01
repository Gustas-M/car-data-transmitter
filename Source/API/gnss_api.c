#include <stdio.h>
#include "cmsis_os.h"
#include "gnss_api.h"
#include "cmd_api.h"
#include "uart_api.h"
#include "gnss_handler.h"

#define GNSS_UART_PORT eUartApiPort_Usart1
#define GNSS_UART_BAUDRATE 9600
#define GNSS_UART_DELIMITER "\r\n"
#define GNSS_UART_DELIMITER_LENGTH (sizeof(GNSS_UART_DELIMITER) - 1)
#define GNSS_RESPONSE_LENGTH 512
#define GNSS_API_THREAD_STACK_SIZE 512

#define DEFINE_CMD(cmd, cmd_handler, sep) { \
	.command_name = (uint8_t*)#cmd, \
	.command_name_size = sizeof(#cmd) - 1, \
	.function = &cmd_handler, \
	.separator = (uint8_t*)sep, \
} \

static const osThreadAttr_t gnss_api_thread_attr = {
	.name = "GNSS API Thread",
	.stack_size = GNSS_API_THREAD_STACK_SIZE,
	.priority = osPriorityBelowNormal
};

static const sCommandDesc_t static_gnss_cmd_lut[eGnssCmd_Last] = {
	DEFINE_CMD($GNGGA, GNSS_GnggaHandler, ","),
	DEFINE_CMD($GNVTG, GNSS_GnvtgHandler, ","),
};

static osThreadId_t gnss_api_thread = NULL;
sMessage_t gnss_nmea_message = {0};
static uint8_t gnss_response_buffer[GNSS_RESPONSE_LENGTH] = {0};
static sCmdLauncherParams_t command_parser = {
	.command_table = static_gnss_cmd_lut,
	.command_table_size = eGnssCmd_Last,
	.reply = gnss_response_buffer,
	.reply_size = GNSS_RESPONSE_LENGTH,
};

static sGnssData_t current_gnss_data = {0};

static void GNSS_API_Task (void *argument);
static void GNSS_API_FormatTime (double numeric_time, char *buffer, size_t buffer_size);

bool GNSS_API_Init (void) {
	if (UART_API_Init(GNSS_UART_PORT, 0, GNSS_UART_DELIMITER, GNSS_UART_DELIMITER_LENGTH) == false) {
		return false;
	}

	if (gnss_api_thread == NULL) {
		gnss_api_thread = osThreadNew(GNSS_API_Task, NULL, &gnss_api_thread_attr);
		if (gnss_api_thread == NULL) {
			return false;
		}
	}

	return true;
}

static void GNSS_API_Task (void *argument) {
	while (1) {
		if (UART_API_Receive(GNSS_UART_PORT, &gnss_nmea_message, osWaitForever) == false) {
			continue;
		}

		if (CMD_API_Process(&gnss_nmea_message, &command_parser) == false) {
			//debug_err
			sMessage_t reply = {.message = command_parser.reply, .message_length = command_parser.reply_size};
//			UART_API_Send(eUartApiPort_Usart2, &reply, osWaitForever);
		}

		HEAP_API_Free(gnss_nmea_message.message);
	}
}

void GNSS_API_UpdateCoordinates (double timestamp, double latitude, double longitude) {
	current_gnss_data.timestamp = timestamp;
	current_gnss_data.lat = latitude;
	current_gnss_data.lon = longitude;
}

void GNSS_API_UpdateSpeed (double speed) {
	current_gnss_data.speed = speed;
}

void GNSS_API_FormatGNSS (uint8_t *buffer, size_t buffer_size) {
    if ((buffer == NULL) || (buffer_size == 0)) {
        return;
    }
    char formatted_time[9] = {0};
    GNSS_API_FormatTime(current_gnss_data.timestamp, formatted_time, sizeof(formatted_time));

    int lat_int = (int)current_gnss_data.lat;
    int lat_dec = (int)((current_gnss_data.lat - lat_int) * 1000000);
    int lon_int = (int)current_gnss_data.lon;
    int lon_dec = (int)((current_gnss_data.lon - lon_int) * 1000000);

    snprintf((char*)buffer, buffer_size, "timestamp: %s, latitude: %d.%06d, longitude: %d.%06d", formatted_time, lat_int, lat_dec, lon_int, lon_dec);
}

static void GNSS_API_FormatTime (double numeric_time, char *buffer, size_t buffer_size) {
    if (buffer == NULL || buffer_size < 9) {
        return;
    }

    int hours = (int) numeric_time / 10000;
    int minutes = ((int) numeric_time % 10000) / 100;
    int seconds = (int) numeric_time % 100;

    snprintf(buffer, buffer_size, "%02d.%02d.%02d", hours, minutes, seconds);
}

