

void 	 UART_Struct_Init(void);
void 	 UART_Config_Transmitter(void);
void 	 UART_Config_Receiver(void);
void 	 UART_Config_Receiver_Interrupt(void);
void 	 UART_Init(uint32_t BAUD);                        //Initialize UART
void 	 UART_Transmit_Byte(uint8_t val);                 //Transmit single byte
void 	 UART_Transmit_Byte_Hex(uint8_t val);             //Transmit single byte in hex
void 	 UART_Transmit_Byte_Bin(uint8_t val);             //Transmit single byte in bin
void 	 UART_Transmit_Text(char *str);                   //Transmit character array
void 	 UART_Determine_Digit_Numbers(uint32_t num);
void 	 UART_Transmit_Number_Digits(void);
void 	 UART_Transmit_Number(int32_t num);               //Transmit integer number
void 	 UART_Transmit_Fixed_Digit_Number(int32_t num, uint8_t digits);
void 	 UART_Print_Debug_Data(int *data_in);
uint8_t  UART_Receive_Byte(void);
uint8_t  UART_Read_From_Buffer(uint16_t index);
uint16_t UART_Current_Buffer_Index(void);
uint16_t UART_Current_Read_Index(void);
uint16_t UART_Bytes_Available_To_Read(void);              //Bytes availabe in RX buffer
uint8_t  UART_Read(void);                                 //Read single byte from RX buffer and auto-increment buffer index
void     UART_Flush_Buffer(void);                         //Erase all RX buffer
uint32_t UART_Last_Byte_Recevied_Time_Stamp(void);        //Timestamp of last byte received
void     UART_Interrupt_Service_Routine(void);