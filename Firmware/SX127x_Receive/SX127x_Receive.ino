/*
  RadioLib SX1276 Receive Example using the RFM97C 915MHz

  Gotchas:
    * The RadioLib defaults the SX1276 to 434MHz so the reception is pretty
      poor. This is fixed with a radio.begin(915.0);
    * The RadioLib really requires DIO0 and DIO1 be connected. Reset is optional.
    * Avoid ESP pins 34/35/36/39 as they are inputs only
*/

#include <RadioLib.h> //Click here to get the library: http://librarymanager/All#RadioLib

// SX1276 has the following connections on the SparkFun ESP32 Thing Plus C:
int pin_cs = 15;
int pin_dio0 = 26; //aka A0
int pin_nrst = 32;
int pin_dio1 = 25; //aka A1
SX1276 radio = new Module(pin_cs, pin_dio0, pin_nrst, pin_dio1);

void setup() {
  Serial.begin(115200);
  delay(200); //Wait for ESP32 to be able to print

  Serial.print(F("[SX1276] Initializing ... "));
  //int state = radio.begin(); //-121dBm
  //int state = radio.begin(868.0); //-20dBm
  int state = radio.begin(915.0); //-23dBm
  if (state == ERR_NONE) {
    Serial.println(F("init success!"));
  } else {
    Serial.print(F("failed, code "));
    Serial.println(state);
    while (true);
  }
}

void loop() {
  Serial.print(F("[SX1276] Waiting for incoming transmission ... "));

  String str;
  int state = radio.receive(str);

  if (state == ERR_NONE) {
    // packet was successfully received
    Serial.println(F("success!"));

    // print the data of the packet
    Serial.print(F("[SX1276] Data:\t\t\t"));
    Serial.println(str);

    // print the RSSI (Received Signal Strength Indicator)
    // of the last received packet
    Serial.print(F("[SX1276] RSSI:\t\t\t"));
    Serial.print(radio.getRSSI());
    Serial.println(F(" dBm"));

    // print the SNR (Signal-to-Noise Ratio)
    // of the last received packet
    Serial.print(F("[SX1276] SNR:\t\t\t"));
    Serial.print(radio.getSNR());
    Serial.println(F(" dB"));

    // print frequency error
    // of the last received packet
    Serial.print(F("[SX1276] Frequency error:\t"));
    Serial.print(radio.getFrequencyError());
    Serial.println(F(" Hz"));

  } else if (state == ERR_RX_TIMEOUT) {
    // timeout occurred while waiting for a packet
    Serial.println(F("timeout!"));

  } else if (state == ERR_CRC_MISMATCH) {
    // packet was received, but is malformed
    Serial.println(F("CRC error!"));

  } else {
    // some other error occurred
    Serial.print(F("failed, code "));
    Serial.println(state);

  }
}
