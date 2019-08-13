#include <Servo.h>

#define PIN_SERVO 3
#define PIN_SENSOR 4
#define PIN_SERVO_TRANSISTOR 5

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
  delay(1000);
  servo.detach();
  digitalWrite(PIN_SERVO_TRANSISTOR, HIGH); // switch off servo power
}

void loop() {

  delay(100);
  
  sensorDetect = digitalRead(PIN_SENSOR);

  if ( sensorStatus == false && sensorDetect == LOW ) { // Sensor ON
    attachServo();
    digitalWrite(PIN_SERVO_TRANSISTOR, LOW); // switch on servo power
    delay(100);
    servo.write(180);
    delay(1000);
    servo.detach();
    digitalWrite(PIN_SERVO_TRANSISTOR, HIGH); // switch off servo power
    sensorStatus = true;
  } else if ( sensorStatus == true && sensorDetect == HIGH ) { // Sensor OFF
    attachServo();
    digitalWrite(PIN_SERVO_TRANSISTOR, LOW); // switch on servo power
    delay(100);
    servo.write(0);
    delay(1000);
    servo.detach();
    digitalWrite(PIN_SERVO_TRANSISTOR, HIGH); // switch off servo power
    sensorStatus = false;
  }
}

void attachServo() {
  servo.attach(PIN_SERVO, 500, 2000);
}
