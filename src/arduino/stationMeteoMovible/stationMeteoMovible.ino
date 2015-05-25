#include <SPI.h>

#include <RF24_config.h>
#include <RF24.h>
#include <nRF24L01.h>
#include <printf.h>


// Pin configuration
int leftM = 6;       // left motor
int rightM = 7;      // right motor
int lightSensor = 7;
int tempSensor = 9;

// Motor command variables
int leftSpeed;
int rightSpeed;
struct _motorCmd {
  uint16_t speed;
  uint16_t steer;
} motorCmd;

// Sensors handling
enum {TEMP, LIGHT};
struct _sensorMsg {
  uint16_t type;
  uint16_t value;
} sensorMsg;
float voltage;

// Timers
int tempDelay = 1000;
int lightDelay = 100;
int timerTemp, timerLight;
int time;

// nrf configuration
int nrfCEpin = 40;
int nrfCSpin = 53;
uint8_t addresses[][6] = {"meteo", "motor"};
RF24 radio(nrfCEpin, nrfCSpin);





void setup()  { 
  Serial.begin(9600);
  radio.begin();
  radio.openWritingPipe(0x0000000001LL);
  radio.openReadingPipe(1,0x0000000002LL);
  radio.startListening();
  time = millis();
  timerTemp = time;
  timerLight = time;
  analogWrite(leftM, 92);
  analogWrite(rightM,92);
}


void loop()  {
  // read incoming commands for the car
  if(radio.available()){
    radio.read(&motorCmd,sizeof(struct _motorCmd));
    Serial.print("Motor message : SPEED=");
    Serial.print(motorCmd.speed);
    Serial.print("; STEER=");
    Serial.println(motorCmd.steer);
    // map speed
    if(motorCmd.speed > 50){
      leftSpeed  = map(motorCmd.speed, 51, 100, 91, 0);
      rightSpeed = map(motorCmd.speed, 51, 100, 93, 180);
    }
    else if(motorCmd.speed < 50){
      leftSpeed  = map(motorCmd.speed, 49, 0, 93, 180);
      rightSpeed = map(motorCmd.speed, 49, 0, 91, 0);
    }
    else{
      leftSpeed  = 92;
      rightSpeed = 92;
    }
    // map steering
    if(motorCmd.steer < 50)
      leftSpeed  += map(motorCmd.steer, 49, 0, 1, 30);
    else if(motorCmd.steer > 50)
      rightSpeed -= map(motorCmd.steer, 51, 100, 1, 30);
      
    // right to pins
    leftSpeed  = constrain(leftSpeed , 0, 180);
    rightSpeed = constrain(rightSpeed, 0, 180);
    analogWrite(leftM, leftSpeed);
    analogWrite(rightM, rightSpeed);
  }
  
  // send sensors' data if necessary
  time = millis();
  if(time - timerTemp > tempDelay){
    radio.stopListening();
    voltage = analogRead(tempSensor);
    voltage = (float)(1023-voltage)*10000/voltage;
    sensorMsg.type = TEMP;
    sensorMsg.value = (uint16_t)(1/(log(voltage/10000)/3975+1/298.15)-273.15);
    Serial.print("Sending temperature ");
    Serial.println(sensorMsg.value);
    if(!radio.write(&sensorMsg,sizeof(struct _sensorMsg))){
      Serial.println("Message sending failed");
    }
    else
      Serial.println("Temperature sent");
    timerTemp = millis();
    radio.startListening();   
  }
  if(time - timerLight > lightDelay){
    radio.stopListening();
    sensorMsg.type = LIGHT;
    sensorMsg.value = (int)(analogRead(lightSensor));
    Serial.print("Sending light ");
    Serial.println(sensorMsg.value);
    if(!radio.write(&sensorMsg,sizeof(struct _sensorMsg))){
      Serial.println("Message sending failed");
    }
    else
      Serial.println("Light sent");
    timerLight = millis();
    radio.startListening();
  }
}
