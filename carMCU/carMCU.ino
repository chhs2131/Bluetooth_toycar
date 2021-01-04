///////////////////////////////////////
//
// BlueTooth miniCar
// ver alpha , 2021-01-04
//
///////////////////////////////////////
#include <SoftwareSerial.h>
#define DATASIZE 4

const int LED_FRONT = 1;
const int LED_BACK = 2;
const int BLUE_RX = 3;
const int BLUE_TX = 4;
const int MOTOR_DCF = 5;
const int MOTOR_DCB = 6;
const int MOTOR_SER = 7;

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
        //UART Check
        if(BlueSerial.available() == DATASIZE) {
            for(int i=0; i<DATASIZE; i++) {
                reciveData[i] = BlueSerial.read();
                
            }
        } else {
            if(BlueSerial.available()) {
                writeErrorMsg("[warning] Bluetooth module sended strange value ... so arduino delete that");
            }
            while(BlueSerial.available())
                BlueSerial.read();
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
