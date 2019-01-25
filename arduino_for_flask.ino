#include  <dht.h>
dht DHT;
// sensors pins :
int greenled = 2;
int redled = 3;
int relay = 13;
int dht_11sensor = 5;
int lock = 6;
int door = 8;

String serialin="ON";// a string to hold incoming data
String alarm ="ON";
// the setup routine runs once when you press reset:
void setup() {

  Serial.begin(9600);
  serialin.reserve(2);

  pinMode(greenled, OUTPUT);
  pinMode(redled,OUTPUT);
  pinMode(relay,OUTPUT);
  pinMode(lock ,INPUT_PULLUP);
  pinMode(door,INPUT_PULLUP);
}
 void loop() {

 serialin.trim();

 if ( (serialin.equalsIgnoreCase("OFF"))||(serialin.equalsIgnoreCase("ON"))){
      alarm=serialin;}  // that is to make sure data are correct no matter what, it's a bug fix, the program crashes when it gets anything else than it should


  int chk = DHT.read11(dht_11sensor);
//  Serial.println("Folowing string : temperature,humidity,doors(1 if open),alarm(on or off)");
  Serial.print(DHT.temperature);
  Serial.print(",");
  Serial.print(DHT.humidity);
  Serial.print(",");
  Serial.print(digitalRead(door)); // prints 1 if door open
  Serial.print(",");
  Serial.print(alarm);
  Serial.print(",");
  delay(1000);

  // controler is OFF red led is ON :
  if(digitalRead(lock)==HIGH){
    digitalWrite(redled,HIGH);
    digitalWrite(relay,LOW);
   Serial.println("M-OFF,");
   delay(1000);
   }
 // alarm is set to off controler is on green led blinking :
    else if ((alarm.equalsIgnoreCase("OFF"))&&(digitalRead(lock)==LOW)){
    Serial.println("M-ON");  // manualy ON
    digitalWrite(greenled,LOW);
    digitalWrite(redled,LOW);
    delay(1000);
    digitalWrite(greenled,HIGH);
    delay(2000);
    digitalWrite(greenled,LOW);
    }
//alarm is set to on controller is on, doors are open :
      else if ((alarm.equalsIgnoreCase("ON"))&&(digitalRead(lock)==LOW)&&(digitalRead(door)==1)){
      Serial.println("M-ON");
	    delay(5000);     // 5 seconds chance :) kind of backdoor :)
	    if((alarm.equalsIgnoreCase("ON"))&&(digitalRead(lock)==LOW)&&(digitalRead(door)==1)){
//	digitalWrite(redled,LOW);
        delay(1000);
        digitalWrite(relay,HIGH);
        for (int i=0; i<40 ; i++){
        Serial.println("alarm!");
        delay(30000);    // set in whatever value you want alarm yields - 20 mins of loudness is 1200000
        }
        digitalWrite(relay,LOW);
      }
      }
//alarm has been set to on ,controller is on ,doors closed:
    else if ((alarm.equalsIgnoreCase("ON"))&&(digitalRead(lock)==LOW)&&(digitalRead(door)==0)){
     Serial.println("M-ON");
     digitalWrite(redled,LOW);
     digitalWrite(greenled,HIGH);
     delay(50);
     digitalWrite(greenled,LOW);
     delay(300);
    }
}
/*
  SerialEvent occurs whenever a new data comes in the hardware serial RX.  This routine is run between each time loop() runs,
 so using delay inside loop can delay
 response.  Multiple bytes of data may be available.
 */
void serialEvent() {
   digitalWrite(redled, HIGH);
  delay(100);
    digitalWrite(redled,LOW);
    digitalWrite(greenled,HIGH);
  delay(100);
    digitalWrite(redled,HIGH);
    digitalWrite(greenled,LOW);
  delay(100);
     digitalWrite(redled,LOW);
     digitalWrite(greenled,HIGH);
  delay(100);
      digitalWrite(greenled,LOW);
      digitalWrite(relay,HIGH);
  delay(100);
      digitalWrite(relay,LOW);

  serialin="";
  while (Serial.available()) {
    char inChar = Serial.read();
    serialin += inChar;
  }
}
