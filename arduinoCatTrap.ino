#include <Servo.h>
#include <SoftwareSerial.h>

#define PIN_SERVO 3
#define PIN_SENSOR 4
#define PIN_SERVO_TRANSISTOR 5

SoftwareSerial GSM(7, 8); // RX, TX
char phone_no[]="+XXYYYYYYYYY"; // your phone number to receive sms with country prefix

Servo servo;
byte sensorDetect;
bool sensorStatus = false;

void setup() {

  pinMode(PIN_SENSOR, INPUT);

  pinMode(PIN_SERVO_TRANSISTOR , OUTPUT);
  digitalWrite(PIN_SERVO_TRANSISTOR, LOW); // switch on servo power
  
  pinMode(PIN_SERVO, OUTPUT);
  attachServo();
  
  servo.write(0);
  delay(500);
  
  servo.detach();
  digitalWrite(PIN_SERVO_TRANSISTOR, HIGH); // switch off servo power

  GSM.begin(9600);

  // synchronize A6 module with Arduino 9600 boudrate
  for( int i=0; i<=10; i++) {
    delay(500);
    GSM.println("AT");  
  }
  delay(5000);

  GSM.println("AT+CMGF=1"); // select sms text mode
  delay(200);

  GSM.println("AT+CMGD=1,4"); // delete all sms from memory
  delay(200);

  sendSMS("Cat Trap powered on");  
  delay(5000);
  
  // Not really necessary but prevents the serial monitor from dropping any input
  while(GSM.available()) {
    Serial.write(GSM.read());
  }   
}

void loop() {

  delay(50);
  
  sensorDetect = digitalRead(PIN_SENSOR);

  if ( sensorStatus == false && sensorDetect == LOW ) { // Sensor ON
    
    attachServo();
    digitalWrite(PIN_SERVO_TRANSISTOR, LOW); // switch on servo power
    delay(100);
    
    servo.write(180);
    delay(2000);
    
    servo.write(0);
    delay(1000);
    
    servo.detach();
    digitalWrite(PIN_SERVO_TRANSISTOR, HIGH); // switch off servo power

    sendSMS("Cat is trapped!");  
    delay(5000);
    
    sensorStatus = true; // trigger servo only once
  }
}

void attachServo() {
  servo.attach(PIN_SERVO, 500, 2000);
}

void sendSMS(String msg) {
  GSM.print("AT+CMGS=\"");
  GSM.print(phone_no); 
  GSM.write(0x22);  // double quotes "
  GSM.write(0x0D);  // hex equivalent of Carraige return    
  GSM.write(0x0A);  // hex equivalent of newline
  delay(1000);
  GSM.print(msg);
  delay(500);
  GSM.println (char(26));//the ASCII code of the ctrl+z is 26  
}
