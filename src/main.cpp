#include <Arduino.h>

#include "config.h"

boolean mqtt_connect()
{
  // Create a random client ID
  String clientId = MQTT_CLIENT_ID;
  clientId += String(random(0xffff), HEX);
  if (mqttClient.connect(clientId.c_str()))
  {
    Serial.print("MQTT connected as ");
    Serial.println(clientId);
    // Once connected, publish an announcement ...
    publish_connected();
    publish_configuration();
    publish_status();
  }
  return mqttClient.connected();
}

void callback(char *topic, uint8_t *payload, unsigned int payloadLength)
{
}

/*
 *************** Configure Flowmeter ***************
 */
// https://www.seeedstudio.com/G3-4-Water-Flow-Sensor-p-1083.html
// https://wiki.seeedstudio.com/G3-4_Water_Flow_sensor/

volatile int NbTopsFan; //measuring the rising edges of the signal
int flowrate;
const unsigned int hallsensor = 2; //The pin location of the sensor

void rpm() //This is the function that the interrupt calls
{
  NbTopsFan++; //This function measures the rising and falling edge of the hall effect sensors signal
}

const unsigned long FLOWRATE_PUBLISH_INTERVAL = 5000UL;
unsigned long flowratePreviousMillis = 0UL;


/*
 *************** Arduino methods ***************
 */

void setup()
{
  Serial.begin(9600);
  Serial.println("Setup");

  // configure ethernet
  ethernet_init();
  
  // configure flowmeter
  pinMode(hallsensor, INPUT); //initializes digital pin 2 as an input
  attachInterrupt(0, rpm, RISING); //and the interrupt is attached
}

void loop()
{
  NbTopsFan = 0;                 //Set NbTops to 0 ready for calculations
  sei();                         //Enables interrupts
  delay(1000);                   //Wait 1 second
  cli();                         //Disable interrupts
  flowrate = (NbTopsFan * 60 / 5.5); //(Pulse frequency x 60) / 5.5Q, = flow rate in L / hour

  unsigned long now = millis();

  // ensure connected to MQTT broker
  if (!mqttClient.connected())
  {
    mqttClientConnected = false;
    if (now - lastReconnectAttempt > RECONNECTION_ATTEMPT_INTERVAL)
    {
      lastReconnectAttempt = now;
      // Attempt to reconnect
      if (mqtt_connect())
      {
        lastReconnectAttempt = 0;
        mqttClientConnected = true;
      }
    }
  }
  
  // publish status message
  if (now - statusPreviousMillis >= STATUS_UPDATE_INTERVAL)
  {
    if (mqttClientConnected)
    {
      statusPreviousMillis = now;
      publish_status();
    }
  }

  // publish flowrate
  if (now - flowratePreviousMillis >= FLOWRATE_PUBLISH_INTERVAL)
  {
    if (flowrate > 1)
    {
      if (mqttClientConnected)
      {
        flowratePreviousMillis = now;
        publish_flowrate(flowrate);
      }
    }
  }
}
