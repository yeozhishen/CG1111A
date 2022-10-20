#include "MeMCore.h"
#define TURNING_TIME_MS 330 // The time duration (ms) for turning
MeDCMotor leftMotor(M1); // assigning leftMotor to port M1
MeDCMotor rightMotor(M2); // assigning RightMotor to port M2
uint8_t motorSpeed = 255;
// Setting motor speed to an integer between 1 and 255
// The larger the number, the faster the speed

// Define time delay before the next RGB colour turns ON to allow LDR to stabilize
#define RGBWait 200 //in milliseconds 
// Define time delay before taking another LDR reading
#define LDRWait 10 //in milliseconds 
#define LDR A3   //LDR sensor pin at A3
#define LED 13  //Check Indicator to signal Calibration Completed
//pins for led in r,g,b
int led_array[3][2]= {{255,255},{255,0},{0,255}}
//placeholders for colour detected
int red = 0;
int green = 0;
int blue = 0;
//floats to hold colour arrays
float colourArray[] = {0,0,0};
float whiteArray[] = {0,0,0};
float blackArray[] = {0,0,0};
float greyDiff[] = {0,0,0};
char colourStr[3][5] = {"R = ", "G = ", "B = "};

int getAvgReading(int times){      
//find the average reading for the requested number of times of scanning LDR
  int reading;
  int total =0;
//take the reading as many times as requested and add them up
  for(int i = 0;i < times;i++){
     reading = analogRead(LDR);
     total = reading + total;
     delay(LDRWait);
  }
//calculate the average and return it
  return total/times;
}

void Calibrate(){
//set white balance
  Serial.println("Put White Sample For Calibration ...");
  delay(5000);           //delay for five seconds for getting sample ready
  digitalWrite(LED,LOW); //Check Indicator OFF during Calibration
//scan the white sample.
//go through one colour at a time, set the maximum reading for each colour -- red, green and blue to the white array
  for(int i = 0;i<=2;i++){
     analogWrite(A0,led_array[i][0]);
     analogWrite(A1,led_array[i][1]);
     delay(RGBWait);
     whiteArray[i] = getAvgReading(5);         //scan 5 times and return the average, 
     analogWrite(A0,0);
     analogWrite(A1,0);
     delay(RGBWait);
  }
//done scanning white, time for the black sample.
//set black balance
  Serial.println("Put Black Sample For Calibration ...");
  delay(5000);     //delay for five seconds for getting sample ready 
//go through one colour at a time, set the minimum reading for red, green and blue to the black array
  for(int i = 0;i<=2;i++){
     analogWrite(A0,led_array[i][0]);
     analogWrite(A1,led_array[i][1]);
     delay(RGBWait);
     blackArray[i] = getAvgReading(5);
     analogWrite(A0,0);
     analogWrite(A1,0);
     delay(RGBWait);
//the differnce between the maximum and the minimum gives the range
     greyDiff[i] = whiteArray[i] - blackArray[i];
  }

void getcolor(){
for(int c = 0;c<=2;c++){    
    Serial.print(colourStr[c]);
    analogWrite(A0,led_array[i][1]);//on led
    analogWrite(A1,led_array[i][2]);
    delay(RGBWait);
//get the average of 5 consecutive readings for the current colour and return an average 
    colourArray[c] = getAvgReading(5);
//the average reading returned minus the lowest value divided by the maximum possible range, multiplied by 255 will give a value between 0-255, representing the value for the current reflectivity (i.e. the colour LDR is exposed to)
    colourArray[c] = (colourArray[c] - blackArray[c])/(greyDiff[c])*255;
    analogWrite(A0,0);//off led
    analogWrite(A1,0);
    delay(RGBWait);
    Serial.println(int(colourArray[c])); //show the value for the current colour LED, which corresponds to either the R, G or B of the RGB code
  } 
}

void setup()
{
// Any setup code here runs only once:
pinMode(A0,OUTPUT);
pinMode(A1,OUTPUT);
pinMode(A3,INPUT);
pinMode(A2,INPUT);
Serial.begin(9600);
}
void loop()
{
  //leftMotor.run(200);
  //rightMotor.run(200);
  analogWrite(A1,0);
  analogWrite(A0,255);
}
