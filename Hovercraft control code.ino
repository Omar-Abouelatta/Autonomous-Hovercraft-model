#include <Wire.h>

#include <Servo.h>


unsigned long timer = 0;

int state = 0;
int maxfan = 255;
int highpropfan = 230;
int lowpropfan = 200;
int highliftfan = 250 ;
int lowliftfan = 180;


const int mainfan = 6;
const int propfan = 5;

//us
long duration1 ;
long duration2 ;
long duration3 ;

float value1;
float value2;
float value3;

float distance1;
float distance2;
float distance3;

float percent1;
float percent2;


const int trig1 = 11;
const int trig2 = 13;
const int trig3 = 13;

const int echo1 = 2;
const int echo2 = 3;
const int echo3 = 8;
const int trigdelay = 5;


//servo
const int servo_pin = 9;
Servo servo;





#define SENSOR_ADRS 0x40 // I2C address of GP2Y0E03 
#define DISTANCE_ADRS 0x5E


void setup() {

  Wire.begin();
  servo.attach(servo_pin);
  
  Serial.begin(9600);

}

void loop() {
   Serial.print("\n ");
   Serial.print(timer);
   analogWrite(propfan,230);
  if ((millis() - timer) > 500) {
    if(state == 0){
      analogWrite(mainfan,highliftfan);
      state = 1;
    }
    else if (state == 1){
      analogWrite(mainfan,lowliftfan);
      state = 0;

    }
  timer = millis();
  }

 
//ir left
int ans ;
byte c[2];

  Wire.beginTransmission (SENSOR_ADRS); // start communication processing
  Wire.write (DISTANCE_ADRS); // specify the address of the table storing the distance value
  ans = Wire.endTransmission(); // send and close data
    ans = Wire.requestFrom(SENSOR_ADRS, 2) ;
    c[0] = Wire.read(); // Read the 11th to 4th bits of data c [1]
    c[1] = Wire.read(); // Read the 3rd and 0th bits of the data
    ans = ((c [0] * 16 + c [1]) / 16) / 4; // distance

    Serial.print(ans);
    Serial.println ("cm"); //to display on serial monit/lP:??



//us1 left

 pinMode(trig1 , OUTPUT);
 digitalWrite(trig1, LOW);
 delayMicroseconds(10);
 digitalWrite(trig1, HIGH);
 delayMicroseconds(trigdelay);
 digitalWrite(trig1, LOW);

 pinMode(echo1 , INPUT);
 duration1 = pulseIn (echo1, HIGH);


 

//us2 right
 pinMode(trig2 , OUTPUT);
 digitalWrite(trig2, LOW);
 delayMicroseconds(10);
 digitalWrite(trig2, HIGH);
 delayMicroseconds(trigdelay);
 digitalWrite(trig2, LOW);
 pinMode(echo2 , INPUT);
 duration2 = pulseIn (echo2, HIGH);




 

 //CONVERT TIME - DISTANCE
 Serial.print(" ir sensor reading");
 Serial.print(ans);
 Serial.println(" cm\n");
 
distance1 = duration1 / 58.0;
 Serial.print(" US sensor1 reading ");
 Serial.print(distance1);
 Serial.println(" cm\n");


distance2 = duration2 / 58.0;
 Serial.print(" US sensor2 reading ");
 Serial.print(distance2);
 Serial.println(" cm\n");
 



//centering the hovercraft

if(distance1 < 40 && distance2 < 40){
  percent1 = distance2 - distance1;
  if(percent1 < 30 && percent1 > -30){
    servo.write(87 + percent1);
    }
}
else{
  servo.write(87);
}


 



//hovercraft turn right
if( ans > 9 && ans < 30 && distance1 > 40 && distance2 < 30 ){
  analogWrite(mainfan,lowliftfan);
  delay(1000);
  analogWrite(mainfan,maxfan);
  analogWrite(propfan,maxfan);
  for (int i = 87 ; i > 37; i-= 5 ){
      servo.write(i);
      delay(50);
    }

  for (int i = 37 ; i < 87; i+= 5 ){
    servo.write(i);
    delay(50);
    }
    analogWrite(propfan,highpropfan);
    analogWrite(mainfan,lowliftfan);
  }

//hovercraft turn left
else if( ans > 9 && ans < 30 && distance2 > 40 && distance1 < 30 ){
  analogWrite(mainfan,lowliftfan);
  delay(1000); 
  analogWrite(mainfan,maxfan);
  analogWrite(propfan,maxfan);
  for (int i = 87 ; i < 137; i+= 5 ){
      servo.write(i);
      delay(50);
    }
    
  for (int i = 137 ; i > 87; i-= 5 ){
      servo.write(i);
      delay(50);
    }
  analogWrite(propfan,highpropfan);
  analogWrite(mainfan,lowliftfan);
}                                   //anti stuck
else if (ans <= 9){
  percent2 = distance2 - distance1;
  if (percent2 < 0) {
     analogWrite(mainfan,lowliftfan);
     delay(1000);
     analogWrite(mainfan,maxfan);
     analogWrite(propfan,maxfan);
    for (int i = 87 ; i > 47; i-= 5 ){
      servo.write(i);
      delay(50);
    }
    delay(300);
  
    for (int i = 47 ; i < 87; i+= 5 ){
      servo.write(i);
      delay(50);
    }
    analogWrite(propfan,highpropfan);
    analogWrite(mainfan,lowliftfan);


  }
  else if(percent2 > 0){
    analogWrite(mainfan,lowliftfan);
    delay(1000);
    analogWrite(mainfan,maxfan);
    analogWrite(propfan,maxfan);
    for (int i = 87 ; i < 147; i+= 5 ){
      servo.write(i);
      delay(50);
    }
    delay(300);
    for (int i = 147 ; i > 87; i-= 5 ){
      servo.write(i);
      delay(50);
    }
    analogWrite(propfan,highpropfan);
    analogWrite(mainfan,lowliftfan);
  }


}
Serial.print(" ");
Serial.print(percent1);
Serial.print(" ");
Serial.print(percent2);
percent1 = 0;
percent2 = 0;
}
