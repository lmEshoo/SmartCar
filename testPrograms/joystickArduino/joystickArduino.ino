#include <Bounce.h>
#include <LiquidCrystal.h>

//LCD Hookups
#define RS 10
#define EN 11
#define D4 2
#define D5 3
#define D6 4
#define D7 5

//LCD Setup
LiquidCrystal lcd(EN, RS, D7, D6, D5, D4);
byte THIS_CHAR[8] = {
        B11111,
        B10001,
        B10101,
        B11111,
        B11111,
        B10101,
        B10001,
        B11111
};
byte NOT_THIS_CHAR[8] = {
        B11111,
        B10001,
        B10001,
        B10001,
        B10001,
        B10001,
        B10001,
        B11111
};
//Send data from JoyStick to Xbee
int UD=0, LR=0, OUT_UD=0, OUT_LR=0;

Bounce bouncer  = Bounce( A2 , 25 );
Bounce bouncer1  = Bounce( A3 , 25 );
boolean b = false;
boolean b1 = false;

void setup() {
  Serial.begin(9600);
  pinMode(A2,INPUT);
  pinMode(A3,INPUT);
  pinMode(A4,OUTPUT);
  digitalWrite(A4,HIGH);
  pinMode(7,OUTPUT);
  lcd.createChar(0, NOT_THIS_CHAR);
  lcd.createChar(1, THIS_CHAR);
  lcd.begin(16, 2);
  lcd.print("    WELCOME");
  lcd.setCursor(1,1);
  lcd.print("TO LENNY's CAR");
  delay(1000);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("SELF DRIVE: ");
  lcd.write(byte(0));
  lcd.setCursor(0,1);
  lcd.print("YOU DRIVE: ");
  lcd.write(byte(0));
}
/*
HIGHEST LR 1023 228
HIGHEST UD 990  489
*/
void loop() {
  bouncer.update();
  bouncer1.update();
  if( bouncer.read() || b){
    lcd.setCursor(12,0);
    lcd.write(byte(0));
    digitalWrite(7,HIGH);
    lcd.setCursor(11,1);
    lcd.write(byte(1));
    
    UD = analogRead(A0);
    LR = analogRead(A1);
    OUT_LR = map(LR, 0, 1024, 100, 355);
    OUT_UD = map(UD, 0, 995, 360, 615);
    Serial.write("U");
    Serial.println(OUT_UD);
    Serial.write("L");
    Serial.println(OUT_LR);
    b=true;
    b1=false;
  }
  if( bouncer1.read() || b1){
    lcd.setCursor(12,0);
    lcd.write(byte(1));
    lcd.setCursor(11,1);
    lcd.write(byte(0));
    digitalWrite(7,LOW);
    Serial.write("B");
    b1=true;
    b=false;
  }
  
  delay(150);
} 
  
  
  /*
  Serial.print("UD = "); //Serial.print(OUT_UD);
  if(OUT_UD == 389){
    Serial.print("UD_SET ");
  }else if( OUT_UD > 389){
    Serial.print("UP ");
  }else{
    Serial.print("DOWN ");
  }
  Serial.print(" , LR = ");
  if(OUT_LR == 128){
    Serial.println("UD_SET ");
  }else if( OUT_LR > 128){
    Serial.println("RIGHT ");
  }else{
    Serial.println("LEFT ");
  }*/

