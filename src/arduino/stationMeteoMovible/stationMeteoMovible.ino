#include <SPI.h>

#include <RF24_config.h>
#include <RF24.h>
#include <nRF24L01.h>
#include <printf.h>


// Pin configuration
int leftM = 6;       // left motor
int rightM = 7;      // right motor
int startCar = 3;    // start car
int lightSensor = 0;
int tempSensor = 1;

// Motor command variables
int leftSpeed;
int rightSpeed;
struct _motorCmd {
  int speed;
  int steer;
} motorCmd;

// Sensors handling
enum {TEMP, LIGHT};
struct _sensorMsg {
  uint16_t type;
  float value;
} sensorMsg;
float voltage;

// Timers
int tempDelay = 1000;
int lightDelay = 100;
int timerTemp, timerLight;
int time;

// nrf configuration
int nrfCEpin = 9;
int nrfCSpin = 10;
uint8_t addresses[][6] = {"meteo", "motor"};
RF24 radio = RF24(nrfCEpin, nrfCSpin);

void setup()  { 
  Serial.begin(9600);
  radio.openWritingPipe(addresses[0]);
  radio.openReadingPipe(0,addresses[1]);
  time = millis();
  timerTemp = time;
  timerLight = time;
  leftSpeed = 0;
  rightSpeed = 0;
  pinMode(startCar, OUTPUT);
  digitalWrite(startCar, HIGH);
}


void loop()  {
  // read incoming commands for the car
  if(radio.available(0)){
    radio.read(&motorCmd,sizeof(struct _motorCmd));
    //leftSpeed = motorCmd.speed + (motorCmd.steer - 50)
    // command the motors
    analogWrite(leftM, leftSpeed);
    analogWrite(rightM, rightSpeed);
  }
  
  // send sensors' data if necessary
  time = millis();
  if(time - timerTemp > tempDelay){
    voltage = analogRead(tempSensor) * 3.3;
    sensorMsg.type = TEMP;
    sensorMsg.value = (voltage - 0.5) * 100;
    radio.write(&sensorMsg,sizeof(struct _sensorMsg));
    timerTemp = millis();
  }
  if(time - timerLight > lightDelay){
    sensorMsg.type = LIGHT;
    sensorMsg.value = analogRead(lightSensor);
    radio.write(&sensorMsg,sizeof(struct _sensorMsg));
    timerTemp = millis();
  }
}
