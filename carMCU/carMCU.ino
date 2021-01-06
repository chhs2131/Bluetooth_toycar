///////////////////////////////////////
//
// BlueTooth miniCar
// ver 1.00 , 2021-01-06
//
///////////////////////////////////////
#include <SoftwareSerial.h>
#include <Servo.h>
#define DATASIZE 4

const int LED_FRONT = 12;
const int LED_BACK = 13;
const int BLUE_RX = 2;
const int BLUE_TX = 3;
const int MOTOR_DCF = 6;
const int MOTOR_DCB = 5;
const int MOTOR_SER = 10;

char reciveData[DATASIZE] = {0, 0, 127, 0}; // DC Motor F, DC Motor B, Servo, function
void writeErrorMsg(char*);

Servo servo;
SoftwareSerial BlueSerial(BLUE_RX,BLUE_TX);

void setup() {
    pinMode(LED_FRONT, OUTPUT);
    pinMode(LED_BACK, OUTPUT);
    pinMode(MOTOR_DCF, OUTPUT);
    pinMode(MOTOR_DCB, OUTPUT);
    servo.attach(MOTOR_SER);
    
    Serial.begin(115200);
    BlueSerial.begin(115200); 
}

void loop() {
    while(true) {
        //UART recive & check
        if(BlueSerial.available()) {
            if(BlueSerial.read() == 'S') {
              for(int i=0; i<DATASIZE; i++) {
                  reciveData[i] = BlueSerial.read();
                  Serial.write(reciveData[i]);
              }
              if(BlueSerial.read() != 'E') {
                writeErrorMsg("[error] End sign is not send.\n");
                continue;
              }
              else if( (reciveData[3] & 0b00000110) != 0b00000010) {
                writeErrorMsg("[error] check bit is wrong... 0&1\n");
                continue;
              }
              /*
              else if( ((reciveData[0] & 0b00000001) + (reciveData[1] & 0b00000001) + (reciveData[2] & 0b00000001)) & 0b00000001 != reciveData[3] & 0b00000001 ) {
                char compare_value = ((reciveData[0] & 0b00000001) + (reciveData[1] & 0b00000001) + (reciveData[2] & 0b00000001)) & 0b00000001 + 0x30;
                Serial.write(compare_value);
                writeErrorMsg("[error] check bit is wrong... P\n");
                continue;
              }
              */
            }
            else {
              continue;
            }
        } else {
            continue;
        }
   
        //Servo Motor
        char servo_degree = 90; //if(101 <= reciveData[2] && reciveData[2] <= 155)
        if(reciveData[2] <= 100)
          servo_degree = 90-reciveData[2];
        else if(156 <= reciveData[2])
          servo_degree = 90+(reciveData[2]-155);
        else
          writeErrorMsg("[warning] servo value is wrong type");
        servo.write(servo_degree);
        
        //DC Motor
        analogWrite(MOTOR_DCF,reciveData[0]);
        analogWrite(MOTOR_DCB,reciveData[1]);
        
        //Front LED Set
        if(reciveData[3] & 0b10000000) {
            digitalWrite(LED_FRONT,HIGH);
        } else {
            digitalWrite(LED_FRONT,LOW);
        }
    
        //Back LED Set
        if(reciveData[3] & 0b01000000) {
            digitalWrite(LED_BACK,HIGH);
        } else {
            digitalWrite(LED_BACK,LOW);
        }

        delay(200);

    }
}

void writeErrorMsg(char* message) {
    Serial.write(message);
    BlueSerial.write(message);
}
