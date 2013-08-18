#include <LiquidCrystal.h>
#include <Servo.h>

//LCD Hookups
#define RS 26
#define EN 27
#define D4 22
#define D5 23
#define D6 24
#define D7 25
#define LCD_BACKLIGHT A8

//Motors Hookups
#define BACK_MOTOR 12
#define FRONT_MOTOR 13
#define BACK_BRAKE 9
#define FRONT_BRAKE 8

#define SERVO_PIN 28

LiquidCrystal lcd(EN, RS, D7, D6, D5, D4);

Servo headServo;
const int pingPin = 7;
int servoAngle = 90;
int angleStep = 30;


void setup(){
  pinMode(LCD_BACKLIGHT,OUTPUT);
  digitalWrite(LCD_BACKLIGHT,HIGH);
  lcd.begin(16, 2);
  lcd.print("    WELCOME");
  lcd.setCursor(1,1);
  lcd.print(" TO  MY  CAR");
  headServo.attach(SERVO_PIN);
  pinMode(BACK_MOTOR,OUTPUT);
  pinMode(FRONT_MOTOR, OUTPUT);
  pinMode(BACK_BRAKE, OUTPUT);
  pinMode(FRONT_BRAKE, OUTPUT);  
  digitalWrite(BACK_BRAKE, LOW);
  digitalWrite(FRONT_BRAKE, HIGH);
  analogWrite(3,200);  //activate drive motor
  digitalWrite(BACK_MOTOR, HIGH);
  pinMode(10, OUTPUT);
  Serial.begin(9600);
}  //setup

void loop(){
  //turnRight();
  //delay(1000);
  //turnLeft();
  //delay(1000);
  //drive();
  long duration, inches=0;
  pinMode(pingPin, OUTPUT);
  digitalWrite(pingPin, LOW);
  delayMicroseconds(2);
  digitalWrite(pingPin, HIGH);
  delayMicroseconds(5);
  digitalWrite(pingPin, LOW);
  pinMode(pingPin, INPUT);
  duration = pulseIn(pingPin, HIGH);

  inches = microsecondsToInches(duration);
  Serial.println(inches);
  if( inches < 12 ){
    digitalWrite(BACK_BRAKE, HIGH);
    delay(100);
    turnRight();
    digitalWrite(BACK_BRAKE, LOW); 
    analogWrite(3, 200);  //activate drive motor
    digitalWrite(BACK_MOTOR, LOW);
    delay(1000);
    //not sure
    digitalWrite(BACK_BRAKE, HIGH);
    digitalWrite(FRONT_BRAKE, HIGH);
    
  }else{
    drive();
  }delay(100);
}  //loop

void updateHeadAngle()
{
  headServo.write(servoAngle);
  servoAngle += angleStep;  
  if(servoAngle >= 150)
  {
    servoAngle = 150;
    angleStep *= -1;
  }
  if(servoAngle <= 30)
  {
    servoAngle = 30; 
    angleStep *= -1;
  }
}  //updateHeadAngle

void turnLeft(){
  digitalWrite(FRONT_BRAKE, LOW);
  digitalWrite(FRONT_MOTOR, LOW);
  analogWrite(11, 255);  //activate turn motor
}
void turnRight(){
  digitalWrite(FRONT_BRAKE, LOW);
  digitalWrite(FRONT_MOTOR, HIGH);
  analogWrite(11, 255);  //activate turn motor
}
void drive(){
  digitalWrite(BACK_BRAKE, LOW); 
  analogWrite(3, 200);  //activate drive motor
  digitalWrite(BACK_MOTOR, HIGH);
}

long microsecondsToInches(long microseconds)
{
  return microseconds / 74 / 2;
}
