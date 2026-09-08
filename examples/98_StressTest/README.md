# 98_StressTest

🇻🇳 **Tiếng Việt:**
Example này dùng để kiểm tra độ ổn định của giao tiếp I2C giữa Master và Slave ở tốc độ cao (Fast Mode 400kHz) và với tần suất gửi lệnh liên tục (Spam commands). 
Chương trình sẽ cố gắng xác thực và đọc Block 4 liên tục 1000 lần, xen kẽ với các lệnh `GetFirmwareVersion()`. Sau khi hoàn tất, nó thống kê tỷ lệ đọc thành công và tốc độ phản hồi. Việc test cường độ cao này giúp xác nhận xem bộ đệm (`rxBuffer`) trên I2C Slave (PY32) có chịu tải được hay không, có xảy ra hiện tượng rớt gói tin hoặc Race Condition hay không.

🇬🇧 **English:**
This example is designed to test the stability of the I2C communication between the Master and Slave at high speeds (Fast Mode 400kHz) and under high-frequency command spamming.
The program attempts to authenticate and read Block 4 continuously for 1000 iterations, interleaved with `GetFirmwareVersion()` commands. Afterward, it reports the success rate and response speed. This high-intensity test helps confirm whether the `rxBuffer` on the I2C Slave (PY32) can handle the heavy load without dropping packets or encountering Race Conditions.
