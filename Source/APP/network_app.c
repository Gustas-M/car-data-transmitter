#include "cmsis_os.h"
#include "tcp_api.h"
#include "network_app.h"
#include "message.h"

#define NETWORK_APP_THREAD_STACK_SIZE 512
#define NETWORK_APP_QUEUE_MSG_SIZE 10

#define NETWORK_APP_DEFAULT_ATTR_BITS 0
#define NETWORK_APP_DEFAULT_CB_MEM NULL
#define NETWORK_APP_DEFAULT_CB_SIZE 0
#define NETWORK_APP_DEFAULT_MQ_MEM NULL
#define NETWORK_APP_DEFAULT_MQ_SIZE 0
#define NETWORK_APP_THREAD_NO_ARG NULL

#define MESSAGE_PRIORITY 0

static const osThreadAttr_t network_app_thread_attr = {
	.name = "Network App Thread",
	.stack_size = NETWORK_APP_THREAD_STACK_SIZE,
	.priority = osPriorityNormal
};

static const osMessageQueueAttr_t network_app_mq_attr = {
	.name = "Network App Message Queue",
	.attr_bits = NETWORK_APP_DEFAULT_ATTR_BITS,
	.cb_mem = NETWORK_APP_DEFAULT_CB_MEM,
	.cb_size = NETWORK_APP_DEFAULT_CB_SIZE,
	.mq_mem = NETWORK_APP_DEFAULT_MQ_MEM,
	.mq_size = NETWORK_APP_DEFAULT_MQ_SIZE
};

static osThreadId_t network_app_thread = NULL;
static osMessageQueueId_t network_app_mq = NULL;
static sNetworkAppTaskParams_t received_task = {0};
static uint8_t socket = 0;

static void Network_APP_Task (void *arguments);

bool Network_APP_Init (void) {
	if (network_app_mq == NULL) {
		network_app_mq = osMessageQueueNew(NETWORK_APP_QUEUE_MSG_SIZE, sizeof(sMessage_t), &network_app_mq_attr);
		if (network_app_mq == NULL) {
			return false;
		}
	}

	if (network_app_thread == NULL) {
		network_app_thread = osThreadNew(Network_APP_Task, NETWORK_APP_THREAD_NO_ARG, &network_app_thread_attr);
		if (network_app_thread == NULL) {
			return false;
		}
	}

	return true;
}

bool Network_APP_AddTask (sNetworkAppTaskParams_t *task_params) {
	if ((task_params->params == NULL) || (task_params->task < eNetworkTask_First) || (task_params->task >= eNetworkTask_Last)) {
		return false;
	}

	if (osMessageQueuePut(network_app_mq, task_params, MESSAGE_PRIORITY, osWaitForever) != osOK) {
		return false;
	}

	return true;
}

static void Network_APP_Task (void *argument) {
    while (1) {
        if (osMessageQueueGet(network_app_mq, &received_task, MESSAGE_PRIORITY, osWaitForever) == osOK) {
            switch (received_task.task) {
                case eNetworkTask_Connect: {
                    sNetworkAppConnParams_t *connect_params = (sNetworkAppConnParams_t*)received_task.params;
                    if (TCP_API_Connect(connect_params->ip, connect_params->port, connect_params->socket) == false) {
                    	//debug_err
                    }

                    socket = connect_params->socket;
                    break;
                }
                case eNetworkTask_Send: {
                    sNetworkAppSendParams_t *send_params = (sNetworkAppSendParams_t*)received_task.params;
                    if (TCP_API_Send(send_params->data, send_params->data_length) == false) {
                    	//debug_err
                    }
                    break;
                }
                case eNetworkTask_Disconnect: {
                    if (TCP_API_Disconnect(socket) == false) {
                    	//debug_err
                    }
                    break;
                }
                default: {
                	break;
                }
            }
        }
    }
}

