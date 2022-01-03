#ifndef FLOWSENSOR_CONFIG_H_
#define FLOWSENSOR_CONFIG_H_

#ifndef DEFAULT_BUFFER_SIZE
#define DEFAULT_BUFFER_SIZE 32
#endif

// external libraries
#include <MemoryFree.h>

// Macros
#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))

// global variable definitions
const byte BUFFER_SIZE = DEFAULT_BUFFER_SIZE;
char topicBuffer[BUFFER_SIZE];
char payloadBuffer[BUFFER_SIZE];

const unsigned long STATUS_UPDATE_INTERVAL = 5UL * 60UL * 1000UL; // 5 minutes
unsigned long statusPreviousMillis = 0UL;

#include "ethernetConfig.h"
#include "mqttConfig.h"

#endif /* FLOWSENSOR_CONFIG_H_ */
