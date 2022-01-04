#include <Ethernet.h>
#include <ArduinoHA.h>


/*
 *************** Configure Home Assistant ***************
 */

#define BROKER_ADDR IPAddress(192, 168, 1, 186)
// IPAddress mqttServerAddr(192, 168, 1, 186); // home-assistant

byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xEA};

EthernetClient client;
HADevice device(mac, sizeof(mac));
HAMqtt mqtt(client, device);
HASensor flowmeter("flowmeter"); // "temp" is unique ID of the sensor. You should define your own ID.

void onMqttConnected()
{
  Serial.println("Connected to the broker!");

  // You can subscribe to custom topic if you need
  mqtt.subscribe("myCustomTopic");
}

void onMqttConnectionFailed()
{
  Serial.println("Failed to connect to the broker!");
}

void onBeforeSwitchStateChanged(bool state, HASwitch *s)
{
  // this callback will be called before publishing new state to HA
  // in some cases there may be delay before onStateChanged is called due to network latency
}


/*
 *************** Configure Flowmeter ***************
 */
// https://www.seeedstudio.com/G3-4-Water-Flow-Sensor-p-1083.html
// https://wiki.seeedstudio.com/G3-4_Water_Flow_sensor/

volatile int NbTopsFan; //measuring the rising edges of the signal
int flowrate = 0;
const unsigned int hallsensor = 2; //The pin location of the sensor

void rpm() //This is the function that the interrupt calls
{
  NbTopsFan++; //This function measures the rising and falling edge of the hall effect sensors signal
}

const unsigned long SENSOR_PUBLISH_INTERVAL = 5000UL;
unsigned long sensorLastSent = 0UL;


/*
 *************** Arduino methods ***************
 */

void setup()
{
  Serial.begin(9600);
  Serial.println("Setup");

  // you don't need to verify return status
  Ethernet.begin(mac);

  // set device's details (optional)
  device.setName("Flowmeter");
  device.setSoftwareVersion("1.0.0");

  // configure sensor (optional)
  flowmeter.setUnitOfMeasurement("L/h");
  flowmeter.setDeviceClass("None");
  flowmeter.setIcon("mdi:pump");
  flowmeter.setName("Irrigation flowrate");

  mqtt.begin(BROKER_ADDR);

  // configure flowmeter
  pinMode(hallsensor, INPUT);      //initializes digital pin 2 as an input
  attachInterrupt(0, rpm, RISING); //and the interrupt is attached
}

void loop()
{
  // Flowrate measurement
  NbTopsFan = 0;                     //Set NbTops to 0 ready for calculations
  sei();                             //Enables interrupts
  delay(1000);                       //Wait 1 second
  cli();                             //Disable interrupts
  flowrate = (NbTopsFan * 60 / 5.5); //(Pulse frequency x 60) / 5.5Q, = flow rate in L / hour

  Ethernet.maintain();
  mqtt.loop();

  unsigned long now = millis();

  if ((now - sensorLastSent) >= SENSOR_PUBLISH_INTERVAL)
  {
    sensorLastSent = now;
    flowmeter.setValue(flowrate);
    Serial.print("Flowrate: ");
    Serial.print(flowrate);
    Serial.println(" L/h");

    // Supported data types:
    // uint32_t (uint16_t, uint8_t)
    // int32_t (int16_t, int8_t)
    // double
    // float
    // const char*
  }

  // TODO: add a condition for flowrate < 1
}
