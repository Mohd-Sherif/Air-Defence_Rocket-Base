/**
 * @author Mohamed Sherif
 */

// Shift-Registers
#define DATA_PIN 8
#define SH_CP_PIN 6
#define ST_CP_PIN 7
byte arr[] = {
  0b00000000, 
  0b00000001, 
  0b00000011, 
  0b00000111, 
  0b00001111, 
  0b00011111, 
  0b00111111, 
  0b01111111, 
  0b11111111
  };
char readyCtr = 0;
char fireCtr = 0;

// Buttons
#define READY_BUTTON 3
#define FIRE_BUTTON 2

// Joystick
#define X A0
#define Y A1

// DC-Motor
#define IN1 10
#define IN2 9
#define ENA 6

// Servo-Motor
#include <Servo.h>
#define SERVO 12
Servo myServo;
char pos = 0;

/**
 * Configurations Function.
 * Operates only one time in the beginning of executing the code.
 */
void setup() {
  // Shift-Registers Configurations
  pinMode(DATA_PIN, OUTPUT);
  pinMode(SH_CP_PIN, OUTPUT);
  pinMode(ST_CP_PIN, OUTPUT);

  // Initially LEDs are OFF
  digitalWrite(ST_CP_PIN, LOW);
  shiftOut(DATA_PIN, SH_CP_PIN, LSBFIRST, 0);
  digitalWrite(ST_CP_PIN, HIGH);

  // Buttons Configurations
  pinMode(READY_BUTTON, INPUT_PULLUP);
  pinMode(FIRE_BUTTON, INPUT_PULLUP);
  // Enabling Interrupt
  attachInterrupt(digitalPinToInterrupt(READY_BUTTON), readyISR, FALLING);
  attachInterrupt(digitalPinToInterrupt(FIRE_BUTTON), fireISR, FALLING);

  // DC-Motor Configurations
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENA, OUTPUT);

  // Servo-Motor Configurations
  myServo.attach(SERVO);
  myServo.write(0);
}

/**
 * Main behavior of the Application.
 * Continously Repeated until the power off.
 */
void loop() {
  // Recieving the values from Joystick
  // Reading joystick X-Axis value
  double x = analogRead(X);
  // Reading joystick Y-Axis value
  double y = analogRead(Y);
  
  // DC-Motor Behavior
  if(x < 400){
    rotateLeft(x);
  }
  else if(x > 600){
    rotateRight(x);
  }
  else{
    stopMotor();
  }

  if(y > 600 && pos > 0){
  // Servo-Motor Behavior
    angleDown();
  }
  else if(y < 400 && pos < 180){
    angleUp();
  }
}

// ISR

/**
 * Interrupt Service Routine for Ready LEDs in the Control Tower
 * Indicates that the rockets are ready to be fired if the yellow LEDs Emits.
 */
void readyISR(){
  if(readyCtr <= fireCtr && readyCtr != 8){
    readyCtr++;
    digitalWrite(ST_CP_PIN, LOW);
    shiftOut(DATA_PIN, SH_CP_PIN, LSBFIRST, arr[readyCtr]);
    digitalWrite(ST_CP_PIN, HIGH);
  }
}

/**
 * Interrupt Service Routine for Fire LEDs on the Car.
 * Indicates that the rockets are fired if the red LED Emits.
 */
void fireISR(){
  if(fireCtr < readyCtr && fireCtr != 8){
    fireCtr++;
    digitalWrite(ST_CP_PIN, LOW);
    shiftOut(DATA_PIN, SH_CP_PIN, LSBFIRST, arr[fireCtr]);
    digitalWrite(ST_CP_PIN, HIGH);
  }
}

// DC-Motor Functions

void rotateLeft(double rotatingSpeed){
  analogWrite(ENA, map(1023 - rotatingSpeed - 900, 0, 1023, 0, 255));
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
}

void rotateRight(double rotatingSpeed){
  analogWrite(ENA, map(rotatingSpeed - 900, 0, 1023, 0, 255));
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
}

void stopMotor(){
  analogWrite(ENA, LOW);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
}

// Servo-Motor Functions

void angleUp(){
  pos++;
  myServo.write(pos);
  delay(50);
}

void angleDown(){
  pos--;
  myServo.write(pos);
  delay(50);
}
