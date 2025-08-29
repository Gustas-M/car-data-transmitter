#ifndef INC_GNSS_HANDLER_H_
#define INC_GNSS_HANDLER_H_

#include <stdbool.h>
#include "cmd_api.h"

typedef enum {
	eGnssCmd_Gngga,
	eGnssCmd_Gnvtg,
	eGnssCmd_Last,
} eGnssCmd_t;

bool GNSS_GnggaHandler (sCommandParams_t *cmd_params);
bool GNSS_GnvtgHandler (sCommandParams_t *cmd_params);

#endif /* INC_GNSS_HANDLER_H_ */
