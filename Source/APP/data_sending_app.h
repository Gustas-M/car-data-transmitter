#ifndef INC_DATA_SENDING_APP_H_
#define INC_DATA_SENDING_APP_H_

#include <stdint.h>
#include <stdbool.h>
#include "network_app.h"

typedef enum {
	eDataSendingTask_First = 0,
	eDataSendingTask_Start = eDataSendingTask_First,
	eDataSendingTask_Stop,
	eDataSendingTask_Last
} eDataSendingTask_t;

typedef struct {
	eDataSendingTask_t task;
	sNetworkAppConnParams_t *params;
} sDataSendingTask_t;

bool DataSending_APP_Init (void);
bool DataSending_APP_AddTask (sDataSendingTask_t *task);

#endif /* INC_DATA_SENDING_APP_H_ */
