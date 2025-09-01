#include <string.h>
#include <stdio.h>
#include "tcp_api.h"
#include "modem_api.h"

#define TCP_API_TIMEOUT 500
#define END_OF_DATA_CHAR "0x1A"

const static sTcpCmdData_t static_tcp_cmd_lut[eTcpCmd_Last] = {
	[eTcpCmd_CpinCheck] = {.cmd = (uint8_t*)"AT+CPIN?\r\n", .delay = 0, .repeat = 1},
	[eTcpCmd_CsqCheck] = {.cmd = (uint8_t*)"AT+CSQ\r\n", .delay = 0, .repeat = 1},
	[eTcpCmd_CregCheck] = {.cmd = (uint8_t*)"AT+CREG?\r\n", .delay = 0, .repeat = 1},
	[eTcpCmd_CgAtt] = {.cmd = (uint8_t*)"AT+CGATT=1\r\n", .delay = 500, .repeat = 3},
	[eTcpCmd_CgattCheck] = {.cmd = (uint8_t*)"AT+CGATT?\r\n", .delay = 0, .repeat = 1},
	[eTcpCmd_CgdCont] = {.cmd = (uint8_t*)"AT+CGDCONT=1,\"IP\",\"ezys\"\r\n", .delay = 500, .repeat = 3},
	[eTcpCmd_CgAct] = {.cmd = (uint8_t*)"AT+CGACT=1,%d\r\n", .delay = 500, .repeat = 3},
	[eTcpCmd_CipStart] = {.cmd = (uint8_t*)"AT+CIPSTART=\"TCP\",\"%s\",\"%d\"\r\n", .delay = 2500, .repeat = 5},
	[eTcpCmd_CipSend] = {.cmd = (uint8_t*)"AT+CIPSEND\r\n", .delay = 0, .repeat = 1},
	[eTcpCmd_CipClose] = {.cmd = (uint8_t*)"AT+CIPCLOSE\r\n", .delay = 1000, .repeat = 3},
	[eTcpCmd_CipShut] = {.cmd = (uint8_t*)"AT+CIPSHUT\r\n", .delay = 500, .repeat = 3},
	[eTcpCmd_CgDeact] = {.cmd = (uint8_t*)"AT+CGACT=0,%d\r\n", .delay = 500, .repeat = 3},
	[eTcpCmd_CgDeatt] = {.cmd = (uint8_t*)"AT+CGATT=0\r\n", .delay = 500, .repeat = 3}
};

static char command_buffer[256] = {0};


bool TCP_API_Connect (uint8_t *ip, uint16_t port, uint8_t socket) {
	if (MODEM_API_Lock() == false) {
		return false;
	}

	eModemStatus_t status;

	status = MODEM_API_RepeatedSendAndWait(static_tcp_cmd_lut[eTcpCmd_CpinCheck].cmd, TCP_API_TIMEOUT, static_tcp_cmd_lut[eTcpCmd_CpinCheck].repeat, static_tcp_cmd_lut[eTcpCmd_CpinCheck].delay);
	if (status != eModemStatus_Ok) {
		MODEM_API_Unlock();
		return false;
	}

	status = MODEM_API_RepeatedSendAndWait(static_tcp_cmd_lut[eTcpCmd_CsqCheck].cmd, TCP_API_TIMEOUT, static_tcp_cmd_lut[eTcpCmd_CsqCheck].repeat, static_tcp_cmd_lut[eTcpCmd_CsqCheck].delay);
	if (status != eModemStatus_Ok) {
		goto error;
	}

	status = MODEM_API_RepeatedSendAndWait(static_tcp_cmd_lut[eTcpCmd_CregCheck].cmd, TCP_API_TIMEOUT, static_tcp_cmd_lut[eTcpCmd_CregCheck].repeat, static_tcp_cmd_lut[eTcpCmd_CregCheck].delay);
	if (status != eModemStatus_Ok) {
		goto error;
	}

	status = MODEM_API_RepeatedSendAndWait(static_tcp_cmd_lut[eTcpCmd_CgAtt].cmd, TCP_API_TIMEOUT, static_tcp_cmd_lut[eTcpCmd_CgAtt].repeat, static_tcp_cmd_lut[eTcpCmd_CgAtt].delay);
	if (status != eModemStatus_Ok) {
		goto error;
	}

	status = MODEM_API_RepeatedSendAndWait(static_tcp_cmd_lut[eTcpCmd_CgattCheck].cmd, TCP_API_TIMEOUT, static_tcp_cmd_lut[eTcpCmd_CgattCheck].repeat, static_tcp_cmd_lut[eTcpCmd_CgattCheck].delay);
	if (status != eModemStatus_Ok) {
		goto error;
	}

	status = MODEM_API_RepeatedSendAndWait(static_tcp_cmd_lut[eTcpCmd_CgdCont].cmd, TCP_API_TIMEOUT, static_tcp_cmd_lut[eTcpCmd_CgdCont].repeat, static_tcp_cmd_lut[eTcpCmd_CgdCont].delay);
	if (status != eModemStatus_Ok) {
		goto error;
	}

	snprintf(command_buffer, sizeof(command_buffer), (const char*)static_tcp_cmd_lut[eTcpCmd_CgAct].cmd, socket);
	status = MODEM_API_RepeatedSendAndWait((uint8_t*)command_buffer, TCP_API_TIMEOUT, static_tcp_cmd_lut[eTcpCmd_CgAct].repeat, static_tcp_cmd_lut[eTcpCmd_CgAct].delay);
	if (status != eModemStatus_Ok) {
		goto error;
	}

	snprintf(command_buffer, sizeof(command_buffer), (const char*)static_tcp_cmd_lut[eTcpCmd_CipStart].cmd, ip, port);
	status = MODEM_API_RepeatedSendAndWait((uint8_t*)command_buffer, TCP_API_TIMEOUT, static_tcp_cmd_lut[eTcpCmd_CipStart].repeat, static_tcp_cmd_lut[eTcpCmd_CipStart].delay);
	if (status != eModemStatus_Ok) {
		goto error;
	}

	MODEM_API_Unlock();
	return true;

error:
	MODEM_API_Unlock();
	return false;
}

bool TCP_API_Disconnect (uint8_t socket) {
	if (MODEM_API_Lock() == false) {
		return false;
	}

	eModemStatus_t status;

	status = MODEM_API_RepeatedSendAndWait(static_tcp_cmd_lut[eTcpCmd_CipClose].cmd, TCP_API_TIMEOUT, static_tcp_cmd_lut[eTcpCmd_CipClose].repeat, static_tcp_cmd_lut[eTcpCmd_CipClose].delay);
	if (status != eModemStatus_Ok) {
		goto error;
	}

	status = MODEM_API_RepeatedSendAndWait(static_tcp_cmd_lut[eTcpCmd_CipShut].cmd, TCP_API_TIMEOUT, static_tcp_cmd_lut[eTcpCmd_CipShut].repeat, static_tcp_cmd_lut[eTcpCmd_CipShut].delay);
	if (status != eModemStatus_Ok) {
		goto error;
	}

	snprintf(command_buffer, sizeof(command_buffer), (const char*)static_tcp_cmd_lut[eTcpCmd_CgDeact].cmd, socket);
	status = MODEM_API_RepeatedSendAndWait((uint8_t*)command_buffer, TCP_API_TIMEOUT, static_tcp_cmd_lut[eTcpCmd_CgDeact].repeat, static_tcp_cmd_lut[eTcpCmd_CgDeact].delay);
	if (status != eModemStatus_Ok) {
		goto error;
	}

	status = MODEM_API_RepeatedSendAndWait(static_tcp_cmd_lut[eTcpCmd_CgDeatt].cmd, TCP_API_TIMEOUT, static_tcp_cmd_lut[eTcpCmd_CgDeatt].repeat, static_tcp_cmd_lut[eTcpCmd_CgDeatt].delay);
	if (status != eModemStatus_Ok) {
		goto error;
	}

	MODEM_API_Unlock();
	return true;

error:
	MODEM_API_Unlock();
	return false;
}

bool TCP_API_Send (uint8_t *data, size_t data_length) {
	if (MODEM_API_Lock() == false) {
		return false;
	}

	eModemStatus_t status;

	status = MODEM_API_RepeatedSendAndWait(static_tcp_cmd_lut[eTcpCmd_CipSend].cmd, TCP_API_TIMEOUT, static_tcp_cmd_lut[eTcpCmd_CipSend].repeat, static_tcp_cmd_lut[eTcpCmd_CipSend].delay);
	if (status != eModemStatus_Ok) {
		goto error;
	}

	strncpy(command_buffer, (char*)data, sizeof(command_buffer));
	strncat(command_buffer, END_OF_DATA_CHAR, 1);

	status = MODEM_API_RepeatedSendAndWait((uint8_t*)command_buffer, TCP_API_TIMEOUT, 3, 500);
	if (status != eModemStatus_Ok) {
		goto error;
	}

	MODEM_API_Unlock();
	return true;

error:
	MODEM_API_Unlock();
	return false;
}
