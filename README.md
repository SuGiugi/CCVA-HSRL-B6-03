# 🚗 Xe tự hành - Bảng B6 - WRO 2026

Chương trình điều khiển robot tự hành sử dụng **Matrix Mini R4**, bộ công cụ **MATRIX WRO Future Innovators Set V2**. Robot thực hiện nhiệm vụ di chuyển theo luật thi đấu bảng B6-WRO2026 với 2 vòng thi đấu Open Challenge và Obstacle Challenge bằng laser sensor V2, color sensor V3 và M-vision camera.

---

## 👥 Giới thiệu

* **Tên nhóm:** CCVA-HSLR-B6-03
* **Thành viên (3 người):**
  * 👑 **Trần Gia Hoàng Nam** - Nhóm trưởng
  * 🛠️ **Nguyễn Trọng Minh**
  * 🛠️ **Ngô Đức An**
* 🏫 **Đơn vị đại diện:** Hung Steam Robotics Lab - THPT Chu Văn An

---

## 💻 Cài đặt môi trường phát triển

Để lập trình và vận hành robot, cần cài đặt các phần mềm và thư viện theo các bước sau:

### 1. 🛠️ Cài đặt Arduino IDE
1. Truy cập trang tài nguyên của MATRIX Robotics: [MATRIX Robotics Resources](https://www.matrixrobotics.com/adv-program-resources)
2. Tải và cài đặt **Arduino IDE** theo hướng dẫn.
3. Để biết chi tiết cách cài đặt và sử dụng thư viện MATRIX Mini R4, tham khảo [API Docs](https://matrix-robotics.github.io/Programming-API-Docs/#).
4. Mở tài liệu `MATRIX_R4_Guidebook_EN_V3.0.pdf`, sau đó đến **Mục 2.11.2 – Arduino Programming & Library Overview (Trang 45)** và thực hiện theo hướng dẫn.

### 2. 📷 Cài đặt OpenMV IDE
1. Sau khi cài đặt Arduino IDE, tiếp tục cài đặt **OpenMV IDE** để lập trình cho camera OpenMV. 
2. Hướng dẫn cài đặt xem tại [WRO Learn Guide](https://wro-learn.org/en_us/wiki/m-vision-camera).
3. ⚠️ *Lưu ý:* Thay vì sử dụng mẫu OpenMV trong link hướng dẫn, hãy truy cập thư mục [OpenMV](https://github.com/SuGiugi/CCVA-HSRL-B6-03/tree/main/OpenMV) trong repo này và cài đặt tương tự.

### 3. 📚 Cài đặt thư viện giao tiếp với camera
Để MATRIX Mini R4 có thể giao tiếp với camera OpenMV:
1. Tải tệp thư viện từ thư mục [Arduino libraries](https://github.com/SuGiugi/CCVA-HSRL-B6-03/tree/main/Adurino%20libraries).
2. Sao chép tệp `MiniR4SmartCamReader.h` vào thư mục:
   ```text
   Documents/
   └── Arduino/
       └── libraries/
           └── Matrix Mini R4/
               └── src/
                   └── module/
                       └── sensor/
   ```

3. Nếu hệ thống hiển thị thông báo ghi đè tệp, chọn **Replace** để thay thế tệp hiện có.

### ✅ Hoàn tất

Sau khi hoàn thành các bước trên, môi trường phát triển đã sẵn sàng để:

* ⚙️ Biên dịch chương trình trên Arduino IDE.
* 📥 Nạp chương trình cho MATRIX Mini R4.
* 📸 Lập trình và sử dụng camera OpenMV.

---

## 🧩 Bộ dụng cụ & Hướng dẫn lắp ráp

Robot được phát triển trên nền tảng **MATRIX WRO Future Innovators Set V2**, cung cấp đầy đủ các chi tiết cơ khí, bộ điều khiển và phụ kiện cần thiết.

Toàn bộ robot được lắp ráp dựa trên hướng dẫn chính thức của MATRIX Robotics. Người dùng nên thực hiện đúng trình tự để đảm bảo kết cấu cơ khí chính xác.

📄 **Hướng dẫn lắp ráp chi tiết:** [ CCVA-HSRL-B6-03.pdf](https://github.com/SuGiugi/CCVA-HSRL-B6-03/blob/main/Instruction/CCVA-HSRL-B6-03.pdf)

---

## 📡 Hệ thống cảm biến & Phần cứng

* 🔴 **2 x Laser Sensor V2** (*Matrix Laser Sensor V2*)
* 🎨 **1 x Color Sensor V3** (*Matrix Color Sensor V3*)
* 📷 **1 x M-vision Camera** (*M-Vision Cam*)
* ⚙️ **1 x Động cơ DC** (*Main Drive Motor - Kèm Encoder*)
* 🔄 **1 x Động cơ Servo** (*Micro Servo MG90S*)

---

## 📖 Hướng dẫn sử dụng & Mã nguồn

Repository bao gồm hai chương trình điều khiển:

* 🚧 **Obstacle Challenge** (Thử thách vượt chướng ngại vật): [`Osbtacle_Challenge.ino`](https://github.com/SuGiugi/CCVA-HSRL-B6-03/blob/main/Code/Obstacle_Challenge/Obstacle_Challenge.ino)
* 🏁 **Open Challenge** (Thử thách mở): [`Open_Challenge.ino`](https://github.com/SuGiugi/CCVA-HSRL-B6-03/blob/main/Code/Open_Challenge/Open_Challenge.ino)

---

### 🚧 1. Obstacle Challenge

#### ⚙️ Các hàm chính:

| Hàm | Chức năng |
| --- | --- |
| `limit(float value, float min, float max)` | Giới hạn giá trị trong khoảng cho phép trước khi điều khiển góc lái. |
| `servoMotor(float value, float l = 50)` | Điều khiển góc quay của servo lái dựa trên giá trị điều khiển. |
| `doduong_laser_trai(float khoang_cach, float kp, float kd)` | Bám tường bên trái bằng cảm biến laser và bộ điều khiển PD. |
| `doduong_laser_phai(float khoang_cach, float kp, float kd)` | Bám tường bên phải bằng cảm biến laser và bộ điều khiển PD. |
| `line_check()` | Phát hiện vạch màu trên sân và đếm số vòng robot đã hoàn thành. |
| `turn()` | Kiểm tra khi robot đến góc sân, thực hiện quét camera tìm khối và điều chỉnh hướng. |
| `dichuyen_cm(float quang_duong)` | Di chuyển theo quãng đường xác định bằng encoder. |
| `last_step(bool check = true)` | Thực hiện thao tác hoàn tất sau khi vượt/tránh khối chướng ngại vật. |
| `setup()` | Khởi tạo bộ điều khiển, cảm biến, camera và thiết bị ngoại vi. |
| `loop()` | Chương trình điều khiển chính của vòng Obstacle Challenge. |

#### 🎯 Chức năng chương trình:

* 🔌 Khởi tạo toàn bộ cảm biến.
* 🔄 Xác định chiều chạy dựa trên dữ liệu cảm biến laser.
* 🧱 Bám tường bằng cảm biến laser.
* 🔍 Phát hiện khối bằng camera OpenMV.
* 🎯 Điều chỉnh hướng tiếp cận khối.
* 🔴🟢 Tránh khối đỏ hoặc khối xanh theo thuật toán.
* 🏁 Đếm số vòng bằng cảm biến màu và dừng robot khi hoàn thành.

---

### 🏁 2. Open Challenge

#### ⚙️ Các hàm chính:

| Hàm | Chức năng |
| --- | --- |
| `ChieuBenTrai()` | Điều khiển robot chạy ngược chiều kim đồng hồ (bên trái). |
| `ChieuBenPhai()` | Điều khiển robot chạy theo chiều kim đồng hồ (bên phải). |
| `DICHUYEN_TOCDO_n_cm_n(float tocdo, float cm)` | Di chuyển quãng đường xác định với tốc độ đặt trước. |
| `DoTuongTrai_TocDo_n_Khoangcach_mm_n_kp_n_kd_n(...)` | Bám tường bên trái bằng cảm biến laser và bộ điều khiển PD. |
| `BamTuongPhai_TocDo_n_KC_MM_n_kp_n_kd_n(...)` | Bám tường bên phải bằng cảm biến laser và bộ điều khiển PD. |
| `DoDuong_Trai_Cm_n_TocDo_n_KhoangCach_Mm_n_kp_n_kd_n(...)` | Di chuyển theo quãng đường kết hợp bám tường trái. |
| `DoDuongPhai_Cm_n_TocDo_n_KC_MM_n_kp_n_kd_n(...)` | Di chuyển theo quãng đường kết hợp bám tường phải. |
| `camera()` | Xử lý dữ liệu nhận được từ camera OpenMV. |
| `TranhKhoi_Do()` | Điều khiển robot tránh khối màu đỏ. |
| `TranhKhoi_Xanh()` | Điều khiển robot tránh khối màu xanh. |
| `setup()` | Khởi tạo bộ điều khiển, cảm biến và thiết bị ngoại vi. |
| `loop()` | Chương trình điều khiển chính của vòng Open Challenge. |

#### 🎯 Chức năng chương trình:

* 🔘 Chờ người dùng nhấn nút khởi động.
* 🚦 Di chuyển đến vị trí nhận biết màu vạch đầu tiên.
* 🔄 Xác định chiều chạy dựa trên màu của vạch (`ChieuBenTrai()` hoặc `ChieuBenPhai()`).
* 🧱 Bám tường bằng cảm biến laser.
* 🏁 Đếm đủ số vòng và dừng robot sau khi hoàn thành nhiệm vụ.

---

## 📂 Cấu trúc thư mục

```text
CCVA-HSRL-B6-03
│
├── 📚 Adurino libraries/  # Thư viện sử dụng cho Arduino IDE
├── 💻 Code/               # Mã nguồn chương trình điều khiển robot
│   ├── 🚧 Osbtacle_Challenge/ # Mã vòng thử thách vượt chướng ngại vật
│   └── 🏁 Open_Challenge/    # Mã vòng thử thách mở
├── 📄 Instruction/        # Hướng dẫn cài đặt và lắp ráp
├── 📷 OpenMV/             # Chương trình xử lý ảnh cho OpenMV
├── 🖼️ Pictures/            # Hình ảnh robot và các thành phần
├── 🎥 Videos/              # Video minh họa quá trình hoạt động
└── 📘 README.md            # Tài liệu giới thiệu dự án

```

### 📝 Mô tả chi tiết thư mục:

* 📚 **`Adurino libraries/`**: Chứa các thư viện cần thiết cho Arduino IDE.
* 💻 **`Code/`**: Mã nguồn chính chứa thuật toán điều khiển và xử lý cảm biến.
* 📄 **`Instruction/`**: Tài liệu hướng dẫn cài đặt môi trường và lắp ráp robot.
* 📷 **`OpenMV/`**: Code chạy trên camera OpenMV phục vụ nhận diện đối tượng.
* 🖼️ **`Pictures/`**: Hình ảnh thiết kế, linh kiện và tổng thể robot.
* 🎥 **`Videos/`**: Video chạy thử nghiệm và quá trình robot hoàn thành nhiệm vụ.

```

```
