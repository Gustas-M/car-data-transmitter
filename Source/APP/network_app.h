#ifndef INC_NETWORK_APP_H_
#define INC_NETWORK_APP_H_

#include <stdint.h>
#include <stdbool.h>

typedef enum {
	eNetworkTask_First,
	eNetworkTask_Connect = eNetworkTask_First,
	eNetworkTask_Send,
	eNetworkTask_Disconnect,
	eNetworkTask_Last
} eNetworkTask_t;

typedef struct {
	uint8_t ip[16];
	uint16_t port;
	uint8_t socket;
} sNetworkAppConnParams_t;

typedef struct {
	uint8_t *data;
	size_t data_length;
} sNetworkAppSendParams_t;

typedef struct {
	eNetworkTask_t task;
	void *params;
} sNetworkAppTaskParams_t;

bool Network_APP_Init (void);
bool Network_APP_AddTask (sNetworkAppTaskParams_t *task_params);

#endif /* INC_NETWORK_APP_H_ */
