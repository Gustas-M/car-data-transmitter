#include <stdio.h>
#include <string.h>
#include "gnss_handler.h"
#include "gnss_api.h"

#define SPEED_KMH_POS 4
#define CHECKSUM_SYMBOL '*'
#define MESSAGE_LENGTH 64

static bool GNSS_IsNmeaValid (sCommandParams_t *cmd_params, eGnssCmd_t message_type);

bool GNSS_GnggaHandler (sCommandParams_t *cmd_params) {
	if (!CMD_API_CheckParams(cmd_params)) {
		snprintf((char*)cmd_params->reply, cmd_params->reply_length, "Invalid command parameters.\r");
		return false;
	}

	if (GNSS_IsNmeaValid(cmd_params, eGnssCmd_Gngga) == false) {
		snprintf((char*)cmd_params->reply, cmd_params->reply_length, "NMEA message is invalid.\r\n");
		return false;
	}

	const char *delimiter = ",";
	char *saveptr = NULL;
	char *token = strtok_r((char*) cmd_params->params, delimiter, &saveptr);
	if (token == NULL) {
		snprintf((char*)cmd_params->reply, cmd_params->reply_length, "Missing timestamp parameter.\r");
		return false;
	}
	double timestamp = atof(token);
	token = strtok_r(NULL, delimiter, &saveptr);
	if (token == NULL) {
		snprintf((char*)cmd_params->reply, cmd_params->reply_length, "Missing latitude parameter.\r");
		return false;
	}
	double lat_deg = atoi(token) / 100;
	double lat_min = atof(token) - (lat_deg * 100);
	double latitude = lat_deg + (lat_min / 60);
	token = strtok_r(NULL, delimiter, &saveptr);
	if (token == NULL || (*token != 'N' && *token != 'S')) {
		snprintf((char*)cmd_params->reply, cmd_params->reply_length, "Invalid latitude direction.\r");
		return false;
	}
	if (*token == 'S') {
		latitude = -latitude;
	}
	token = strtok_r(NULL, delimiter, &saveptr);
	if (token == NULL) {
		snprintf((char*)cmd_params->reply, cmd_params->reply_length, "Missing longitude parameter.\r");
		return false;
	}
	double lon_deg = atoi(token) / 100;
	double lon_min = atof(token) - (lon_deg * 100);
	double longitude = lon_deg + (lon_min / 60);
	token = strtok_r(NULL, delimiter, &saveptr);
	if (token == NULL || (*token != 'E' && *token != 'W')) {
		snprintf((char*)cmd_params->reply, cmd_params->reply_length, "Invalid longitude direction.\r");
		return false;
	}
	if (*token == 'W') {
		longitude = -longitude;
	}

	GNSS_API_UpdateCoordinates(timestamp, latitude, longitude);
	return true;
}

bool GNSS_GnvtgHandler (sCommandParams_t *cmd_params) {
	if (CMD_API_CheckParams(cmd_params) == false) {
		snprintf((char*)cmd_params->reply, cmd_params->reply_length, "Invalid command parameters.\r\n");
		return false;
	}

	if (GNSS_IsNmeaValid(cmd_params, eGnssCmd_Gnvtg) == false) {
		snprintf((char*)cmd_params->reply, cmd_params->reply_length, "NMEA message is invalid.\r\n");
		return false;
	}

	const char *delimiter = ",";
	char *saveptr = NULL;
	char *token = strtok_r((char*) cmd_params->params, delimiter, &saveptr);
	for (size_t i = 0; i < SPEED_KMH_POS; i++) {
		token = strtok_r(NULL, delimiter, &saveptr);
	}

	if (token == NULL) {
		snprintf((char*)cmd_params->reply, cmd_params->reply_length, "Missing speed parameter.\r");
		return false;
	}

	double speed = atof(token);

	GNSS_API_UpdateSpeed(speed);
	return true;
}


static bool GNSS_IsNmeaValid (sCommandParams_t *cmd_params, eGnssCmd_t message_type) {
	char *checksum_pos = strrchr((char*)cmd_params->params, CHECKSUM_SYMBOL);
	checksum_pos++;
	int checksum_val = (int)strtol(checksum_pos, NULL, 16);

	if (checksum_pos == NULL) {
		return false;
	}

	char full_message[MESSAGE_LENGTH] = {0};
	switch (message_type) {
		case eGnssCmd_Gngga: {
			strncpy(full_message, "GNGGA,", MESSAGE_LENGTH);
			break;
		}
		case eGnssCmd_Gnvtg: {
			strncpy(full_message, "GNVTG,", MESSAGE_LENGTH);
			break;
		}
		default:
			break;
	}

	strncat(full_message, (const char*)cmd_params->params, cmd_params->params_length);
	int checksum = 0;

	char *ptr = full_message;

	while (*ptr && (*ptr != CHECKSUM_SYMBOL)) {
		checksum ^= (unsigned char)(*ptr);
		ptr++;
	}

	return (checksum == checksum_val);
}
