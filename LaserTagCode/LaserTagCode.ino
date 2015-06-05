
/* Raw IR decoder sketch!
 
 This sketch/program uses the Arduno and a PNA4602 to 
 decode IR received. This can be used to make a IR receiver
 (by looking for a particular code)
 or transmitter (by pulsing an IR LED at ~38KHz for the
 durations detected 
 
 Code is public domain, check out www.ladyada.net and adafruit.com
 for more tutorials! 
 */

// We need to use the 'raw' pin reading methods
// because timing is very important here and the digitalRead()
// procedure is slower!
//uint8_t IRpin = 2;
// Digital pin #2 is the same as Pin D2 see
// http://arduino.cc/en/Hacking/PinMapping168 for the 'raw' pin mapping
#define IRpin_PIN      PIND
#define IRpin          2
//int signals[]={0,0,90,554,90,228,90,228,90,228,90,552,90,228,90,552,90,554,90,554,90,226,90};
//int numpulses;
int life = 500;
int firepin = 8;
int fire= 0;
int lifeleds[]={10,13,3,4,5};
int triggerpin = 12;
int speakerpin = 11;
int hitled = 6;
int laserpin = 7;
int deathcounter;
int halfcycle= 13;
int cycles = 38;
byte names[] = {'c', 'd', 'e', 'f', 'g', 'a', 'b', 'C'};  
int tones[] = {1915, 1700, 1519, 1432, 1275, 1136, 1014, 956};
byte melody[] = "9c9d9e9f9g9C";
byte melody1[] = "2d";
int count = 0;
int count2 = 0;
int count3 = 0;
int MAX_COUNT = 24;
int statePin = LOW;

// the maximum pulse we'll listen for - 65 milliseconds is a long time
#define MAXPULSE 65000

// what our timing resolution should be, larger is better
// as its more 'precise' - but too large and you wont get
// accurate timing
#define RESOLUTION 20 

// we will store up to 100 pulse pairs (this is -a lot-)
uint16_t pulses[100][2];  // pair is high and low pulse 
uint8_t currentpulse = 0; // index for pulses we're storing

void setup(void) {  
  Serial.begin(9600);
  Serial.println("Ready to decode IR!");
  for(int i =0 ; i<5 ; i++)
         {
          pinMode(lifeleds[i],OUTPUT);
         }
         pinMode(hitled,OUTPUT);
  pinMode(firepin,OUTPUT);
  pinMode(laserpin,OUTPUT);
  pinMode(triggerpin,INPUT);
  digitalWrite(laserpin,HIGH);
}  

void loop(void) {
  CheckIR(); 
 if (fire!=0)
 {
   SendPulse();
   Serial.println("SHOOT");
}
CheckTrigger();
 }
 
  


 void CheckIR (void)
 {
   //if you are hit
       if (! (IRpin_PIN & _BV(IRpin))) {
         digitalWrite(hitled,HIGH);
  life-=20;
  updatelife();
  Serial.print("Minus 20");
  Serial.print("You have ");
  Serial.println(life);
   
  
   digitalWrite(hitled,HIGH);
     delayMicroseconds(RESOLUTION);   
       }
      
       if (life<=0)
       {
         Serial.println("You Are Dead");
         digitalWrite(laserpin,LOW);
         Dead();
         life=500;
         Serial.println("You Are Alive");
         digitalWrite(laserpin,HIGH);
       }
       
       //if u are not hit
       else if (IRpin_PIN & (1 << IRpin))
       {
         digitalWrite(hitled,LOW);
//         Serial.print("I am turning on ");
//         Serial.print(life/200);
//         Serial.println(" leds");
         if (life<500)
         {
         life+=1;
          Serial.print("Add 1");
          Serial.print("You got ");
  Serial.println(life);
  updatelife();
         }
         
       }
       
//  return highpulse;
    }
 
 void SendPulse()
{
for (int i=0;i<cycles;i++)
{
   
    digitalWrite(firepin,HIGH);
    delayMicroseconds(halfcycle);
    digitalWrite(firepin,LOW);
    delayMicroseconds(halfcycle-4);
}
if (life<500)
{
life+=20;
}

  }

void CheckTrigger()
{
  if (digitalRead(triggerpin)==HIGH)
  {
    fire=0;
    
  }
  else 
  {
    fire=1;
    
  }
}
void Dead()
 {
    deathcounter++;
    if (deathcounter==5)
    {
      for(int i =0 ; i<5 ; i++)
         {
           digitalWrite(lifeleds[i],HIGH);
         }
      delay(500);
      while (IRpin_PIN & (1 << IRpin))//loop loud noise to prevent cheating
      {
        Serial.println("Game Over");
        DeadSound();
        delay(300);
      }
      deathcounter=0;
       Serial.println("Game Reset");
       for(int i =0 ; i<5 ; i++)
         {
           digitalWrite(lifeleds[i],LOW);
         }
    }
   for (int i=0;i<=10;i++)
   {
     for(int u =0 ; u<5 ; u++)
     
         {
           digitalWrite(lifeleds[u],LOW);
         }
         digitalWrite(hitled,LOW);
         Serial.println("FLASH");
         delay(500);
         for(int u =0 ; u<deathcounter ; u++)
         {
           digitalWrite(lifeleds[u],HIGH);
         }
        
         delay(500);
   }
  
 }
 
 void DeadSound()
 {
   analogWrite(speakerpin, 0);     
  for (count = 0; count < MAX_COUNT; count++) {
    statePin = !statePin;
    
    for (count3 = 0; count3 <= (melody[count*2] - 48) * 30; count3++) {
      for (count2=0;count2<8;count2++) {
        if (names[count2] == melody[count*2 + 1]) {       
          analogWrite(speakerpin,250);
          delayMicroseconds(tones[count2]);
          analogWrite(speakerpin, 0);
          delayMicroseconds(tones[count2]);
        } 
        if (melody[count*2 + 1] == 'p') {
          // make a pause of a certain size
          analogWrite(speakerpin, 0);
          delayMicroseconds(500);
        }
      }
    }
  }
 }
 
 void updatelife()
 {
   for(int i =0 ; i<5 ; i++)
         {
           digitalWrite(lifeleds[i],HIGH);
         }
for(int i = 5 ; i>(life/100) ;i--)
         {
           digitalWrite(lifeleds[i],LOW);
         }
 }
