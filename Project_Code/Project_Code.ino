/**
 * A Rocket-Base simulation, by LEDs instead of Rockets, that can rotates 360 degrees
 * around X-Axis and 77 degrees around Y-Axis. There're button, Joystick, Servo-Motor,
 * DC-Motor and 16 LEDs (8 Red LEDs & 8 Yellow LEDs).
 * 
 * Operation:-
 * By moving the joystick up and down, the rockets goes up and down, respectively.
 * By moving the joystick right and left, the Base rotates right and left, respectively.
 * By pressing on the joystick button, a yellow LED will emit light that indicates that the
 * corrosponding rocket is ready to be fired.
 * By pressing on the seperate button, a red LED will emit light 
 * -only when corrosponding yellow LED emits- that indicates that the corrosponding rocket is fired.
 * 
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
  myServo.write(90);
}

/**
 * Main behavior of the Application.
 * Continously Repeated until the power off.
 */
void loop() {
  // Recieving the values of Joystick
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

  // Servo-Motor
  if(y > 600 && pos < 157){
    angleUp();
  }
  else if(y < 400 && pos > 80){
    angleDown();
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

/**
 * Controls the DC-Motor to rotate left with respect to the value of 
 * the sliding value of the joystick X-Axis.
 * 
 * @param rotatingSpeed used to control the speed of rotating of the motor.
 * @return void
 */
void rotateLeft(double rotatingSpeed){
  analogWrite(ENA, map(1023 - rotatingSpeed, 0, 1023, 0, 255));
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
}

/**
 * Controls the DC-Motor to rotate right with respect to the value of 
 * the sliding value of the joystick X-Axis.
 * 
 * @param rotatingSpeed used to control the speed of rotating of the motor.
 * @return void
 */
void rotateRight(double rotatingSpeed){
  analogWrite(ENA, map(rotatingSpeed, 0, 1023, 0, 255));
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
}

/**
 * Stops the DC-Motor.
 * 
 * @param void
 * @return void
 */
void stopMotor(){
  analogWrite(ENA, LOW);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
}

// Servo-Motor Functions

/**
 * Controls the Servo-Motor's angle to increase it up.
 * 
 * @param void
 * @return void
 */
void angleUp(){
  myServo.write(++pos);
  delay(30);
}

/**
 * Controls the Servo-Motor's angle to decrease it down.
 * 
 * @param void
 * @return void
 */
void angleDown(){
  myServo.write(--pos);
  delay(30);
}
