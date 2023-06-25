import time, sensor, image, math, pyb
from image import SEARCH_EX, SEARCH_DS
#从imgae模块引入SEARCH_EX和SEARCH_DS。使用from import仅仅引入SEARCH_EX,
#SEARCH_DS两个需要的部分，而不把image模块全部引入。

import ustruct
from pyb import UART
from pyb import LED
from pyb import Pin
from pyb import SPI
# Select an area in the Framebuffer to copy the color settings.
#led3 = pyb.LED(3) # Red LED = 1, Green LED = 2, Blue LED = 3, IR LEDs = 4.
#led1 = pyb.LED(1) # Red LED = 1, Green LED = 2, Blue LED = 3, IR LEDs = 4.

pin1 = pyb.Pin("P0", Pin.OUT_PP) # MSB
pin2 = pyb.Pin("P1", Pin.OUT_PP) # LSB

# Reset sensor
sensor.reset()

# Set sensor settings
sensor.set_contrast(1)
sensor.set_gainceiling(16)
# Max resolution for template matching with SEARCH_EX is QQVGA
sensor.set_framesize(sensor.QQVGA)
# You can set windowing to reduce the search image.
#sensor.set_windowing(((640-80)//2, (480-60)//2, 80, 60))
sensor.set_pixformat(sensor.GRAYSCALE)


# Load template.
# Template should be a small (eg. 32x32 pixels) grayscale image.
# templates = ["/up1.pgm", "/up2.pgm", "/up3.pgm", "/up4.pgm", "/up5.pgm", "/up6.pgm", "/left1.pgm", "/right1.pgm"] #保存多个模板
#加载模板图片
templates = ["/up1.pgm", "/up2.pgm", "/up3.pgm", "/right1.pgm", "/right2.pgm", "/right3.pgm", "/left1.pgm", "/left2.pgm", "/left3.pgm"]


clock = time.clock()

flag=0;

# Run template matching
while (flag==0):
    led=LED(1)
    led.on()
    clock.tick()
    img = sensor.snapshot()

    # find_template(template, threshold, [roi, step, search])
    # ROI: The region of interest tuple (x, y, w, h).
    # Step: The loop step used (y+=step, x+=step) use a bigger step to make it faster.
    # Search is either image.SEARCH_EX for exhaustive search or image.SEARCH_DS for diamond search
    #
    # Note1: ROI has to be smaller than the image and bigger than the template.
    # Note2: In diamond search, step and ROI are both ignored.
    for t in templates:
        template = image.Image(t)
        #对每个模板遍历进行模板匹配
        r = img.find_template(template, 0.70, step=4, search=SEARCH_EX)#roi=(0, 0, 90, 90))
    #find_template(template, threshold, [roi, step, search]),threshold中
    #的0.7是相似度阈值,roi是进行匹配的区域（左上顶点为（10，0），长80宽60的矩形），
    #注意roi的大小要比模板图片大，比frambuffer小。
    #把匹配到的图像标记出来
        if r:
            img.draw_rectangle(r)
            if t == '/right1.pgm':
                flag=1;
                break;
            elif t == '/right2.pgm':
                flag=1;
                break;
            elif t == '/right3.pgm':
                flag=1;
                break;
            elif t == '/up1.pgm':
                flag=2;
                break;
            elif t == '/up2.pgm':
                flag=2;
                break;
            elif t == '/up3.pgm':
                flag=2;
                break;
            elif t == '/left1.pgm':
                flag=3;
                break;
            elif t == '/left2.pgm':
                flag=3;
                break;
            elif t == '/left3.pgm':
                flag=3;
                break;

#right 执行path1，靠右边走，01
while (flag==1):
    start_time = time.time() # 获取当前时间
    while time.time() - start_time < 25: # 如果过去的时间小于10秒，就继续循环
        pin1.low()
        pin2.high()
        print(flag)
        pass
    pin1.low() # 把两个pin都设为low
    pin2.low()
    break

#up 执行path2，中间直行，10
while (flag==2):
    start_time = time.time() # 获取当前时间
    while time.time() - start_time < 25: # 如果过去的时间小于10秒，就继续循环
        pin1.high()
        pin2.low()
        print(flag)
        pass
    pin1.low() # 把两个pin都设为low
    pin2.low()
    break

#left 执行path3，靠最左走，11
while (flag==3):
    start_time = time.time() # 获取当前时间
    while time.time() - start_time < 25: # 如果过去的时间小于10秒，就继续循环
        pin1.high()
        pin2.high()
        print(flag)
        pass
    pin1.low() # 把两个pin都设为low
    pin2.low()
    break
