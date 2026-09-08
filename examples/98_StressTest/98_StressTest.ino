#include <Wire.h>
#include "MKE_I2C_RFID.h"

MKE_I2C_RFID rfid;

void setup() {
    Serial.begin(115200);
    Wire.begin();
    
    // Tăng tốc độ I2C lên Fast Mode (400kHz) để test khả năng chịu tải của Slave
    Wire.setClock(400000);
    
    Serial.println(F("========================================"));
    Serial.println(F("   MKE I2C RFID - STRESS TEST"));
    Serial.println(F("========================================"));
    Serial.println(F("Configured I2C Speed: 400kHz"));
    
    Serial.println(F("Initializing MKE_I2C_RFID..."));
    if (!rfid.begin()) {
        Serial.println(F("Error: Failed to communicate. Check wiring."));
        while (1) delay(10);
    }
    
    Serial.print(F("Firmware Version: "));
    Serial.println(rfid.getFirmwareVersion());
    Serial.println(F("Place a MIFARE Classic card to start the stress test..."));
}

void loop() {
    // Đo thời gian thực thi của lệnh isCardPresent (lệnh này chạy thường xuyên nhất)
    unsigned long startDetect = micros();
    bool present = rfid.isCardPresent();
    unsigned long timeDetect = micros() - startDetect;
    
    if (present) {
        Serial.println(F("\n========================================"));
        Serial.println(F("--- Card Detected ---"));
        Serial.print(F("[Perf] isCardPresent() Time: ")); 
        Serial.print(timeDetect); Serial.println(F(" us"));
        
        // Đo thời gian thực thi của lệnh getUIDFull
        uint8_t uid[10];
        unsigned long startUID = micros();
        uint8_t uidSize = rfid.getUIDFull(uid);
        unsigned long timeUID = micros() - startUID;
        
        Serial.print(F("[Perf] getUIDFull() Time:    ")); 
        Serial.print(timeUID); Serial.println(F(" us"));
        
        if (uidSize > 0) {
            Serial.print(F("UID (")); Serial.print(uidSize); Serial.print(F(" bytes): "));
            for (int i = 0; i < uidSize; i++) {
                if (uid[i] < 0x10) Serial.print(F("0"));
                Serial.print(uid[i], HEX);
                Serial.print(F(" "));
            }
            Serial.println();
        }
        
        uint8_t key[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
        rfid.setAuthKey(key);
        
        uint8_t targetBlock = 4;
        Serial.println(F("Authenticating Block 4..."));
        uint8_t status = rfid.authenticateKeyA(targetBlock);
        
        if (status == 0) {
            Serial.println(F("Auth Success! Starting High-Frequency Read Stress Test (1000 iterations)..."));
            
            int successCount = 0;
            int failCount = 0;
            uint8_t buffer[16];
            unsigned long startTime = millis();
            
            for (int i = 0; i < 1000; i++) {
                // Spam lệnh đọc block liên tục không nghỉ
                uint8_t st = rfid.readBlock(targetBlock, buffer);
                if (st == 0) {
                    successCount++;
                } else {
                    failCount++;
                }
                
                // Xen kẽ một lệnh Get ngắn để test việc xử lý đa luồng (Multi-packet stress)
                rfid.getFirmwareVersion();
            }
            
            unsigned long duration = millis() - startTime;
            
            Serial.println(F("\n--- STRESS TEST RESULTS ---"));
            Serial.print(F("Total Time: ")); Serial.print(duration); Serial.println(F(" ms"));
            Serial.print(F("Successful Reads: ")); Serial.println(successCount);
            Serial.print(F("Failed Reads: ")); Serial.println(failCount);
            float rate = 1000.0 / (duration / 1000.0);
            Serial.print(F("Speed: ")); Serial.print(rate); Serial.println(F(" commands/sec"));
            Serial.println(F("========================================"));
            
        } else {
            Serial.println(F("Auth Failed. Ensure card is fully placed."));
        }
        
        rfid.haltCard();
        rfid.stopCrypto();
        Serial.println(F("Remove card to restart."));
        delay(3000);
        
        while (true) {
            unsigned long t1 = micros();
            bool stillPresent = rfid.isCardPresent();
            unsigned long t2 = micros();
            
            if (!stillPresent) break;
            
            // In ra thi thoảng để user theo dõi tốc độ Polling lúc thẻ vẫn đang đặt trên máy
            static unsigned long lastPrint = 0;
            if (millis() - lastPrint > 1000) {
                Serial.print(F("[Perf] Polling isCardPresent() while card holds... Time: "));
                Serial.print(t2 - t1); Serial.println(F(" us"));
                lastPrint = millis();
            }
            delay(100);
        }
    }
    
    delay(50);
}
