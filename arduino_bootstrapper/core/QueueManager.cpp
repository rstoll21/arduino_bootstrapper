/*
  QueueManager.cpp - Managing MQTT queue
  
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

#include "QueueManager.h"

WiFiClient espClient;
PubSubClient mqttClient(espClient);

/********************************** SETUP MQTT QUEUE *****************************************/
void QueueManager::setupMQTTQueue(void (*callback)(char*, byte*, unsigned int)) {
  
  mqttClient.setServer(mqtt_server, mqtt_port);
  mqttClient.setCallback(callback);

}

/********************************** MQTT RECONNECT *****************************************/
void QueueManager::mqttReconnect(void (*manageDisconnections)(), void (*manageQueueSubscription)(), void (*manageHardwareButton)()) {

  // Helpers classes
  Helpers helper;
  
  // how many attemps to MQTT connection
  int brokermqttcounter = 0;
  // Loop until we're reconnected
  while (!mqttClient.connected()) {   
    if (PRINT_TO_DISPLAY) {
      display.clearDisplay();
      display.setTextSize(1);
      display.setCursor(0,0);
    }
    if (brokermqttcounter <= 20) {
      helper.smartPrintln(F("Connecting to"));
      helper.smartPrintln(F("MQTT Broker..."));
    }
    helper.smartDisplay();

    // Manage hardware button if any
    manageHardwareButton();

    // Attempt to connect to MQTT server with QoS = 1 (pubsubclient supports QoS 1 for subscribe only, published msg have QoS 0 this is why I implemented a custom solution)
    if (mqttClient.connect(WIFI_DEVICE_NAME, mqtt_username, mqtt_password, 0, 1, 0, 0, 1)) {

      helper.smartPrintln(F(""));
      helper.smartPrintln(F("CONNECTED"));
      helper.smartPrintln(F(""));
      helper.smartPrintln(F("Reading data from"));
      helper.smartPrintln(F("the network..."));
      helper.smartDisplay();

      // Subscribe to MQTT topics
      manageQueueSubscription();

      delay(DELAY_2000);
      brokermqttcounter = 0;

      // reset the lastMQTTConnection to off, will be initialized by next time update
      lastMQTTConnection = OFF_CMD;

    } else {

      helper.smartPrintln(F("MQTT attempts="));
      helper.smartPrintln(brokermqttcounter);
      helper.smartDisplay();

      // after 10 attemps all peripherals are shut down
      if (brokermqttcounter >= MAX_RECONNECT) {
        helper.smartPrintln(F("Max retry reached, powering off peripherals."));
        helper.smartDisplay();
        // Manage disconnections, powering off peripherals
        manageDisconnections();
      } else if (brokermqttcounter > 10000) {
        brokermqttcounter = 0;
      }
      brokermqttcounter++;
      // Wait 500 millis before retrying
      delay(500);
    }
  }
  
}