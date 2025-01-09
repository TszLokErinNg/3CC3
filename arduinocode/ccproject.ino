#include <Servo.h>
#include <LiquidCrystal.h>

const int PWM_PIN = 11; // PWM pin
const int DIR_PIN1 = 9;  // Direction pin
const int DIR_PIN2 = 10;  // Direction pin

//Servo motors
Servo myServo1;
Servo myServo2;
//ServoPin
const int servoPin1 = 13; 
const int servoPin2 = 12;

//IR Sensor
const int irSensorPin1 = 3; // Pin connected to the IR sensor    
const int irSensorPin2 = 2;
const int irSensorPin3 = 24;  //digital


//Color sensor
const int S0 = 4;
const int S1 = 5;
const int S2 = 6;
const int S3 = 7;
const int Out = 8;
// Speed value (0-255 for PWM)
int motorSpeed = 255;

//LCD display pins 
LiquidCrystal lcd(49, 47, 45, 43, 41, 39);
int r = 0;
int g = 0;
int b = 0;

void setup() {
  //Motor
    pinMode(PWM_PIN, OUTPUT);
    pinMode(DIR_PIN1, OUTPUT);
    pinMode(DIR_PIN2, OUTPUT);

  //Color sensor
  pinMode(S0,OUTPUT); 
  pinMode(S1,OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(Out, INPUT);
  digitalWrite(S0, HIGH);
  digitalWrite(S1, LOW);

  //servo motor
  myServo1.attach(servoPin1);  // Attach the servo to the designated pin
  myServo2.attach(servoPin2);  
  myServo1.write(270); // Start with the servo at 0 degrees
  myServo2.write(0); 


  //LCD 16x2 display
  lcd.begin(16, 2);
  lcd.print("Color Sorting");
  lcd.setCursor(0,1);
  lcd.print("Project!");
  delay(4000);
  lcd.clear();
  
  Serial.begin(9600); // Initialize serial communication for debugging
  

}


  
void loop() {

 int red = 0;
 int green = 0;
 int blue = 0;

 // Thresholds for color detection
const int redThreshold = 200;  // Adjust based on calibration
const int greenThreshold = 200;
const int blueThreshold = 200;
 //IR Sensor States

  int irSensorState1 = digitalRead(irSensorPin1); // Read the state of the IR sensor
  int irSensorState2 = digitalRead(irSensorPin2);
  int irSensorState3 = digitalRead(irSensorPin3); //digital


  
   

 if(irSensorState3 == HIGH){

   stopMotor();//Motor stop

   while (digitalRead(irSensorPin3) == HIGH);
 delay(2000);
   
  int rawRed = readColor(LOW, LOW);    // Raw Red
  int rawGreen = readColor(HIGH, HIGH); // Raw Green
  int rawBlue = readColor(LOW, HIGH);  // Raw Blue

   //Color sensor read
 //Setting Red filtered photodiodes to be read
 digitalWrite(S2,LOW);
 digitalWrite(S3,LOW);
 red = pulseIn(Out,LOW); //reading output frequency
 red = map(red,110,150,255,0); //Remap
 

 //Setting green filtered photodiodes to be read
 digitalWrite(S2,HIGH);
 digitalWrite(S3,HIGH);
 green = pulseIn(Out,LOW); //reading output frequency
 green = map(green,110,150,255,0); //Remap
 

 //Setting blue filtered photodiodes to be read
 digitalWrite(S2,LOW);
 digitalWrite(S3,HIGH);
 blue = pulseIn(Out,LOW); //reading output frequency
 blue = map(blue,110,150,255,0); //Remap*/

 //color display
 Serial.print("R= "); 
 Serial.print(red);
 Serial.print("  ");
 Serial.print("G= ");
 Serial.print(green);
 Serial.print("  ");
 Serial.print("B= ");
 Serial.println(blue);
 

 /*//LCD Display
 lcd.setCursor(0, 0);
 lcd.print("R= "); 
 lcd.print(red);
 lcd.print("  ");
 lcd.print("G= ");
 lcd.print(green);
 lcd.print("  ");
 lcd.print("B= ");
 lcd.println(blue);*/
  
  if(blue > red && blue > green){
    b++;
  lcd.setCursor(0, 1);
    lcd.print("Blue Color");

 } else if(red > blue && red > green){
  r++;

    lcd.setCursor(0, 1);
    lcd.print("Red Color");
 } else if (green > red && green > blue){
  g++;
 
    lcd.setCursor(0, 1);
    lcd.print("Green Color");
 }
    ColorCount(r,g,b);

 } 
   forward();//stop
  delay(1000);
  stopMotor();//go
  delay(2000);
  


  
    //blue s1
  if( blue > red && blue > green){
   while (digitalRead(irSensorPin1) == HIGH);
  
  forward();//Motor stop
  delay(2000);
  //myServo1.write(0); // Turn the servo to 180 degrees
  for (int angle = 270; angle >= 0; angle--) {
    myServo1.write(angle);  // Set the servo to the current angle
    delay(20);             // Wait 20 milliseconds (adjust to control speed)
  }
   delay(1000);
   myServo1.write(270); // Turn the servo back to 0 degrees
    delay(500);
  } 
  //continue 
  stopMotor();//go
  delay(100);
  

  //red s2
    if(red > blue && red > green){
  while (digitalRead(irSensorPin2) == HIGH);
    forward();//Motor stop
    delay(2000);
    //myServo2.write(180); // Turn the servo to 180 degrees
    for (int angle = 0; angle <= 180; angle++) {
    myServo2.write(angle);  // Set the servo to the current angle
    delay(20);             // Wait 20 milliseconds (adjust to control speed)
  }
   delay(1000);
   myServo2.write(0); // Turn the servo back to 0 degrees
    delay(1000);
  }

 
  //forward();//stop
  //delay(1000);
  stopMotor();//go
  delay(100);


}

// Function to read color
int readColor(bool s2State, bool s3State) {
  // Set S2 and S3 for the desired color
  digitalWrite(S2, s2State);
  digitalWrite(S3, s3State);

  // Measure frequency output from TCS3200
  int frequency = pulseIn(Out, LOW);

  // Return the frequency value
  return frequency;
}

//Motor functions(requires an H-bridge L298D Motor driver)
void stopMotor() {
 digitalWrite(DIR_PIN1, HIGH); // Set direction to forward
 digitalWrite(DIR_PIN2, LOW);
 analogWrite(PWM_PIN, motorSpeed); // Set speed
}

/*void reverse() {
 digitalWrite(DIR_PIN1, LOW); // Set direction to reverse
 digitalWrite(DIR_PIN2, HIGH);
 analogWrite(PWM_PIN, motorSpeed); // Set speed
}*/

void forward() {
 digitalWrite(DIR_PIN1, LOW);
 digitalWrite(DIR_PIN2, LOW);
 analogWrite(PWM_PIN, 0); // Ensure speed is zero
}

// Function to display message on LCD
void ColorCount(int rcount, int gcount, int bcount) {
  
  lcd.setCursor(0, 0);
  lcd.print("R= ");
  lcd.print(rcount);
  lcd.print(" ");
  lcd.print("G= ");
  lcd.print(gcount);
  lcd.print(" ");
  lcd.print("B= ");
  lcd.print(bcount);
  lcd.print(" ");

}
