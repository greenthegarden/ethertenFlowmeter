#include <Arduino.h>

#include "config.h"

void show_mqtt_connected_lcd();

boolean mqtt_connect()
{
  // Create a random client ID
  String clientId = MQTT_CLIENT_ID;
  clientId += String(random(0xffff), HEX);
  if (mqttClient.connect(clientId.c_str()))
  {
    show_mqtt_connected_lcd();
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
int Calc;
int hallsensor = 2; //The pin location of the sensor

void rpm() //This is the function that the interrupt calls
{
  NbTopsFan++; //This function measures the rising and falling edge of the hall effect sensors signal
}

const unsigned long FLOWRATE_PUBLISH_INTERVAL = 5000UL;
unsigned long flowratePreviousMillis = 0UL;

/*
 *************** Configure LCD ***************
 Based on example code for the Freetronics LCD & Keypad Shield:
  
    http://www.freetronics.com/products/lcd-keypad-shield
  
  by Marc Alexander, 7 September 2011
  This example code is in the public domain.
  
  ---------------------------------------------------------------------
  
  This program demonstrates button detection, LCD text/number printing,
  and LCD backlight control on the Freetronics LCD & Keypad Shield, connected to an Arduino board.
  
  Pins used by LCD & Keypad Shield:
  
    A0: Buttons, analog input from voltage ladder
    D4: LCD bit 4
    D5: LCD bit 5
    D6: LCD bit 6
    D7: LCD bit 7
    D8: LCD RS
    D9: LCD E
    D3: LCD Backlight (high = on, also has pullup high so default is on)
  
  ADC voltages for the 5 buttons on analog input pin A0:
  
    RIGHT:  0.00V :   0 @ 8bit ;   0 @ 10 bit
    UP:     0.71V :  36 @ 8bit ; 145 @ 10 bit
    DOWN:   1.61V :  82 @ 8bit ; 329 @ 10 bit
    LEFT:   2.47V : 126 @ 8bit ; 505 @ 10 bit
    SELECT: 3.62V : 185 @ 8bit ; 741 @ 10 bit
*/
#include <Wire.h>
#include <LiquidCrystal.h>

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

// Pins in use
#define BUTTON_ADC_PIN A0   // A0 is the button ADC input
#define LCD_BACKLIGHT_PIN 3 // D3 controls LCD backlight
// ADC readings expected for the 5 buttons on the ADC input
#define RIGHT_10BIT_ADC 0    // right
#define UP_10BIT_ADC 145     // up
#define DOWN_10BIT_ADC 329   // down
#define LEFT_10BIT_ADC 505   // left
#define SELECT_10BIT_ADC 741 // right
#define BUTTONHYSTERESIS 10  // hysteresis for valid button sensing window
//return values for ReadButtons()
#define BUTTON_NONE 0   //
#define BUTTON_RIGHT 1  //
#define BUTTON_UP 2     //
#define BUTTON_DOWN 3   //
#define BUTTON_LEFT 4   //
#define BUTTON_SELECT 5 //
//some example macros with friendly labels for LCD backlight/pin control, tested and can be swapped into the example code as you like
#define LCD_BACKLIGHT_OFF() digitalWrite(LCD_BACKLIGHT_PIN, LOW)
#define LCD_BACKLIGHT_ON() digitalWrite(LCD_BACKLIGHT_PIN, HIGH)
#define LCD_BACKLIGHT(state)                 \
  {                                          \
    if (state)                               \
    {                                        \
      digitalWrite(LCD_BACKLIGHT_PIN, HIGH); \
    }                                        \
    else                                     \
    {                                        \
      digitalWrite(LCD_BACKLIGHT_PIN, LOW);  \
    }                                        \
  }

/*--------------------------------------------------------------------------------------
  Variables
--------------------------------------------------------------------------------------*/
byte buttonJustPressed = false;  //this will be true after a ReadButtons() call if triggered
byte buttonJustReleased = false; //this will be true after a ReadButtons() call if triggered
byte buttonWas = BUTTON_NONE;    //used by ReadButtons() for detection of button events

/*--------------------------------------------------------------------------------------
  ReadButtons()
  Detect the button pressed and return the value
  Uses global values buttonWas, buttonJustPressed, buttonJustReleased.
--------------------------------------------------------------------------------------*/
byte ReadButtons()
{
  unsigned int buttonVoltage;
  byte button = BUTTON_NONE; // return no button pressed if the below checks don't write to btn

  //read the button ADC pin voltage
  buttonVoltage = analogRead(BUTTON_ADC_PIN);
  //sense if the voltage falls within valid voltage windows
  if (buttonVoltage < (RIGHT_10BIT_ADC + BUTTONHYSTERESIS))
  {
    button = BUTTON_RIGHT;
  }
  else if (buttonVoltage >= (UP_10BIT_ADC - BUTTONHYSTERESIS) && buttonVoltage <= (UP_10BIT_ADC + BUTTONHYSTERESIS))
  {
    button = BUTTON_UP;
  }
  else if (buttonVoltage >= (DOWN_10BIT_ADC - BUTTONHYSTERESIS) && buttonVoltage <= (DOWN_10BIT_ADC + BUTTONHYSTERESIS))
  {
    button = BUTTON_DOWN;
  }
  else if (buttonVoltage >= (LEFT_10BIT_ADC - BUTTONHYSTERESIS) && buttonVoltage <= (LEFT_10BIT_ADC + BUTTONHYSTERESIS))
  {
    button = BUTTON_LEFT;
  }
  else if (buttonVoltage >= (SELECT_10BIT_ADC - BUTTONHYSTERESIS) && buttonVoltage <= (SELECT_10BIT_ADC + BUTTONHYSTERESIS))
  {
    button = BUTTON_SELECT;
  }
  //handle button flags for just pressed and just released events
  if ((buttonWas == BUTTON_NONE) && (button != BUTTON_NONE))
  {
    //the button was just pressed, set buttonJustPressed, this can optionally be used to trigger a once-off action for a button press event
    //it's the duty of the receiver to clear these flags if it wants to detect a new button change event
    buttonJustPressed = true;
    buttonJustReleased = false;
  }
  if ((buttonWas != BUTTON_NONE) && (button == BUTTON_NONE))
  {
    buttonJustPressed = false;
    buttonJustReleased = true;
  }

  //save the latest button value, for change event detection next time round
  buttonWas = button;

  return (button);
}

// based on https://github.com/Ayresindustries/EXT_IP_Tracker/blob/master/EXTIP_Rev_B.ino
void show_dhcp_lcd()
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("DHCP IP: ");
  lcd.setCursor(0, 1);
  for (byte thisByte = 0; thisByte < 4; thisByte++)
  { // print the value of each byte of the IP address:
    lcd.print(Ethernet.localIP()[thisByte], DEC);
    if (thisByte < 3)
      lcd.print(".");
  }
}

// based on https://github.com/Ayresindustries/EXT_IP_Tracker/blob/master/EXTIP_Rev_B.ino
void show_mqtt_connected_lcd()
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("MQTT");
  lcd.setCursor(0, 1);
  lcd.print("Connected!!");
}


/*
 *************** Arduino methods ***************
 */

void setup()
{
  Serial.begin(9600);
  Serial.println("Setup");

  //button adc input
  pinMode(BUTTON_ADC_PIN, INPUT);     //ensure A0 is an input
  digitalWrite(BUTTON_ADC_PIN, LOW);  //ensure pullup is off on A0
  pinMode(LCD_BACKLIGHT_PIN, OUTPUT); //D3 is an output
  //lcd backlight control
  digitalWrite(LCD_BACKLIGHT_PIN, HIGH); //backlight control pin D3 is high (on)

  //set up the LCD number of columns and rows:
  lcd.begin(16, 2); // 16 characters, 2 rows
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Startup...");

  // configure ethernet
  ethernet_init();
  
  pinMode(hallsensor, INPUT); //initializes digital pin 2 as an input
  attachInterrupt(0, rpm, RISING); //and the interrupt is attached
}

void loop()
{
  NbTopsFan = 0;                 //Set NbTops to 0 ready for calculations
  sei();                         //Enables interrupts
  delay(1000);                   //Wait 1 second
  cli();                         //Disable interrupts
  Calc = (NbTopsFan * 60 / 5.5); //(Pulse frequency x 60) / 5.5Q, = flow rate in L / hour

  unsigned long now = millis();

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
  
  if (now - statusPreviousMillis >= STATUS_UPDATE_INTERVAL)
  {
    if (mqttClientConnected)
    {
      statusPreviousMillis = now;
      publish_status();
    }
  }

  if (now - flowratePreviousMillis >= FLOWRATE_PUBLISH_INTERVAL)
  {
    if (mqttClientConnected)
    {
      flowratePreviousMillis = now;
      publish_status();
    }
  }
}
