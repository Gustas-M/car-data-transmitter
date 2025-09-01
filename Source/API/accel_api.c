#include <stdint.h>
#include <stdio.h>
#include "accel_api.h"
#include "i2c_driver.h"

#define ACCEL_I2C_PORT eI2cPort_Accelerometer

#define AXIS_VAL_REG 0x32
#define AXIS_REG_NUM 6

#define SCALE_FACTOR 0.0039f

#define POWER_CTL_REG 0x2D
#define POWER_CTL_VAL 0x08
#define DATA_FORMAT_REG 0x31
#define DATA_FORMAT_VAL 0x00

static sAccelRegConfData_t static_accel_api_conf_lut[eAccelApiConf_Last] = {
	[eAccelApiConf_PowerCtl] = {
		.reg = POWER_CTL_REG,
		.val = POWER_CTL_VAL
	},
	[eAccelApiConf_DataFormat] = {
		.reg = DATA_FORMAT_REG,
		.val = DATA_FORMAT_VAL
	}
};

static bool ACCEL_API_WriteConfiguration (void);
static bool ACCEL_API_GetAcceleration (sAccelData_t *accel_data);

bool ACCEL_API_Init (void) {
	if (I2C_Driver_Init(ACCEL_I2C_PORT) == false) {
		return false;
	}

	if (ACCEL_API_WriteConfiguration() == false) {
		return false;
	}

	return true;
}

void ACCEL_API_FormatAcceleration (uint8_t *buffer, size_t buffer_size) {
	if ((buffer == NULL) || (buffer_size <= 0)) {
		return;
	}

	sAccelData_t accel_data = {0};

	if (ACCEL_API_GetAcceleration(&accel_data) == false) {
		return;
	}

	int accel_x_int = (int)accel_data.x_val;
	int accel_x_dec = (int)((accel_data.x_val - accel_x_int) * 100);
	int accel_y_int = (int)accel_data.y_val;
	int accel_y_dec = (int)((accel_data.y_val - accel_y_int) * 100);

    snprintf((char*)buffer, buffer_size, "acceleration_x: %d.%02d, acceleration_y: %d.%02d", accel_x_int, accel_x_dec, accel_y_int, accel_y_dec);
}

static bool ACCEL_API_WriteConfiguration (void) {
	for (uint8_t reg_data_idx = eAccelApiConf_First; reg_data_idx < eAccelApiConf_Last; reg_data_idx++) {
		if (I2C_Driver_Write(ACCEL_I2C_PORT, &static_accel_api_conf_lut[reg_data_idx].reg, 1) == false) {
			return false;
		}
		if (I2C_Driver_Write(ACCEL_I2C_PORT, &static_accel_api_conf_lut[reg_data_idx].val, 1) == false) {
			return false;
		}
	}

	return true;
}

static bool ACCEL_API_GetAcceleration (sAccelData_t *accel_data) {
	uint8_t axis_data[6];

	for (uint8_t axis_reg_idx = 0; axis_reg_idx < AXIS_REG_NUM; axis_reg_idx++) {
		if (I2C_Driver_Write(ACCEL_I2C_PORT, (uint8_t*)(AXIS_VAL_REG + axis_reg_idx), 1) == false) {
			return false;
		}

		if (I2C_Driver_Read(ACCEL_I2C_PORT, &axis_data[axis_reg_idx], 1) == false) {
			return false;
		}
	}

	int16_t raw_x_val = (int16_t)(axis_data[1] << 8) | axis_data[0];
	int16_t raw_y_val = (int16_t)(axis_data[3] << 8) | axis_data[2];
	int16_t raw_z_val = (int16_t)(axis_data[5] << 8) | axis_data[4];

	accel_data->x_val = raw_x_val * SCALE_FACTOR;
	accel_data->y_val = raw_y_val * SCALE_FACTOR;
	accel_data->z_val = raw_z_val * SCALE_FACTOR;

	return true;
}
