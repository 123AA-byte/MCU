#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "usart.h"

/**************************************************************************
 * 函数名：PB1_Init
 * 函数功能：初始化PB1引脚为上拉输入
 * 参数：无
 * 返回值：无
 **************************************************************************/
void PB1_Init(void)
{
    // 使能GPIOB时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    
    // 配置PB1为上拉输入
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
}

/**************************************************************************
 * 函数名：main
 * 函数功能：主函数
 * 参数：无
 * 返回值：无
 **************************************************************************/
int main(void)
{
    // 初始化USART1
    US_Init();
    
    // 初始化PB1引脚
    PB1_Init();
    
    while (1)
    {
        // 读取PB1引脚状态
        uint8_t pb1_state = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1);
        
        // 发送原始引脚状态到串口
        US_SendByte(0xAA);  // 起始标志
        US_SendByte(pb1_state ? 0x01 : 0x00);  // 发送引脚状态
        US_SendByte(0xBB);  // 结束标志
        
        // 延时500ms
        Delay_ms(500);
    }
}
