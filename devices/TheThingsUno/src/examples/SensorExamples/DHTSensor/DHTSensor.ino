//This library is not installed by default, istall from library manager
#include "DHT.h" //DHT sensor library by Adafruit, supports DHT11/21/22 Sensors, more info: https://github.com/adafruit/DHT-sensor-library
#include "TheThingsUno.h"
#include "lora-serialization/src/LoraEncoder.cpp"
#include "lora-serialization/src/LoraMessage.cpp"

// Set your AppEUI and AppKey
const byte appEui[8] = { <insert AppEui> }; //for example: {0x70, 0xB3, 0xD5, 0x7E, 0xE0, 0xE0, 0x01, 0x4A1};
const byte appKey[16] = { <insert AppKey> }; //for example: {0x73, 0x6D, 0x24, 0xD2, 0x69, 0xBE, 0xE3, 0xAE, 0x0E, 0xCE, 0xF0, 0xBB, 0x6C, 0xA4, 0xBA, 0xFE};

#define DHTPIN 2

//Choose your DHT sensor moddel
//#define DHTTYPE DHT11
//#define DHTTYPE DHT21
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);

//data array for transmitting data
byte data[4];

#define debugSerial Serial
#define loraSerial Serial1

#define debugPrintLn(...) { if (debugSerial) debugSerial.println(__VA_ARGS__); }
#define debugPrint(...) { if (debugSerial) debugSerial.print(__VA_ARGS__); }

TheThingsUno ttu;

void setup() {
  debugSerial.begin(115200);
  loraSerial.begin(57600);

  dht.begin();

  delay(1000);
  ttu.init(loraSerial, debugSerial);
  ttu.reset();

  //the device will attempt a join every second till the join is successfull
  while(!ttu.join(appEui, appKey)){
      delay(6000);
  }

  digitalWrite(13, HIGH); //turn on LED to confirm join

  delay(6000);
  ttu.showStatus();
  debugPrintLn("Setup for The Things Network complete");

  delay(1000);
}

void loop() {

  float temperature = dht.readTemperature(false); //false = temp-Celsius //true = temp-farenheit
  float humidity = dht.readHumidity(false);

  //put data into the message
  LoraMessage message;
  message
    .addTemperature(temperature)
    .addHumidity(humidity);
  //debug print
  debugPrint("Transmitting Temperature: ");
  debugPrintLn(temperature);
  debugPrint("Humidity: ");
  debugPrintLn(humidity);
  //send data
  ttu.sendMessage(message);

  delay(20000);
}
