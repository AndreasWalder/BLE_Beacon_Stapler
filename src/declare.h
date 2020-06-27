#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include <BLEBeacon.h>
#include "heltec.h"

bool deviceConnected = false;
bool oldDeviceConnected = false;
uint8_t txValue = 0;

BLEServer *pServer;
BLEAdvertising *pAdvertising;
BLEBeacon oBeacon;
BLECharacteristic * pTxCharacteristic;