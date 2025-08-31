#ifndef INC_ACCEL_API_H_
#define INC_ACCEL_API_H_

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

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
} sAccelData_t;

bool ACCEL_API_Init (void);
void ACCEL_API_FormatAcceleration (uint8_t *buffer, size_t buffer_size);

#endif /* INC_ACCEL_API_H_ */
