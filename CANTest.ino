// demo: CAN-BUS Shield, send data
// loovee@seeed.cc

#include <shield_mcp_can.h>
#include <SPI.h>

// the cs pin of the version after v1.1 is default to D9
// v0.9b and v1.0 is default D10
const int SPI_CS_PIN = 10;

MCP_CAN CAN(SPI_CS_PIN);                                    // Set CS pin

void setup()
{
    Serial.begin(115200);

    while (CAN_OK != CAN.begin(CAN_500KBPS))              // init can bus : baudrate = 500k
    {
        Serial.println("CAN BUS Shield init fail");
        Serial.println(" Init CAN BUS Shield again");
        delay(100);
    }
    Serial.println("CAN BUS Shield init ok!");
}

unsigned char stmp[8] = {0, 0, 0, 0, 0, 0, 0, 0};
int randomNumber;

void loop()
{
    // send data:  id = 0x00, standrad frame, data len = 8, stmp: data buf
//
//    stmp[0] = 2;
//    stmp[1] = 5;
//    stmp[2] = 2;
//    stmp[3] = 5;
//    stmp[4] = 2;
//    stmp[5] = 5;
//    stmp[6] = 2;
//    stmp[7] = 5;

    //Shift Light tests
    if(stmp[6] == 1){
      stmp[0] = 0;
      stmp[2] = 0;
      stmp[4] = 0;
      stmp[6] = 0;
    }
    else{
      if(stmp[2] == 0){
        stmp[2] = 1;
      }
      else{
        if(stmp[4] == 0){
          stmp[4] = 1;
        }
        else{
          stmp[6] = 1;
        }
      }
    }

    Serial.print("1: ");
    Serial.print(stmp[0], DEC);

    Serial.print(" 2: ");
    Serial.print(stmp[1], DEC);
    
    Serial.print(" 3: ");
    Serial.print(stmp[2], DEC);

    Serial.print(" 4: ");
    Serial.print(stmp[3], DEC);

    Serial.print(" 5: ");
    Serial.print(stmp[4], DEC);

    Serial.print(" 6: ");
    Serial.print(stmp[5], DEC);

    Serial.print(" 7: ");
    Serial.print(stmp[6], DEC);

    Serial.print(" 8: ");
    Serial.println(stmp[7], DEC);

    
    CAN.sendMsgBuf(0x2EC, 0, 8, stmp);
    delay(100);
    CAN.sendMsgBuf(0x2ED, 0, 8, stmp);
    delay(100);
    CAN.sendMsgBuf(0x2EE, 0, 8, stmp);
    delay(100);
    CAN.sendMsgBuf(0x2EF, 0, 8, stmp);
    delay(1000);                       // send data per 1000ms
}

// END FILE
