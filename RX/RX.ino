int data_UD, data_LR;

void setup(){
 Serial1.begin(9600);
 Serial.begin(9600);
 pinMode(34,OUTPUT);
 pinMode(35,OUTPUT);
 pinMode(31,OUTPUT);
 delay(500);
}
void loop(){
  int Ucounter=-1, Lcounter=-1;
  boolean UU=false, UUTR=false,
          LL=false, LLTR=false;
  char Umsg[4];
  char Lmsg[4];
  while( Serial1.available() > 0){
    digitalWrite(34,HIGH);
    char inChar = Serial1.read();
    if(UU){
      Umsg[Ucounter] = inChar;
      Ucounter++;
    }
    if( Ucounter==3) UU=false;
    if( inChar == 'U'){
      digitalWrite(35,HIGH);
      Ucounter++;
      UU=true;
    }
    if(LL){
      Lmsg[Lcounter] = inChar;
      Lcounter++;
    }
    if( Lcounter==3) LL=false;
    if( inChar == 'L'){
      digitalWrite(31,HIGH);
      Lcounter++;
      LL=true;
    }
    //else digitalWrite(31,LOW);
    //Serial.println(inChar);
  }
  delay(250);
  int UD = atoi(Umsg);  //489
  int LR = atoi(Lmsg);  //228
  Serial.print(UD);
  Serial.println(LR);
  
  /*Serial.print("END   UD=");
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
}
