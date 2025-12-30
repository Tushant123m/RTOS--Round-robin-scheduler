#include <stdio.h>
#include "stm32f401xe.h"
#include <stdint.h>
#define GPIOAEN    (1U<<0)
#define UART2EN    (1U<<17)

#define CR1_TE    (1U<<3)
#define CR1_RE    (1U<<2)
#define CR1_UE    (1U<<13)
#define SR_TXE    (1U<<7)
#define SR_RXNE   (1U<<5)
#define SYS_FREQ    16000000
#define APB1_CLK    SYS_FREQ

#define UART_BAUDRATE    115200

static void uart_set_baudrate(uint32_t PeriphClk,uint32_t Baudrate);
 void uart2_write(int ch);

//static void compute_uart_bd(uint32_t Periphclk,uint32_t Baudrate);
int fputc(int ch, FILE *f)
{
 uart2_write(ch);
	 return ch;
}

 void uar2_txrx_init(void){

	 /*******configure uart gpio pin******/
	 /*enable clock access to gpioa*/
	 RCC->AHB1ENR |= GPIOAEN;
	 /*set pa2 to alternate function mode*/
	 GPIOA->MODER &=~ (1U<<4);
	 GPIOA->MODER |= (1U<<5);

	 GPIOA->AFR[0]|=(1U<<8);
	 GPIOA->AFR[0]|=(1U<<9);
	 GPIOA->AFR[0]|=(1U<<10);
	 GPIOA->AFR[0]&=~(1U<<11);

	 /*set pa3 to alternate function mode*/
	 GPIOA->MODER&=~(1U<<6);
	 GPIOA->MODER|=(1U<<7);

	 GPIOA->AFR[0]|=(1U<<12);
	 GPIOA->AFR[0]|=(1U<<13);
	 GPIOA->AFR[0]|=(1U<<14);
	 GPIOA->AFR[0]&=~(1U<<15);

	 /****CONFIGURE THE UART2 MODULE****/
	 /*enable clock access to uart2 module*/
	 RCC->APB1ENR|=UART2EN;
	 /*config baudrate*/
	 uart_set_baudrate(APB1_CLK,UART_BAUDRATE);
	 /*configure the transfer direction*/
	 USART2->CR1 = CR1_TE | CR1_RE;
	 /*enable uart module*/
	 USART2->CR1|=CR1_UE;

 }

 static void uart_set_baudrate(uint32_t PeriphClk, uint32_t Baudrate){

	 USART2->BRR = ((PeriphClk + (Baudrate/2U))/Baudrate);
 }



 void uart2_write(int ch){
	 /*make sure that transmit data register is empty*/
	 while(!(USART2->SR & SR_TXE)){}
	 /*write to transmit data register*/
	 USART2->DR = (ch&0xFF);
 }

 char uart2_read(void){
	 /*make sure that transmit data register is empty*/
	 while(!(USART2->SR & SR_RXNE)){}
	 /*return the read data from data register*/
	 return (char)USART2->DR;
 }


 int __io_putchar(int ch){
	 uart2_write(ch);
	 return ch;
 }
