#define BLYNK_TEMPLATE_ID "TMPLDF1sIRDL"
#define BLYNK_TEMPLATE_NAME "Liquid meter"
#define BLYNK_AUTH_TOKEN "O1yV3In9dXQ6_IMGl1Oe-I-6Qg0eLbe5"
#define BLYNK_PRINT Serial
#define TINY_GSM_MODEM_SIM800
#include <TinyGsmClient.h>
#include <BlynkSimpleTinyGSM.h>
#include <LiquidCrystal.h>

HardwareSerial sim(1);
TinyGsm modem (sim);

char auth[] = "O1yV3In9dXQ6_IMGl1Oe-I-6Qg0eLbe5";
char apn[]  = "web.gprs.mtnnigeria.net";
char user[] = "";
char pass[] = "";

BlynkTimer timer;
int pointIndex = 1; 

const int TRIGGER_PIN = 5; // ultrasonic sensor trigger pin on arduino

const int ECHO_PIN = 18; //ultrasonic sensor echo pin on arduino

float lengthToLiquid =0.00; // Time before pluse is recieved from the sensor

long Time;
float Level;

void setup(){
  
  Serial.begin(9600);
    
  sim.begin(9600, SERIAL_8N1,16,17);

  Blynk.begin (auth, modem, apn, user, pass);
   

  pinMode(TRIGGER_PIN, OUTPUT);
  // Sets trigger pin as input on the arduino
  pinMode(ECHO_PIN, INPUT);  
  
  delay(3000);
  lcd.clear();      
  delay(100);
  
  }
  
void loop()
{  
   syncWithIot();
   Blynk.run();
   timer.run();   

}

void syncWithIot()
{
  digitalWrite(TRIGGER_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIGGER_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER_PIN, LOW);

  // recieve pulse time from the ultasonic sensor
  Time = pulseIn(ECHO_PIN, HIGH);

  // divide the Time by 2 to get time taken to the surface of water
  // then multiply with the speed of sound in centimeters
  lengthToLiquid = ((Time * 0.034) / 2);
  Level = map(lengthToLiquid,44,0,0,100);    
  Serial.println(lengthToLiquid);
  
  display(Level);
  Blynk.virtualWrite(V0, lengthToLiquid);
  Blynk.virtualWrite(V1, Level); 

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

void send_SMS()
{
  sim.println("AT+CMGF=1"); // Configuring TEXT mode
  updateSerialsim();
  sim.println("AT+CMGS=\"+2348033510338\"");//change ZZ with country code and xxxxxxxxxxx with phone number to sms
  updateSerialsim();
  sim.print("LOW LIQUID LEVEL" );
  sim.print(":" );  
  sim.print(Level ); //text content
  sim.print("Refil Liquid"); //text content
  
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
