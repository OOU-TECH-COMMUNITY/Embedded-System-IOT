#define TINY_GSM_MODEM_SIM800
#include <TinyGPSPlus.h>
#include <TinyGsmClient.h>

HardwareSerial sim(2);
TinyGsm modem (sim);

int Sms_pin = 23;

int Call_pin = 22;

void setup(){
  Serial.begin(9600);
  sim.begin(9600, SERIAL_8N1,16,17);

  pinMode (Sms_pin,INPUT_PULLUP);
  
  pinMode (Call_pin,INPUT_PULLUP);
  
  delay(3000);
  }
  
void loop()
{
   
     int sms = digitalRead(Sms_pin); 
     int call =digitalRead(Call_pin);
     Call();     

     delay(500);      

     if ( sms == 0) {
        send_SMS();
              
      }   

        //delay(1000);  
            
      if (call == 0) {
        Call();
          
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


void Call(){

  sim.println("AT"); //Once the handshake test is successful, i t will back to OK
  updateSerialsim();
  Serial.println("Calling");
  //sim.println("ATD+ +2349065458791;"); //  change ZZ with country code and xxxxxxxxxxx with phone number to dial
  //sim.println("ATD+ +2349131841928;");  
  sim.println("ATD+ +2347036074909;");    
  updateSerialsim();
  delay(20000); // wait for 20 seconds...
  sim.println("ATH"); //hang up
  updateSerialsim();
  sim.begin(9600, SERIAL_8N1,16,17);
  Blynk.begin (auth, modem, apn, user, pass);  
  delay(2000);
}

void send_SMS()
{
  sim.println("AT+CMGF=1"); // Configuring TEXT mode
  updateSerialsim();
  //sim.println("AT+CMGS=\"+2349131841928\"");//change ZZ with country code and xxxxxxxxxxx with phone number to sms  
  sim.println("AT+CMGS=\"+2347036074909\"");//change ZZ with country code and xxxxxxxxxxx with phone number to sms
  updateSerialsim();
  sim.print("I need help please " );
  //sim.print(" Dem dey carry me go where i no know " );  
  sim.print("http://maps.google.com/maps?q=loc:" ); //text content
  sim.print((gps.location.lat()) ); //text content
  sim.print("," ); //text content
  sim.print(( gps.location.lng()) ); //text content
  
  updateSerialsim();
  
  Serial.println();
  Serial.println("Message Sent");
  sim.write(26);
  Serial.println(" Sent");
  delay(1000);
  sim.begin(9600, SERIAL_8N1,16,17);
  Blynk.begin (auth, modem, apn, user, pass);  
  delay(2000);
}
