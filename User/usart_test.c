#include "stm32f10x.h"                  // Device header
#include "Delay.h"

int main(void)
{
    // 使能GPIOA时钟
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
    // 使能USART2时钟
    RCC->APB1ENR |= RCC_APB1ENR_USART2EN;
    
    // 配置PA2引脚为复用推挽输出
    GPIOA->CRL &= ~GPIO_CRL_MODE2;
    GPIOA->CRL |= GPIO_CRL_MODE2_1;
    GPIOA->CRL &= ~GPIO_CRL_CNF2;
    GPIOA->CRL |= GPIO_CRL_CNF2_1;
    
    // 配置USART2
    // 设置波特率：对于72MHz系统时钟，APB1时钟为36MHz
    // 波特率 = 36MHz / (16 * 234.375) = 9600
    USART2->BRR = 0x1D4C; // 36MHz / 9600 = 3750 = 0x1D4C
    
    // 启用USART，启用发送
    USART2->CR1 = USART_CR1_UE | USART_CR1_TE;
    
    while(1)
    {
        // 发送字符 'A'
        USART2->DR = 'A';
        // 等待发送完成
        while((USART2->SR & USART_SR_TXE) == 0);
        
        // 发送回车换行
        USART2->DR = '\r';
        while((USART2->SR & USART_SR_TXE) == 0);
        USART2->DR = '\n';
        while((USART2->SR & USART_SR_TXE) == 0);
        
        // 延时1秒
        Delay_ms(1000);
    }
}
