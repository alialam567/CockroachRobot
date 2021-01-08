 /*
 * Program to delay an action based on dual light sensors
 * Name: Ali & Alex
 * Date: Oct 16, 2017
 * Version: 6.74
 * Left LED (left side of robot) and Right LED (right side)
 * Left Light Sensor and a Right Light Sensor
 * GOAL: Cockroach like program that moves robot to darker corners
 * darker levels than the other side.
 * Turn OFF an LED for 2 seconds after light conditions are equalized.
 * 
 * I/O connection
 * A7 - left light sensor
 * A8 - right light sensor
 * D15 - left LED
 * D16 - right LED
 * 
 *  
 * 
 */
int LLED = 15; // integer for left LED on pin 15
int RLED = 16; // integer for right LED on pin 16
int was_dark = 0;   // keeps track of when dark happened
                    // 0 = very little difference in darkness on LEFT or RIGHT
                    // 1 = one side was darker than the other
int pin = 13; // integer for built in LED on pin 13
long end_timer; // long for AVOID state
long stop_timer; // long for stopping program when in dark corner
const int M1 = 3; // const integer for motor 1 (Forward)
const int M2 = 5; // const integer for motor 1 (Backward)
const int M3 = 6; // const integer for motor 2 (Backward)
const int M4 = 11; // const integer for motor 2 (Forward)
const int ROAM = 1; // const integer for state ROAM
const int DARK_ON_LEFT = 2; // const integer for state DARK_ON_LEFT
const int DARK_ON_RIGHT = 7; // const integer for state DARK_ON_RIGHT
const int AVOID = 4; // const integer for state AVOID

volatile int state = HIGH;
long timer1; //delay for 275 ms so keeps moving left or right for that log even if brightness changed
long delay_timer; //delat for 2 seconds before turning LED's off
int val = 0; //integer for on/off buttons


void moveFWD(){ // void to move robot forward
  digitalWrite(M1,HIGH); // Drive motor 1 forward
  digitalWrite(M2,LOW);
  analogWrite(M1, 200); // drive motor 1 in 200/255 of it's power

  digitalWrite(M3,LOW);
  digitalWrite(M4,HIGH); // Drive motor 2 forward
  analogWrite(M4, 200); // drive motor 2 in 200/255 of it's power
}
void moveLEFT(){ // void to move robot left
  digitalWrite(M1,HIGH); // Drive motor 1 forward
  digitalWrite(M2,LOW);
  analogWrite(M1, 200); // drive motor 1 in 200/255 of it's power

  digitalWrite(M3,HIGH); // Drive motor 2 reverse
  digitalWrite(M4,LOW);
  analogWrite(M3, 200); // drive motor 2 in 200/255 of it's power
}
void moveRIGHT(){ // void to move robot right
  digitalWrite(M1,LOW);
  digitalWrite(M2,HIGH); // Drive motor 1 reverse
  analogWrite(M2, 200); // drive motor 1 in 200/255 of it's power

  digitalWrite(M3,LOW);
  digitalWrite(M4,HIGH); // drive motor 2 forward
  analogWrite(M4, 200); // drive motor 2 in 200/255 of it's power
}
void moveREV(){ // void to move robot reverse
  digitalWrite(M1,LOW);
  digitalWrite(M2,HIGH); // drive motor 1 reverse
  analogWrite(M2, 200); // drive motor 1 in 200/255 of it's power

  digitalWrite(M3,HIGH); // drive motor 2 reverse
  digitalWrite(M4,LOW);
  analogWrite(M3, 200); // drive motor 2 in 200/255 of it's power
}
void STOP(){ // void to stop robot
  digitalWrite(M1,HIGH); // drive motor 1 forward
  digitalWrite(M2,LOW);
  analogWrite(M1, 0); // drive motor 1 in 0/255 of it's power (Stop moving motor)

  digitalWrite(M3,LOW);
  digitalWrite(M4,HIGH); // drive motor 2 forward
  analogWrite(M4, 0); // drive motor 2 in 0/255 of it's power (Stop moving motor)
}

void setup() {
  pinMode(M1,OUTPUT); // setting up motor 1 (Forward)
  pinMode(M2,OUTPUT); // setting up motor 1 (Reverse)
  pinMode(M3,OUTPUT); // setting up motor 2 (Reverse)
  pinMode(M4,OUTPUT); // setting up motor 2 (Forward)
  pinMode(LLED,OUTPUT); // setting up left LED
  pinMode(RLED,OUTPUT); // setting up right LED
  pinMode(18, INPUT_PULLUP); // setting up push-button 1 (On button)
  pinMode(20, INPUT_PULLUP); // setting up push-button 2 (Off button)
  Serial.begin(9600); // starts communications from USB port at 9600 bits per second
  digitalWrite(LLED,HIGH);  // turn off left LED
  digitalWrite(RLED,HIGH);  // turn off right LED
  pinMode(pin, OUTPUT); // setting up built in LED from Arduino in pin 13
  attachInterrupt(0, blink, CHANGE); //use interrupt zero! Function “blink” is called whenever a CHANGE on pin 2 occurs.
  digitalWrite(pin,LOW); // turn on built in LED in pin 13
  end_timer=millis()+3000; // start timer from the time that the LED turns on
  delay_timer=millis()+2000; // delay for LED
  stop_timer=millis()+2000; // delay to end program when in dark corner
  timer1 = millis()+275;
}

void loop() {
  int Lsensor = analogRead(A7); // integer for left sensor
  int Rsensor = analogRead(A8); // integer for right sensor
  int diff = Rsensor - Lsensor; // integer for right and left sensor difference   
    // left side darker diff = at least -55
    // right side darker diff = more than 50
  if (digitalRead(18)== LOW ){ // button 1 has been pressed
    val = val + 1; // add value to val (turn on robot)
  }
  if (digitalRead(20)== LOW ){ // button 1 has been pressed
    val = 0; // equal val to zero (turn off robot)
  }
  if (val >= 1){ // when program is turned on by val equaling to one,
     
    if ((Rsensor >= 700) && (Lsensor >= 665)){ // when right sensor is greater than or equal to 725, when left sensor is greater than or equal to 685, and when robot drives against wall for 2 seconds, set val to zero (turn off robot)
      if (millis() > stop_timer){
        val = 0;
      }
    }
  
    if (diff < -55) {  // left side is darker
      delay_timer=millis()+2000;
      digitalWrite(RLED,HIGH);
      digitalWrite(LLED,LOW);
      was_dark = 1;
    }

    else if (diff > 50) {  // right side is darker
      delay_timer=millis()+2000;  
      digitalWrite(LLED,HIGH);
      digitalWrite(RLED,LOW);
      was_dark = 1;
    }
    else if (was_dark == 1){  //if it was dark it checks if 2 seconds have passed since then 
      if (millis() < delay_timer){ //if the 2 seconds have passed then it turs both leds off and set was_dark variable back to 0
        digitalWrite(RLED,HIGH);  
        digitalWrite(LLED,HIGH);
        was_dark = 0;  
        }        
    }
    if (millis() > end_timer){ //if 3 seconds have not passed since the robot last moved then turn pin 13 off
      digitalWrite(pin,HIGH);
    }
    else if (millis() < end_timer){ //if 3 seconds have passed since the robot last moved then turn pin 13 on
      digitalWrite(pin,LOW);
    } 
    switch(state){ //state machine
      case ROAM: //if we are roaming then 
      moveFWD(); //move the robot forward
     // if(millis() > timer1){ //if 100ms have passed check if
        if (diff <= -55){ //darker on left side then switch states to DARK_ON_LEFT (turn left) reset timer
          state = DARK_ON_LEFT;
         // timer1 = millis()+50;
        }
        if (diff >= 50){ //darker on right side then switch states to DARK_ON_RIGHT (turn right) reset timer
          state = DARK_ON_RIGHT;
         // timer1 = millis()+50;
        }
        if (millis() > end_timer){ //if 3 seconds have passed without moving then switch to case AVOID
          state = AVOID;
       // }
      }
      break;
      case DARK_ON_LEFT: //if light value is darker on left then
      moveLEFT(); //move the robot left
      if(millis() > timer1){ //if 100ms has passed check if
        if ((diff < 50) && (diff > -55)){ //if the brightness is the same on both sides then witch state to roam and reset timer1
          state = ROAM;
          timer1 = millis()+275;
        }
      if (diff >= 50){ //darker on right side then switch states to DARK_ON_RIGHT (turn right) reset timer
      state = DARK_ON_RIGHT;
      timer1 = millis()+275;
    }
      }
      break;
      case DARK_ON_RIGHT: //if light value is darker on right then
      moveRIGHT(); //move the robot right
      if(millis() > timer1){ //if 100ms has passed check if
        if ((diff < 50) && (diff > -55)){ //if the brightness is the same on both sides then witch state to roam and reset timer1
          state = ROAM;
          timer1 = millis()+275;
        }
        if (diff <= -55){ //darker on left side then switch states to DARK_ON_LEFT (turn left) reset timer
        state = DARK_ON_LEFT;
        timer1 = millis()+275;
        }
      }
      break;
    
      case AVOID: //if robot hits wall for 3000 miliseconds then
      moveREV(); //robot reverses for two seconds
      delay(2000);
      moveRIGHT(); //robot moves right for two seconds
      delay(2100);
      if ((diff < 50) && (diff > -55)){ //if the brightness is the same on both sides then witch state to roam and reset timer1
        state = ROAM;
        timer1 = millis()+275;
      }
      if (diff <= -55){ //darker on left side then switch states to DARK_ON_LEFT (turn left) reset timer
        state = DARK_ON_LEFT;
        timer1 = millis()+275;
      }
      if (diff >= 50){ //darker on right side then switch states to DARK_ON_RIGHT (turn right) reset timer
        state = DARK_ON_RIGHT;
        timer1 = millis()+275;
      }
    
      break;
    }
  }
  else if (val == 0){ //if val were to equal to zero, set motor's power value to zero (stops moving)
    digitalWrite(M1,HIGH);
    digitalWrite(M2,LOW);
    analogWrite(M1, 0);

    digitalWrite(M3,LOW);
    digitalWrite(M4,HIGH);
    analogWrite(M4, 0);
  }
}
void blink(){
 end_timer=millis()+3000; //reset end_timer everytime the wheel moves
 stop_timer=millis()+2000; //reset stop_timer everytime the wheel moves
}
