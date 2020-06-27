#include <Arduino.h>
#include <declare.h> //Includes und Variablen
#include <Settings.h> //Einstellungen
#include <ReverseUUID.h> // UUID richtig stellen
#include <LiebherrLogo.h> //Liebherr Logo
#include <Beacon.h>
#include <Setup.h> //Arduino 

void IncludeSetup(); 
void initBle(); 


void setup() {
  IncludeSetup();
  initBle(); 
}

class MyCallbacks: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) {
      std::string rxValue = pCharacteristic->getValue();

      if (rxValue.length() > 0) {
        Serial.println("*********");
        Serial.print("Erhaltener Wert: ");
        
        for (int i = 0; i < rxValue.length(); i++) {
          Serial.print(rxValue[i]);  
          Value += rxValue[i]; 
        }               
        Serial.println();
        Serial.println("*********"); 
      }
    }
};

class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      deviceConnected = true;
    };

    void onDisconnect(BLEServer* pServer) {
      deviceConnected = false;
    }
};

void initBle() {
  // Create the BLE Device
  BLEDevice::init("Stapler 1 Beacon");
  // Create the BLE Server
  pServer = BLEDevice::createServer(); 
  //Setting to +7dbm will actually result to +9dbm
  //BLEDevice::setPower(ESP_PWR_LVL_P7); //Setting to +7dbm will actually result to +9dbm
  pServer->setCallbacks(new MyServerCallbacks());
  // Create the BLE Service
  BLEService *pService = pServer->createService(SERVICE_UUID);

  // Create a BLE Characteristic
  pTxCharacteristic = pService->createCharacteristic(CHARACTERISTIC_UUID_TX,BLECharacteristic::PROPERTY_NOTIFY);
  pTxCharacteristic->addDescriptor(new BLE2902());
  BLECharacteristic * pRxCharacteristic = pService->createCharacteristic(CHARACTERISTIC_UUID_RX,BLECharacteristic::PROPERTY_WRITE);

  pRxCharacteristic->setCallbacks(new MyCallbacks());
   // Starten Sie den Dienst
  pService->start();
 
  // Create advertising manager
  pAdvertising = pServer->getAdvertising();
  // Stellen Sie die Beacon-Daten ein
  oBeacon = setBeacon(oBeacon); 
  // start Übertragung
  pAdvertising->start();
  
  Serial.println("Stapler 1 Beacon gestartet");
  Heltec.display->clear();
  Heltec.display->drawString(0, 0, "Stapler 1 Beacon");
  Heltec.display->drawString(0, 10, "Modus: Übertragung");
  float power_val = oBeacon.getSignalPower(); 
  char power_valstr[15];
  dtostrf(power_val,7, 2, power_valstr);
  Heltec.display->drawString(0, 30, "Power Level: " + String(power_valstr));
  Heltec.display->drawString(0, 40, "Major: " + String(Major) + " / Minor: " +  String(Minor));
  Heltec.display->drawString(0, 50, "UID: " + UUID_Real);
  Heltec.display->display();
}

void loop() {
  
 if (deviceConnected) {
        pTxCharacteristic->setValue(&txValue, 1);
        pTxCharacteristic->notify();
        txValue++;
        
        delay(10); // Der Bluetooth-Stack wird überlastet, wenn zu viele Pakete gesendet werden
        Heltec.display->clear();
        Heltec.display->drawString(0, 0,  "Stapler 1 Beacon");
        Heltec.display->drawString(0, 10, "Modus: Einstellung");
        Heltec.display->drawString(0, 30, "Erhaltener Wert:");
        Heltec.display->drawString(0, 40, Value);
        Heltec.display->display();
        Heltec.display->display();
  }

  // disconnecting
  if (!deviceConnected && oldDeviceConnected) {
        delay(500); // Geben Sie dem Bluetooth-Stack die Möglichkeit, die Dinge vorzubereiten
        pServer->startAdvertising(); // Werbung neu starten
        Serial.println("starte Übertragung");
        Heltec.display->clear();
        Heltec.display->drawString(0, 0, "Stapler 1 Beacon");
        Heltec.display->drawString(0, 10, "Modus: Übertragung");
        float power_val = oBeacon.getSignalPower(); 
        char power_valstr[15];
        dtostrf(power_val,7, 2, power_valstr);
        Heltec.display->drawString(0, 30, "Power Level: " + String(power_valstr));
        Heltec.display->drawString(0, 40, "Major: " + String(Major) + " / Minor: " +  String(Minor));
        Heltec.display->drawString(0, 50, "UID: " + UUID_Real);
        Heltec.display->display();
        oldDeviceConnected = deviceConnected;
    }
    // connecting
    if (deviceConnected && !oldDeviceConnected) {
        // Mach hier Sachen beim Verbinden
        Serial.println("stop Übertragung");
        Heltec.display->clear();
        Heltec.display->drawString(0, 0, "Stapler 1 Beacon");
        Heltec.display->drawString(0, 10, "starte Einstellung ...");
        Heltec.display->display();
        delay(2000);
        Value = "";
        oldDeviceConnected = deviceConnected;
    }
    delay(1000);
    //BLEDevice::setPower(ESP_PWR_LVL_P9); //Setting to +7dbm will actually result to +9dbm

    esp_err_t errRc=esp_ble_tx_power_set(ESP_BLE_PWR_TYPE_DEFAULT,ESP_PWR_LVL_P9);
    esp_ble_tx_power_set(ESP_BLE_PWR_TYPE_ADV, ESP_PWR_LVL_P9);
    esp_ble_tx_power_set(ESP_BLE_PWR_TYPE_SCAN ,ESP_PWR_LVL_P9); 
}