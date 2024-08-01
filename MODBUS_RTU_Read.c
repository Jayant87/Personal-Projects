#include <stdint.h>
#include <HardwareSerial.h>

#define SLAVE_ADDRESS 170
#define RXD2 17
#define TXD2 16

uint16_t crc;
byte new_arr[7];
uint8_t recieved_data[48 * 4 * 2];

uint16_t Division_U16[48] = {1000, 1000,1000, 1000,1000, 1000,1000, 1000,
1000, 1000,1000, 1000,1000, 1000,1000, 1,1,100,100,1,100
,1,1,1,10,100,100,100,1000,1000,1000,1000,1000,1000,1000,1000,1000
,1000,1000,1000,1000,1,1,1,1,1,1,1}; 

char *Labels[48]={"Cum KVAH ","Cum KVAH DG ","Cum KVARH ","Cum KVARH DG ","Cum EB KWH ","Cum DG KWH ","Total KW ","Total KVA ",
         "Total KVAR  ","EB R Phase LOAD SETTING","EB Y Phase LOAD SETTING","EB B Phase LOAD SETTING  ", "DG R Phase LOAD SETTING ",
         "DG y Phase LOAD SETTING ", "DG b Phase LOAD SETTING", "RELAY STATUS", "EB/DG STATUS", "EB LOAD SETTING",
         "DG LOAD SETTING ", "Meter Serial Number ", "balance amount ", "no of over load check ",
         "over load delay between two attemps in seconds ", "OVER LOAD CHECK TIME IN SECOND ", "FREQUENCY ",
        "VOLTAGE-R ","VOLTAGE-Y ","VOLTAGE-B ","CURRENT-R ","CURRENT-Y", "CURRENT-B ", "PF ", "KW LOAD-R ",
         "KW LOAD-Y ", "KW LOAD-B ", "KVA LOAD-R ", "KVA LOAD-Y ", "KVA LOAD-B ","KVAR LOAD-R ", "KVAR LOAD-Y ",
         "KVAR LOAD-B ","EB Phase Enable/Disable ", "DG Phase Enable/Disabl ", "RTC Date (DDMMYY) ", "RTC Time (HHMMSS)",
       "Meter BUS ID ", "READ BUS_ID Common Cmd ","Reset Energy Register "};

uint16_t ModRTU_CRC() {
  uint16_t crc = 0xFFFF;

  for (int pos = 0; pos <= 5; pos++) {
    crc ^= (uint16_t)new_arr[pos];  // XOR byte into least sig. byte of crc

    for (int i = 8; i != 0; i--) {  // Loop over each bit
      if ((crc & 0x0001) != 0) {    // If the LSB is set
        crc >>= 1;                  // Shift right and XOR 0xA001
        crc ^= 0xA001;
      } else        // Else LSB is not set
        crc >>= 1;  // Just shift right
    }
  }
  uint8_t current_upper = crc >> 8;
  uint8_t current_lower = crc & ~(0xFF << 8);
  crc = (current_lower << 8) | current_upper;
  
  return crc;
}

void Request_HoaldingRegister(uint16_t starting_adderess, uint16_t no_of_registers) {
  uint64_t var;
  new_arr[0] = SLAVE_ADDRESS;
  new_arr[1] = (3);
  new_arr[2] = ((starting_adderess >> 8) & 0xFF);
  new_arr[3] = (starting_adderess & 0xFF);
  new_arr[4] = ((no_of_registers >> 8) & 0xFF);
  new_arr[5] = (no_of_registers & 0xFF);
  uint16_t new_crc = ModRTU_CRC();
  new_arr[6] = ((new_crc >> 8) & 0xFF);
  new_arr[7] = (new_crc & 0xFF);
}



void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);
  Serial.println("Sending request . ");
  Request_HoaldingRegister(1, 48);

  for (int i = 0; i < 8; i++) {
    Serial2.write(new_arr[i]);
  }

  uint16_t index = 0;
  uint16_t count = 0;
  uint8_t v_RcvdByte_U8;
  while (Serial2.available() == 0) {
  }

  uint32_t v_TimeStart_U32 = millis();
  while (millis() - v_TimeStart_U32 < 2000) {
    while (Serial2.available() > 0) {
      recieved_data[index] = Serial2.read();
      index++;
    }
  }

  Serial.print("Total Bytes Rcvd = ");
  Serial.println(index);

  uint32_t v_RcvdInfo_U32 = 0;
  uint8_t var_U8 = 0;
  for (count = 4; count < (index - 2); count += 4) {
    Serial.print((count / 4));
    Serial.print(" | ");
    Serial.print(recieved_data[count]);
    Serial.print("-");
    Serial.print(recieved_data[count + 1]);
    Serial.print("-");
    Serial.print(recieved_data[count + 2]);
    Serial.print("-");
    Serial.print(recieved_data[count + 3]);
    Serial.print("   |    ");
    v_RcvdInfo_U32 = (recieved_data[count + 3] << 0);
    v_RcvdInfo_U32 |= (recieved_data[count + 2] << 8);
    v_RcvdInfo_U32 |= (recieved_data[count + 1] << 16);
    v_RcvdInfo_U32 |= (recieved_data[count + 0] << 24);
    v_RcvdInfo_U32 = v_RcvdInfo_U32/Division_U16[var_U8];
    Serial.print(v_RcvdInfo_U32);
    Serial.print("    ");
    Serial.print(Labels[var_U8]);
    var_U8++;
    Serial.println(" ");
  }
}


void loop() {
  // put your main code here, to run repeatedly:


  /* uint32_t convert[48*2];
  int a = 0,b = 1,c = 2,d = 3;
  for(int i = 0;i<sizeof(recieved_data);i++)
  {
    convert[i] = (recieved_data[a] << 24| recieved_data[b] << 16| recieved_data[c] << 8| recieved_data[d]);
    a++;
    b++;
    c++;
    d++;
  }
  for(int i = 0;i<sizeof(recieved_data);i++)
  {
   Serial.println(convert[i]);
  }*/
  /* for(int i = 0;i<48;i++)
  {
    Serial.println(str[i],HEX);
  }*/
  delay(2000);
}