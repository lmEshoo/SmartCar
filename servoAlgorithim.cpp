//180 -> far left
//0 -> far right

if( (inches_180 < 12) || (inches_0 < 12) ){
	//check which way to go
	digitalWrite(BACK_BRAKE, HIGH);
	if( inches_180 < inches_0){
		delay(75);
		//reverse & go right
		turnLeft();  //put the tire in left dir
		//go backward
		goBackward();
		//stop
		digitalWrite(BACK_BRAKE, HIGH);  //stop
		digitalWrite(FRONT_BRAKE, HIGH);  //straight
	}
	else if( inches_0 < inches_180){
		delay(75);
		//reverse and go left
		turnRight();  //put the tire in right dir
		//go backward
		goBackward();
		//stop
		digitalWrite(BACK_BRAKE, HIGH);  //stop
		digitalWrite(FRONT_BRAKE, HIGH);  //straight
	}
	else if( inches_180 == inches_0 ){
			/* GOING STRAIGHT BACK */
		delay(75);
		goBackward(1000);
		digitalWrite(BACK_BRAKE, HIGH);  //stop
		digitalWrite(FRONT_BRAKE, HIGH);  //straight
	}
}	//if
else{
	//go FORWARD
	drive();
}	//else
delay(100);