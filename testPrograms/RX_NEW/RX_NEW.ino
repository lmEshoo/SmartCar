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

//lights 30,31,32,33,34,35
#define LED_RED_1 34
#define LED_RED_2 32
#define LED_RED_3 30
#define LED_GRE_1 35
#define LED_GRE_2 33
#define LED_GRE_3 31

#define SERVO_PIN 28
#define OBSTECALE 12
#define COMMENT 

LiquidCrystal lcd(EN, RS, D7, D6, D5, D4);

Servo headServo;
const int pingPin = 7;
unsigned angle =0;
int counterVal=0;
long duration_0, distance;

boolean fLeft = false;
boolean fRight = false;
boolean forWard = false;
boolean forwardAgain = false;
//boolean beenHere=false;

int data_UD, data_LR;

void setup(){
   Serial1.begin(9600);
   Serial.begin(9600);
   pinMode(34,OUTPUT);
   pinMode(35,OUTPUT);
   pinMode(31,OUTPUT);
   pinMode(32,OUTPUT);
   lcdSetup();
   headServo.attach(SERVO_PIN);
   carSetupAndStart(); 

   delay(500);
}
void loop(){
  int Ucounter=-1, Lcounter=-1;
  boolean UU=false, UUTR=false,
          LL=false, LLTR=false,
          driveAlone=false,
          Udrive=false;
  char Umsg[4];
  char Lmsg[4];
  delay(250);
  while( Serial1.available() > 0){
    digitalWrite(34,HIGH);
    char inChar = Serial1.read();
    //Serial.println(inChar);
    if( inChar == 'B' ){
      Udrive=false;
      digitalWrite(32,HIGH);
      digitalWrite(35,LOW);
      digitalWrite(31,LOW);
      Serial.println("DRIVE_ALONE");
      driveAlone=true;
      
    }
    if( inChar == 'U'){
      Udrive=true;
      driveAlone=false;
      digitalWrite(35,HIGH);
      Ucounter++;
      UU=true;
    }
    if(UU){
      Umsg[Ucounter] = inChar;
      Ucounter++;
    }
    if( Ucounter==3) UU=false;    
    
    if(LL){
      Lmsg[Lcounter] = inChar;
      Lcounter++;
    }
    if( Lcounter==3) LL=false;
    if( inChar == 'L'){
      Udrive=true;
      driveAlone=false;
      digitalWrite(31,HIGH);
      Lcounter++;
      LL=true;
    }
    
    //else digitalWrite(31,LOW);
    //Serial.println(inChar);
  }
  delay(150);
  if(driveAlone){
    Serial.println("DRIVE_ALONE_ON!");
    selfDrive();
  }
  /////////////////////////////////////////////
  //                YOU DRIVE               //
  ////////////////////////////////////////////
  else if(Udrive){
    Serial.println("YOU_ON!");
    int OUT_UD = atoi(Umsg);  //129 | MIN:0 | MAX:255
    int OUT_LR = atoi(Lmsg);  //128 | MIN:0 | MAX:255
    for(unsigned i=0; i <3 ; i++){
     Serial.print(Umsg[i]);
     //UD[i]=Umsg[i] - '0';
     //Serial.print(" UD: ");
     //Serial.print(UD[i]);
    }
    Serial.println(OUT_UD);
    int LR = map(OUT_LR, 100, 355, 0, 255);
    int UD = map(OUT_UD, 360, 615, 0, 255);
    LR = constrain( LR, 0, 255);
    Serial.print(UD);
    Serial.print("jhkjh ");
    Serial.println(LR);
    
    if( (UD == 129 && LR == 128) || (UD < 0 && LR < 0)
        || UD < 0){
      Serial.println("STOP1");
      digitalWrite(BACK_BRAKE, HIGH);
      digitalWrite(FRONT_BRAKE, HIGH);
    }
    else if( UD >= 129 ){
      //GO_STRAIGHT
      if( LR > 128 && LR < 255){
       //GO RIGHT 
       Serial.println("GORIGHT");
       digitalWrite(FRONT_BRAKE, LOW);
       digitalWrite(FRONT_MOTOR, HIGH);
       analogWrite(11, LR);
       digitalWrite(BACK_BRAKE, LOW);
       digitalWrite(BACK_MOTOR, HIGH);
       analogWrite(3,UD);
      }else if( LR < 128 ){
       //GO LEFT 
       Serial.println("GOLEFT");
       digitalWrite(FRONT_BRAKE, LOW);
       digitalWrite(FRONT_MOTOR, LOW);
       analogWrite(11, 255-LR);
       digitalWrite(BACK_BRAKE, LOW);
       digitalWrite(BACK_MOTOR, HIGH);
       analogWrite(3,UD);
      }else{
        // GO STRAIGHT
        Serial.println("GOSTRAIGHT");
        digitalWrite(BACK_BRAKE, LOW);
        digitalWrite(FRONT_BRAKE, HIGH);
        digitalWrite(BACK_MOTOR, HIGH);
        analogWrite(3,UD);
      }
    }else if( UD < 128){
     //REVERSE 
       if( LR > 128 ){
         //reverse & TURN WHELE RIGHT
         Serial.println("GOREVERSE& RIGHT");
         digitalWrite(FRONT_BRAKE, LOW);
         digitalWrite(FRONT_MOTOR, HIGH);
         analogWrite(11, LR);
         digitalWrite(BACK_BRAKE, LOW); 
         analogWrite(3, 255-UD);
         digitalWrite(BACK_MOTOR, LOW);
       }else if( LR < 128){
         //reverse & TURN WHELE LEFT
         Serial.println("GOREVERSE&LEFT");
         digitalWrite(FRONT_BRAKE, LOW);
         digitalWrite(FRONT_MOTOR, LOW);
         analogWrite(11, 255-LR);
         digitalWrite(BACK_BRAKE, LOW); 
         analogWrite(3, 255-UD);
         digitalWrite(BACK_MOTOR, LOW);
       }else{
         //GO REVERSE
         Serial.println("GOREVERSE");
         digitalWrite(BACK_BRAKE, LOW); 
         analogWrite(3, 255-UD);
         digitalWrite(BACK_MOTOR, LOW);
       }
    }
    else{
     //STOP 
     Serial.println("STOP");
     digitalWrite(BACK_BRAKE, HIGH);
     digitalWrite(FRONT_BRAKE, HIGH);
    }
    
  }//else
}//loop
void selfDrive(){
  //* CHECKING RIGHT | LEFT | MIDDLE 
  long RIGHT=0, LEFT=0, distance=0,
    MIDDLE=0, FAR_RIGHT=0, FAR_LEFT=0;
  int count=0;
  for(angle = 5; angle <= 175; angle += 15){        
    headServo.write(angle);
    //Serial.println();
    //Serial.print("ANGLE: ");
    //Serial.println(angle);
    delay(100);
    clearSensor();
    pinMode(pingPin, INPUT);
    duration_0 = pulseIn(pingPin, HIGH);
    distance = microsecondsToInches(duration_0);
    //if go forward is up do V
    if(distance != 0 && distance < 15
         && ( (angle >= 30 && angle < 75) ) )
        {
          STOP();
          forWard=false;
          fLeft=false;
          fRight=false;
          //beenHere=false;
          //Serial.print("A ");
        }
    else if( distance != 0 && angle > 75 && angle < 125
        && distance < 30 )//checking middle
    {
       STOP();
       //Serial.print("M_ ");
    }
    else if( distance != 0 && distance < 15 
         &&   (angle > 125 && angle < 150) ) 
        {
           STOP();
           forWard=false;
           fLeft=false;
           fRight=false;
           //beenHere=false;
           //Serial.print("A_11 ");
        }//if
    
    
    //Serial.println(distance);
    
    if(distance != 0 && distance < 10  
             && angle >= 20 && angle < 30 )
             {
               FAR_RIGHT++;
               //beenHere=false;
               //Serial.print(" ->");
               //Serial.println(angle);
             }

    else if( distance != 0 && distance < OBSTECALE 
              && angle > 30 && angle < 60 )
             { 
               //Serial.print(" ->");
               //Serial.println(angle);
               RIGHT++;
             }

    else if( distance != 0 && distance < 25 
              && angle >= 60 && angle < 120)
             {
               //Serial.print(" ->");
               //Serial.println(angle);
               MIDDLE++;
             }

    else if( distance != 0 && distance < OBSTECALE 
             && angle >=120 && angle < 150)
             {
               //Serial.print(" ->");
               //Serial.println(angle);
               LEFT++;
             }

    else if( distance != 0 && distance < 10 
             && angle >= 150 )
             {
               //Serial.print(" ->");
               //beenHere=false;
               //Serial.println(angle);
               FAR_LEFT++;
             }
  }  //for 
  //going backwards_servo
  for(angle=175; angle>=10 ; angle-=10){
    headServo.write(angle);
    delay(35);
    clearSensor();
    pinMode(pingPin, INPUT);
    duration_0 = pulseIn(pingPin, HIGH);
    distance = microsecondsToInches(duration_0);
    if(distance != 0 && distance < 15
         && ( (angle >= 30 && angle < 75) ) )
        {
          STOP();
          forWard=false;
          fLeft=false;
          fRight=false;
          //beenHere=false;
          //Serial.print("A_2 ");
        }
    else if( distance != 0 && angle > 75 && angle < 125
        && distance < 30 && forWard==true )//checking middle
    {
       STOP();
       //Serial.print("M_2 ");
    }
    else if( distance != 0 && distance < 15 
         &&   (angle > 125 && angle < 150) ) 
        {
           STOP();
           forWard=false;
           fLeft=false;
           fRight=false;
           //beenHere=false;
           //Serial.print("A_3 ");
        }//if
  }
  //headServo.write(10);
  //delay(5);
  /*TESTING*/
  /*
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
  */
              /* CHECK IF !=0 && equal 
                 RIGHT MIDDLE AND LEFT */
  if( RIGHT && MIDDLE && LEFT){
      //Serial.print("1");
      if( RIGHT < LEFT && RIGHT < MIDDLE){
       //Serial.print("a");
       //GO RIGHT 
       reverseAndGo_RIGHT(1500);
      }else if( LEFT < RIGHT && LEFT < MIDDLE ){
       // Serial.print("b");
       //GO LEFT 
       reverseAndGo_LEFT(1500);
       /* CHECK THESE STATMENTS*/
      }else if( RIGHT == MIDDLE && RIGHT < LEFT ){
        //GO RIGHT
        reverseAndGo_RIGHT(2000);
        //Serial.print("N");
      }else if( MIDDLE == LEFT && MIDDLE < RIGHT ){
        //GO LEFT
        reverseAndGo_LEFT(2000);
        //Serial.print("m");
      }else{
        //Serial.print("c");
        //GO BACKWARD AND REVERSE  RIGHT!!!
       // RIGHT:2 MIDDLE:2 LEFT:3
       //   A1c
       //RIGHT:2 MIDDLE:1 LEFT:1
       //A1c
        goBackward(500);
        reverseAndGo_LEFT(2000);
      }
  }else if( RIGHT && MIDDLE ){
        //Serial.print("2");
     if( RIGHT < MIDDLE ){
       //Serial.print("d");
      //GO RIGHT
      reverseAndGo_RIGHT(1500);
     }else{  //includes equal
    // Serial.print("e");
      //GO LEFT
      reverseAndGo_LEFT(1500);
     }
    }else if( LEFT && MIDDLE ){
         // Serial.print("3");
      if( LEFT < MIDDLE ){
       // Serial.print("f");
       //GO LEFT
       reverseAndGo_LEFT(1500);
      }else{  //includes equal
       // Serial.print("g");
       //GO RIGHT
       reverseAndGo_RIGHT(1500);
      }
  }else if( LEFT && RIGHT ){
     // Serial.print("4");
     if( LEFT < RIGHT ){
      // Serial.print("h");
        //GO LEFT
        reverseAndGo_LEFT(1500);
      }else{  //includes equal
       // Serial.print("i");
       //GO RIGHT 
         reverseAndGo_RIGHT(1500);
      }
  }else if( RIGHT ){
      //Serial.print("5");
      //GO LEFT
      reverseAndGo_LEFT(1500);
  }else if( MIDDLE ){
     // Serial.print("6");
     //GO EITHER WAY  ????
     reverseAndGo_RIGHT(1500);
  }else if( LEFT ){
    // Serial.print("7");
     //GO RIGHT 
   reverseAndGo_RIGHT(1500);
  }else if( FAR_RIGHT || FAR_LEFT ){
    //Serial.print("8");
    if( FAR_RIGHT < FAR_LEFT ){
     // Serial.print("j");
      if(fLeft==true){
      //  Serial.print("z");
        reverseAndGo_RIGHT(1000);
      }else{
       // Serial.print("l");
          fLeft=true;
          //merge right
          delay(25);
          turnRight();
          goFORWARD();
      }
    }else{
      //Serial.print("k");
      if(fRight==true){
        reverseAndGo_LEFT(1000);
      }else{
        //Serial.print("y");
         fRight=true;
         //merge left 
         delay(25);
         turnLeft();
         goFORWARD();
      }
    }
  }else{
    //Serial.print("9");
     //GO FORAWRD
     goFORWARD();
  }
}//driveAlone
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
  analogWrite(3, 200);  //activate drive motor w/ 200 speed
  digitalWrite(BACK_MOTOR, LOW);
  delay(sec); 
  forwardAgain=false;
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
  forWard=true;
  digitalWrite(BACK_BRAKE, LOW);
  digitalWrite(FRONT_BRAKE, HIGH);
  digitalWrite(BACK_MOTOR, HIGH);
  if(forwardAgain==true){
    analogWrite(3,1750);
  }else{
    analogWrite(3,225);  //activate SFast
    forwardAgain=true;
  }
  //check90();
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
  
  pinMode(LED_RED_1, OUTPUT); 
  pinMode(LED_RED_2, OUTPUT); 
  pinMode(LED_RED_3, OUTPUT); 
  pinMode(LED_GRE_1, OUTPUT); 
  pinMode(LED_GRE_2, OUTPUT); 
  pinMode(LED_GRE_3, OUTPUT); 
  ledAll_ON();
  counterVal++;
  //goFORWARD();
}
void ledAll_ON(){
  digitalWrite(LED_GRE_1,HIGH);
  digitalWrite(LED_GRE_2,HIGH);
  digitalWrite(LED_GRE_3,HIGH);
  digitalWrite(LED_RED_1,HIGH);
  digitalWrite(LED_RED_2,HIGH);
  digitalWrite(LED_RED_3,HIGH);
}
  /*
  
  if( UD > 129 ){
    //STRAIGHT
    if( LR > 128 ){
     //GO RIGHT 
    }else if( LR < 128 ){
     //GO LEFT 
    }else{
      // GO STRAIGHT
    }
  }else if( UD < 129){
   //REVERSE 
     if( LR > 128 ){
       //reverse & TURN WHELE RIGHT
     }else if( LR < 128){
       //reverse & TURN WHELE LEFT
     }else{
       //GO REVERSE
     }
  }
  else{
   //STOP 
  }
  
  
  Serial.print("END   UD=");
  for(unsigned i=0; i <3 ; i++){
   //Serial.print(Umsg[i]);
   UD[i]=Umsg[i] - '0';
   //Serial.print(" UD: ");
   Serial.print(UD[i]);
  }
  Serial.print("  ,L=");
  for(unsigned i=0; i <3 ; i++){
   //Serial.print(Lmsg[i]);
   LR[i]=Lmsg[i] - '0';
   Serial.print(LR[i]);
  }
  int UUUU = atoi(Umsg);
  Serial.println(UUUU);
  Serial.println();*/

