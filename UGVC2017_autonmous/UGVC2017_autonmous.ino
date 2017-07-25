#include <Adafruit_GPS.h> //Load the GPS Library. Make sure you have installed the library form the adafruit site above
#include <SoftwareSerial.h> //Load the Software Serial Library. This library in effect gives the arduino additional serial ports
#include <Wire.h>
#include <HMC5883L.h>

#define motorR_dir 2
#define motorR_pwm 3
#define motorL_dir 4
#define motorL_pwm 5
#define sz 2

#define EPS 1e-6
#define trigPin1 22
#define echoPin1 23
#define trigPin2 24
#define echoPin2 25
#define trigPin3 26
#define echoPin3 27

void stop_motors();
void forward();
void right();
void turn90();
void turn_90();
void stop_motors();
float read_ultrasonic(int trigPin , int echoPin);
static const double gates_lat[2]={29,29};
static const double gates_lng[2]={29,29};

float Long , Lat;
String NMEA1;  //We will use this variable to hold our first NMEA sentence
String NMEA2;  //We will use this variable to hold our second NMEA sentence
char c;       //Used to read the characters spewing from the GPS module
int X,degree;
int dist1,dist2,dist3;


SoftwareSerial mySerial(2, 3); //Initialize SoftwareSerial, and tell it you will be connecting through pins 2 and 3
Adafruit_GPS GPS(&mySerial); //Create GPS object
HMC5883L compass;



void setup() {
  // put your setup code here, to run once:
  // Initialize Initialize HMC5883L
  //Serial.println("Initialize HMC5883L");
  Serial.begin(115200);  //Turn on the Serial Monitor
  
  pinMode(trigPin1, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin1, INPUT); // Sets the echoPin as an Input
  pinMode(trigPin2, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin2, INPUT); // Sets the echoPin as an Input
  pinMode(trigPin3, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin3, INPUT); // Sets the echoPin as an Input
  while (!compass.begin())
  {
    Serial.println("Could not find a valid HMC5883L sensor, check wiring!");
    delay(500);
  }

  GPS.begin(9600);       //Turn GPS on at baud rate of 9600
  GPS.sendCommand("$PGCMD,33,0*6D"); // Turn Off GPS Antenna Update
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA); //Tell GPS we want only $GPRMC and $GPGGA NMEA sentences
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);   // 1 Hz update rate
  delay(2000);  //Pausek
   compass.setRange(HMC5883L_RANGE_1_3GA);
 
  // Set measurement mode
  compass.setMeasurementMode(HMC5883L_CONTINOUS);
 
  // Set data rate
  compass.setDataRate(HMC5883L_DATARATE_30HZ);
 
  // Set number of samples averaged
  compass.setSamples(HMC5883L_SAMPLES_8);
 
  // Set calibration offset. See HMC5883L_calibration.ino
  compass.setOffset(0, 0);

}
 

void loop() {
  Get_Gps(&Long,&Lat);
  composs();
 for(int i=0;i<sz;i++){
       while(abs(Lat- gates_lat[i])>EPS || abs(Long-gates_lng[i])>EPS){
            dist1= read_ultrasonic(trigPin1,echoPin1);
             dist2= read_ultrasonic(trigPin2,echoPin2);
              dist3= read_ultrasonic(trigPin3,echoPin3);
              if(dist1<200||dist2<200||dist3<200){
                 escape();
              }
            Get_Gps(&Long,&Lat);  //This is a function we define below which reads two NMEA sentences from GPS
            Lat=GPS.latitudeDegrees;
            Long=GPS.longitudeDegrees;
              if(Lat-gates_lat[i]<-1*EPS){
                Serial.print("1");
                 while(!(degree>340||degree<20)){
                    right();
                    composs();
                 }
              }
              else if(Lat-gates_lat[i]>EPS){
                  Serial.print("2");
                while(!(degree>160&&degree<200)){
                  right();
                  composs();
                }
              }
              else if(Long-gates_lng[i]<-1*EPS){
                    Serial.print("3");
                while(!(degree>70&&degree<110)){
                  right();
                  composs();
                }
              }
              else if(Long-gates_lng[i]>EPS){
                      Serial.print("4");
                 while(!(degree>250&&degree<290)){
                  right();
                  composs();
                }
              }
              forward();
       }
   }
stop_motors();
}
int Get_Gps(float* at, float* lon){
    clearGPS();    //Serial port probably has old or corrupt data, so begin by clearing it all out
     while(!GPS.newNMEAreceived()) {
        c=GPS.read();
      }
    NMEA1=GPS.lastNMEA();
    if (!GPS.parse(GPS.lastNMEA()))   // this also sets the newNMEAreceived() flag to false
      return X;
    X=(GPS.hour)*10000 +(GPS.minute)*100 +(GPS.seconds);
     
   
    
    Serial.print("Fix: "); Serial.print((int)GPS.fix);
    Serial.print(" quality: "); Serial.println((int)GPS.fixquality); 
    if (GPS.fix) {
      Serial.print("Location: ");
      Serial.print(GPS.latitude, 4); Serial.print(GPS.lat);
      Serial.print(", "); 
      Serial.print(GPS.longitude, 4); Serial.println(GPS.lon);
      Serial.print("Location (in degrees, works with Google Maps): ");
      Serial.print(GPS.latitudeDegrees, 4);
      Serial.print(", "); 
      Serial.println(GPS.longitudeDegrees, 4);
 
      Serial.print("Speed (knots): "); Serial.println(GPS.speed);
      Serial.print("Angle: "); Serial.println(GPS.angle);
      Serial.print("Altitude: "); Serial.println(GPS.altitude);
      Serial.print("Satellites: "); Serial.println((int)GPS.satellites);
    }
  Serial.println('\n');
 
}
 
 
void composs(){
  Serial.println("i got into the composs function");
  Vector norm = compass.readNormalize();
 
  // Calculate heading
  float heading = atan2(norm.YAxis, norm.XAxis);
 
  // Set declination angle on your location and fix heading
  // You can find your declination on: http://m...content-available-to-author-only...n.com/
  // (+) Positive or (-) for negative
  // For Bytom / Poland declination angle is 4'26E (positive)
  // Formula: (deg + (min / 60.0)) / (180 / M_PI);
  float declinationAngle = (4.0 + (26.0 / 60.0)) / (180 / M_PI);
  heading += declinationAngle;
 
  // Correct for heading < 0deg and heading > 360deg
  if (heading < 0)
  {
    heading += 2 * PI;
  }
 
  if (heading > 2 * PI)
  {
    heading -= 2 * PI;
  }
 
  // Convert to degrees
  float headingDegrees = heading * 180/M_PI; 
  degree=headingDegrees;
  // Output
  Serial.print(" Heading = ");
  Serial.print(heading);
  Serial.print(" Degress = ");
  Serial.print(headingDegrees);
  Serial.println();
 
  delay(100);
 
 
}
 
void clearGPS() {  //Since between GPS reads, we still have data streaming in, we need to clear the old data by reading a few sentences, and discarding these
while(!GPS.newNMEAreceived()) {
  c=GPS.read();
  }
GPS.parse(GPS.lastNMEA());
while(!GPS.newNMEAreceived()) {
  c=GPS.read();
  }
GPS.parse(GPS.lastNMEA());
   while(!GPS.newNMEAreceived()) { //Keep reading characters in this loop until a good NMEA sentence is received
  c=GPS.read(); //read a character from the GPS
  }
GPS.parse(GPS.lastNMEA());  //Once you get a good NMEA, parse it
 
while(!GPS.newNMEAreceived()) {  //Go out and get the second NMEA sentence, should be different type than the first one read above.
  c=GPS.read();
  }
 }
 
float read_ultrasonic(int trigPin , int echoPin){
  // Clears the trigPin
   digitalWrite(trigPin, LOW);
   delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
   digitalWrite(trigPin, HIGH);
   delayMicroseconds(10);
   digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
   int duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
   int distance= duration*0.034/2;
  // Prints the distance on the Serial Monitor
   Serial.print("Distance: ");
   Serial.println(distance);
   return distance;
}


void forward(){
      digitalWrite(motorR_dir, HIGH);
      digitalWrite(motorR_pwm, HIGH);
      digitalWrite(motorL_dir, HIGH);
      digitalWrite(motorL_pwm, HIGH);
      delay(1000);           
}

void turn_90(){
      digitalWrite(motorR_dir, LOW);
      digitalWrite(motorR_pwm, HIGH);
      digitalWrite(motorL_dir, HIGH);
      digitalWrite(motorL_pwm, HIGH);
      delay(2000);
}
void turn90(){
      digitalWrite(motorR_dir, HIGH);
      digitalWrite(motorR_pwm, HIGH);
      digitalWrite(motorL_dir, LOW);
      digitalWrite(motorL_pwm, HIGH);
      delay(2000);
}

 void right(){
      digitalWrite(motorR_dir, HIGH);
      digitalWrite(motorR_pwm, HIGH);
      digitalWrite(motorL_dir, LOW);
      digitalWrite(motorL_pwm, HIGH);
      delay(200);
}
void stop_motors(){
  digitalWrite(motorR_pwm,LOW);
  digitalWrite(motorL_pwm,LOW);
}
void escape(){
  right();
  delay(2000);
  forward();
  delay(2000);
}
