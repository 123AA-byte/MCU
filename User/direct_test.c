#include "stm32f10x.h"                  // Device header

/**************************************************************************
 * 函数名：USART1_Init
 * 函数功能：初始化USART1
 * 参数：无
 * 返回值：无
 **************************************************************************/
void USART1_Init(void)
{
    // 使能USART1和GPIOA时钟
    RCC->APB2ENR |= RCC_APB2ENR_USART1EN | RCC_APB2ENR_IOPAEN;
    
    // 配置PA9为USART1_TX（复用推挽输出）
    GPIOA->CRH &= ~(GPIO_CRH_CNF9 | GPIO_CRH_MODE9);
    GPIOA->CRH |= GPIO_CRH_CNF9_1 | GPIO_CRH_MODE9_1 | GPIO_CRH_MODE9_0;
    
    // 配置USART1：115200波特率，8位数据，无校验，1位停止位
    USART1->BRR = 0x341;  // 72MHz时钟下115200波特率
    USART1->CR1 = USART_CR1_TE | USART_CR1_UE;
}

/**************************************************************************
 * 函数名：USART1_SendByte
 * 函数功能：通过USART1发送一个字节
 * 参数：Byte：要发送的字节
 * 返回值：无
 **************************************************************************/
void USART1_SendByte(uint8_t Byte)
{
    // 等待发送数据寄存器为空
    while (!(USART1->SR & USART_SR_TXE));
    
    // 发送数据
    USART1->DR = Byte;
    
    // 等待发送完成
    while (!(USART1->SR & USART_SR_TC));
}

/**************************************************************************
 * 函数名：PB1_Init
 * 函数功能：初始化PB1引脚为浮空输入
 * 参数：无
 * 返回值：无
 **************************************************************************/
void PB1_Init(void)
{
    // 使能GPIOB时钟
    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
    
    // 配置PB1为浮空输入
    GPIOB->CRL &= ~(GPIO_CRL_CNF1 | GPIO_CRL_MODE1);
    GPIOB->CRL |= GPIO_CRL_CNF1_0;
}

/**************************************************************************
 * 函数名：Delay_ms
 * 函数功能：毫秒级延时
 * 参数：ms：延时毫秒数
 * 返回值：无
 **************************************************************************/
void Delay_ms(uint32_t ms)
{
    for (uint32_t i = 0; i < ms; i++)
    {
        for (uint32_t j = 0; j < 8000; j++);  // 大约1ms延时
    }
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
    USART1_Init();
    
    // 初始化PB1引脚
    PB1_Init();
    
    while (1)
    {
        // 直接读取GPIOB_IDR寄存器的第1位（PB1引脚状态）
        uint8_t pb1_state = (GPIOB->IDR & GPIO_IDR_IDR1) ? 0x01 : 0x00;
        
        // 发送详细的调试信息
        USART1_SendByte(0xCC);  // 起始标志
        USART1_SendByte(pb1_state);  // 发送引脚状态
        USART1_SendByte((GPIOB->IDR & 0xFF));  // 发送整个低8位寄存器值
        USART1_SendByte(0xDD);  // 结束标志
        
        // 延时500ms
        Delay_ms(500);
    }
}
