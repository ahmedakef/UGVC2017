#define motorRF 2
#define motorRB 3
#define motorLF 4
#define motorLB 5
#define linkFF 22
#define linkFB 23
#define linkBF 24
#define linkBB 25
#define gripperlock 26
#define gripperunlock 27
#define gripperturnR 6
#define gripperturnL 7
#define linearup1 28
#define lineardown1 29
#define linearup2 30
#define lineardown2 31
#define linearup3 32
#define lineardown3 33
#define armR 8
#define armL 9



void up(int motorF,int motorB);
void down(int motorF,int motorB);
void forward(int motorf,int motorb,int pwm);
void backward(int motorf,int motorb,int pwm);
void stop_motors();
void stopmove();

int pwm = 150;

char serialData;

int motor_plus_speed = 0;

int motor1up = 5; //right
int motor1down = 6;
int motor1speed=9;

int motor2up = 7; //left
int motor2down = 8;
int motor2speed=10;

//int reset_button =13; // connected to reset pin with high voltage, the arduino will restart if it has LOW voltage

void setup() {
  
  Serial.begin(9600);
  //Serial.setTimeout(2000);


  
  pinMode(motor1up, OUTPUT);
  pinMode(motor1down, OUTPUT);
  pinMode(motor1speed, OUTPUT);
  pinMode(motor2up, OUTPUT);
  pinMode(motor2down, OUTPUT);
  pinMode(motor2speed, OUTPUT);
  


}

void loop() { // run over and over
  if (Serial.available())
  {
    serialData = Serial.read();

    switch (serialData)
    {
    case 'F': // F =  go forward
      forward(motorRF,motorRB,pwm);
      forward(motorLF,motorLB,pwm);
      break; 
    case'B':  // B =  go back
      backward(motorRF,motorRB,pwm);
      backward(motorLF,motorLB,pwm);
      break; 
    case'R' : // R =  right
      forward(motorRF,motorRB,0);
      forward(motorLF,motorLB,200);
      break; 
    case 'L':  // L =  left
      forward(motorRF,motorRB,200);
      forward(motorLF,motorLB,0);
      break; 

    case 's':
      stopmove();
      break;
    
    case 'O' : // O = +10 pwm 
      pwm+=10;
      pwm = min(pwm,255);
      break; 
   
    case 'P':  // P =  -10 pwm
      pwm-=10;
      pwm = max(pwm,0);
      break; 


    case 'Q':   //Q = reset
      pwm = 150;
      break;
 
    
    case 'T':  // T = link front up
      up(linkFF,linkFB);
      break; 

    case 'U': // U = link front down
      down(linkFF,linkFB);
      break;

    case 'V' : // V = link back up
      up(linkBF,linkBB);
      break; 

    case 'W' : // V = link back up
      down(linkBF,linkBB);
      break; 
    


   
    case 'A':  // A = gripper right
      up(gripperturnR,linkFB);
      break; 

    case 'C': // C = gripper left
      down(gripperturnL,linkFB);
      break;

    case 'D' : // D = gripper unlock
      up(gripperunlock,linkBB);
      break; 

    case 'E' : // E = gripper lock
      down(gripperlock,linkBB);
      break; 


    case 'I':  // I = linear 1 up
      up(linearup1,lineardown1);
      break; 

    case 'J': // j = liear 1 down
      down(linearup1,lineardown1);
      break;

    case 'K' : // K = linear 2 up
      up(linearup2,lineardown2);
      break; 

    case 'Y' : // Y = liear 2 down
      down(linearup2,lineardown2);
      break; 
    
    case 'M' : // M = linear 3 up
      up(linearup3,lineardown3);
      break; 

    case 'N' : // N = liear 3 down
      down(linearup3,lineardown3);
      break; 


    case 'G' : // G = arm right
      up(armR,armL);
      break; 

    case 'H' : // H = arm left
      down(armR,armL);
      break; 
    case 'S':
      stop_motors();
      break;
    default :
      stop_motors();
      break;
  }
  
 }

 delay(100);
 stopmove();

 
}


void up(int motorF,int motorB) {

      
      digitalWrite(motorF, HIGH);
      digitalWrite(motorB, LOW);
}
void down(int motorF,int motorB) {

      
      digitalWrite(motorF, LOW);
      digitalWrite(motorB, HIGH);
}
void forward(int motorf,int motorb,int pwm){
      analogWrite(motorf, pwm);
      analogWrite(motorb, 0);
}
void backward(int motorf,int motorb,int pwm){
      analogWrite(motorb, pwm);
      analogWrite(motorf, 0);
}

void stop_motors(){
      digitalWrite(motorRF, LOW);
      digitalWrite(motorRB, LOW);
      digitalWrite(motorLF, LOW);
      digitalWrite(motorLB, LOW);
      digitalWrite(linkFF, LOW);
      digitalWrite(linkFB, LOW);
      digitalWrite(linkBF, LOW);
      digitalWrite(linkBB, LOW);
      digitalWrite(gripperlock, LOW);
      digitalWrite(gripperunlock, LOW);
      digitalWrite(gripperturnR, LOW);
      digitalWrite(gripperturnL, LOW);
      digitalWrite(linearup1, LOW);
      digitalWrite(lineardown1, LOW);
      digitalWrite(linearup2, LOW);
      digitalWrite(lineardown2, LOW);
      digitalWrite(linearup3, LOW);
      digitalWrite(lineardown3, LOW);
      digitalWrite(armR, LOW);
      digitalWrite(armL, LOW);

}  
void stopmove(){
      digitalWrite(motorRF, LOW);
      digitalWrite(motorRB, LOW);
      digitalWrite(motorLF, LOW);
      digitalWrite(motorLB, LOW);
}

