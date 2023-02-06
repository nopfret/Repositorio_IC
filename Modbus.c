#include <ModbusMaster.h>                             //Library for using ModbusMaster
#define FC 4                                          //RE & DE are connected with the GPIO4 (for flow control) of the Norvi ESP32 Device.
ModbusMaster node;                                    //object node for class ModbusMaster

union u16I_x_Float{
        uint16_t ints[2];
        float Float;
};


void preTransmission()                                //Function for setting stste of Flow control pin of RS-485
{
  digitalWrite(FC, 0xff);
}

void postTransmission()
{
  digitalWrite(FC, 0xff);
}

void setup()
{
  pinMode(FC, OUTPUT);
  digitalWrite(FC, 0xff);
  Serial.begin(9600);  
  Serial2.begin(9600);                                //Baud Rate as 9600
  node.begin(1, Serial);                              //Slave ID as 1
  node.preTransmission(preTransmission);              //Callback for configuring RS-485 Transreceiver correctly
  node.postTransmission(postTransmission);
  Serial.print("INICIANDO TESTE:\n");
}

float modbus_to_float(uint16_t low, uint16_t high){
    union u16I_x_Float to_float;

    to_float.ints[1] = high;
    to_float.ints[0] = low;
    
    return to_float.Float;
}


void loop()
{
    float temp;
    
    uint8_t busret = node.readHoldingRegisters(0x00FE, 2);

    if (busret == node.ku8MBSuccess){
          temp = modbus_to_float(node.getResponseBuffer(1), node.getResponseBuffer(0));
          Serial.print("Sucesso: ");
          Serial.println(temp);
    }else{
      Serial.print("Erro: ");
      Serial.println(busret);                                              
    }

    delay(1000);
}

