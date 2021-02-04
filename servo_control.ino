#include <Servo.h>

Servo myservo;

int a1=10, a2=140;
int wTime = 1000;
int servPin = 9;

void setup()
{
  myservo.attach(servPin);
  Serial.begin(9600);
  Serial.println("Pet Feeder Initialising.");
  Serial.println("Enter 1 to feed, 2 for setup.");
}

void loop()
{
  myservo.write(a1);
  int opt = Serial.parseInt();
  if (opt==1)
  {
    Serial.println("Dispensing");
    myservo.write(a2);
    delay(wTime);
    Serial.println("Enter 1 to feed, 2 for setup.");
  }
  else if (opt==2)
  {
    Serial.println("Setup: 1 for Angle-1, 2 for Angle-2, 3 for Wait-Time, 4 for Servo-Pin and any other number to go back.");
    opt = 0;
    do
    {
      opt = Serial.parseInt();
    } while(!opt);
    switch(opt)
    {
      case 1:
        Serial.print("Current Angle-1 = "); Serial.print(a1); Serial.println(". Enter new.");
        do
        {
          a1 = Serial.parseInt();
        } while(!a1);
        break;
      case 2:
        Serial.print("Current Angle-2 = "); Serial.print(a2); Serial.println(". Enter new.");
        do
        {
          a2 = Serial.parseInt();
        } while(!a2);
        break;
      case 3:
        Serial.print("Current Wait-Time = "); Serial.print(wTime); Serial.println(". Enter new.");
        do
        {
          wTime = Serial.parseInt();
        } while(!wTime);
        break;
      case 4:
        Serial.print("Current Servo-Pin = "); Serial.print(servPin); Serial.println(". Enter new.");          do
        {
          servPin = Serial.parseInt();
        } while(!servPin);
        myservo.attach(servPin);
        break;
      default:
        Serial.println("Going Back");
    }
    Serial.println("Enter 1 to feed, 2 for setup.");
  }
}
