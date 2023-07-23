//#include <BLEDevice.h>
//#include <BLEUtils.h>
//#include <BLEServer.h>
#include <BLE2902.h>

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>


// Define the UUIDs for the services and characteristics
#define SERIAL_HID_SERVICE_UUID "00001812-0000-1000-8000-00805f9b34fb"
#define SERIAL_HID_TX_CHARACTERISTIC_UUID "00002a4d-0000-1000-8000-00805f9b34fb"
#define SERIAL_HID_RX_CHARACTERISTIC_UUID "00002a4c-0000-1000-8000-00805f9b34fb"

#define KEYBOARD_HID_SERVICE_UUID "00001803-0000-1000-8000-00805f9b34fb"
#define KEYBOARD_HID_CHARACTERISTIC_UUID "00002a2d-0000-1000-8000-00805f9b34fb"

// Create the BLE Server, services, and characteristics
BLEServer* bleServer;
BLEService* serialHidService;
BLEService* keyboardHidService;
BLECharacteristic* serialHidTxCharacteristic;
BLECharacteristic* serialHidRxCharacteristic;
BLECharacteristic* keyboardHidCharacteristic;

// Button pin
const int buttonPin = 4;

// Callbacks for BLE server events
class MyServerCallbacks : public BLEServerCallbacks {
  void onConnect(BLEServer* server) {
    // Handle device connection
  }

  void onDisconnect(BLEServer* server) {
    // Handle device disconnection
  }
};


// Callbacks for receiving data on Serial HID characteristic
class SerialHidRxCallbacks : public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic* characteristic) {
    std::string value = characteristic->getValue();
    if (value.length() > 0) {
      // Handle incoming data (Serial HID) here
    }
  }
};

// Callbacks for receiving data on Keyboard HID characteristic
class KeyboardHidCallbacks : public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic* characteristic) {
    std::string value = characteristic->getValue();
    if (value.length() > 0) {
      // Handle incoming data (Keyboard HID) here
    }
  }
};

void setup() {
  Serial.begin(115200);
  pinMode(buttonPin, INPUT_PULLUP);

  Serial.println("Starting BLE work!");
  // Initialize BLE
  BLEDevice::init("ESP32");
  bleServer = BLEDevice::createServer();
  bleServer->setCallbacks(new MyServerCallbacks());
   Serial.println("Create Serial HID Service");
  // Create Serial HID Service
  serialHidService = bleServer->createService(SERIAL_HID_SERVICE_UUID);
  serialHidTxCharacteristic = serialHidService->createCharacteristic(
    SERIAL_HID_TX_CHARACTERISTIC_UUID,
    BLECharacteristic::PROPERTY_NOTIFY
  );

  serialHidRxCharacteristic = serialHidService->createCharacteristic(
    SERIAL_HID_RX_CHARACTERISTIC_UUID,
    BLECharacteristic::PROPERTY_WRITE
  );
  serialHidRxCharacteristic->setCallbacks(new SerialHidRxCallbacks());
  serialHidService->start();
  
  Serial.println("Create Keyboard HID Service");
  // Create Keyboard HID Service
  keyboardHidService = bleServer->createService(KEYBOARD_HID_SERVICE_UUID);
  keyboardHidCharacteristic = keyboardHidService->createCharacteristic(
    KEYBOARD_HID_CHARACTERISTIC_UUID,
    BLECharacteristic::PROPERTY_WRITE
  );
   
  keyboardHidCharacteristic->setCallbacks(new KeyboardHidCallbacks());
  keyboardHidService->start();

  Serial.println("Start advertising");
  BLEAdvertising* advertising = BLEDevice::getAdvertising();
  advertising->addServiceUUID(SERIAL_HID_SERVICE_UUID);
  advertising->addServiceUUID(KEYBOARD_HID_SERVICE_UUID);
  advertising->setScanResponse(true);
  advertising->setMinPreferred(0x06);  // functions that help with iPhone connections issue
  advertising->setMinPreferred(0x12);
  BLEDevice::startAdvertising();
  Serial.println("Characteristic defined! Now you can read it in your phone!");
}

void loop() {
  // Your main code here, if needed
}

