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
#define FIRE_BUTTON 3

// Joystick
#define SW 2
#define X A0
#define Y A1

// DC-Motor
#define IN1 10
#define IN2 9
#define ENA 6

  // DC-Motor Configurations
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENA, OUTPUT);

