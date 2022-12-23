#include <Arduino.h>

// Import libraries (BLEPeripheral depends on SPI)
#include <SPI.h>
#include <BLEPeripheral.h>
#include <BLECharacteristic.h>


#define LED_BUILTIN 2
#define DEVICE_VALUE_CHAR "HOL!"
#define LED_PIN   LED_BUILTIN

//custom boards may override default pin definitions with BLEPeripheral(PIN_REQ, PIN_RDY, PIN_RST)
BLEPeripheral                    blePeripheral                            = BLEPeripheral();

// create service
BLEService               ledService           = BLEService("19b10000e8f2537e4f6cd104768a1214");

// create switch characteristic
//uid , propiedades, tamaño valor enviado/recibido
//BLECharacteristic switchCharacteristic = BLECharacteristic("19b10000e8f2537e4f6cd104768a1214", BLERead | BLEWrite | BLENotify, 20);
BLEFloatCharacteristic switchCharacteristic = BLEFloatCharacteristic("19b10000e8f2537e4f6cd104768a1214", BLERead | BLEWrite | BLENotify);

void setup() {
  Serial.begin(9600);
#if defined (__AVR_ATmega32U4__)
  delay(5000);  //5 seconds delay for enabling to see the start up comments on the serial board
#endif

  // set LED pin to output mode
  pinMode(LED_PIN, OUTPUT);


  // set advertised local name and service UUID
  blePeripheral.setLocalName("nRF52840");
  blePeripheral.setDeviceName("nRF52840");
  blePeripheral.setAdvertisedServiceUuid(ledService.uuid());

  // add service and characteristic
  blePeripheral.addAttribute(ledService);
  blePeripheral.addAttribute(switchCharacteristic);


  // begin initialization
  blePeripheral.begin();
}
  
//Temperatura: -100 +200

void loop() {

  BLECentral central = blePeripheral.central();
  digitalWrite(LED_PIN, HIGH);

  if (central) {

    // central connected to peripheral
    
    while (central.connected()) {
      digitalWrite(LED_PIN, LOW);
      // central still connected to peripheral
      float scale = rand() / (float) RAND_MAX; /* [0, 1.0] */
      switchCharacteristic.setValue((rand() % 40) + scale);

      delay(3000);
      //switchCharacteristic.broadcast();
      
      /*if (switchCharacteristic.written()) {

        // central wrote new value to characteristic, update LED
        if (switchCharacteristic.value()) {
          Serial.println(F("LED on"));
          digitalWrite(LED_PIN, HIGH);
          delay(1000);
        } else {
          Serial.println(F("LED off"));
          digitalWrite(LED_PIN, LOW);
          delay(1000);
        }
      }*/
    }
    digitalWrite(LED_PIN, HIGH);
    // central disconnected
  }
} 
