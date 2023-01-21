

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#define  F_CPU                 8000000UL

#define UCSRxA_REG             UCSR0A
#define UCSRxA_MPCMx_bp        0x00U
#define UCSRxA_U2Xx_bp         0x01U
#define UCSRxA_UPEx_bp         0x02U
#define UCSRxA_DORx_bp         0x03U
#define UCSRxA_FEx_bp          0x04U
#define UCSRxA_UDREx_bp        0x05U
#define UCSRxA_TXCx_bp         0x06U
#define UCSRxA_RXCx_bp         0x07U
#define UCSRxA_MPCMx_bm        (1<<UCSRxA_MPCMx_bp)
#define UCSRxA_U2Xx_bm         (1<<UCSRxA_U2Xx_bp)
#define UCSRxA_UPEx_bm         (1<<UCSRxA_UPEx_bp)
#define UCSRxA_DORx_bm         (1<<UCSRxA_DORx_bp)
#define UCSRxA_FEx_bm          (1<<UCSRxA_FEx_bp)
#define UCSRxA_UDREx_bm        (1<<UCSRxA_UDREx_bp)
#define UCSRxA_TXCx_bm         (1<<UCSRxA_TXCx_bp)
#define UCSRxA_RXCx_bm         (1<<UCSRxA_RXCx_bp)

#define UCSRxB_REG             UCSR0B
#define UCSRxB_TXB8x_bp        0x00U
#define UCSRxB_RXB8x_bp        0x01U
#define UCSRxB_UCSZx2_bp       0x02U
#define UCSRxB_TXENx_bp        0x03U
#define UCSRxB_RXENx_bp        0x04U
#define UCSRxB_UDRIEx_bp       0x05U
#define UCSRxB_TXCIEx_bp       0x06U
#define UCSRxB_RXCIEx_bp       0x07U
#define UCSRxB_TXB8x_bm        (1<<UCSRxB_TXB8x_bp)
#define UCSRxB_RXB8x_bm        (1<<UCSRxB_RXB8x_bp)
#define UCSRxB_UCSZx2_bm       (1<<UCSRxB_UCSZx2_bp)
#define UCSRxB_TXENx_bm        (1<<UCSRxB_TXENx_bp)
#define UCSRxB_RXENx_bm        (1<<UCSRxB_RXENx_bp)
#define UCSRxB_UDRIEx_bm       (1<<UCSRxB_UDRIEx_bp)
#define UCSRxB_TXCIEx_bm       (1<<UCSRxB_TXCIEx_bp)
#define UCSRxB_RXCIEx_bm       (1<<UCSRxB_RXCIEx_bp)

#define UCSRxC_REG             UCSR0C
#define UCSRxC_UCPOLx_bp       0x00U
#define UCSRxC_UCSZx0_bp       0x01U
#define UCSRxC_UCSZx1_bp       0x02U
#define UCSRxC_USBSx_bp        0x03U
#define UCSRxC_UPMx0_bp        0x04U
#define UCSRxC_UPMx1_bp        0x05U
#define UCSRxC_UMSELx0_bp      0x06U
#define UCSRxC_UMSELx1_bp      0x07U
#define UCSRxC_UCPOLx_bm       (1<<UCSRxC_UCPOLx_bp)
#define UCSRxC_UCSZx0_bm       (1<<UCSRxC_UCSZx0_bp)
#define UCSRxC_UCSZx1_bm       (1<<UCSRxC_UCSZx1_bp)
#define UCSRxC_USBSx_bm        (1<<UCSRxC_USBSx_bp)
#define UCSRxC_UPMx0_bm        (1<<UCSRxC_UPMx0_bp)
#define UCSRxC_UPMx1_bm        (1<<UCSRxC_UPMx1_bp)
#define UCSRxC_UMSELx0_bm      (1<<UCSRxC_UMSELx0_bp)
#define UCSRxC_UMSELx1_bm      (1<<UCSRxC_UMSELx1_bp)

#define UBRRxH_REG             UBRR0H
#define UBRRxL_REG             UBRR0L
#define UDRx_REG               UDR0


#define UART_ENABLE_TX         1U
#define UART_ENABLE_RX         1U
#define UART_ENABLE_RX_INT     1U
#define UART_BUF_SIZE          250UL

/*Don't change anything below this line*/
typedef struct uart_t{
    volatile uint8_t   Error;
    uint8_t            Digits[8];
    uint8_t            InputNumberDigits;
    volatile uint8_t   Buffer[UART_BUF_SIZE];
    volatile uint16_t  BufferSize;
    volatile uint16_t  BufferIndex;
    volatile uint16_t  AvailableBytes;
    volatile uint16_t  ReadIndex;
    volatile uint32_t  LastRecivedTimeStamp;
}uart_t;


uart_t UART;

void UART_Struct_Init(void){
    UART.BufferSize=UART_BUF_SIZE;
    UART.Error=0x00;
    UART.AvailableBytes=0x00;
    UART.ReadIndex=0x00;
    for(uint8_t i=0;i<UART.BufferSize;i++){
        UART.Buffer[i]=0;
    }
    for(uint8_t i=0;i<8;i++){
        UART.Digits[i]=0;
    }
    UART.InputNumberDigits=0;
    UART.BufferIndex=0;
    UART.LastRecivedTimeStamp=0;
}

void UART_Config_Transmitter(void){
    UCSRxB_REG|=UCSRxB_TXENx_bm;
    UCSRxC_REG|=UCSRxC_UCSZx0_bm|UCSRxC_UCSZx1_bm;
}

void UART_Config_Receiver(void){
    UCSRxB_REG|=UCSRxB_RXENx_bm;
    UCSRxC_REG|=UCSRxC_UCSZx0_bm|UCSRxC_UCSZx1_bm;
}

void UART_Config_Receiver_Interrupt(void){
    UCSRxB_REG|=UCSRxB_RXCIEx_bm;
    sei();
}

void UART_Init(uint32_t BAUD){
    uint32_t BRG_VAL=F_CPU;
    BRG_VAL=((BRG_VAL/16)/BAUD)-1;
    UBRRxH_REG =0x00;
    UBRRxL_REG =BRG_VAL;
    UCSRxA_REG=0x00;
    UCSRxB_REG=0x00;
    UCSRxC_REG=0x00;
    UART_Struct_Init();
    if(UART_ENABLE_TX){
      UART_Config_Transmitter();
    }
    if(UART_ENABLE_RX){
      UART_Config_Receiver();
    }
    if(UART_ENABLE_RX_INT){
      UART_Config_Receiver_Interrupt();
    }
}

void UART_Transmit_Byte(uint8_t val){
    UDRx_REG=val;
    while((UCSRxA_REG & UCSRxA_UDREx_bm)==0);
}

void UART_Transmit_Byte_Hex(uint8_t val){
    uint8_t hex_digits[2];
    hex_digits[0]=(uint8_t)(val>>4);
    hex_digits[1]=(uint8_t)(val & 0x0F);
    if(hex_digits[0]>9){hex_digits[0]+=55;}
    else {hex_digits[0]+=48;}
    if(hex_digits[1]>9){hex_digits[1]+=55;}
    else {hex_digits[1]+=48;}
    UART_Transmit_Byte('0');
    UART_Transmit_Byte('x');
    UART_Transmit_Byte(hex_digits[0]);
    UART_Transmit_Byte(hex_digits[1]);
}

void UART_Transmit_Byte_Bin(uint8_t val){
    uint8_t bin_digits[8];
    for(uint8_t i=0;i<8;i++){
        bin_digits[i]= (uint8_t)((val & 0x01)+48);
        val>>=1;
    }
    UART_Transmit_Byte('0');
    UART_Transmit_Byte('b');
    for(uint8_t i=8;i>0;i--){
        uint8_t index=(uint8_t)(i-1);
        UART_Transmit_Byte(bin_digits[index]);
    }
}

void UART_Transmit_Text(char *str){
    uint8_t i=0;
    while(str[i]!='\0'){
        UART_Transmit_Byte(str[i]);
        i++;
    }
}

void UART_Determine_Digit_Numbers(uint32_t num){
  uint8_t i=0;
  if(num==0){
    UART.Digits[0]=0;
    UART.InputNumberDigits=1;
  }else{
    while(num!=0){
      UART.Digits[i]=num%10;
      num/=10;
      i++;
    }
	UART.InputNumberDigits=i;
  }
}

void UART_Transmit_Number_Digits(void){
  for(uint8_t i=UART.InputNumberDigits; i>0; i--){
    uint8_t temp=i;
    temp-=1;
    temp=UART.Digits[temp];
    temp+=48;
    UART_Transmit_Byte(temp);
  }
}

void UART_Transmit_Number(int32_t num){
  if(num<0){UART_Transmit_Byte('-');num=-num;}
  UART_Determine_Digit_Numbers((uint32_t)num);
  UART_Transmit_Number_Digits();
}

void UART_Transmit_Fixed_Digit_Number(int32_t num, uint8_t digits){
  if(num<0){UART_Transmit_Byte('-');num=-num;}
  UART_Determine_Digit_Numbers((uint32_t)num);
  for(int i=0;i<(digits-UART.InputNumberDigits);i++){
    UART_Transmit_Byte(48);
  }
  UART_Transmit_Number_Digits();
}

void UART_Print_Debug_Data(int *data_in){
  for(int i=1;i<=data_in[0];i++){
    UART_Transmit_Fixed_Digit_Number(data_in[i], 4);
    UART_Transmit_Text("  ");
  }
}

uint8_t UART_Receive_Byte(void){
  volatile uint8_t val=0;
  if(UCSRxA_REG & UCSRxA_FEx_bm){
    val=UDRx_REG;
    UART.Error=0x01;
  }else if(UCSRxA_REG & UCSRxA_DORx_bm){
    val=UDRx_REG;
    UART.Error=0x02;
  }else{
    val=UDRx_REG;
    UART.Error=0x00;
  }
return val;
}


uint8_t UART_Read_From_Buffer(uint16_t index){
    return UART.Buffer[index];
}

uint16_t UART_Current_Buffer_Index(void){
    return UART.BufferIndex;
}

uint16_t UART_Current_Read_Index(void){
    return UART.ReadIndex;
}

uint16_t UART_Bytes_Available_To_Read(void){
    return UART.AvailableBytes;
}

uint8_t UART_Read(void){
    uint8_t current_byte=UART.Buffer[UART.ReadIndex];
    UART.ReadIndex++;
    if(UART.ReadIndex>=UART.BufferSize){
      UART.ReadIndex=0;
    }
    if(UART.AvailableBytes>0){
        UART.AvailableBytes--;
    }
  return current_byte;
}

void UART_Flush_Buffer(void){
    UART.Error=0x00;
    for(uint16_t i=0;i<UART.BufferSize;i++){
        UART.Buffer[i]=0x00;
    }
    UART.ReadIndex=0;
    UART.BufferIndex=0;
    UART.AvailableBytes=0;
}

uint32_t UART_Last_Byte_Recevied_Time_Stamp(void){
    return UART.LastRecivedTimeStamp;
}

void UART_Interrupt_Service_Routine(void){
    volatile uint8_t received_byte=UART_Receive_Byte();
    if(UART.Error==0x00){
         UART.Buffer[UART.BufferIndex]=received_byte;
         UART.BufferIndex++;
         UART.AvailableBytes++;
         if(UART.BufferIndex>=UART.BufferSize){UART.BufferIndex=0;}
         //UART.LastRecivedTimeStamp=millis();
    }
}

ISR(USART_RX_vect){
  UART_Interrupt_Service_Routine();
}