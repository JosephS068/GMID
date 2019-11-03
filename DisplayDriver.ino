/* Takes 8 bytes and an ID from the can network. Checks ID then processes information accordingly.
 * Formats information returns the created String from data and an ID to match up with the UI's IDs.
 */
#include <SPI.h>
#include <SoftwareSerial.h>
#include <Nextion.h>
#include <shield_mcp_can.h>

SoftwareSerial nextion(2, 3);// nextion TX to pin 2 and RX to pin 3 of Arduino

Nextion my_nextion(nextion, 9600); //create a nextion object named my_nextion using the nextion serial port @ 9600bps

const int SPI_CS_PIn = 10;

String data;
String num;
char lastChar;
unsigned int value;

MCP_CAN CAN(SPI_CS_PIn);                                    // Set CS pin

void setup() {
  Serial.begin(115200);
  while (CAN_OK != CAN.begin(CAN_1000KBPS))              // init can bus : baudrate = 500k
  {
      Serial.println("CAN BUS Shield init fail");
      Serial.println(" Init CAN BUS Shield again");
      delay(100);
  }
  Serial.println("CAN BUS Shield init ok!");
  my_nextion.init();
}

void loop()
{
  unsigned char len = 0;
    unsigned char buf[8];
    // Serial.println(CAN.checkReceive());

    if(CAN_MSGAVAIL == CAN.checkReceive())            // check if data coming
    {
        CAN.readMsgBuf(&len, buf);    // read data,  len: data length, buf: data buf
        //Serial.println("Received");
        unsigned int canId = CAN.getCanId();
        char* command = "USL.pic=#";
        switch(canId){
          //Data for RPM, Engine Temperature, Oil Temperature and Oil Pressure
          case 0x2EC:
            //RPM
            value = (buf[0]<<8) + buf[1]; 
            data = (String)value;
            my_nextion.setComponentText("RPM", data); 

            // Engine Temperature, We are of by a power of ten, during string creation we fix this issue
            value = (buf[2]<<8) + buf[3];
            data = value;
            lastChar = data.charAt(data.length()-1);
            data.setCharAt(data.length()-1, '.');
            data += lastChar;
            my_nextion.setComponentText("ETM", data); 

            // Oil Temperature, Same as Engine Temperature
            value = (buf[4]<<8) + buf[5];
            data = value;
            lastChar = data.charAt(data.length()-1);
            data.setCharAt(data.length()-1, '.');
            data += lastChar;
            my_nextion.setComponentText("OTM", data); 

            // Oil Pressure, Multiplied by 145 to get proper value
            value = (buf[6]<<8) + buf[7];
            value*=145;
            data = value;//changed num to data
            data = data.substring(0, data.length()-3) + "." + data.charAt(data.length()-3);
            my_nextion.setComponentText("OPR", data); 
          break;

          //Data for Lap Delta, Lateral Acceleration and Ground Speed
          case 0x2ED:
            // Lap Delta
            value = (buf[0]<<8)+buf[1];
            data = (String)value; //Changed num to data
            data = data.substring(0, data.length()-3) + "`" + data.substring(data.length()-3, data.length()-1);
            my_nextion.setComponentText("LDT", data);
             
            // Lateral Acceleration
            value = (buf[2]<<8) + buf[3];
            data = (String)value;
            my_nextion.setComponentText("LAC", data); 

            // Ground Speed
            value = (buf[4]<<8) + buf[5];
            data = (String)value;
            my_nextion.setComponentText("GSP", data); 
          break;

          //Data for Up Shift Light
          case 0x2EE:
            value = 0;
            if(buf[0] || buf[1]) value++;
            if(buf[2] || buf[3]) value++;
            if(buf[4] || buf[5]) value++;
            if(buf[6] || buf[7]) value++;
            Serial.println("USL.pic=" + (String)value);
           command[8]=value+'0';
            my_nextion.sendCommand(command); 
            my_nextion.sendCommand("ref USL");  
          break;

          //Data for Gear Position
          case 0x2EF:
            if(buf[0] || buf[1]) data = "N";
            else data = (String)((buf[2]<<8) + buf[3]);
            my_nextion.setComponentText("GEAR", data);
          break;
        }
    }
}
