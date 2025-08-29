#ifndef INC_GNSS_API_H_
#define INC_GNSS_API_H_

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct {
	double timestamp;
	double lat;
	double lon;
	double speed;
} sGnssData_t;

bool GNSS_API_Init (void);
void GNSS_API_FormatData (char* buffer, size_t buffer_size);
void GNSS_API_UpdateCoordinates (double timestamp, double latitude, double longitude);
void GNSS_API_UpdateSpeed (double speed);

#endif /* INC_GNSS_API_H_ */
