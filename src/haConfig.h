#include <ArduinoHA.h>

#define BROKER_ADDR IPAddress(192, 168, 0, 17)

byte mac[] = {0x00, 0x10, 0xFA, 0x6E, 0x38, 0x4A};

EthernetClient client;
HADevice device(mac, sizeof(mac));
HAMqtt mqtt(client, device);