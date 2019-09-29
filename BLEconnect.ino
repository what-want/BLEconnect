#include <M5StickC.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <BLE2902.h>

#define SERVICE_UUID        "454f62bd-2b3e-43de-8cd5-e64f5bc15092"
#define CHARACTERISTIC_UUID "593744ae-3348-44f6-b00f-853e2b3b2b60"


BLEServer* pServer = NULL;
BLECharacteristic* pCharacteristic = NULL;
bool deviceConnected = false;



class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      M5.Lcd.println("connected");
      deviceConnected = true;
    };
 
    void onDisconnect(BLEServer* pServer) {
      M5.Lcd.println("disconnected");
      deviceConnected = false;
    }
};



class MyCallbacks: public BLECharacteristicCallbacks {
  void onRead(BLECharacteristic *pCharacteristic) {
    M5.Lcd.println("read");
    pCharacteristic->setValue("Hello World!");
    std::string value = pCharacteristic->getValue();
    M5.Lcd.println(value.c_str());
  }
 
  void onWrite(BLECharacteristic *pCharacteristic) {
    M5.Lcd.println("write");
    std::string value = pCharacteristic->getValue();
    M5.Lcd.println(value.c_str());
  }
};



 
void setup() {
  Serial.begin(115200);
  M5.begin();
  M5.Lcd.println("BLE start");

 
  BLEDevice::init("M5StickC");
  BLEServer *pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());
  BLEService *pService = pServer->createService(SERVICE_UUID);

  
  pCharacteristic = pService->createCharacteristic(
                                         CHARACTERISTIC_UUID,
                                         BLECharacteristic::PROPERTY_READ |
                                         BLECharacteristic::PROPERTY_WRITE |
                                         BLECharacteristic::PROPERTY_NOTIFY |
                                         BLECharacteristic::PROPERTY_INDICATE
                                       );

  pCharacteristic->setCallbacks(new MyCallbacks());
  pCharacteristic->addDescriptor(new BLE2902());


 
  pService->start();
  BLEAdvertising *pAdvertising = pServer->getAdvertising();
  pAdvertising->start();
}



void loop() {

  if (deviceConnected) {
    if(M5.BtnA.wasPressed()) {
      M5.Lcd.println("Btn A press!");
      pCharacteristic->setValue("Btn A press!");
      pCharacteristic->notify();
    }else if(M5.BtnB.wasPressed()) {
      M5.Lcd.println("Btn B press!");
      pCharacteristic->setValue("Btn B press!");
      pCharacteristic->notify();
    }
  }
  M5.update();
}
