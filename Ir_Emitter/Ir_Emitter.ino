#include <DigitShield.h>


int val;
int length=13;
const int ledPin = 5;
#define irPin  4
const int switchPin = 13;

void setup()
{
pinMode(ledPin, OUTPUT);
pinMode(irPin,OUTPUT);
pinMode(switchPin, INPUT);
}

void loop()
{
val = digitalRead(switchPin);
if (val==HIGH)
  {
digitalWrite(ledPin,HIGH);
while (val==HIGH)
  {
    val = digitalRead(switchPin);
    digitalWrite(irPin,HIGH);
    delay(length);
    digitalWrite(irPin,LOW);
    delay(length);
  }
  }
else
  {
digitalWrite(ledPin,LOW);
  }
}

