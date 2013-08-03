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
LiquidCrystal lcd(EN, RS, D7, D6, D5, D4);

Servo headServo;
const int pingPin = 7;
int servoAngle = 90;
int angleStep = 30;

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

  for(angle = 0; angle < 180; angle += 20){        
    headServo.write(angle);
    delay(75);
    clearSensor();
    pinMode(pingPin, INPUT);
    duration_0 = pulseIn(pingPin, HIGH);
    distance = microsecondsToInches(duration_0);
    Serial.println(distance);
    
    if( distance != 0 && distance < 6 && angle > 10 
              && angle < 80 ) RIGHT++;
              
    else if( distance != 0 && distance < OBSTECALE 
              && angle >= 80 && angle < 120) MIDDLE++;
              
    else if( distance != 0 && distance < 6 
              && angle < 170) LEFT++;
  }
  Serial.print("RIGHT:");
  Serial.print(RIGHT);
  Serial.print(" MIDDLE:");
  Serial.print(MIDDLE);
  Serial.print(" LEFT:");
  Serial.println(LEFT);
  if( RIGHT || MIDDLE || LEFT ){
   STOP(); 
   delay(250);
  }
                                  /* CHECK IF !=0 && equal */
  if( RIGHT && MIDDLE && LEFT){
    if( RIGHT < LEFT && MIDDLE < LEFT){
     //GO LEFT 
    }else if( MIDDLE < RIGHT && LEFT < RIGHT){
     //GO RIGHT 
    }else{
      //GO BACKWARD AND REVERSE
    }
  }else if( RIGHT && MIDDLE ){
   if( RIGHT < MIDDLE ){
    //GO STRAIGHT
   }else{
    //GO RIGHT
   }
  }else if( LEFT && MIDDLE ){
    if( LEFT < MIDDLE ){
     //GO STRAIGHT 
    }else{
     //GO LEFT 
    }
  }else if( LEFT && RIGHT ){
   if( LEFT < RIGHT ){
    //GO RIGHT
    }else{
     //GO LEFT 
    }
  }else if( RIGHT ){
    //GO LEFT
  }else if( MIDDLE ){
   //GO EITHER WAY
  }else if( LEFT ){
   //GO RIGHT 
  }else{
   //GO FORAWRD
  }
  
  
  
  
    /*if( angle == 30 ){
      delay(5);
      clearSensor();
      pinMode(pingPin, INPUT);
      duration_0 = pulseIn(pingPin, HIGH);
      RIGHT = microsecondsToInches(duration_0);
      a[0]=RIGHT;
      Serial.print("RIGHT: ");
      Serial.print(RIGHT);
      Serial.print(", LEFT: ");
      //clearSensor();
    }else if( angle == 120 ){
      delay(5);
      clearSensor();
      pinMode(pingPin, INPUT);
      duration_90 = pulseIn(pingPin, HIGH);
      LEFT = microsecondsToInches(duration_90);
      a[1]=LEFT;
      Serial.print(LEFT);
      Serial.println(" ");
      
    }*/

  
  /*
   for(angle = 60; angle < 180; angle += 5){        
    headServo.write(angle);
    
    delay(15);
   }*/
    /*else if( angle == 140){
      delay(5);
      clearSensor();
      pinMode(pingPin, INPUT);
      duration_180 = pulseIn(pingPin, HIGH);
      inches_180 = microsecondsToInches(duration_180);
      a[2]=inches_180;
      Serial.print(inches_180);
      //delay(50);
    }
    delay(50);
  }
  Serial.print(" | SORT: ");
  sort(a,3);
  for(unsigned i=0; i<3; i++){
   Serial.print(a[i]); 
   Serial.print(" ");
  }
  Serial.println();*/
  /*
  //lcd.clear();
  //get inches for both angles
  Serial.print(RIGHT);
  Serial.print(", ");
  Serial.println(inches_180);
  lcd.clear();
  lcd.print("180: ");
  lcd.setCursor(4,0);
  lcd.print(inches_180);
  lcd.setCursor(0,1);
  lcd.print("0: ");
  lcd.setCursor(2,1);
  lcd.print(RIGHT);
  delay(500);
  //180 -> far left
  //0 -> far right
*/
/*
  if( (LEFT < 12) || (RIGHT < 12) ){
  	//check which way to go
  	digitalWrite(BACK_BRAKE, HIGH);
  	if(  ((LEFT < 12) && (RIGHT < 12))
             //  && ((LEFT < 12) || (RIGHT < 12)) )
               &&( LEFT < RIGHT ) ){
                Serial.println(" = > 180<0 ");
  		delay(75);
  		//reverse & go right
  		turnLeft();  //put the tire in left dir
  		//go backward
  		goBackward(750);
  		//stop
  		digitalWrite(BACK_BRAKE, HIGH);  //stop
  		digitalWrite(FRONT_BRAKE, HIGH);  //straight
  	}    /*90: 7, 0: 23 

             = >drive
             

  	else if(  ((LEFT < 12) && (RIGHT < 12))
              // && ((LEFT < 12) || (RIGHT < 12)) )
                   && (RIGHT < LEFT) ){
                Serial.println(" = > 0<180");
  		delay(75);
  		//reverse and go left
  		turnRight();  //put the tire in right dir
  		//go backward
  		goBackward(750);
  		//stop
  		digitalWrite(BACK_BRAKE, HIGH);  //stop
  		digitalWrite(FRONT_BRAKE, HIGH);  //straight
  	}
  	else if(  ((LEFT < 12) && (RIGHT < 12))
             //  && ((LEFT < 12) || (RIGHT < 12)) )
                   && (LEFT == RIGHT) ){
  			//* GOING STRAIGHT BACK 
                Serial.println(" = > == ");
  		delay(75);
  		goBackward(1000);
  		digitalWrite(BACK_BRAKE, HIGH);  //stop
  		digitalWrite(FRONT_BRAKE, HIGH);  //straight
  	}else{
      	//go FORWARD
            Serial.println(" = >drive ");
      	    goFORWARD();
        }	//else
  }	//if
  
  delay(500);*/
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
  analogWrite(3, 200);  //activate drive motor
  digitalWrite(BACK_MOTOR, HIGH);
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
  
  digitalWrite(BACK_BRAKE, LOW);
  digitalWrite(FRONT_BRAKE, HIGH);
  analogWrite(3,200);  //activate drive motor
  digitalWrite(BACK_MOTOR, HIGH);
}
