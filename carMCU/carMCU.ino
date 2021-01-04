///////////////////////////////////////
//
// BlueTooth miniCar
// ver 1.00 , 2021-01-04
//
///////////////////////////////////////
#include <SoftwareSerial.h>
#define DATASIZE 4

const int LED_FRONT = 12;
const int LED_BACK = 13;
const int BLUE_RX = 3;
const int BLUE_TX = 4;
const int MOTOR_DCF = 6;
const int MOTOR_DCB = 5;
const int MOTOR_SER = 10;

int reciveData[DATASIZE] = {0, 0, 127, 0}; // DC Motor F, DC Motor B, Servo, function
void writeErrorMsg(char*);

SoftwareSerial BlueSerial(BLUE_RX,BLUE_TX);

void setup() {
    pinMode(LED_FRONT, OUTPUT);
    pinMode(LED_BACK, OUTPUT);
    pinMode(MOTOR_DCF, OUTPUT);
    pinMode(MOTOR_DCB, OUTPUT);
    pinMode(MOTOR_SER, OUTPUT);
    
    Serial.begin(9600);
    BlueSerial.begin(9600); 
}

void loop() {
    while(true) {
        //UART recive & check
        if(BlueSerial.available()) {
            if(BlueSerial.read() == 'S') {
              for(int i=0; i<DATASIZE; i++) {
                  reciveData[i] = BlueSerial.read();
              }
              if(BlueSerial.read() != 'E') {
                writeErrorMsg("[error] End sign is not send.");
                continue;
              }
              else if( (reciveData[3] & 0b00000110) != 0b00000010) {
                writeErrorMsg("[error] check bit is wrong... 0&1");
                continue;
              }
              else if( ((reciveData[0] & 1) | (reciveData[1] & 1) | (reciveData[2] & 1)) & 1 != reciveData[3] & 1 ) {
                writeErrorMsg("[error] check bit is wrong... P");
                continue;
              }
            }
            else {
              continue;
            }
        } else {
            continue;
        }
   
        //Servo Motor
        analogWrite(MOTOR_SER, reciveData[2]); //0~127 LEFT , 128~256 RIGHT
        
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

    }
}

void writeErrorMsg(char* message) {
    Serial.write(message);
    BlueSerial.write(message);
}
