#include <Arduino.h>
// PD6 = RX, PD5 = TX
HardwareSerial Serial2(PD6, PD5);

#define ENCODED_LEN 24
#define MAX_RETRIES 10

uint8_t crc8(const uint8_t* data, size_t len) {
  uint8_t crc = 0x00;
  for (size_t i = 0; i < len; i++) {
    crc ^= data[i];
    for (int j = 0; j < 8; j++) {
      crc = (crc & 0x80) ? (crc  ^ 0x07) << 1 : (crc << 1);
    }
  }
  return crc;
}


void encodeRepetition(uint8_t input, uint8_t* output) {
    for (int j = 0; j < 3; j++) {
      for (int i = 7; i >= 0; i--) {
        bool bit = (input >> i) & 1;
        *output++ = bit ? 0xFF : 0x00;
    }
  }
}

void setup() {
  Serial.begin(115200);
  while (!Serial);
  Serial.println("Board A: Ready to send");
  Serial2.begin(115200);
  randomSeed(analogRead(0)); 
  delay(1000);
}

void loop() {
  if (Serial.available()) {
    char c = Serial.read();

    uint8_t encoded[ENCODED_LEN];
    encodeRepetition(c, encoded);
    uint8_t crc = crc8((uint8_t*)&c, 1);
    
    Serial.print("Sending char: ");
    Serial.println((char)c);
    Serial.print("Encoded: ");
    for (int i = 0; i < ENCODED_LEN; i++) {
      Serial.print(encoded[i], HEX);
      Serial.print(" ");
    }
    Serial.print(" CRC: 0x");
    Serial.println(crc, HEX);
    
    for (int attempt = 1; attempt <= MAX_RETRIES; attempt++) {
      Serial.print("Attempt ");
      Serial.println(attempt);
      
      uint8_t encodedCopy[ENCODED_LEN];
      memcpy(encodedCopy, encoded, ENCODED_LEN);
      
      while (Serial2.available()) Serial2.read();
      
      Serial2.write(encodedCopy, ENCODED_LEN);
      Serial2.write(crc);
      Serial2.flush();
      
      unsigned long start = millis();
      while (!Serial2.available()) {
        if (millis() - start > 1000) {
          Serial.println("Timeout waiting for ACK/NACK");
          break;
        }
      }
      
      if (Serial2.available()) {
        char response = Serial2.read();
        while (Serial2.available()) Serial2.read();
        if (response == 'A') {
          Serial.println("ACK received — success");
          break;
        } else if (response == 'N') {
          Serial.println("NACK received — retrying...");
        } else {
          Serial.print("Unexpected response: 0x");
          Serial.println((int)response, HEX);
        }
      }
      
      if (attempt == MAX_RETRIES) {
        Serial.println("Failed after max retries");
        return;
      }
      
      delay(200);
    }
  }
}
