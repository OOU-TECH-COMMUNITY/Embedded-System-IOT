#include <SoftwareSerial.h>

//Create software serial object to communicate with SIM800L
SoftwareSerial sim(3, 2); //SIM800L Tx & Rx is connected to Arduino #3 & #2

const int buzzer = 13

const int Button = 12;

const int Sensor= 11;

const int Led = 10;

void setup(){

  //Begin all communications with Arduino at 9600
  Serial.begin(9600);
  sim.begin(9600);

  pinMode (Button,INPUT);
  
  pinMode (Sensor,INPUT);

  pinMode (Led, OUTPUT);

  pinMode (buzzer, OUTPUT);

  
  delay(3000);
  }
  
void loop()
{
   
     int But_state = digitalRead(Button); 
     int Sensor_state =digitalRead(Sensor);  

     delay(500);      

     if ( But_state == 0) {
        digitalWrite(buzzer, HIGH);
        digitalWrite(Led, LOW);
        Call_Landlord();
                      
      }   

      else{
        digitalWrite(Led, LOW);
        digitalWrite(buzzer, LOW);
      }
        //delay(1000);  
            
      if (Sensor_state == 0) {
        
        DropAmessage(); 
      }         

}


void test_sim800_module()
{
  //Serial.println("LOOP PREPARING");
  sim.println("AT");
  Serial.println(  sim.println("AT"));
  updateSerialsim();
  Serial.println();
  sim.println("AT+CSQ");
  updateSerialsim();
  sim.println("AT+CCID");
  updateSerialsim();
  sim.println("AT+CREG?");
  updateSerialsim();
  sim.println("ATI");
  updateSerialsim();
  sim.println("AT+CBC");
  updateSerialsim();
}

void updateSerialsim()
{
  delay(100);
  //Serial.println("PREPARING up");
  while (Serial.available())
  {
    sim.write(Serial.read());//Forward what Serial received to Software Serial Port
  }
  while (sim.available())
  {
    Serial.write(sim.read());//Forward what Software Serial received to Serial Port
  }
}


void Call_Landlord(){

  sim.println("AT"); //Once the handshake test is successful, i t will back to OK
  updateSerialsim();
  Serial.println("Call in progress");
  sim.println("ATD+ +234----------;");    
  updateSerialsim();
  delay(20000); // wait for 20 seconds...
  sim.println("ATH"); //hang up
  updateSerialsim();
  delay(2000);
}

void DropAmessage()
{
  sim.println("AT+CMGF=1"); // Configuring TEXT mode
  updateSerialsim();
  sim.println("AT+CMGS=\"+234----------\"");//change ZZ with country code and xxxxxxxxxxx with phone number to But_state
  updateSerialsim();
  sim.print("There is an intruder around" ); //text content
  
  updateSerialsim();
  
  Serial.println();
  Serial.println("Message Sent");
  sim.write(26);
  Serial.println(" Sent");
  delay(1000);
  delay(2000);
}
