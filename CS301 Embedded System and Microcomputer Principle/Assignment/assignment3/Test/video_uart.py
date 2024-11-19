import cv2
import numpy as np
import serial
import time

print("Processing video...")
# 视频文件的路径
video_path = "./video_material.mp4"
# 创建视频读取对象
cap = cv2.VideoCapture(video_path)

# 检查视频是否成功打开
if not cap.isOpened():
    print("Error: Could not open video.")
    exit()

# 获取视频的帧率
fps = cap.get(cv2.CAP_PROP_FPS)

# 计算每秒需要处理的帧数
frames_per_second = 10

# 计算每帧的时间间隔（以帧为单位）
frame_interval = int(fps / frames_per_second)

# 创建串口对象
ser = serial.Serial("/dev/tty.usbserial-1220", 1000000, 8, "N", 1, timeout=None)

# 存储处理后的帧数据
all_frame_data = []

# 读取视频帧
frame_count = 0
while True:
    ret, frame = cap.read()
    if not ret:
        break

    # 检查是否是我们需要处理的帧
    if frame_count % frame_interval == 0:
        # 转换为RGB颜色空间
        frame_rgb = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)

        # 缩放图片到45x60
        resized_frame = cv2.resize(frame_rgb, (60, 45), interpolation=cv2.INTER_AREA)

        # 获取图像的尺寸
        height, width = resized_frame.shape[:2]

        # 创建一个空的数组来存储16位的像素值
        frame_16bit = np.zeros((height, width), dtype=np.uint16)

        # 将RGB图像转换为16位真色彩（RGB565格式）
        for y in range(height):
            for x in range(width):
                r = np.uint16((resized_frame[y, x, 0] >> 3)) << 11  # 红色5位
                g = np.uint16((resized_frame[y, x, 1] >> 2)) << 5   # 绿色6位
                b = (resized_frame[y, x, 2] >> 3) & 0x1F            # 蓝色5位
                frame_16bit[y, x] = r | g | b

        # 将图像转换为一维数组
        flat_array = frame_16bit.flatten()

        # 存储处理后的帧数据
        all_frame_data.append(flat_array)

    frame_count += 1

# 发送处理后的帧数据
input("Press Enter to start sending...")
print("Start sending...")
for frame_index, frame_data in enumerate(all_frame_data):
    # 发送固定标志位
    frame_marker = 0x1234
    ser.write(frame_marker.to_bytes(2, byteorder='little'))  # 发送16位固定标志位

    # 检查是否接收到停止信号
    signal = ser.read(2)
    if signal.startswith(b'\x15\x15'):
        print("Stopped")
        resume = ser.read(2)
        if resume.startswith(b'\x14\x14'):
            print("Resumed")
        else:
            print("Exit")
            break
        time.sleep(0.0001)
    elif signal.startswith(b'\x16\x16'):
        print("Exit")
        break

    # 发送帧数据
    frames_all_bytes = [i.astype(np.uint16).tobytes() for i in frame_data]
    ser.write(b''.join(frames_all_bytes))  # 发送16位数据

    time.sleep(1 / frames_per_second)


# 释放视频读取对象
cap.release()
# 关闭串口
ser.close()

print("所有帧已发送完毕。")
