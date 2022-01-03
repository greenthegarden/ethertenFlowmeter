#ifndef FLOWSENSOR_ETHERNET_CONFIG_H_
#define FLOWSENSOR_ETHERNET_CONFIG_H_

// Ethernet libraries
#include <SPI.h>

#ifndef ETHERNET_SHIELD_VERSION
#define ETHERNET_SHIELD_VERSION 1
#endif

#if ETHERNET_SHIELD_VERSION == 1
#include <Ethernet.h>
byte mac[] = { 0xDD, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
#elif ETHERNET_SHIELD_VERSION == 2
#include <Ethernet2.h>
byte mac[] = { 0x90, 0xA2, 0xDA, 0x0F, 0xFC, 0xA9 };
#endif

EthernetClient ethernetClient;

const unsigned long NETWORK_STARTUP_DELAY = 1500UL;

void show_dhcp_lcd();

void ethernet_init()
{
  // Configure Ethernet
  if (Ethernet.begin(mac) == 0) {
    // try to configure using IP address instead of DHCP:
    IPAddress ip(192, 168, 1, 42);
    Ethernet.begin(mac, ip);
  }
  Serial.print("DHCP IP: ");
  for (byte thisByte = 0; thisByte < 4; thisByte++)
  { // print the value of each byte of the IP address:
    Serial.print(Ethernet.localIP()[thisByte], DEC);
    Serial.print(".");
  } //end for
  Serial.println();
  show_dhcp_lcd(); // display on LCD

  // give the Ethernet shield a second to initialize
  delay(NETWORK_STARTUP_DELAY);
}

#endif /* FLOWSENSOR_ETHERNET_CONFIG_H_ */
