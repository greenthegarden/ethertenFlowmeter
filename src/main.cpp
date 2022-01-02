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

/*
 *************** Configure Network ***************
 */

#include <Ethernet.h>

byte mac[] = {0x90, 0xA2, 0xDA, 0x0F, 0xFC, 0xA9};

EthernetClient net;


/*
 *************** Configure Flowmeter ***************
 */
// https://www.seeedstudio.com/G3-4-Water-Flow-Sensor-p-1083.html
// https://wiki.seeedstudio.com/G3-4_Water_Flow_sensor/

volatile int NbTopsFan; //measuring the rising edges of the signal
int Calc;
int hallsensor = 2; //The pin location of the sensor

void rpm() //This is the function that the interupt calls
{
  NbTopsFan++; //This function measures the rising and falling edge of the hall effect sensors signal
}


/*
 *************** Configure MQTT ***************
 */

#include <MQTT.h>
MQTTClient client;

unsigned long lastMillis = 0;

void connect()
{
  Serial.print("connecting...");
  while (!client.connect("arduino"))
  {
    Serial.print(".");
    delay(1000);
  }

  Serial.println("\nconnected!");

  client.subscribe("/hello");
  // client.unsubscribe("/hello");
}

void messageReceived(String &topic, String &payload)
{
  Serial.println("incoming: " + topic + " - " + payload);

  // Note: Do not use the client in the callback to publish, subscribe or
  // unsubscribe as it may cause deadlocks when other things arrive while
  // sending and receiving acknowledgments. Instead, change a global variable,
  // or push to a queue and handle it in the loop after calling `client.loop()`.
}

/*
 *************** Arduino methods ***************
 */

void setup()
{
  Serial.begin(9600);

  // Ethernet.begin(mac);

  // client.begin("192.168.1.186", net);
  // client.onMessage(messageReceived);

  // connect();

  // configure lcd

  lcd.begin(16, 2);
  lcd.clear();

  //button adc input
  pinMode(BUTTON_ADC_PIN, INPUT);    //ensure A0 is an input
  digitalWrite(BUTTON_ADC_PIN, LOW); //ensure pullup is off on A0
  //lcd backlight control
  digitalWrite(LCD_BACKLIGHT_PIN, HIGH); //backlight control pin D3 is high (on)
  pinMode(LCD_BACKLIGHT_PIN, OUTPUT);    //D3 is an output
  //set up the LCD number of columns and rows:
  lcd.begin(16, 2);
  //Print some initial text to the LCD.
  lcd.setCursor(0, 0); //top left
  //         0123456789012345
  lcd.print("Relay: ");
  //
  lcd.setCursor(0, 1); //bottom left
  //         0123456789012345
  lcd.print("Rate: ");

  // configure flowmeter sensor

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
  //          0123456789012345
  //         "Rate: "
  lcd.setCursor(6, 1);           //Move display cursor to after "Rate: "
  lcd.print(Calc, DEC);          //Displays the number calculated above
  lcd.print(" L/h");             //Displays "L/h"
}
