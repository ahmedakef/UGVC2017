//#include <Servo.h> 
//Servo camServo;
//int pos = 0;

//#include <SoftwareSerial.h>
//SoftwareSerial IP(11,12); // RX, TX
//char IPData;


char serialData;

int motor_plus_speed = 0;

int motor1up = 4; //right
int motor1down = 5;
int motor11up = 8; //right
int motor11down = 9;

int motor2up = 6; //left
int motor2down = 7;
int motor22up = 10; //left
int motor22down = 11;

//int reset_button =13; // connected to reset pin with high voltage, the arduino will restart if it has LOW voltage

void setup() {
  
  //IP.begin(9600);
  Serial.begin(9600);
  //Serial.setTimeout(2000);

  //camServo.attach(3);
  //camServo.write(90);
  
  pinMode(motor1up, OUTPUT);
  pinMode(motor11up, OUTPUT);
  pinMode(motor1down, OUTPUT);
  pinMode(motor11down, OUTPUT);
  
  pinMode(motor2up, OUTPUT);
  pinMode(motor22up, OUTPUT);
  pinMode(motor2down, OUTPUT);
  pinMode(motor22down, OUTPUT);
  

//  pinMode(reset_button, OUTPUT); 
//  digitalWrite(reset_button, HIGH); 
}

void motors(int motor1,int motor11,int motor2,int motor22,int speed1,int speed2) {
      int speed1_a = speed1 + motor_plus_speed;
      speed1_a =min(speed1_a,255);
      
      int speed2_b = speed2 + motor_plus_speed;
      speed2_b =min(speed2_b,255);
      
      analogWrite(motor1,speed1_a);
      analogWrite(motor11,speed1_a);
      analogWrite(motor2,speed2_b);
      analogWrite(motor22,speed2_b);
}
void stopmotors(){
      analogWrite(motor1up,0);
      analogWrite(motor1down,0);
      analogWrite(motor11up,0);
      analogWrite(motor11down,0);
      analogWrite(motor2up,0);
      analogWrite(motor2down,0);
      analogWrite(motor22up,0);
      analogWrite(motor22down,0);
}  
void loop() { // run over and over
  if (Serial.available())
  {
    serialData = Serial.read();

    switch (serialData)
    {
    case 'w': // W = go forward
      motors(motor1up,motor11up,motor2up,motor22up,180,180);
      break; 
    case's':  // s =  go back
      motors(motor1down,motor11down,motor2down,motor22down,180,180);
      break;
    case'd' : // d =  right
      motors(motor1down,motor11down,motor2up,motor22up,190,190);
      break;
    case'a':  // a =  left
      motors(motor1up,motor11up,motor2down,motor22down,190,190);
      break;

    case'm' : // M =  top right
      motors(motor1up,motor11up,motor2up,motor22up,70,160);
      break;
    case'v':  // v =  top left
      motors(motor1up,motor11up,motor2up,motor22up,160,70);
      break;


    case'S': // S = STOP 
      stopmotors();
      break;
      
/*    case'K': // move camera to right
      pos += 10;
      pos=min(pos,180);
      camServo.write(pos);
      //IP.println(pos);
      break;
    case'D': // move camera to left
      pos -= 10;
      pos=max(pos,0);
      camServo.write(pos);
      //IP.println(pos);
      break;  
*/
    case'I': // I up the speed
      motor_plus_speed += 30;
      motor_plus_speed=min(motor_plus_speed,135);
      //IP.println(motor_plus_speed);
      break;
    case'E': // E down the speed
      motor_plus_speed -= 30;
      motor_plus_speed=max(motor_plus_speed,0);
      //IP.println(motor_plus_speed);
      break;  



         
    /*case'U': // u = reset the arduino
      digitalWrite(reset_button,LOW);
      break;*/

    
    default :
      stopmotors();
      break;
  }
Serial.println("ahmed");
  
}

 delay(100);

}

