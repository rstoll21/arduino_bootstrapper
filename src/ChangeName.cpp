/*
  ChangeName.cpp - Main Arduino File - Utility classes for bootstrapping arduino projects with Wifi management, 
  OTA upload management, memory management, MQTT and queue management.

  Copyright (C) 2020  Davide Perini
  
  Permission is hereby granted, free of charge, to any person obtaining a copy of 
  this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell 
  copies of the Software, and to permit persons to whom the Software is 
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in 
  all copies or substantial portions of the Software.
  
  You should have received a copy of the MIT License along with this program.  
  If not, see <https://opensource.org/licenses/MIT/>.
*/

#include "Configuration.h"

// Initialize the display
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET); 
#include "ChangeName.h"

void setup() {

  Serial.begin(SERIAL_RATE);
  Serial.print("setup");
  // LED_BUILTIN
  pinMode(LED_BUILTIN, OUTPUT);
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  display.setTextColor(WHITE);
  // Bootsrap setup() with Wifi and MQTT functions
  bootstrapManager.bootstrapSetup(manageDisconnections, manageHardwareButton, callback);

  // ENTER YOUR CODE HERE

}

/********************************** MANAGE WIFI AND MQTT DISCONNECTION *****************************************/
void manageDisconnections() {
 
}

/********************************** MQTT SUBSCRIPTIONS *****************************************/
void manageQueueSubscription() {
  
}

/********************************** MANAGE HARDWARE BUTTON *****************************************/
void manageHardwareButton() {
 
}

/********************************** START CALLBACK *****************************************/
void callback(char* topic, byte* payload, unsigned int length) {
  
}

void loop() {

  // Bootsrap loop() with Wifi, MQTT and OTA functions
  bootstrapManager.bootstrapLoop(manageDisconnections, manageQueueSubscription, manageHardwareButton);

  // ENTER YOUR CODE HERE

  Serial.print("Hello World");
  delay(1000);
  
}