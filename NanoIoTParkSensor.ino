#include <ArduinoBLE.h>
#include <NewPing.h>
#define TRIGGER_PIN 2
#define ECHO_PIN 3
#define MAX_DISTANCE 300
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

#define MAXGBAR1 257
#define MINGBAR1 247

#define MAXGBAR2 246
#define MINGBAR2 236

#define MAXGBAR3 235
#define MINGBAR3 224

#define MAXGBAR4 223
#define MINGBAR4 213

#define MAXYBAR1 212
#define MINYBAR1 202

#define MAXYBAR2 201
#define MINYBAR2 171

#define MAXRBAR1 170
#define MINRBAR1 160

BLEService ledService("FFE0"); // Servicio
BLEByteCharacteristic switchCharacteristic("FFE1", BLERead | BLEWrite | BLENotify);

const int BLE_LED_PIN = LED_BUILTIN;

void setup() {
  Serial.begin(9600);
  pinMode(TRIGGER_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  
  pinMode(BLE_LED_PIN,OUTPUT);
  digitalWrite( BLE_LED_PIN, LOW );

  // begin initialization
  if (!BLE.begin()) {
    Serial.println("Failed");
    while (1);
  }

  //************************************************************************
  //************************************************************************
  
  BLE.setLocalName("ParkSensor");
  
  //************************************************************************
  //************************************************************************
  
  BLE.setAdvertisedService(ledService);
  ledService.addCharacteristic(switchCharacteristic);
  BLE.addService(ledService);
  BLE.advertise();

  Serial.println("BLE Periférico");
}

void loop() {
  BLEDevice central = BLE.central();

  if (central) {
    Serial.print("Connected to central: ");
    Serial.println(central.address());

    while (central.connected()) {
      digitalWrite( BLE_LED_PIN, HIGH );
      
      int distance = sonar.ping_cm();

      if (distance >= MAXGBAR1) {switchCharacteristic.writeValue((byte)0x01);}
      if ((distance >= MINGBAR2) and (distance <= MAXGBAR2)) {switchCharacteristic.writeValue((byte)0x02);}
      if ((distance >= MINGBAR3) and (distance <= MAXGBAR3)) {switchCharacteristic.writeValue((byte)0x03);}
      if ((distance >= MINGBAR4) and (distance <= MAXGBAR4)) {switchCharacteristic.writeValue((byte)0x04);}
      if ((distance >= MINYBAR1) and (distance <= MAXYBAR1)) {switchCharacteristic.writeValue((byte)0x05);}
      if ((distance >= MINYBAR2) and (distance <= MAXYBAR2)) {switchCharacteristic.writeValue((byte)0x06);}
      if ((distance <= MINRBAR1) and (distance > 0)) {switchCharacteristic.writeValue((byte)0x07);}      
      if (distance == 0) {switchCharacteristic.writeValue((byte)0x00);}      

      delay(100);
      //Serial.println(distance);

    }
    Serial.print(F("Disconnected from central: "));
    Serial.println(central.address());
    digitalWrite( BLE_LED_PIN, LOW );
  }  
}
