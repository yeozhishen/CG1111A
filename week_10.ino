#include "MeMCore.h"

// This section deals with the motors 
#define TURNING_TIME_MS 330 // The time duraction (ms) for turning 90 degrees 
#define motorSpeed 250
#define RED 0
#define GREEN 1
#define ORANGE 2
#define PURPLE 3
#define LIGHT_BLUE 4
#define WHITE 5
#define FORWARD 6
#define STOP 7
#define LEFT 8
#define RIGHT 9

MeDCMotor leftMotor(M1); // assigning leftMotor to port M1 
MeDCMotor rightMotor(M2); // assigning RightMotor to port M2

void move ( int mode ) {
  if ( mode == LEFT ) {
    // Turn left and move straight for abit
    leftMotor.run(motorSpeed); // Positive: wheel turns clockwise 
    rightMotor.run(motorSpeed); // Positive: wheel turns clockwise 
    delay(TURNING_TIME_MS); // Keep turning left for this time duration
  } else if ( mode == RIGHT ) {
    // Turn right     
    leftMotor.run(-motorSpeed); // Negative: wheel turns anti-clockwise 
    rightMotor.run(-motorSpeed); // Negative: wheel turns anti-clockwise 
    delay(TURNING_TIME_MS); // Keep turning left for this time duration

  } else if ( mode == FORWARD ) {
    leftMotor.run(-motorSpeed); // Negative: wheel turns anti-clockwise 
    rightMotor.run(motorSpeed); // Positive: wheel turns clockwise
    delay(100); 
  } else if ( mode == STOP ) {
    leftMotor.run(0); 
    rightMotor.run(0);
  }else if
}

void motor_mode ( int mode ) {
  if ( mode == RED ) {
    move ( LEFT );
  }
  if ( mode == GREEN ) {
    move ( RIGHT );
  }
  if ( mode == ORANGE ) {
    move ( LEFT );
    move ( LEFT );
  }
  if ( mode == PURPLE ) {
    move ( LEFT );
    move ( FORWARD );
    move ( LEFT );
  }
  if ( mode == LIGHT_BLUE ) {
    move ( RIGHT );
    move ( FORWARD );
    move ( RIGHT );
  }
  if ( mode == WHITE ) {
  move ( STOP );
  }
}

// This section defines the purpose of A2 and A3
#define IR_DETECTOR A2
#define LDR A3

// This section deals with the line follower
MeLineFollower lineFinder(PORT_2); // assigning lineFinder to RJ25 port 2
// This function detects a blackstrip 
bool black_strip () {
  int sensorState = lineFinder.readSensors(); // read the line sensor's state
  if ( sensorState == S1_IN_S2_IN ) {
    return true;
  }
  return false;
}

// This section deals with the ultrasonic sensor 
#define ULTRASONIC 12
#define SPEED_OF_SOUND 0.034 // in cm/microsec
// This function determines the distance from wall detected by the ultrasonic sensor 
unsigned long ultra_distance ( ) {
  pinMode ( ULTRASONIC, OUTPUT );
  digitalWrite ( ULTRASONIC, LOW ); 
  delayMicroseconds(2); 
  digitalWrite ( ULTRASONIC, HIGH ); 
  delayMicroseconds(10); 
  digitalWrite ( ULTRASONIC, LOW );
  pinMode(ULTRASONIC, INPUT);
  unsigned long duration = pulseIn (ULTRASONIC, HIGH); 
  return duration / 2 * SPEED_OF_SOUND;
}

// This section deals with the celebratory tone 
MeBuzzer buzzer; // create the buzzer object
void celebrate() {
  buzzer.tone ( 300,200 );
  buzzer.tone ( 500,200 );
  buzzer.tone ( 1020,800 );
  buzzer.noTone ();
}



// This section defines an array of dipping values depending on the distance from wall 
float dipping_array [10] = {};
// To be calibrated at >= 2 mazes 

float IR_distance ( ) {
  float distance_array [10];
  distance_array [0] = 1.0;
  for ( size_t i = 1; i < 10; i += 1 ) {
    distance_array [i] = distance_array [ i - 1 ] + 0.5 ;
  }
  // Due to ambient IR 
  float baseline_voltage = analogRead ( IR_DETECTOR );
  // Set Y0 to LOW and turn on the IR Emitter 
  analogWrite ( A0, 0 );
  analogWrite ( A1, 0 );
  delayMicroseconds (10);
  float final_voltage  = analogRead ( IR_DETECTOR );
  float dipping = baseline_voltage - final_voltage;
  for ( size_t i = 1; i < 10; i += 1 ) {
    if ( dipping > dipping_array [i] ) {
      return ( distance_array [i] + distance_array [ i - 1 ] ) / 2;
    }
  }
}

// This section deals with the color sensors 
#define RGBWait 200 //milliseconds 
#define LDRWait 10 //milliseconds

float get_avg_LDR_reading ( size_t times ) {
  float total = 0;
  for ( size_t i = 0; i < times; i += 1 ) {
    total += analogRead ( LDR );
    delay (LDRWait);
  }
return total / times;  
}
// The dimension of 3 corresponds to red, green and blue in that order
// The dimension of 2 corresponds to the value of A0 and A1
int led_array [3][2] = {
  {255, 255},
  {255, 0},
  {0, 255},
} ;

int colour_array [3];
int white_array [3] = {255, 255, 255};
int black_array [3] = {0, 0, 0};
int grey_diff [3] = {255, 255, 255};

void getcolor(){
for (int i = 0; i < 3; i += 1) { 
  // On LED    
  analogWrite(A0,led_array[i][0]);
  analogWrite(A1,led_array[i][1]);
  delay(RGBWait);
  //get the average of 5 consecutive readings for the current colour and return an average 
  colour_array[i] = get_avg_LDR_reading(5);
  colour_array[i] = ( colour_array[i] - black_array[i] ) / grey_diff[i] * 255;
  // Off LED 
  analogWrite(A0,0);
  analogWrite(A1,0);
  delay(RGBWait);
  } 
}

void setup() {

  
}
void loop () {
  
}
