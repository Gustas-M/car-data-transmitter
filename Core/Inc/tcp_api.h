#ifndef INC_TCP_API_H_
#define INC_TCP_API_H_

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

typedef struct {
	uint8_t *cmd;
	uint32_t delay;
	size_t repeat;
} sTcpCmdData_t;

typedef enum {
	eTcpCmd_CpinCheck,
	eTcpCmd_CsqCheck,
	eTcpCmd_CregCheck,
	eTcpCmd_CgdCont,
	eTcpCmd_CgAtt,
	eTcpCmd_CgattCheck,
	eTcpCmd_CgAct,
	eTcpCmd_CipStart,
	eTcpCmd_CipSend,
	eTcpCmd_CipClose,
	eTcpCmd_CipShut,
	eTcpCmd_CgDeact,
	eTcpCmd_CgDeatt,
	eTcpCmd_Last
} eTcpCmd_t;


bool TCP_API_Connect (uint8_t *ip, uint16_t port, uint8_t socket);
bool TCP_API_Disconnect (uint8_t socket);
bool TCP_API_Send (uint8_t *data, size_t data_length);

#endif /* INC_TCP_API_H_ */
