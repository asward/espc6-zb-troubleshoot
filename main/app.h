#include "driver/gpio.h"

#define GPIO_INPUT_IO_ACTIVE_CLOSED  GPIO_NUM_22
#define GPIO_INPUT_IO_ACTIVE_OPEN  GPIO_NUM_23
#define HA_GARAGE_DOOR_BINARY_INPUT_EP_ID        11    /* esp light bulb device endpoint, used to process light controlling commands */
#define BINARY_INPUT_CLUSTER_ID 0x000f