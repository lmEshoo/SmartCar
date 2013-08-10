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
#define OBSTECALE 15
#define COMMENT 

LiquidCrystal lcd(EN, RS, D7, D6, D5, D4);

Servo headServo;
const int pingPin = 7;
unsigned angle =0;
int counterVal=0;
long duration_0, distance;

boolean fLeft = false;
boolean fRight = false;
//boolean beenHere=false;

void setup(){
  lcdSetup();
  headServo.attach(SERVO_PIN);
  carSetupAndStart();

  Serial.begin(9600);
}  //setup

void loop(){

  //* CHECKING RIGHT | LEFT | MIDDLE 
  long RIGHT=0, LEFT=0, distance=0,
    MIDDLE=0, FAR_RIGHT=0, FAR_LEFT=0;
  int count=0;
  for(angle = 0; angle <= 180; angle += 15){        
    headServo.write(angle);
    delay(65);
    clearSensor();
    pinMode(pingPin, INPUT);
    duration_0 = pulseIn(pingPin, HIGH);
    distance = microsecondsToInches(duration_0);
    
    if( distance != 0 && distance < 20
        && count==0 && angle >= 30 && angle < 150 )
        {
           STOP();
           fLeft=false;
           fRight=false;
           //beenHere=false;
           Serial.print("A ");
           delay(25);
           count++;
        }//if
    Serial.println(distance);
    
    if(distance != 0 && distance < 10  
             && angle >= 20 && angle < 30 )
             {
               FAR_RIGHT++;
               //beenHere=false;
               Serial.print(" ->");
               Serial.println(angle);
             }

    else if( distance != 0 && distance < OBSTECALE 
              && angle > 30 && angle < 60 )
             { 
               Serial.print(" ->");
               Serial.println(angle);
               RIGHT++;
             }

    else if( distance != 0 && distance < 25 
              && angle >= 60 && angle < 120)
             {
               Serial.print(" ->");
               Serial.println(angle);
               MIDDLE++;
             }

    else if( distance != 0 && distance < OBSTECALE 
             && angle >=120 && angle < 150)
             {
               Serial.print(" ->");
               Serial.println(angle);
               LEFT++;
             }

    else if( distance != 0 && distance < 10 
             && angle >= 150 )
             {
               Serial.print(" ->");
               //beenHere=false;
               Serial.println(angle);
               FAR_LEFT++;
             }
  }  //for 
  //headServo.write(10);
  //delay(5);
  /*TESTING*/
  Serial.print("FAR_RIGHT:");
  Serial.print(FAR_RIGHT);
  Serial.print(" RIGHT:");
  Serial.print(RIGHT);
  Serial.print(" MIDDLE:");
  Serial.print(MIDDLE);
  Serial.print(" LEFT:");
  Serial.print(LEFT);
  Serial.print(" FAR_LEFT:");
  Serial.println(FAR_LEFT);

              /* CHECK IF !=0 && equal 
                 RIGHT MIDDLE AND LEFT */
  if( RIGHT && MIDDLE && LEFT){
      Serial.print("1");
      if( RIGHT < LEFT && RIGHT < MIDDLE){
       Serial.print("a");
       //GO RIGHT 
       reverseAndGo_RIGHT(1500);
      }else if( LEFT < RIGHT && LEFT < MIDDLE ){
        Serial.print("b");
       //GO LEFT 
       reverseAndGo_LEFT(1500);
       /* CHECK THESE STATMENTS*/
      }else if( RIGHT == MIDDLE && RIGHT < LEFT ){
        //GO RIGHT
        reverseAndGo_RIGHT(2000);
        Serial.print("N");
      }else if( MIDDLE == LEFT && MIDDLE < RIGHT ){
        //GO LEFT
        reverseAndGo_LEFT(2000);
        Serial.print("M");
      }else{
        Serial.print("c");
        //GO BACKWARD AND REVERSE  RIGHT!!!
       // RIGHT:2 MIDDLE:2 LEFT:3
       //   A1c
       //RIGHT:2 MIDDLE:1 LEFT:1
       //A1c
        goBackward(500);
        reverseAndGo_LEFT(2000);
      }
  }else if( RIGHT && MIDDLE ){
        Serial.print("2");
     if( RIGHT < MIDDLE ){
       Serial.print("d");
      //GO RIGHT
      reverseAndGo_RIGHT(1500);
     }else{  //includes equal
     Serial.print("e");
      //GO LEFT
      reverseAndGo_LEFT(1500);
     }
    }else if( LEFT && MIDDLE ){
          Serial.print("3");
      if( LEFT < MIDDLE ){
        Serial.print("f");
       //GO LEFT
       reverseAndGo_LEFT(1500);
      }else{  //includes equal
        Serial.print("g");
       //GO RIGHT
       reverseAndGo_RIGHT(1500);
      }
  }else if( LEFT && RIGHT ){
      Serial.print("4");
     if( LEFT < RIGHT ){
       Serial.print("h");
        //GO LEFT
        reverseAndGo_LEFT(1500);
      }else{  //includes equal
        Serial.print("i");
       //GO RIGHT 
         reverseAndGo_RIGHT(1500);
      }
  }else if( RIGHT ){
      Serial.print("5");
      //GO LEFT
      reverseAndGo_LEFT(1500);
  }else if( MIDDLE ){
      Serial.print("6");
     //GO EITHER WAY  ????
     reverseAndGo_RIGHT(1500);
  }else if( LEFT ){
     Serial.print("7");
     //GO RIGHT 
   reverseAndGo_RIGHT(1500);
  }else if( FAR_RIGHT || FAR_LEFT ){
    Serial.print("8");
    if( FAR_RIGHT < FAR_LEFT ){
      Serial.print("j");
      if(fLeft==true){
        Serial.print("z");
        reverseAndGo_RIGHT(1000);
      }else{
        Serial.print("l");
          fLeft=true;
          //merge right
          turnRight();
          delay(500);
          goFORWARD();
      }
    }else{
      Serial.print("k");
      if(fRight==true){
        reverseAndGo_LEFT(1000);
      }else{
        Serial.print("y");
         fRight=true;
         //merge left 
         turnLeft();
         delay(500);
         goFORWARD();
      }
    }
  }else{
    Serial.print("9");
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
  check90();
}
          /*CHECK THIS AGAIN*/
void check90(){
  for(angle = 70; angle >= 130; angle += 15){        
    headServo.write(angle);
    delay(65);
    clearSensor();
    pinMode(pingPin, INPUT);
    duration_0 = pulseIn(pingPin, HIGH);
    distance = microsecondsToInches(duration_0);
    if( distance != 0 && angle > 75 && angle < 125
          && distance < 35 )
    {
      STOP();
      Serial.println("    < 20");
      //beenHere=true;
    }
    else goFORWARD();
  }/*
  headServo.write(110);
  delay(5);
  long duration_0, distance;
  delay(25);
  clearSensor();
  pinMode(pingPin, INPUT);
  duration_0 = pulseIn(pingPin, HIGH);
  distance = microsecondsToInches(duration_0);
  Serial.print(" check90: ");
  Serial.println(distance);
  if( distance != 0 && distance < 20 ){
    STOP();
    Serial.println("    < 20");
    beenHere=true;
  }else if(beenHere){
    goBackward(500);
    reverseAndGo_LEFT(2000);
  }else goFORWARD();*/
}
void reverseAndGo_LEFT(int sec){
 delay(75);
 turnRight();
 goBackward(sec);
 STOP(); 
}
void reverseAndGo_RIGHT(int sec){
 delay(75);
 turnLeft();
 goBackward(sec);
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
  counterVal++;
  goFORWARD();
}
