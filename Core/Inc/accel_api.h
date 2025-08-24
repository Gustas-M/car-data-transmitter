#ifndef INC_ACCEL_API_H_
#define INC_ACCEL_API_H_

#include <stdbool.h>
#include <stdint.h>

#define MAX_CONF_COUNT 8

typedef enum {
	eAccelApiConf_First,
	eAccelApiConf_PowerCtl = eAccelApiConf_First,
	eAccelApiConf_DataFormat,
	eAccelApiConf_Last
} eAccelApiConf_t;

typedef struct {
	uint8_t reg;
	uint8_t val;
} sAccelRegConfData_t;

typedef struct {
	uint16_t x_val;
	uint16_t y_val;
	uint16_t z_val;
} sAccelVal_t;

bool ACCEL_API_Init (void);
bool ACCEL_API_GetAcceleration (sAccelVal_t *axis_val);

#endif /* INC_ACCEL_API_H_ */
