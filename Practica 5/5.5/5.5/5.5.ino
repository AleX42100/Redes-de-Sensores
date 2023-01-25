#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h> 
#include <BLE2902.h>

bool _BLEClientConnected = false;

#define BatteryService BLEUUID((uint16_t)0x180F) 

BLECharacteristic BatteryLevelCharacteristic(BLEUUID((uint16_t)0x2A19), BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY);

BLEDescriptor BatteryLevelDescriptor(BLEUUID((uint16_t)0x2901));

class MyServerCallbacks : public BLEServerCallbacks {

    void onConnect(BLEServer* pServer) {

      _BLEClientConnected = true;

    };

    void onDisconnect(BLEServer* pServer) {

      _BLEClientConnected = false;

    }

};

void InitBLE() {
  BLEDevice::init("BLE Battery");
  // Create the BLE Server
  BLEServer *pServer = BLEDevice::createServer();

  pServer->setCallbacks(new MyServerCallbacks());

  // Create the BLE Service
  BLEService *pBattery = pServer->createService(BatteryService);
  pBattery->addCharacteristic(&BatteryLevelCharacteristic);
  BatteryLevelDescriptor.setValue("Counter");
  BatteryLevelCharacteristic.addDescriptor(&BatteryLevelDescriptor);
  BatteryLevelCharacteristic.addDescriptor(new BLE2902());
  pServer->getAdvertising()->addServiceUUID(BatteryService);
  pBattery->start();

  // Start advertising
  pServer->getAdvertising()->start();
}


void setup() {
  Serial.begin(115200);
  Serial.println("Counter :");
  InitBLE();
}

uint8_t battery_level = 0;

void loop() {
  BatteryLevelCharacteristic.setValue(&battery_level, 1);
  BatteryLevelCharacteristic.notify();
  delay(1000);
  battery_level++;
  Serial.println(int(battery_level));
  if (int(battery_level) == 100)
    battery_level = 0;
}
