# 99_Comprehensive_Test

*Read this document in: [🇬🇧 English](#english) | [🇻🇳 Tiếng Việt](#tiếng-việt)*

---

<a id="english"></a>
## 🇬🇧 English

### Description
This example is a comprehensive test designed to verify all major APIs provided by the `MKE_I2C_RFID` library. It systematically tests:
1. **Module Information Retrieval**: Firmware version, I2C Address, Antenna Gain, etc.
2. **Card Detection and Information**: UID, SAK, Card Type.
3. **Data Block Operations**: Authenticating, reading, and writing 16-byte data blocks on a MIFARE Classic card.
4. **Value Block Operations**: Formatting a block as a "Value Block" (for applications like e-wallets or point systems) and testing `setValue`, `incrementValue`, `decrementValue`, and `getValue`.

### Wiring
Connect the MakerEdu I2C RFID RC522 module to your Arduino via I2C:

| MKE Module Pin | Arduino / ESP32 Pin | Description |
| :---: | :---: | :--- |
| **GND** | GND | Ground |
| **VCC** | 5V or 3.3V | Power Supply (Matches your MCU's logic level) |
| **SDA** | SDA (e.g., A4 on Uno) | I2C Data |
| **SCL** | SCL (e.g., A5 on Uno) | I2C Clock |

### Instructions
1. Upload the sketch `99_Comprehensive_Test.ino` to your Arduino.
2. Open the Serial Monitor at **115200 baud**.
3. Bring a MIFARE Classic 1K card close to the module's antenna. **Note: Ensure it is a test card because data will be overwritten on Sector 1.**
4. Observe the Serial Monitor to verify that all operations complete successfully (Status `0`).

---

<a id="tiếng-việt"></a>
## 🇻🇳 Tiếng Việt

### Mô tả
Ví dụ này là một bài kiểm tra toàn diện được thiết kế để xác minh hoạt động của tất cả các API chính do thư viện `MKE_I2C_RFID` cung cấp. Code sẽ kiểm thử theo trình tự:
1. **Lấy thông tin Module**: Phiên bản Firmware, Địa chỉ I2C, Độ lợi Antenna (Antenna Gain), v.v.
2. **Phát hiện thẻ và Thông tin thẻ**: Quét UID, SAK, Loại thẻ.
3. **Thao tác dữ liệu Block (Data Block)**: Xác thực khóa (Authentication), đọc và ghi 16 byte dữ liệu vào một block trên thẻ MIFARE Classic.
4. **Thao tác giá trị Block (Value Block)**: Định dạng block thành "Value Block" (dùng cho các ứng dụng như ví điện tử hoặc hệ thống tích điểm) và chạy thử các lệnh `setValue`, `incrementValue`, `decrementValue`, và `getValue`.

### Đấu nối
Kết nối mạch MakerEdu I2C RFID RC522 với Arduino qua I2C:

| Chân Module MKE | Chân Arduino / ESP32 | Mô tả |
| :---: | :---: | :--- |
| **GND** | GND | Nối Đất |
| **VCC** | 5V hoặc 3.3V | Nguồn cấp (Phù hợp với điện áp giao tiếp của Vi điều khiển) |
| **SDA** | SDA (vd: A4 trên Uno) | Dữ liệu I2C |
| **SCL** | SCL (vd: A5 trên Uno) | Xung nhịp I2C |

### Hướng dẫn
1. Nạp code `99_Comprehensive_Test.ino` vào mạch Arduino của bạn.
2. Mở cửa sổ Serial Monitor ở tốc độ **115200 baud**.
3. Đưa một thẻ MIFARE Classic 1K lại gần ăng-ten của mạch. **Lưu ý: Hãy dùng một thẻ nháp (test card) vì dữ liệu trên Sector 1 sẽ bị ghi đè thay đổi.**
4. Quan sát cửa sổ Serial Monitor để kiểm tra xem mọi thao tác có hoàn tất thành công hay không (Status báo `0`).
