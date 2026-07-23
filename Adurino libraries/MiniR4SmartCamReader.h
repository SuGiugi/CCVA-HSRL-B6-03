/**
 * @file MiniR4SmartCamReader.h
 * @brief Handling MiniR4.SmartCamReader functions.
 * @author MATRIX Robotics
 */
#ifndef MINIR4_SMART_CAM_READER
#define MINIR4_SMART_CAM_READER

#include <Arduino.h>

/**
 * @brief A class for reading data from mVision camera.
 *
 * This class provides functionality to initialize the mVision
 * communication and read valid data from the mVision.
 */

class MiniR4SmartCamReader
{
private:
public:
    MiniR4SmartCamReader() {}
    ~MiniR4SmartCamReader() {}

    bool hasReceivedValidData = false;   ///< Flag to indicate if valid data has been received.

    /**
     * @brief Initializes the serial communication with the smart camera.
     */
    void Begin(void) { Serial1.begin(115200); }

    /**
     * @brief Reads data from the smart camera.
     *
     * This function waits for data from the smart camera and returns
     * the decoded data if it is valid. It uses a timeout to prevent
     * blocking indefinitely.
     *
     * @param data Pointer to an array where the received data will be stored.
     * @param timeout Time in milliseconds to wait for data (default is 500).
     * @return The number of data items read, or a negative error code:
     *         -1 if timeout occurred,
     *         -2 if data is incomplete,
     *         -3 if checksum validation fails.
     */
    int SmartCamReader(unsigned int* data, unsigned int timeout = 500)
    {
        byte         length, check;     // 声明局部变量
        byte         data_buffer[40];   // 数据缓冲区
        unsigned int system_time;       // 系统时间

        system_time = millis();   // 获取当前系统时间
        // 等待至少两个字节的数据或超时
        while ((millis() - system_time < timeout) && (Serial1.available() < 2));

        if (Serial1.available() >= 2) {     // 如果至少有两个字节的数据
            if (Serial1.read() == 0xAA) {   // 检查头字节
                length = Serial1.read();    // 读取数据长度
                check  = 0xAA ^ length;     // 初始化校验位

                system_time = millis();   // 重置系统时间
                // 等待足够多的数据或超时
                while ((millis() - system_time < timeout) &&
                       (Serial1.available() < length * 2 + 1)) {}

                if (Serial1.available() >= length * 2 + 1) {   // 如果数据足够
                    for (unsigned char n = 0; n < length * 2; n++) {
                        data_buffer[n] = Serial1.read();           // 读取数据到缓冲区
                        check          = check ^ data_buffer[n];   // 更新校验位
                    }
                    if (check == Serial1.read()) {      // 如果校验成功
                        while (Serial1.available()) {   // 清空缓冲区
                            Serial1.read();
                        }
                        for (unsigned char n = 0; n < length; n++) {   // 解码数据
                            data[n] = (data_buffer[n * 2 + 1] << 8) + data_buffer[n * 2];
                        }
                        hasReceivedValidData = true;
                        return length;   // 返回数据长度
                    } else {
                        hasReceivedValidData = true;
                        return -3;   // 校验失败
                    }
                } else {
                    hasReceivedValidData = true;
                    return -2;   // 数据不完整
                }
            }
        } else {
            if (!hasReceivedValidData) {
                Serial1.write("start");
            }
            return -1;   // 超时或没有数据
        }
    }

    int Data(unsigned int* data, unsigned int timeout = 200)
    {
        byte length, check;
        byte data_buffer[40];
        unsigned int system_time;

        // BƯỚC 1: XÓA SẠCH BỘ ĐỆM CŨ (Giải quyết vấn đề của bạn)
        // Bỏ hết tất cả dữ liệu cũ đang tồn đọng trong hàng đợi UART
        while (Serial1.available() > 0) {
            Serial1.read();
        }

        // BƯỚC 2: GỬI LỆNH YÊU CẦU DỮ LIỆU MỚI
        // Gửi ký tự 'G' (Get) sang để yêu cầu camera chụp ảnh và xử lý ngay lúc này
        Serial1.write('G');

        // BƯỚC 3: ĐỢI PHẢN HỒI TỪ CAMERA (Đợi nhận Header 0xAA và Length)
        system_time = millis();
        while ((millis() - system_time < timeout) && (Serial1.available() < 2));

        if (Serial1.available() >= 2) {
            if (Serial1.read() == 0xAA) {   // Kiểm tra Byte tiêu đề
                length = Serial1.read();    // Đọc độ dài dữ liệu
                check = 0xAA ^ length;

                // Đợi nhận đủ toàn bộ gói dữ liệu còn lại
                system_time = millis();
                while ((millis() - system_time < timeout) &&
                    (Serial1.available() < length * 2 + 1)) {
                }

                if (Serial1.available() >= length * 2 + 1) {
                    for (unsigned char n = 0; n < length * 2; n++) {
                        data_buffer[n] = Serial1.read();
                        check = check ^ data_buffer[n];
                    }

                    // Kiểm tra mã Checksum
                    if (check == Serial1.read()) {
                        // Giải mã dữ liệu
                        for (unsigned char n = 0; n < length; n++) {
                            data[n] = (data_buffer[n * 2 + 1] << 8) + data_buffer[n * 2];
                        }
                        hasReceivedValidData = true;
                        return length;  // Trả về thành công
                    }
                    else {
                        return -3;      // Sai checksum
                    }
                }
                else {
                    return -2;          // Dữ liệu không đủ (Timeout giữa chừng)
                }
            }
        }
        return -1; // Không nhận được phản hồi từ camera (Timeout)
	}

    int reset() {
        Serial1.write('L');
    }
};

#endif
