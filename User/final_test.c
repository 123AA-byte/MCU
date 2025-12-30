#include "stm32f10x.h"                  // Device header

/**************************************************************************
 * 函数名：main
 * 函数功能：主函数，直接操作寄存器测试PB1引脚
 * 参数：无
 * 返回值：无
 **************************************************************************/
int main(void)
{
    // 使能GPIOB和USART1时钟
    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN | RCC_APB2ENR_USART1EN | RCC_APB2ENR_IOPAEN;
    
    // 配置PB1为浮空输入
    GPIOB->CRL &= ~(GPIO_CRL_CNF1 | GPIO_CRL_MODE1);
    GPIOB->CRL |= GPIO_CRL_CNF1_0;  // 浮空输入
    
    // 配置PA9为USART1_TX（复用推挽输出）
    GPIOA->CRH &= ~(GPIO_CRH_CNF9 | GPIO_CRH_MODE9);
    GPIOA->CRH |= GPIO_CRH_CNF9_1 | GPIO_CRH_MODE9_1 | GPIO_CRH_MODE9_0;
    
    // 配置USART1：115200波特率，8位数据，无校验，1位停止位
    USART1->BRR = 0x341;  // 72MHz时钟下115200波特率
    USART1->CR1 = USART_CR1_TE | USART_CR1_UE;
    
    // 延时函数（简单实现）
    void Delay(uint32_t count)
    {
        for (; count > 0; count--);
    }
    
    while (1)
    {
        // 直接读取PB1引脚状态
        uint8_t pb1_state = (GPIOB->IDR & GPIO_IDR_IDR1) ? 0x01 : 0x00;
        
        // 发送起始标志
        while (!(USART1->SR & USART_SR_TXE));
        USART1->DR = 0xAA;
        while (!(USART1->SR & USART_SR_TC));
        
        // 发送PB1原始状态
        while (!(USART1->SR & USART_SR_TXE));
        USART1->DR = pb1_state;
        while (!(USART1->SR & USART_SR_TC));
        
        // 发送按下状态（0x00表示按下，0x01表示未按下）
        while (!(USART1->SR & USART_SR_TXE));
        USART1->DR = pb1_state ? 0x01 : 0x00;
        while (!(USART1->SR & USART_SR_TC));
        
        // 发送结束标志
        while (!(USART1->SR & USART_SR_TXE));
        USART1->DR = 0x55;
        while (!(USART1->SR & USART_SR_TC));
        
        // 延时约500ms
        Delay(0x100000);
    }
}
