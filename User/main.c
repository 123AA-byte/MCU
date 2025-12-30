#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "AD.h"
#include "usart.h"
#include "shot.h"
uint8_t KeyNum;
uint16_t AD0,AD4;

//uint8_t communicationStatus = 0;
typedef struct {
	uint8_t cmd;   //通信标志位
	uint16_t data_x; //x轴AD数据
	uint16_t data_y; //y轴AD数据
	uint8_t shot;//按键的值
}DATA;


uint16_t ReadY(void)
{
	AD4=AD_GetValue(ADC_Channel_4);
	return AD4;
}
uint16_t ReadX(void)
{
	AD0=AD_GetValue(ADC_Channel_0);
	return AD0;
}



void send_all(void){
	DATA data;
	data.cmd=0x01;
	data.data_x=ReadX();
	data.data_y=ReadY();
	data.shot=get_shot();
	TX_Pack[0]=data.cmd;
	TX_Pack[1]=(uint8_t)(data.data_x & 0xff);
	TX_Pack[2]=(uint8_t)(data.data_x >> 8);
	TX_Pack[3]=(uint8_t)(data.data_y & 0xff);
	TX_Pack[4]=(uint8_t)(data.data_y >> 8);
	TX_Pack[5]=data.shot;
	US_SendPack();
//	US_SendByte(data.cmd);
//	US_SendByte((uint8_t)(data.data_x & 0xff));
//	US_SendByte((uint8_t)(data.data_x >> 8));
//	US_SendByte((uint8_t)(data.data_y & 0xff));
//	US_SendByte((uint8_t)(data.data_y >> 8));
//	US_SendByte(data.shot);
}
//void led_Init(void)
//{
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
//	GPIO_InitTypeDef GPIO_InitStructure;
//	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;
//	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0;
//	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
//	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
//}
//void start(void)
//{
//	 GPIO_InitTypeDef GPIO_InitStructure;
//	 GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;	
//	 GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0;
//	 GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
//	 GPIO_Init(GPIOA,&GPIO_InitStructure);
//}





//uint8_t GetKeyNum(void)
//{
//	if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == 0)
//	{
//		Delay_ms(10);
//        if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == 0) 
//			{	
//				while (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == 0);
//				i=!i;
//			}
//	}
//	return i;
//}
//uint8_t GetKeyNum(void) {
//    static uint8_t lastKeyState = 1; // 记录上一次按键状态，初始为未按下
//    uint8_t currentKeyState = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0);
//    if (currentKeyState == 0 && lastKeyState == 1) {
//        Delay_ms(10);
//        currentKeyState = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0);
//        if (currentKeyState == 0) {
//            while (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == 0);
//            lastKeyState = 0;
//            return 1;
//        }
//    }
//    else if (currentKeyState == 1) {
//        lastKeyState = 1;
//    }
//    return 0;
//}
int main(void) {

    OLED_Init();//初始化
    AD_Init();
    US_Init();
//    led_Init();
//    start();
    shot_Init();
	OLED_ShowString(1, 1, "Welcome!");
	
    


    while (1) {
		// 更新AD值
//		AD0 = ReadX();
//		AD4 = ReadY();
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
		GPIO_ResetBits(GPIOC,GPIO_Pin_13);
		
		
		
		// 发送完整数据包
		send_all();
		
		// 在OLED上显示信息
		OLED_ShowString(2, 1, "Using...");
		OLED_ShowNum(3, 1, AD0, 4);
        OLED_ShowNum(4, 1, AD4, 4);
		
		
//        KeyNum = GetKeyNum();
//        if (KeyNum == 1) {
//            communicationStatus =!communicationStatus;
//        }
//        if (communicationStatus) {
//            send_all();
//            //GPIO_ResetBits(GPIOB, GPIO_Pin_0);
//            OLED_ShowString(2, 1, "Using...");
//            OLED_ShowNum(3, 1, AD1, 4);
//            OLED_ShowNum(4, 1, AD4, 4);
//        }
//        else {
//            OLED_ShowString(2, 1, "Waiting...");
//            //GPIO_SetBits(GPIOB, GPIO_Pin_0);
//        }
    }
}
