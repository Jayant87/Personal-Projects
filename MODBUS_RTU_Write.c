#include <stdint.h>

#define SLAVE_ADDRESS 170
#define RXD2 17
#define TXD2 16

uint16_t CRC_U16;
byte new_arr[10];
byte respone[53];

uint32_t ModRTU_CRC()
{
  uint32_t CRC_U16 = 0xFFFF;
  
  for (int pos = 0; pos <= 7; pos++) {
    CRC_U16 ^= (uint16_t)new_arr[pos];          // XOR byte into least sig. byte of CRC_U16
  
    for (int i = 8; i != 0; i--) {    // Loop over each bit
      if ((CRC_U16 & 0x0001) != 0) {      // If the LSB is set
        CRC_U16 >>= 1;                    // Shift right and XOR 0xA001
        CRC_U16 ^= 0xA001;
      }
      else                            // Else LSB is not set
        CRC_U16 >>= 1;                    // Just shift right
    }
  }
  uint16_t current_upper = CRC_U16 >> 8;
  Serial.print("current_upper ");
  Serial.print(current_upper);
  uint16_t current_lower = CRC_U16 & ~(0xFF << 8); 
  Serial.print("current_lower ");
  Serial.print(current_lower);
  CRC_U16 = (current_lower << 8) | current_upper;
  return CRC_U16;  
}

void Write_HoaldingRegister(uint16_t starting_adderess, uint32_t no_of_registers)
{

  new_arr[0] = SLAVE_ADDRESS; 
  new_arr[1] = (6);
  new_arr[2] = ((starting_adderess >> 8) & 0xFF);
  new_arr[3] =  (starting_adderess & 0xFF);
  new_arr[4] =  ((no_of_registers >> 24) & 0xFF);
  new_arr[5] = ((no_of_registers >> 16) & 0xFF);
  new_arr[6] = ((no_of_registers >> 8) & 0xFF);
  new_arr[7] = (no_of_registers & 0xFF);

  uint16_t new_crc =  ModRTU_CRC();

  new_arr[8] = ((new_crc >> 8) & 0xFF);
  new_arr[9] = new_crc & ~(0xFF << 8);
  Serial.print(" newcrc - ");
  Serial.println(new_crc);
}

void Read_HoaldingRegister()
{
  while(Serial2.available() == 0);
  Serial.print("CHECK");
  for(int i = 0;i <53;i++)
  {
    respone[i] = Serial2.read();
  }
  for(int i = 0;i <53;i++)
  {
    Serial.print("INDEX");
    Serial.print(i);
    Serial.print(respone[i]);
  }
}

void setup() {
Serial.begin(9600);
Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);
}

void loop() {
  
  Write_HoaldingRegister(16,1);
  for(int parameters = 0;parameters<10;parameters++)
 {
    Serial2.write(new_arr[parameters]);
  }
  delay(1000);

}