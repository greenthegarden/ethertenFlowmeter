#ifndef FLOWSENSOR_MQTT_CONFIG_H_
#define FLOWSENSOR_MQTT_CONFIG_H_

#include <PubSubClient.h>

// MQTT parameters
IPAddress mqttServerAddr(192, 168, 1, 186); // home-assistant

// const char MQTT_CLIENT_ID[] = "flowmeter";
// const int  MQTT_PORT        = 1883;

// unsigned long lastReconnectAttempt = 0UL;
// const unsigned long RECONNECTION_ATTEMPT_INTERVAL = 5000UL;

// boolean mqttClientConnected = false;

// // callback function definition
// void callback(char *topic, uint8_t *payload, unsigned int length);

// // mqttclient definition
// PubSubClient mqttClient(mqttServerAddr, MQTT_PORT, callback, ethernetClient);

// const char MQTT_PAYLOAD_CONNECTED[] PROGMEM = "CONN";
// const char MQTT_PAYLOAD_OK[] PROGMEM = "OK";
// const char MQTT_PAYLOAD_ERROR[] PROGMEM = "ERROR";
// const char MQTT_PAYLOAD_START[] PROGMEM = "START";
// const char MQTT_PAYLOAD_END[] PROGMEM = "END";
// const char MQTT_PAYLOAD_SLEEP[] PROGMEM = "SLEEP";

// PGM_P const MQTT_PAYLOADS[] PROGMEM = {
//   MQTT_PAYLOAD_CONNECTED, // idx = 0
//   MQTT_PAYLOAD_OK,        // idx = 1
//   MQTT_PAYLOAD_ERROR,     // idx = 2
//   MQTT_PAYLOAD_START,     // idx = 3
//   MQTT_PAYLOAD_END,       // idx = 4
//   MQTT_PAYLOAD_SLEEP,     // idx = 5
// };

// typedef enum {
//   MQTT_PAYLOAD_CONNECTED_IDX = 0,
//   MQTT_PAYLOAD_OK_IDX = 1,
//   MQTT_PAYLOAD_ERROR_IDX = 2,
//   MQTT_PAYLOAD_START_IDX = 3,
//   MQTT_PAYLOAD_END_IDX = 4,
//   MQTT_PAYLOAD_SLEEP_IDX = 5,
// } mqtt_payloads;

// const char MQTT_STATUS[] PROGMEM = "irrigation/flowsensor/status/mqtt";
// const char INTERVAL_STATUS[] PROGMEM = "irrigation/flowsensor/status/interval";
// const char IP_ADDR_STATUS[] PROGMEM = "irrigation/flowsensor/status/ip_addr";
// const char UPTIME_STATUS[] PROGMEM = "irrigation/flowsensor/status/uptime";
// const char MEMORY_STATUS[] PROGMEM = "irrigation/flowsensor/status/memory";
// const char TIME_STATUS[] PROGMEM = "irrigation/flowsensor/status/time";

// PGM_P const STATUS_TOPICS[] PROGMEM = {
//   MQTT_STATUS,     // idx = 0
//   INTERVAL_STATUS, // idx = 1
//   IP_ADDR_STATUS,  // idx = 2
//   UPTIME_STATUS,   // idx = 3
//   MEMORY_STATUS,   // idx = 4
//   TIME_STATUS,     // idx = 5
// };

// typedef enum {
//   MQTT_STATUS_IDX = 0,
//   INTERVAL_STATUS_IDX = 1,
//   IP_ADDR_STATUS_IDX = 2,
//   UPTIME_STATUS_IDX = 3,
//   MEMORY_STATUS_IDX = 4,
//   TIME_STATUS_IDX = 5,
// } status_topics;

// void print_topic_paylod(char* topic, char* payload) {
//   Serial.print("Topic: ");
//   Serial.print(topic);
//   Serial.print(", Payload: ");
//   Serial.print(payload);
//   Serial.println();
// }

// void publish_connected()
// {
//   topicBuffer[0] = '\0';
//   strcpy_P(topicBuffer, (char *)pgm_read_word(&(STATUS_TOPICS[MQTT_STATUS_IDX])));
//   payloadBuffer[0] = '\0';
//   strcpy_P(payloadBuffer, (char *)pgm_read_word(&(MQTT_PAYLOADS[MQTT_PAYLOAD_CONNECTED_IDX])));
//   print_topic_paylod(topicBuffer, payloadBuffer);
//   mqttClient.publish(topicBuffer, payloadBuffer);
// }

// void publish_status_interval()
// {
//   topicBuffer[0] = '\0';
//   strcpy_P(topicBuffer, (char *)pgm_read_word(&(STATUS_TOPICS[INTERVAL_STATUS_IDX])));
//   payloadBuffer[0] = '\0';
//   print_topic_paylod(topicBuffer, ltoa(STATUS_UPDATE_INTERVAL, payloadBuffer, 10));
//   mqttClient.publish(topicBuffer, ltoa(STATUS_UPDATE_INTERVAL, payloadBuffer, 10));
// }

// void publish_ip_address()
// {
//   topicBuffer[0] = '\0';
//   strcpy_P(topicBuffer, (char *)pgm_read_word(&(STATUS_TOPICS[IP_ADDR_STATUS_IDX])));
//   payloadBuffer[0] = '\0';
//   IPAddress ip = Ethernet.localIP();
//   sprintf(payloadBuffer, "%i%c%i%c%i%c%i", ip[0], '.', ip[1], '.', ip[2], '.', ip[3]);
//   print_topic_paylod(topicBuffer, payloadBuffer);
//   mqttClient.publish(topicBuffer, payloadBuffer);
// }

// void publish_uptime()
// {
//   topicBuffer[0] = '\0';
//   strcpy_P(topicBuffer, (char *)pgm_read_word(&(STATUS_TOPICS[UPTIME_STATUS_IDX])));
//   payloadBuffer[0] = '\0';
//   print_topic_paylod(topicBuffer, ltoa(millis(), payloadBuffer, 10));
//   mqttClient.publish(topicBuffer, ltoa(millis(), payloadBuffer, 10));
// }

// void publish_memory()
// {
//   topicBuffer[0] = '\0';
//   strcpy_P(topicBuffer, (char *)pgm_read_word(&(STATUS_TOPICS[MEMORY_STATUS_IDX])));
//   payloadBuffer[0] = '\0';
//   print_topic_paylod(topicBuffer, itoa(getFreeMemory(), payloadBuffer, 10));
//   mqttClient.publish(topicBuffer, itoa(getFreeMemory(), payloadBuffer, 10));
// }

// void publish_configuration()
// {
//   publish_status_interval();
//   publish_ip_address();
// }

// void publish_status()
// {
//   publish_uptime();
//   publish_memory();
// }

// const char FLOWRATE_SENSOR[] PROGMEM = "irrigation/flowsensor/flowrate";

// PGM_P const SENSOR_TOPICS[] PROGMEM = {
//     FLOWRATE_SENSOR, // idx = 0
// };

// typedef enum
// {
//   FLOWRATE_SENSOR_IDX = 0,
// } sensor_topics;

// void publish_flowrate(int flowrate)
// {
//   topicBuffer[0] = '\0';
//   strcpy_P(topicBuffer, (char *)pgm_read_word(&(SENSOR_TOPICS[FLOWRATE_SENSOR_IDX])));
//   payloadBuffer[0] = '\0';
//   print_topic_paylod(topicBuffer, itoa(flowrate, payloadBuffer, 10));
//   mqttClient.publish(topicBuffer, itoa(flowrate, payloadBuffer, 10));
// }


#endif /* FLOWSENSOR_MQTT_CONFIG_H_ */
