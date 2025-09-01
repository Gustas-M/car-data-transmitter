#include <stdio.h>
#include <string.h>
#include "cmsis_os.h"
#include "data_sending_app.h"
#include "gnss_api.h"
#include "accel_api.h"
#include "heap_api.h"

#define DATA_SENDING_APP_THREAD_STACK_SIZE 512
#define DATA_SENDING_APP_QUEUE_MSG_SIZE 10

#define DATA_SENDING_APP_DEFAULT_ATTR_BITS 0
#define DATA_SENDING_APP_DEFAULT_CB_MEM NULL
#define DATA_SENDING_APP_DEFAULT_CB_SIZE 0
#define DATA_SENDING_APP_DEFAULT_MQ_MEM NULL
#define DATA_SENDING_APP_DEFAULT_MQ_SIZE 0
#define DATA_SENDING_APP_NO_ARG NULL
#define DATA_SENDING_APP_MQ_TIMEOUT 250

#define DATA_SENDING_TIMER_PERIOD 5000

#define MESSAGE_PRIORITY 0

static const osThreadAttr_t data_sending_thread_attr = {
	.name = "Data Sending App",
	.stack_size = DATA_SENDING_APP_THREAD_STACK_SIZE,
	.priority = osPriorityNormal
};

static const osMessageQueueAttr_t data_sending_app_mq_attr = {
	.name = "Network App Message Queue",
	.attr_bits = DATA_SENDING_APP_DEFAULT_ATTR_BITS,
	.cb_mem = DATA_SENDING_APP_DEFAULT_CB_MEM,
	.cb_size = DATA_SENDING_APP_DEFAULT_CB_SIZE,
	.mq_mem = DATA_SENDING_APP_DEFAULT_MQ_MEM,
	.mq_size = DATA_SENDING_APP_DEFAULT_MQ_SIZE
};

static const osTimerAttr_t data_sending_timer_attr = {
	.name = "Data Sending App Callback timer",
	.attr_bits = DATA_SENDING_APP_DEFAULT_ATTR_BITS,
	.cb_mem = DATA_SENDING_APP_DEFAULT_CB_SIZE,
	.cb_size = DATA_SENDING_APP_DEFAULT_CB_SIZE
};

static osThreadId_t data_sending_thread = NULL;
static osMessageQueueId_t data_sending_mq = NULL;
static osTimerId_t data_sending_timer = NULL;

static uint8_t data_buffer[256] = {0};
static sNetworkAppTaskParams_t network_task = {0};
static sNetworkAppConnParams_t network_conn_params = {0};
static sNetworkAppSendParams_t network_send_params = {.data = NULL, .data_length = 0};

static sDataSendingTask_t received_task = {0};

static void DataSending_APP_Callback (void *arguments);
static void DataSending_APP_Task (void *arguments);
static void DataSending_APP_FormatMessage (void);

bool DataSending_APP_Init (void) {
	if (data_sending_mq == NULL) {
		data_sending_mq = osMessageQueueNew(DATA_SENDING_APP_QUEUE_MSG_SIZE, sizeof(sDataSendingTask_t), &data_sending_app_mq_attr);
		if (data_sending_mq == NULL) {
			return false;
		}
	}

	if (data_sending_timer == NULL) {
		data_sending_timer = osTimerNew(DataSending_APP_Callback, osTimerPeriodic, DATA_SENDING_APP_NO_ARG, &data_sending_timer_attr);
		if (data_sending_timer == NULL) {
			return false;
		}
	}

	if (data_sending_thread == NULL) {
		data_sending_thread = osThreadNew(DataSending_APP_Task, DATA_SENDING_APP_NO_ARG, &data_sending_thread_attr);
		if (data_sending_thread == NULL) {
			return false;
		}
	}

	return true;
}

bool DataSending_APP_AddTask (sDataSendingTask_t *task) {
	if ((task->params == NULL) || (task->task < eDataSendingTask_First) || (task->task >= eDataSendingTask_Last)) {
		return false;
	}

	if (data_sending_mq == NULL) {
		return false;
	}

	if (osMessageQueuePut(data_sending_mq, task, MESSAGE_PRIORITY, DATA_SENDING_APP_MQ_TIMEOUT) != osOK) {
		return false;
	}

	return true;
}

static void DataSending_APP_Callback (void *arguments) {
	DataSending_APP_FormatMessage();
    network_send_params.data = (uint8_t*)data_buffer;
    network_send_params.data_length = strlen((char*)data_buffer);
    network_task.params = &network_conn_params;
    network_task.task = eNetworkTask_Connect;
    Network_APP_AddTask(&network_task);
    network_task.params = &network_send_params;
    network_task.task = eNetworkTask_Send;
    Network_APP_AddTask(&network_task);
    network_task.params = NULL;
    network_task.task = eNetworkTask_Disconnect;
    Network_APP_AddTask(&network_task);

}

static void DataSending_APP_Task (void *arguments) {
    while (1) {
        if (osMessageQueueGet(data_sending_mq, &received_task, NULL, osWaitForever) != osOK) {
            continue;
        }

        if (received_task.params == NULL) {
            continue;
        }

        switch (received_task.task) {
            case eDataSendingTask_Start: {
                strncpy((char*)network_conn_params.ip, (char*)received_task.params->ip, sizeof(network_conn_params.ip) - 1);
                network_conn_params.ip[sizeof(network_conn_params.ip) - 1] = '\0';
                network_conn_params.port = received_task.params->port;
                network_conn_params.socket = received_task.params->socket;
                osTimerStart(data_sending_timer, DATA_SENDING_TIMER_PERIOD);
                break;
            }
            case eDataSendingTask_Stop: {
                osTimerStop(data_sending_timer);
                break;
            }
            default: {
            	break;
            }
        }

        HEAP_API_Free(received_task.params);
    }

}

static void DataSending_APP_FormatMessage (void) {
	uint8_t gnss_data[128] = {0};
	uint8_t accel_data[32] = {0};

	GNSS_API_FormatGNSS(gnss_data, sizeof(gnss_data));
	ACCEL_API_FormatAcceleration(accel_data, sizeof(accel_data));

	snprintf((char*)data_buffer, sizeof(data_buffer), "{%s, %s}\r\n", gnss_data, accel_data);
}
