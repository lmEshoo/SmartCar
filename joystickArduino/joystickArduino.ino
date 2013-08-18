//Send data from JoyStick to Xbee
int UD=0, LR=0, OUT_UD=0, OUT_LR=0;


void setup() {
  Serial.begin(9600);
}
/*
HIGHEST LR 1023 228
HIGHEST UD 990  489
*/
void loop() {
  UD = analogRead(A0);
  LR = analogRead(A1);
  OUT_LR = map(LR, 0, 1024, 100, 355);
  OUT_UD = map(UD, 0, 995, 360, 615);
  Serial.write("U");
  Serial.println(OUT_UD);
  Serial.write("L");
  Serial.println(OUT_LR);
  delay(250);
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

