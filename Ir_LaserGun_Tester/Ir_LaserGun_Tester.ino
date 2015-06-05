
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
int signals[]={0,0,90,554,90,228,90,228,90,228,90,552,90,228,90,552,90,554,90,554,90,226,90};
int numpulses;
int ledpin = 8;
int laserpin = 9;

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
  pinMode(ledpin,OUTPUT);
  pinMode(laserpin,OUTPUT);
}
  

void loop(void) {
  if (! (IRpin_PIN & _BV(IRpin)))
 {
   digitalWrite(ledpin,LOW);
   digitalWrite(laserpin,HIGH);
   int pulselength = checksensor();
    Serial.println("Hit Detected");
  Serial.print("Hit was for ");
  Serial.print(pulselength);
  Serial.println(" secnds");
 }
 if (IRpin_PIN & (1 << IRpin))
 {
   digitalWrite(ledpin,HIGH);
   digitalWrite(laserpin,LOW);
 }
 
  
}

int countpulse(void){
  currentpulse=0;
  while(1){
    uint16_t highpulse, lowpulse;  // temporary storage timing
  highpulse = lowpulse = 0; // start out with no pulse length
  
  
//  while (digitalRead(IRpin)) { // this is too slow!
    while (IRpin_PIN & (1 << IRpin)) {
     // pin is still HIGH

     // count off another few microseconds
     highpulse++;
     delayMicroseconds(RESOLUTION);

     // If the pulse is too long, we 'timed out' - either nothing
     // was received or the code is finished, so print what
     // we've grabbed so far, and then reset
     if ((highpulse >= MAXPULSE) && (currentpulse != 0)) {
       return currentpulse;
     }
  }
  // we didn't time out so lets stash the reading
  pulses[currentpulse][0] = highpulse;
  
  // same as above
  while (! (IRpin_PIN & _BV(IRpin))) {
     // pin is still LOW
     lowpulse++;
     delayMicroseconds(RESOLUTION);
     if ((lowpulse >= MAXPULSE)  && (currentpulse != 0)) {
      
       return currentpulse;
     }
  }
  pulses[currentpulse][1] = lowpulse;

  // we read one high-low pulse successfully, continue!
  currentpulse++;
}
  }

 int checksensor (void)
 {
  
    while(1){
       uint16_t highpulse, lowpulse;  // temporary storage timing
  highpulse = 0; // start out with no pulse length
     
       
       while (! (IRpin_PIN & _BV(IRpin))) {
         
   
  highpulse++;
     delayMicroseconds(RESOLUTION);
         
       }
       
  return highpulse;
    }
 }
 
