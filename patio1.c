#include "mbed.h"
#include "HCSR04.h"

DigitalIn A(PC_12);
DigitalIn B(PD_2);

/* 电机引脚设置 */
DigitalOut  AIN_1(PA_9);
DigitalOut  AIN_2(D9);
DigitalOut  BIN_1(PB_5);
DigitalOut  BIN_2(PA_10);
DigitalOut  STBY(D10);

PwmOut PWMA(D7);
PwmOut PWMB(D6);

HCSR04 HCSR04_1(PB_12, PB_11);
HCSR04 HCSR04_2(PB_13, PB_14);
HCSR04 HCSR04_3(PC_11, PC_10);
HCSR04 HCSR04_4(PC_9, PC_8);

BufferedSerial pc(USBTX, USBRX,9600);

float distance1=0;
float distance2=0;
float distance3=0;
float distance4=0;
float d34=0;
float d12=0;
volatile int flag_t=0;
volatile int flag=1;

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

void gostraight_vision(){
    STBY=1;
    PWMA.period(0.0001f);  
    PWMB.period(0.0001f);  
    PWMA.write(0.15f);
    PWMB.write(0.16f);
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

void gostraight_correct_12(){
    if(distance1>distance2){
        //向左偏时，向右修正
        PWMA.write(0.4f);
        PWMB.write(0.35f);
    }
    if(distance1<distance2){
        //向右偏时，向左修正
        PWMA.write(0.32f);
        PWMB.write(0.42f);
    }
    if ((d12)>=-1 && (d12)<=1 ){
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
    wait_us(1e4);
    printf("***** CORRECTING *****\n");
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
void turnleft_vision(){
    STBY=1;
    PWMA.period(0.0001f);  
    PWMB.period(0.0001f);  
    PWMA.write(0.15f);
    PWMB.write(0.35f);
    AIN_1=0;
    AIN_2=1;
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
void turnright_vision(){
    STBY=1;
    PWMA.period(0.0001f);  
    PWMB.period(0.0001f);  
    PWMA.write(0.35f);
    PWMB.write(0.15f);
    AIN_1=0;
    AIN_2=1;
    BIN_1=0;
    BIN_2=1;
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

void tracking(){
    printf("***** ENTER TRACKING *****\n");
    while(1){
        distance1=HCSR04_1.getCm();
        distance2=HCSR04_2.getCm();
        printf("the distance of US-01 = %.1f\n", distance1);
        printf("the distance of US-02 = %.1f\n", distance2);
        printf("Flag = %d\n\n", flag);
        wait_us(1e6);//调试用，实地测试可删
        //用于巡线

        if(distance1 > 40 && distance2 > 40 && flag_t==0){//这一部分用于巡线，在检测到信标边缘之前一直运行（flag_t == 0)
            if (A==0 && B==1){
            printf("turnleft\n");
            turnleft_vision();
            }
            else if (A==1 && B==0){
                printf("turnright\n");
                turnright_vision();
            }
            else if (A==1 && B==1){
                printf("gostraight\n");
                gostraight_vision();
            }
        }
        else if(distance1 < 40 && distance2 <40 && flag_t == 0){//检测到信标边缘之后，立马改变flag_t
            printf("\n\nBeacon detected\n\n");
            flag_t = 1;
        }
        //用于跳出tracking
        else if(flag_t == 1 && distance1 < 40 && distance2 < 40){
                printf("turning RIGHT >>>>>\n");
                stop();
                wait_us(2e6);
                turnright();
                wait_us(1.8e6);//1.8: 0.4&0.413
                gostraight();
                wait_us(15e6);
                break;
        }
    }
    flag=2;
}

void passbridge(){
    while (1){
        distance1=HCSR04_1.getCm();
        distance2=HCSR04_2.getCm();
        //distance3=HCSR04_3.getCm();
        //distance4=HCSR04_4.getCm();
        //d34=distance3-distance4;
        
        printf("the distance of US-01 = %.1f\n", distance1);
        printf("the distance of US-02 = %.1f\n", distance2);
        // printf("the distance of US-03 = %.1f\n", distance3);
        // printf("the distance of US-04 = %.1f\n", distance4);
        // printf("d34 = %.1f\n", d34);
        printf("Flag = %d\n\n", flag);
        wait_us(1e6);//调试用，实地测试可删

        if (distance1>30 && distance2>30){
            gostraight();
        }
        else if (distance1<30 && distance2<30){
            stop();
            wait_us(1e6);
            turnleft();
            printf("<<<<< turning LEFT\n");

            while(1){
                distance3=HCSR04_3.getCm();
                distance4=HCSR04_4.getCm();
                d34=distance3-distance4;
                if(distance3<30 && distance4<30 && abs(d34)<1){
                    stop();
                    printf("===== STOP turning LEFT =====\n");
                    wait_us(1e6);
                    gostraight();
                    wait_us(10e6);
                    break;
                }
            }
            break;
        }
    }
    flag=3;
}

int main()
{   
    wait_us(3e6);
    printf("========55========\n");
    while(1){
        switch (flag){
            case 1: {
                printf("11111 tracking 11111\n");
                tracking();
                flag = 2;
                break;
            }
            case 2: {
                printf("22222 bridge passing 22222\n");
                passbridge();
                flag = 3;
                break;
            }
            case 3: {
                printf("33333 The door 33333\n");
                gostraight();
                wait_us(11e6);
                stop();
                flag = 0;
                break; 
            }
        }
    }
}
