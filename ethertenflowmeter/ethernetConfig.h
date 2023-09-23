#ifndef FLOWSENSOR_ETHERNET_CONFIG_H_
#define FLOWSENSOR_ETHERNET_CONFIG_H_

// Ethernet libraries
#include <SPI.h>

#ifndef ETHERNET_SHIELD_VERSION
#define ETHERNET_SHIELD_VERSION 1
#endif

// to set mac address see information at
// https://www.freetronics.com.au/pages/setting-arduino-ethernet-mac-address
#if ETHERNET_SHIELD_VERSION == 1
#include <Ethernet.h>
byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xEA};
#elif ETHERNET_SHIELD_VERSION == 2
#include <Ethernet2.h>
byte mac[] = { 0x90, 0xA2, 0xDA, 0x0F, 0xFC, 0xA9 };
#endif

EthernetClient ethernetClient;

const unsigned long NETWORK_STARTUP_DELAY = 1500UL;

void print_ip() {
  for (byte thisByte = 0; thisByte < 4; thisByte++)
  { // print the value of each byte of the IP address:
    Serial.print(Ethernet.localIP()[thisByte], DEC);
    Serial.print(".");
  } //end for
  Serial.println();
}

void print_mac() {
  byte macBuffer[6];              // create a buffer to hold the MAC address
  Ethernet.MACAddress(macBuffer); // fill the buffer
  Serial.print("The MAC address is: ");
  for (byte octet = 0; octet < 6; octet++)
  {
    Serial.print(macBuffer[octet], HEX);
    if (octet < 5)
    {
      Serial.print('-');
    }
  }
  Serial.println();
}

void ethernet_init()
{
  // Configure Ethernet
  if (Ethernet.begin(mac) == 0) {
    // try to configure using IP address instead of DHCP:
    IPAddress ip(192, 168, 1, 251);
    Ethernet.begin(mac, ip);
    Serial.print("Static IP: ");
  }
  else {
    Serial.print("DHCP IP: ");
  }
  print_ip();
  print_mac();

  // give the Ethernet shield a second to initialize
  delay(NETWORK_STARTUP_DELAY);
}

#endif /* FLOWSENSOR_ETHERNET_CONFIG_H_ */
