import sensor, image, time  # Import image sensor and time modules
from matrix_mini import send_data  # Import the send_data function from the matrix_mini module

# Initialize the sensor
sensor.reset()  # Reset the sensor
sensor.set_pixformat(sensor.RGB565)  # Set the pixel format to RGB565
sensor.set_framesize(sensor.QVGA)  # Set the image resolution to QVGA (320x240)

# Khởi động camera và đợi cảm biến ổn định phơi sáng
sensor.skip_frames(time=2000)  # Wait for 2 seconds to stabilize the sensor
# -------------------------------------------------------------------

# Set image flipping
sensor.set_vflip(True)  # Vertically flip the image
sensor.set_hmirror(True)  # Horizontally mirror the image

# Define color threshold range (LAB/HSV), suitable for detecting the target color
threshold_obj = [((31, 0, 50, 8, 127, 4)), (0, 38, -128, -18, 127, 5), (14, 21, -128, -13, 127, -52)]  # Red # Green
clock = time.clock()  # Create a clock to calculate frames per second (FPS) of image processing
nearest_data = (-1, 0, 0, 0)


def finding(color_id):
    global nearest_data
    blobs = img.find_blobs([threshold_obj[color_id]], pixels_threshold=500, area_threshold=200)
    if color_id == 2:
        color_id = 1
    if blobs:  # If blobs are found

        # --- SỬA TÌM BLOB: Chọn khối có y_center (cy) lớn nhất ---
        # cy() càng lớn nghĩa là khối đó càng nằm dịch về phía dưới màn hình
        max_blob = max(blobs, key=lambda b: b.cy())
        # ---------------------------------------------------------

        img.draw_rectangle(max_blob.rect())  # Draw a rectangle around the largest blob

        # Calculate the center coordinates of the blob
        x_center = max_blob.cx()
        y_center = max_blob.cy()

        # Calculate the blob's area and round it to an integer
        blob_area = round(max_blob.area() / 2)

        # Send the center coordinates and area to an external system
        if nearest_data[2] <= y_center:
            nearest_data = (color_id, x_center, y_center, blob_area)

        # Mark the center point and coordinates text on the image
        img.draw_cross(x_center, y_center)  # Draw a cross at the center
        img.draw_string(0, 0, f"X:{x_center}, Y:{y_center}")  # Display the coordinates


while True:
    clock.tick()  # Start timing
    img = sensor.snapshot()  # Capture an image
    nearest_data = (-1, 0, 0, 0)
    # Find blobs (color regions) in the image that match the threshold
    finding(0)
    finding(1)
    finding(2)
    if nearest_data != (-1, 0, 0, 0):
        print(nearest_data)
        send_data([nearest_data[0], nearest_data[1], nearest_data[2], nearest_data[3]])
