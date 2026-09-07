#include <Wire.h>
#include "MKE_I2C_RFID.h"

MKE_I2C_RFID rfid;

void setup() {
    Serial.begin(115200);
    Wire.begin();
    
    Serial.println(F("========================================"));
    Serial.println(F("   MKE I2C RFID Comprehensive Test"));
    Serial.println(F("========================================"));
    
    Serial.println(F("Initializing MKE_I2C_RFID..."));
    if (!rfid.begin()) {
        Serial.println(F("Error: Failed to communicate with MKE_I2C_RFID. Check I2C wiring."));
        while (1) {
            delay(10);
        }
    }
    
    Serial.println(F("--> Initialization SUCCESS"));
    
    // Test API: Module Information
    Serial.println(F("--- [API Test: Module Info] ---"));
    Serial.print(F("Current I2C Address : 0x"));
    Serial.println(rfid.getI2CAddress(), HEX);
    Serial.print(F("Firmware Version    : "));
    Serial.println(rfid.getFirmwareVersion());
    Serial.print(F("Module ID           : "));
    Serial.println(rfid.getModuleID());
    Serial.print(F("RC522 FW Version    : 0x"));
    Serial.println(rfid.getRC522FirmwareVersion(), HEX);
    Serial.print(F("Antenna Gain        : "));
    Serial.println(rfid.getAntennaGain());
    
    Serial.println(F("\n--- [API Test: Waiting for Card] ---"));
    Serial.println(F("Please bring a MIFARE Classic 1K card close to the reader..."));
    
    // Clear any stale card present state from the slave (useful if Master was reset but Slave wasn't)
    // We use a while loop to ensure that the command doesn't get dropped by I2C congestion, 
    // and correctly flushes the stale true state from the slave.
    delay(50);
    while (rfid.isCardPresent()) {
        delay(10);
    }
}

void loop() {
    if (rfid.isCardPresent()) {
        Serial.println(F("\n========================================"));
        Serial.println(F("Card Detected! Starting tests..."));
        
        // Test API: Card Info
        Serial.println(F("\n--- [API Test: Card Info] ---"));
        uint8_t uidSize = rfid.getUIDSize();
        Serial.print(F("UID Size: "));
        Serial.print(uidSize);
        Serial.println(F(" bytes"));
        
        uint8_t uidFull[10];
        if (uidSize > 0 && uidSize <= 10) {
            rfid.getUIDFull(uidFull);
            Serial.print(F("Full UID: "));
            for (int i = 0; i < uidSize; i++) {
                if (uidFull[i] < 0x10) Serial.print(F("0"));
                Serial.print(uidFull[i], HEX);
                Serial.print(F(" "));
            }
            Serial.println();
        }
        
        Serial.print(F("SAK: 0x"));
        Serial.println(rfid.getSAK(), HEX);
        
        Serial.print(F("Card Type: 0x"));
        Serial.println(rfid.getCardType(), HEX);
        
        // Test API: Authentication and Block Read/Write
        Serial.println(F("\n--- [API Test: Read/Write Block] ---"));
        
        uint8_t key[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
        rfid.setAuthKey(key);
        Serial.println(F("Set Default Auth Key A: FF FF FF FF FF FF"));
        
        uint8_t targetBlock = 4; // Sector 1, Block 0
        Serial.print(F("Authenticating Block "));
        Serial.print(targetBlock);
        Serial.println(F("..."));
        
        uint8_t status = rfid.authenticateKeyA(targetBlock);
        
        if (status == 0) {
            Serial.println(F("Auth SUCCESS!"));
            
            // Read original data
            uint8_t buffer[16];
            Serial.println(F("Reading block..."));
            status = rfid.readBlock(targetBlock, buffer);
            if (status == 0) {
                Serial.print(F("Data: "));
                for (int i = 0; i < 16; i++) {
                    if (buffer[i] < 0x10) Serial.print(F("0"));
                    Serial.print(buffer[i], HEX);
                    Serial.print(F(" "));
                }
                Serial.println();
            } else {
                Serial.print(F("Read failed. Status: "));
                Serial.println(status);
            }
            
            // Write test data
            Serial.println(F("Writing test data to block..."));
            uint8_t testData[16] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
            status = rfid.writeBlock(targetBlock, testData);
            if (status == 0) {
                Serial.println(F("Write SUCCESS!"));
            } else {
                Serial.print(F("Write failed. Status: "));
                Serial.println(status);
            }
            
            // Re-read data to verify
            Serial.println(F("Re-reading block to verify..."));
            status = rfid.readBlock(targetBlock, buffer);
            if (status == 0) {
                Serial.print(F("Verified Data: "));
                for (int i = 0; i < 16; i++) {
                    if (buffer[i] < 0x10) Serial.print(F("0"));
                    Serial.print(buffer[i], HEX);
                    Serial.print(F(" "));
                }
                Serial.println();
            }
        } else {
            Serial.print(F("Auth FAILED. Status: "));
            Serial.println(status);
            Serial.println(F("Note: Maybe the key is not default FF FF FF FF FF FF."));
        }
        
        // Test API: Value Block Operations
        Serial.println(F("\n--- [API Test: Value Block Operations] ---"));
        uint8_t valueBlock = 5; // Sector 1, Block 1
        Serial.print(F("Testing Value Block: "));
        Serial.println(valueBlock);
        
        // We already authenticated Sector 1 (Block 4) above, so we can access Block 5.
        // If not, we would need to authenticate again. We assume auth is still valid for the sector.
        if (status == 0) {
            Serial.println(F("Setting Value to 1000..."));
            status = rfid.setValue(valueBlock, 1000);
            
            if (status == 0) {
                Serial.println(F("Set Value SUCCESS!"));
                
                int32_t val = 0;
                rfid.getValue(valueBlock, &val);
                Serial.print(F("Current Value: "));
                Serial.println(val);
                
                Serial.println(F("Incrementing by 500..."));
                rfid.incrementValue(valueBlock, 500);
                rfid.getValue(valueBlock, &val);
                Serial.print(F("Current Value: "));
                Serial.println(val);
                
                Serial.println(F("Decrementing by 250..."));
                rfid.decrementValue(valueBlock, 250);
                rfid.getValue(valueBlock, &val);
                Serial.print(F("Current Value: "));
                Serial.println(val);
                
            } else {
                Serial.print(F("Set Value failed. Status: "));
                Serial.println(status);
                Serial.println(F("Ensure the sector is authenticated and the card is still present."));
            }
        }
        
        // Test API: Halt and Crypto
        Serial.println(F("\n--- [API Test: Halt and Stop Crypto] ---"));
        rfid.haltCard();
        Serial.println(F("Halt Card command sent."));
        rfid.stopCrypto();
        Serial.println(F("Stop Crypto command sent."));
        
        Serial.println(F("========================================"));
        Serial.println(F("Tests Completed. Remove card to restart."));
        Serial.println(F("Waiting for 3 seconds..."));
        
        delay(3000);
        
        // Clear any card bouncing event that might have latched while removing the card
        // rfid.isCardPresent();
    }
    
    delay(50); // Polling delay
}
