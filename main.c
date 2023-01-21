

#include <avr/io.h>
#include <util/delay.h>
#include "uart.h"

int main(void){
  
  UART_Init(9600);
  
  
  while(1){
  
    if(UART_Bytes_Available_To_Read()>0){
      UART_Transmit_Byte(UART_Read());
      //UART_Transmit_Text("\r\n");
    }
    _delay_ms(10);
    
  }
}