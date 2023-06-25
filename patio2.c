#include "mbed.h"
#include "HCSR04.h"
#include <cstdio>

/* 电机引脚设置 */
DigitalOut  AIN_1(PA_9);
DigitalOut  AIN_2(D9);
DigitalOut  BIN_1(PB_5);
DigitalOut  BIN_2(PA_10);
DigitalOut  STBY(D10);


PwmOut PWMA(D7);
PwmOut PWMB(D6);
/* 电机引脚设置 */

//与openmv通信
DigitalIn A(PC_12);
DigitalIn B(PD_2);
// int A=0;
// int B=1;

/* 定义HC-SR04模块的引脚 */
HCSR04 HCSR04_1(PB_12, PB_11);
HCSR04 HCSR04_2(PB_13, PB_14);
HCSR04 HCSR04_3(PC_11, PC_10);
HCSR04 HCSR04_4(PC_9, PC_8);
/* 定义HC-SR04模块的引脚 */


/* 定义陀螺仪模块的引脚 */
BufferedSerial gyro(PC_4,PC_5);
/* 定义陀螺仪模块的引脚 */

DigitalOut  COM(PC_2);//黑色线
DigitalOut  SER(PC_3);//棕色线


float distance1=0;
float distance2=0;
float distance3=0;
float distance4=0;
float d34=0;
float ax,ay,az,gx,gy,gz,roll,pitch,yaw;

volatile int flag_u=1;//超声波标识符
volatile int flag_s=0;//超声波左转标识符

/* 标识符 */
volatile int flag=1;
/* 标识符 */

BufferedSerial pc(USBTX, USBRX,9600);

void data_print(){
    distance1=HCSR04_1.getCm();
    distance2=HCSR04_2.getCm();
    distance3=HCSR04_3.getCm();
    distance4=HCSR04_4.getCm();
    d34=distance3-distance4;

    printf("the distance of US-01 = %.1f\n", distance1);
    printf("the distance of US-02 = %.1f\n", distance2);
    printf("the distance of US-03 = %.1f\n", distance3);
    printf("the distance of US-04 = %.1f\n", distance4);
    printf("d34 = %.1f\n", d34);
    printf("Flag_u = %d\n\n", flag_u);
    printf("Flag_s = %d\n\n", flag_s);
    printf("Flag = %d\n\n", flag);
}

void communication(){
    COM=1;
}

void droptheball(){
    SER=1;
}

void getJY61data(void){
    short a[3], w[3], Angle[3], T;

    uint8_t chrTemp[256];
    static unsigned short RxLength = 0;
    uint8_t stcAcc[8],stcGyro[8],stcAngle[8];

    uint8_t chrBuffer[256];
    unsigned short Length=0,Cnt=0;

    while(gyro.readable()){
        gyro.read(&chrBuffer[Length++],1);
    }
    if (Length >= sizeof(chrBuffer)) {
            Length = 0;

            //function
            RxLength = 0;
            memcpy(chrTemp,chrBuffer,sizeof(chrBuffer));
	        RxLength += sizeof(chrBuffer);
            while (RxLength >= 11)
                {
                    if (chrTemp[0] != 0x55)
                    {
			            RxLength--;
			            memcpy(&chrTemp[0],&chrTemp[1],RxLength);                        
                        continue;
                    }
		            switch(chrTemp[1])
		            {
			            case 0x51:	memcpy(&stcAcc,&chrTemp[2],8);
                                        a[0] = (short)((short)(stcAcc[1]) << 8 | stcAcc[0]);
                                        a[1] = (short)((short)(stcAcc[3]) << 8 | stcAcc[2]);
                                        a[2] = (short)((short)(stcAcc[5]) << 8 | stcAcc[4]);
                                        break;
			            case 0x52:	memcpy(&stcGyro,&chrTemp[2],8);
                                        w[0] = (short)((short)(stcGyro[1]) << 8 | stcGyro[0]);
                                        w[1] = (short)((short)(stcGyro[3]) << 8 | stcGyro[2]);
                                        w[2] = (short)((short)(stcGyro[5]) << 8 | stcGyro[4]);
                                        break;
			            case 0x53:	memcpy(&stcAngle,&chrTemp[2],8);
                                        Angle[0] = (short)((short)(stcAngle[1]) << 8 | stcAngle[0]);
                                        Angle[1] = (short)((short)(stcAngle[3]) << 8 | stcAngle[2]);
                                        Angle[2] = (short)((short)(stcAngle[5]) << 8 | stcAngle[4]);
                                        break;
                        //default:    break;
		            }
		            RxLength -= 11;
		            memcpy(&chrTemp[0],&chrTemp[11],RxLength);                     
                }
    }
    else if (Length>0)
	{
	    RxLength = 0;

        //function
        memcpy(chrTemp,chrBuffer,Length);
	    RxLength += Length;
        while (RxLength >= 11)
        {
            if (chrTemp[0] != 0x55)
            {
			    RxLength--;
			    memcpy(&chrTemp[0],&chrTemp[1],RxLength);                        
                continue;
            }
		    switch(chrTemp[1])
		    {
			    case 0x51:	memcpy(&stcAcc,&chrTemp[2],8);
                            a[0] = (short)((short)(stcAcc[1]) << 8 | stcAcc[0]);
                            a[1] = (short)((short)(stcAcc[3]) << 8 | stcAcc[2]);
                            a[2] = (short)((short)(stcAcc[5]) << 8 | stcAcc[4]);
                            break;
			    case 0x52:	memcpy(&stcGyro,&chrTemp[2],8);
                            w[0] = (short)((short)(stcGyro[1]) << 8 | stcGyro[0]);
                            w[1] = (short)((short)(stcGyro[3]) << 8 | stcGyro[2]);
                            w[2] = (short)((short)(stcGyro[5]) << 8 | stcGyro[4]);
                            break;
			    case 0x53:	memcpy(&stcAngle,&chrTemp[2],8);
                            Angle[0] = (short)((short)(stcAngle[1]) << 8 | stcAngle[0]);
                            Angle[1] = (short)((short)(stcAngle[3]) << 8 | stcAngle[2]);
                            Angle[2] = (short)((short)(stcAngle[5]) << 8 | stcAngle[4]);
                            break;
        //default:    break;
		    }
		    RxLength -= 11;
		    memcpy(&chrTemp[0],&chrTemp[11],RxLength);                     
        }
	}
		wait_us(100000);
		
		if (Cnt++>=0)
		{
			Cnt=0;
            ax=(float)a[0]/32768*16;
            ay=(float)a[1]/32768*16;
            az=(float)a[2]/32768*16;
            gx=(float)w[0]/32768*2000;
            gy=(float)w[1]/32768*2000;
            gz=(float)w[2]/32768*2000;
            roll=(float)Angle[0]/32768*180;
            pitch=(float)Angle[1]/32768*180;
            yaw=(float)Angle[2]/32768*180;
		}	
   
}

void gostraight(){
    STBY=1;
    PWMA.period(0.0001f);  
    PWMB.period(0.0001f);  
    PWMA.write(0.4f);
    PWMB.write(0.413f);
    AIN_1=0;
    AIN_2=1;
    BIN_1=0;
    BIN_2=1;
}

void goback(){
    STBY=1;
    PWMA.period(0.0001f);  
    PWMB.period(0.0001f);  
    PWMA.write(0.30f);
    PWMB.write(0.31f);
    AIN_1=1;
    AIN_2=0;
    BIN_1=1;
    BIN_2=0;
}

void turnleft(){
    STBY=1;
    PWMA.period(0.0001f);  
    PWMB.period(0.0001f);  
    PWMA.write(0.15f);
    PWMB.write(0.15f);
    AIN_1=1;
    AIN_2=0;
    BIN_1=0;
    BIN_2=1;
}

void turnright(){
    STBY=1;
    PWMA.period(0.0001f);  
    PWMB.period(0.0001f);  
    PWMA.write(0.20f);
    PWMB.write(0.20f);
    AIN_1=0;
    AIN_2=1;
    BIN_1=1;
    BIN_2=0;
}

void stop(){
    STBY=0;
    PWMA.period(0.0001f);  
    PWMB.period(0.0001f);  
    PWMA.write(0.15f);
    PWMB.write(0.15f);
    AIN_1=0;
    AIN_2=1;
    BIN_1=0;
    BIN_2=1;
}

void gostraight_correct(){
    if(distance3>distance4 && distance4>20){
        //向左偏时，向右修正
        PWMA.write(0.4f);
        PWMB.write(0.35f);
    }
    if(distance3<distance4 && (distance3<20 || distance4<20)){
        //向右偏时，向左修正
        PWMA.write(0.32f);
        PWMB.write(0.42f);
    }
    if ((d34)>=-1 && (d34)<=1 ){
        //不偏时直走
        PWMA.write(0.3f);
        PWMB.write(0.3135f);
    }
    STBY=1;
    PWMA.period(0.0001f);  
    PWMB.period(0.0001f);  
    AIN_1=0;
    AIN_2=1;
    BIN_1=0;
    BIN_2=1;
    wait_us(5e4);
    printf("***** CORRECTING *****\n");
}

float initial_yaw=0;


float difference;

/* openMV调试 */
// int A=1;
// int B=1;
/* openMV调试 */

void trace_find(){
    //执行到转向点的程序
        gostraight();
        wait_us(9.5e6);
        stop();
        wait_us(1e6);
        goback();
        wait_us(2e6);
        stop();
        wait_us(1e6);
        //停在转向点，等待。

        getJY61data();
        
        printf("the yaw = %.1f\n", yaw);
        printf("Flag = %d\n\n", flag);

        //判断状态和读取到的AB数据。

        //右箭头，01
        if(A==0 && B==1){
            turnleft();//转45°
            while(1){
                getJY61data();
	            printf("yaw during turning= %.3f\n", yaw);
                if(initial_yaw<145){
                    difference=yaw-initial_yaw;
                }else{
                    difference=yaw-initial_yaw+360;
                }
                if(difference>35){
                    stop();
	                printf("yaw after turning= %.3f\n", yaw);
                    wait_us(1e6);
                    break;
                }
            }    

            //转向AB=01的上线路。        
            gostraight();
            wait_us(10.5e6);
            stop();
            wait_us(1e6);
            goback();
            wait_us(3e6);
            stop();
            wait_us(1e6);

            //向右转，直走至栏杆边，结束后flag=1，进入阶段2.
            turnright();
            while(1){
                getJY61data();
                if((yaw-initial_yaw)<15){
                    stop();
                    wait_us(1e6);
                    flag=2;
                    break;
                }
                flag=2;
            }            
        }
        //上箭头，10
        else if(A==1 && B==0){
            turnleft();//转90°
            while(1){
                getJY61data();
	            printf("yaw during turning= %.3f\n", yaw);
                if(initial_yaw<100){
                    difference=yaw-initial_yaw;
                }else{
                    difference=yaw-initial_yaw+360; 
                }
                if(difference>80){
                    stop();
	                printf("yaw after turning= %.3f\n", yaw);
                    wait_us(1e6);
                    break;
                }
            }
            gostraight();
            wait_us(17e6);
            stop();
            wait_us(1e6);
            goback();
            wait_us(1e6);
            stop();
            wait_us(1e6);

            turnright();//转90°
            while(1){
                getJY61data();
                if((yaw-initial_yaw)<15){
                    stop();
                    wait_us(1e6);
                    flag=2;
                    break;
                }
            }
        }
        //左箭头，11
        else if(A==1 && B==1){
            turnleft();//转135°
            while(1){
                getJY61data();
	            printf("yaw during turning= %.3f\n", yaw);
                if(initial_yaw<55){
                    difference=yaw-initial_yaw;
                }else{
                    difference=yaw-initial_yaw+360;
                }
                if(difference>125){
                    stop();
	                printf("yaw after turning= %.3f\n", yaw);
                    wait_us(1e6);
                    break;
                }
            }
            gostraight();
            wait_us(13e6);
            stop();
            wait_us(1e6);
            goback();
            wait_us(6e6);
            stop();
            wait_us(1e6);

            turnright();
            while(1){
                getJY61data();
                if((yaw-initial_yaw)<15){
                    stop();
                    wait_us(1e6);
                    flag=2;
                    break;
                }
            }
        }
}

void ultrasonic_2(){
    //这个函数是超声波patio2的封装函数，非必要勿改
        

        distance1=HCSR04_1.getCm();
        distance2=HCSR04_2.getCm();
        distance3=HCSR04_3.getCm();
        distance4=HCSR04_4.getCm();
        d34=distance3-distance4;
        
        data_print();

        // wait_us(5e5);

        if(distance1>40 && distance2>40 && flag_u==1){  //也许有问题
            gostraight();
            printf("^^^^^ GO STRAIGHT ^^^^^\n");
        }

        else if(distance1<40 && distance2<40 && flag_u ==1){
            stop();
            wait_us(1e6);
            turnleft();
            printf("<<<<< turning LEFT 1\n");
            
            while(1){
                // distance1=HCSR04_1.getCm();
                // distance2=HCSR04_2.getCm();
                distance3=HCSR04_3.getCm();
                distance4=HCSR04_4.getCm();
                d34=distance3-distance4;
                //持续更新distance的数据

                if(distance3<25 && distance4<25 && abs(d34)<1){
                    // distance1>60 && distance2 > 60 && distance3<20 && distance4<20 && (d34)<20
                    stop();
                    printf("===== STOP turning LEFT =====\n");
                    wait_us(1e6);
                    break;
                }
            }

        flag_u = 2 ;

        }
        else if(distance1>60 && distance2>60 && distance3<60 && distance4<60 && flag_u == 2){
            //进入修正前进状态
            gostraight_correct();
            //PID
        }
        else if(distance1>60 && distance2>60 && distance3>60 && distance4<30 && flag_u == 2){
            gostraight();
            printf("!!!!! ready for turning RIGHT !!!!!\n");
            wait_us(0.3e6);//0.7
            stop();
            wait_us(1e6);
        }
        else if(distance1>60 && distance2>60 && distance3>60 && distance4>60 && flag_u == 2){
            printf("turning RIGHT >>>>>\n");
            turnright();
            wait_us(1.9e6);//1.9e6 3&3.135 
            gostraight();
            wait_us(2e6);
        }
        else if(distance3<80 && distance4<80 && distance1<30 && distance2<30 && flag_u == 2){
            printf("<<<<< turning LEFT 2\n");
            turnleft();
            flag_s=flag_s + 1;
            while(1){
                distance1=HCSR04_1.getCm();
                distance2=HCSR04_2.getCm();
                distance3=HCSR04_3.getCm();
                distance4=HCSR04_4.getCm();
                d34=distance3-distance4;
                if(distance1>60 && distance2>60 && abs(d34)<1){
                    stop();
                    wait_us(1.1e6);
                    break;
                }
            }
        }

        else if(flag_s == 2){//
            printf("@@@@@ TRANSH CAN DETECTING @@@@@\n");
            while(1){
                data_print();
                gostraight_correct();
                if(distance1<10 && distance2<10){
                    printf("@@@@@ DETECTTED!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! @@@@@,%d\n",flag_u);
                    stop();
                    wait_us(3e6);
                    break;
                }
            }
            flag_u=3;
            flag=3;
        }
}

void ball_drop(){
        printf("||||| BALL DROPPING |||||\n");
        droptheball();
        wait_us(2e6);
        gostraight();
        wait_us(2e6);
        goback();
        wait_us(1e6);
        stop();
        flag=4;
}

void communicating(){
    //转向，陀螺仪参与
        printf("COMMUNICTING++++++++++++++++++++++++\n");
        getJY61data();
        //printf("yaw=%.3f",yaw);
        float mid_yaw=yaw;
        printf("middle yaw=%.3f",mid_yaw);

        turnleft();//转90°
        wait_us(3.2e6);
        gostraight();
        wait_us(40e6);
        // while(1){
        //     printf("ready to stop");
        //     getJY61data();
        //     printf("yaw=%.3f",yaw);
        //     if(mid_yaw<100){//最后根据difference参数修改
        //         difference=yaw-mid_yaw;
        //     } else {
        //         difference=yaw-mid_yaw+360;
        //     }
        //     if(difference>80){//参数可调
        //         stop();
        //         wait_us(1e6);
        //         break;
        //     }
        //  }  
        
        // gostraight();
        // wait_us(40e6);

        communication();
        printf("<<<<< COMMUNICATING >>>>>");
        wait_us(5e6);
}


int main()
{
    wait_us(3e6);
    printf("1111111111111111111111111111");

    COM = 0;
    SER = 0;

    //整个系统开始执行任务之前就读取初始角度，并赋值给initial_yaw。
    getJY61data();
    printf("yaw=%.3f\n",yaw);
    getJY61data();
    printf("yaw=%.3f\n",yaw);
    //两次读取数据、怕不稳定
    initial_yaw=yaw;
    printf("initial yaw=%.3f\n",initial_yaw);

    printf("initial flag=%d\n",flag);
    
    while(1){
    switch (flag){
        case 1: {
            printf("11111 IN CASE I 11111\n");
            trace_find();
            flag = 2;
            break;
        }
        case 2: {
            printf("22222 IN CASE II 22222\n");
            while(1){
                ultrasonic_2();
                if(flag==3) {
                    printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
                    break;
                    }
            }
            break;
        }
        case 3: {
            printf("33333 IN CASE III 33333\n");
            ball_drop();
            flag = 4;
            break; 
        }
        case 4: {
            printf("44444 IN CASE IV 44444\n");
            communicating();
            break; 
        }
    }
    }
}
