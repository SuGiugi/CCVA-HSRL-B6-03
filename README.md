# Xe tự hành-bảng B6-WRO 2026
Chương trình điều khiển robot tự hành sử dụng **Matrix Mini R4**, bộ công cụ **MATRIX WRO Future Innovators Set V2**. Robot thực hiện nhiệm vụ di chuyển theo luật thi đấu bảng B6-WRO2026 với 2 vòng thi đấu Open Challange, Obstacle Challenge bằng laser sensor V2, color sensor V3 và M-vision camera.
# Giới thiệu
Tên nhóm CCVA-HSLR-B6-03
Nhóm gồm 3 thành viên
- Trần Gia Hoàng Nam-Nhóm trưởng
- Nguyễn Trọng Minh
- Ngô Đức An
- Đơn vị đại diện: Hung Steam Robotics Lab-THPT Chu Văn An

# Cài đặt môi trường phát triển

Để lập trình và vận hành robot, cần cài đặt các phần mềm và thư viện theo các bước sau.

## 1. Cài đặt Arduino IDE

Truy cập trang tài nguyên của MATRIX Robotics:

> https://www.matrixrobotics.com/adv-program-resources

Tải và cài đặt **Arduino IDE** theo hướng dẫn.

Để biết chi tiết cách cài đặt và sử dụng thư viện MATRIX Mini R4, tham khảo tài liệu:

> https://username.github.io/repository

Mở tài liệu **MATRIX_R4_Guidebook_EN_V3.0.pdf**, sau đó đến **Mục 2.11.2 – Arduino Programming & Library Overview (Trang 45)** và thực hiện theo hướng dẫn.

---

## 2. Cài đặt OpenMV IDE

Sau khi cài đặt Arduino IDE, tiếp tục cài đặt **OpenMV IDE** để lập trình cho camera OpenMV.

Hướng dẫn cài đặt:

> https://wro-learn.org/en_us/wiki/m-vision-camera

---
- Thay vì sử dụng mẫu Open MV trong Link thì truy cập: https://github.com/SuGiugi/CCVA-HSRL-B6-03/tree/main/OpenMV và cài đặt giống hướng dẫn 

## 3. Cài đặt thư viện giao tiếp với camera

Để MATRIX Mini R4 có thể giao tiếp với camera OpenMV, tải tệp sau:

> https://github.com/minhkt1/Cu-c-thi-wro-B6-2026/blob/main/MiniR4SmartCamReader.h

Sau khi tải về, sao chép tệp **MiniR4SmartCamReader.h** vào thư mục:

```text
Documents/
└── Arduino/
    └── libraries/
        └── Matrix Mini R4/
            └── src/
                └── module/
                    └── sensor/
                    
```

Nếu hệ thống hiển thị thông báo ghi đè tệp, chọn **Replace** để thay thế tệp hiện có.

---

## Hoàn tất

Sau khi hoàn thành các bước trên, môi trường phát triển đã được thiết lập đầy đủ và sẵn sàng để:

- Biên dịch chương trình trên Arduino IDE.
- Nạp chương trình cho MATRIX Mini R4.
- Lập trình và sử dụng camera OpenMV.

## Bộ dụng cụ và hướng dẫn lắp ráp

Robot được phát triển trên nền tảng **MATRIX WRO Future Innovators Set V2**, cung cấp đầy đủ các chi tiết cơ khí, bộ điều khiển và phụ kiện cần thiết để xây dựng một robot phục vụ cho các cuộc thi WRO. Việc sử dụng bộ dụng cụ này giúp đảm bảo tính tương thích giữa các linh kiện, đồng thời thuận tiện cho quá trình lắp ráp, bảo trì và nâng cấp trong quá trình phát triển dự án.
Toàn bộ robot được lắp ráp dựa trên hướng dẫn chính thức của MATRIX Robotics. Người dùng nên thực hiện theo đúng trình tự các bước trong tài liệu để đảm bảo kết cấu cơ khí chính xác và các bộ phận được lắp đặt đúng vị trí. Sau khi hoàn thành việc lắp ráp, cần kiểm tra lại độ chắc chắn của các mối ghép, khả năng chuyển động của các cơ cấu và vị trí lắp đặt của các thiết bị trước khi tiến hành cài đặt phần mềm và nạp chương trình cho robot.

Hướng dẫn lắp ráp chi tiết được cung cấp tại liên kết dưới đây:https://github.com/SuGiugi/CCVA-HSRL-B6-03/blob/main/Instruction/CCVA-HSRL-B6-03.pdf

## Hệ thống cảm biến
- Hai laser sensor V2
- Color senser V3
- M-vision camera
- Động cơ DC
- Servo
## 🚗 Chức năng chính

# Hướng dẫn sử dụng

Repository bao gồm hai chương trình:

- **Obstacle Challenge**: Chương trình vượt chướng ngại vật. Link: https://github.com/SuGiugi/CCVA-HSRL-B6-03/blob/main/Code/Osbtacle_Challenge/Osbtacle_Challenge.ino

- **Open Challenge**: Chương trình thử thách mở. Link: https://github.com/SuGiugi/CCVA-HSRL-B6-03/blob/main/Code/Open_Challenge/Open_Challenge.ino

Mỗi chương trình sử dụng các hàm khác nhau để điều khiển robot.

---

# Obstacle Challenge

## Các hàm chính

| Hàm | Chức năng |
|------|-----------|
| `limit(float value, float min, float max)` | Giới hạn giá trị trong khoảng cho phép trước khi điều khiển góc lái. |
| `servoMotor(float value, float l = 50)` | Điều khiển góc quay của servo lái dựa trên giá trị điều khiển. |
| `doduong_laser_trai(float khoang_cach, float kp, float kd)` | Bám tường bên trái bằng cảm biến laser và bộ điều khiển PD. |
| `doduong_laser_phai(float khoang_cach, float kp, float kd)` | Bám tường bên phải bằng cảm biến laser và bộ điều khiển PD. |
| `line_check()` | Phát hiện vạch màu trên sân và đếm số vòng robot đã hoàn thành. |
| `turn()` | Kiểm tra khi robot đến góc sân, thực hiện quét bằng camera để tìm khối tiếp theo và điều chỉnh hướng di chuyển. |
| `dichuyen_cm(float quang_duong)` | Di chuyển theo quãng đường xác định bằng encoder, đồng thời vẫn theo dõi điều kiện chuyển trạng thái. |
| `last_step(bool check = true)` | Thực hiện thao tác hoàn tất sau khi robot vượt qua hoặc tránh một khối chướng ngại vật. |
| `setup()` | Khởi tạo bộ điều khiển, cảm biến, camera và các thiết bị ngoại vi. |
| `loop()` | Chương trình điều khiển chính của robot trong vòng Obstacle Challenge. |

---

## Chức năng chương trình

Chương trình thực hiện các nhiệm vụ:
- Khởi tạo toàn bộ cảm biến.
- Xác định chiều chạy dựa trên dữ liệu cảm biến laser.
- Bám tường bằng cảm biến laser.
- Phát hiện khối bằng camera OpenMV.
- Điều chỉnh hướng tiếp cận khối.
- Tránh khối đỏ hoặc khối xanh theo thuật toán.
- Đếm số vòng bằng cảm biến màu.
- Dừng robot sau khi hoàn thành số vòng quy định.

---

# Open Challenge

## Các hàm chính

| Hàm | Chức năng |
|------|-----------|
| `ChieuBenTrai()` | Điều khiển robot hoàn thành vòng thi theo chiều ngược chiều kim đồng hồ (bên trái). |
| `ChieuBenPhai()` | Điều khiển robot hoàn thành vòng thi theo chiều kim đồng hồ (bên phải). |
| `DICHUYEN_TOCDO_n_cm_n(float tocdo, float cm)` | Di chuyển một quãng đường xác định với tốc độ đặt trước. |
| `DoTuongTrai_TocDo_n_Khoangcach_mm_n_kp_n_kd_n(...)` | Bám tường bên trái bằng cảm biến laser và bộ điều khiển PD. |
| `BamTuongPhai_TocDo_n_KC_MM_n_kp_n_kd_n(...)` | Bám tường bên phải bằng cảm biến laser và bộ điều khiển PD. |
| `DoDuong_Trai_Cm_n_TocDo_n_KhoangCach_Mm_n_kp_n_kd_n(...)` | Di chuyển theo quãng đường xác định đồng thời bám tường bên trái. |
| `DoDuongPhai_Cm_n_TocDo_n_KC_MM_n_kp_n_kd_n(...)` | Di chuyển theo quãng đường xác định đồng thời bám tường bên phải. |
| `camera()` | Xử lý dữ liệu nhận được từ camera OpenMV. |
| `TranhKhoi_Do()` | Điều khiển robot tránh khối màu đỏ. |
| `TranhKhoi_Xanh()` | Điều khiển robot tránh khối màu xanh. |
| `setup()` | Khởi tạo bộ điều khiển, cảm biến và các thiết bị ngoại vi. |
| `loop()` | Chương trình điều khiển chính của robot trong vòng Open Challenge. |

---

## Chức năng chương trình

Chương trình thực hiện các nhiệm vụ:

- Chờ người dùng nhấn nút khởi động.
- Di chuyển đến vị trí nhận biết màu vạch đầu tiên.
- Xác định chiều chạy dựa trên màu của vạch.
- Gọi `ChieuBenTrai()` hoặc `ChieuBenPhai()` tương ứng.
- Bám tường bằng cảm biến laser.
- Đếm đủ số vòng theo chương trình.
- Dừng robot sau khi hoàn thành nhiệm vụ.
## Cấu trúc thư mục

Repository được tổ chức theo các thư mục chức năng nhằm thuận tiện cho việc cài đặt, phát triển và tham khảo tài liệu của dự án.

```text
CCVA-HSRL-B6-03
│
├── Adurino libaries/     # Thư viện sử dụng cho Arduino IDE    
├── Code/       # Mã nguồn chương trình điều khiển robot
        ├── Osbtacle_Challenge/ # Mã vòng thử thách vượt chướng ngại vật
        └── Open_Challenge/  # Mã vòng thử thách mở
├── Instruction/           # Hướng dẫn cài đặt và sử dụng
├── OpenMV/                # Chương trình xử lý ảnh cho OpenMV
├── Pictures/              # Hình ảnh robot và các thành phần
├── Videos/                # Video minh họa quá trình hoạt động
└── README.md              # Tài liệu giới thiệu dự án
```

### Mô tả các thư mục

#### `Adurino libaries/`

Chứa các thư viện cần thiết để biên dịch và chạy chương trình trên **Arduino IDE**. Người dùng nên cài đặt các thư viện này trước khi nạp chương trình cho robot.

#### `Code/`

Chứa mã nguồn chính của robot, bao gồm các thuật toán điều khiển, xử lý cảm biến và các hàm phục vụ quá trình thi đấu.

#### `Instruction/`

Chứa các tài liệu hướng dẫn cài đặt môi trường phát triển, lắp ráp robot và các hướng dẫn cần thiết để sử dụng dự án.

#### `OpenMV/`

Chứa chương trình chạy trên camera **OpenMV**, phục vụ việc xử lý ảnh và nhận diện đối tượng trong quá trình robot hoạt động.

#### `Pictures/`

Lưu trữ hình ảnh của robot, các bộ phận, mô hình lắp ráp và hình ảnh minh họa phục vụ tài liệu dự án.

#### `Videos/`

Chứa các video trình diễn robot, quá trình thử nghiệm và hoạt động của hệ thống.
