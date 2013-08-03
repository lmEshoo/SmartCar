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
#define OBSTECALE 12
#define COMMENT 

LiquidCrystal lcd(EN, RS, D7, D6, D5, D4);

Servo headServo;
const int pingPin = 7;
unsigned angle =0;

void setup(){
  lcdSetup();
  headServo.attach(SERVO_PIN);
  carSetupAndStart();

  Serial.begin(9600);
}  //setup

void loop(){

  //* CHECKING RIGHT | LEFT | MIDDLE 
  long duration_180, duration_0,
      inches_180, RIGHT=0, duration_90,
      LEFT=0, distance=0, MIDDLE=0;

  for(angle = 10; angle < 180; angle += 20){        
    headServo.write(angle);
    delay(65);
    clearSensor();
    pinMode(pingPin, INPUT);
    duration_0 = pulseIn(pingPin, HIGH);
    distance = microsecondsToInches(duration_0);
    Serial.println(distance);
    
    if( distance != 0 && distance < OBSTECALE && angle > 10 
              && angle < 80 ) RIGHT++;
              
    else if( distance != 0 && distance < OBSTECALE 
              && angle >= 80 && angle < 120) MIDDLE++;
              
    else if( distance != 0 && distance < OBSTECALE 
              && angle < 170) LEFT++;
    if( RIGHT || MIDDLE || LEFT ){
     Serial.print("A ");
     STOP(); 
     delay(150);
    }
  }
  /*TESTING*/
  Serial.print("RIGHT:");
  Serial.print(RIGHT);
  Serial.print(" MIDDLE:");
  Serial.print(MIDDLE);
  Serial.print(" LEFT:");
  Serial.println(LEFT);
  
  
              /* CHECK IF !=0 && equal */
  if( RIGHT && MIDDLE && LEFT){
      Serial.print("1");
      if( RIGHT < LEFT && RIGHT < MIDDLE){
       Serial.print("a");
       //GO RIGHT 
       reverseAndGo_RIGHT();
      }else if( LEFT < RIGHT && LEFT < MIDDLE ){
        Serial.print("b");
       //GO LEFT 
       reverseAndGo_LEFT();
      }else{
        Serial.print("c");
        //GO BACKWARD AND REVERSE  RIGHT!!!
       // RIGHT:2 MIDDLE:2 LEFT:3
       //   A1c
       //RIGHT:2 MIDDLE:1 LEFT:1
       //A1c
        goBackward(200);
        reverseAndGo_LEFT();
      }
  }else if( RIGHT && MIDDLE ){
        Serial.print("2");
     if( RIGHT < MIDDLE ){
       Serial.print("d");
      //GO RIGHT
      reverseAndGo_RIGHT();
     }else{  //includes equal
     Serial.print("e");
      //GO LEFT
      reverseAndGo_LEFT();
     }
    }else if( LEFT && MIDDLE ){
          Serial.print("3");
      if( LEFT < MIDDLE ){
        Serial.print("f");
       //GO LEFT
       reverseAndGo_LEFT();
      }else{  //includes equal
        Serial.print("g");
       //GO RIGHT
       reverseAndGo_RIGHT();
      }
  }else if( LEFT && RIGHT ){
      Serial.print("4");
     if( LEFT < RIGHT ){
       Serial.print("h");
        //GO LEFT
        reverseAndGo_LEFT();
      }else{  //includes equal
        Serial.print("i");
       //GO RIGHT 
         reverseAndGo_RIGHT();
      }
  }else if( RIGHT ){
      Serial.print("5");
      //GO LEFT
      reverseAndGo_LEFT();
  }else if( MIDDLE ){
      Serial.print("6");
     //GO EITHER WAY  ????
     reverseAndGo_RIGHT();
  }else if( LEFT ){
     Serial.print("7");
     //GO RIGHT 
   reverseAndGo_RIGHT();
  }else{
    Serial.print("8");
     //GO FORAWRD
   goFORWARD();
  }
      Serial.println();
      Serial.println();
}  //loop

void clearSensor(){
  pinMode(pingPin, OUTPUT);
  digitalWrite(pingPin, LOW);
  delayMicroseconds(2);
  digitalWrite(pingPin, HIGH);
  delayMicroseconds(5);
  digitalWrite(pingPin, LOW);
}  //clearSensor

void goBackward( int sec ){
  digitalWrite(BACK_BRAKE, LOW); 
  analogWrite(3, 200);  //activate drive motor
  digitalWrite(BACK_MOTOR, LOW);
  delay(sec); 
}
void turnLeft(){
  digitalWrite(FRONT_BRAKE, LOW);
  digitalWrite(FRONT_MOTOR, LOW);
  analogWrite(11, 255);  //activate turn motor
}
void turnRight(){
  digitalWrite(FRONT_BRAKE, LOW);
  digitalWrite(FRONT_MOTOR, HIGH);
  analogWrite(11, 255);  //activate turn motor
}  //turnRight()
void goFORWARD(){
  digitalWrite(BACK_BRAKE, LOW);
  digitalWrite(FRONT_BRAKE, HIGH);
  analogWrite(3,200);  //activate drive motor
  digitalWrite(BACK_MOTOR, HIGH);
}
void reverseAndGo_LEFT(){
 delay(75);
 turnRight();
 goBackward(750);
 STOP(); 
}
void reverseAndGo_RIGHT(){
 delay(75);
 turnLeft();
 goBackward(750);
 STOP();
}
void STOP(){
  digitalWrite(BACK_BRAKE, HIGH);
  digitalWrite(FRONT_BRAKE, HIGH);  //straight
}

long microsecondsToInches(long microseconds)
{
  return microseconds / 74 / 2;
}

void lcdSetup(){
  pinMode(LCD_BACKLIGHT,OUTPUT);
  digitalWrite(LCD_BACKLIGHT,HIGH);
  lcd.begin(16, 2);
  lcd.print("    WELCOME");
  lcd.setCursor(1,1);
  lcd.print("TO LENNY's CAR");
}  

void carSetupAndStart(){
  pinMode(BACK_MOTOR,OUTPUT);
  pinMode(FRONT_MOTOR, OUTPUT);
  pinMode(BACK_BRAKE, OUTPUT);
  pinMode(FRONT_BRAKE, OUTPUT); 
  
  goFORWARD();
}
