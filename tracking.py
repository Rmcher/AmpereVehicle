import sensor, image, time, math, pyb

sensor.reset()                      # Reset and initialize the sensor.
sensor.set_pixformat(sensor.GRAYSCALE) # Set pixel format to RGB565 (or GRAYSCALE)
sensor.set_framesize(sensor.QQVGA)   # Set frame size to QQVGA (160x120)
sensor.skip_frames(time = 2000)     # Wait for settings take effect.
clock = time.clock()                # Create a clock object to track the FPS.
kernel_size = 1
kernel = [-1, -1,  -1, \
            -1,  8,  -1, \
            -1, -1,  -1]
line_threshold = (134,255)
white_threshold = (255,255)

pin1 = pyb.Pin("P0", pyb.Pin.OUT_PP)
pin2 = pyb.Pin("P1", pyb.Pin.OUT_PP)

while(True):
    clock.tick()
    img = sensor.snapshot()
    #img.find_edges(image.EDGE_CANNY, threshold=(50, 80))
    img.morph(kernel_size, kernel) #核滤波
    img.binary([line_threshold],invert=False)#根据阈值将像素设置为黑白（<黑）
    center_x = img.width() // 2
    blobs = img.find_blobs([white_threshold],area_threshold=5, pixels_threshold=5)
    if blobs:
        max_blob = max(blobs, key=lambda b: b.pixels())# 获取像素数量最大的白色区域
        white_center = max_blob.cx()#取白色区域的中心点坐标
    diff = center_x - white_center

   #img.erode(2,threshold=1)#腐蚀-threshold值越大，边缘周围白色杂点越少
    line_segments = img.find_line_segments(merge_distance=0, max_theta_difference=20)#查找线段
    if line_segments:
        for line_segment in line_segments:#line_noisy依次代表line_noises中的元素
            img.draw_line(line_segment.line(),color=0,thickness=3)#画黑线

    line_regression = img.get_regression([white_threshold])

    if line_regression:
        img.draw_line(line_regression.line(), color=100,thickness=8)
        original=line_regression.line()
        #QQVGA大小为160x120
        if (original[0]==0):
            if (original[2]==159):#起点在左边缘（x=0）&终点在右边缘（x=159）
                a=int(original[3]*1.33)#将坐标映射到QVGA（320x240）图像尺寸的坐标-在更高分辨率下更好的显示直线
                b=0
                c=int(original[1]*1.33)
                d=119
            if (original[3]==0):
                a=0
                b=int((160-original[2])*0.75)
                c=int(original[1]*1.33)
                d=119
            if (original[3]==119):
                a=int(original[1]*1.33)
                b=119
                c=159
                d=int((160-original[2])*0.75)
        if (original[1]==0):#在图像顶部检测到线（顶部端点y坐标）
            a=0
            b=int((160-original[0])*0.75)#线的底端点向右移动，使线条居中
            c=159
            d=int((160-original[2])*0.75)#线条居中
        if (1):
            final=(a,b,c,d)
        img.draw_line((final), color=50,thickness=8)#水平线
        theta=line_regression.theta()-90#偏转角
        rho=int((a+c)/2-80)
        print(rho,theta,diff)

        if theta>0 and theta<70:#左转
            duty1 = 50
            duty2 = 75
            print("left")
            pin1.low()
            pin2.high()   #左转01
        elif theta<0 and theta>-65:#右转
            duty1 = 75
            duty2 = 50
            print("right")
            pin1.high()
            pin2.low()  #右转10
        else:#直线
            while(True):
                if diff>40:
                    print("left")
                    pin1.low()
                    pin2.high()
                    break
                elif diff<-40:
                    print("right")
                    pin1.high()
                    pin2.low()
                    break
                else:
                    print("go stright")
                    pin1.high()
                    pin2.high()
                    break
