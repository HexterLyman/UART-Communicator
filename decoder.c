#include <Arduino.h>
// PD6 = RX, PD5 = TX
HardwareSerial Serial2(PD6, PD5);

#define ENCODED_LEN 24

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

uint8_t decodeRepetition(uint8_t* input) {
  uint8_t result = 0;
  for (int i = 0; i < 8; i++) {
    int ones = 0;
    for (int j = 0; j < 3; j++) { 
      if (input[j * 8 + i] > 0x7F) ones++;
    }
    result = result << 1;
    if (ones >= 2) {
      result = result + 1;
    }
  }
  return result;
}

void setup() {
  Serial.begin(115200);
  while (!Serial);
  Serial.println("Board B: Ready to receive");
  Serial2.begin(115200); 
  while (Serial2.available()) Serial2.read();
}

void loop() {
  if (Serial2.available()) {
    static uint8_t buffer[ENCODED_LEN + 1];
    int bytesRead = 0;
    
    unsigned long startTime = millis();
    while (bytesRead < ENCODED_LEN + 1) {
      if (Serial2.available() > 64) {
        Serial.println("Warning: UART buffer overflow");
        while (Serial2.available()) Serial2.read();
        return;
      }
      if (Serial2.available()) {
        buffer[bytesRead] = Serial2.read();
        bytesRead++;
        startTime = millis();
      }
      
      if (millis() - startTime > 200) {
        while (Serial2.available()) Serial2.read();
        return;
      }
    }
    
    Serial.print("Received ");
    Serial.print(bytesRead);
    Serial.println(" bytes");
    
    uint8_t decoded = decodeRepetition(buffer);
    uint8_t received_crc = buffer[ENCODED_LEN];
    uint8_t computed_crc = crc8(&decoded, 1);
    
    Serial.print("Decoded: ");
    Serial.print((char)decoded);
    Serial.print(" (0x");
    Serial.print(decoded, HEX);
    Serial.println(")");
    
    Serial.print("Received CRC: 0x");
    Serial.println(received_crc, HEX);
    Serial.print("Computed CRC: 0x");
    Serial.println(computed_crc, HEX);
    
    if (computed_crc == received_crc) {
      Serial.println("CRC OK - Sending ACK");
      Serial2.write('A');
    } else {
      Serial.println("CRC mismatch - Sending NACK");
      Serial2.write('N');
    }
    
    Serial2.flush();
    delay(10);
  }
}