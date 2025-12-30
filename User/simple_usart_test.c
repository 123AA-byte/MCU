#include "stm32f10x.h"                  // Device header
#include "Delay.h"

// 简单的USART2初始化函数
void USART2_Init(void)
{
    // 使能GPIOA时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    // 使能USART2时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
    
    // 配置PA2引脚为USART2_TX
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    // 配置USART2
    USART_InitTypeDef USART_InitStructure;
    USART_InitStructure.USART_BaudRate = 9600;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_Mode = USART_Mode_Tx;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_Init(USART2, &USART_InitStructure);
    
    // 启用USART2
    USART_Cmd(USART2, ENABLE);
}

// 发送一个字节
void USART2_SendByte(uint8_t Byte)
{
    USART_SendData(USART2, Byte);
    while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
}

// 发送一个字符串
void USART2_SendString(char *String)
{
    uint8_t i = 0;
    while(String[i] != '\0')
    {
        USART2_SendByte(String[i]);
        i++;
    }
}

int main(void)
{
    // 初始化USART2
    USART2_Init();
    
    while(1)
    {
        // 发送测试字符串
        USART2_SendString("Hello from STM32!\r\n");
        // 延时1秒
        Delay_ms(1000);
    }
}
