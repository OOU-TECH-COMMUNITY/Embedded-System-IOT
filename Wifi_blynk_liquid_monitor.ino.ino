#define BLYNK_TEMPLATE_ID "---------------" // Copy from your Blynk profile
#define BLYNK_TEMPLATE_NAME "Template Name" // Copy from your Blynk profile
#define BLYNK_AUTH_TOKEN "--------------------------------" // Copy from your Blynk profile


#include <BlynkSimpleTinyGSM.h>
#include <LiquidCrystal.h>
#include <WiFi.h>

#define WIFI_SSID "SWEP 2023" // Change this to your SSID
#define WIFI_PASS "IoTSwep2023" // // Change this to your password

BlynkTimer timer;

int pointIndex = 1; 

const int TRIGGER_PIN = 18; // ultrasonic sensor trigger pin on Esp32

const int ECHO_PIN = 5; //ultrasonic sensor echo pin on Esp32

float DepthOfLiquid = 0.00; // Time before pluse is recieved from the sensor

long Time;
float Level;

void setupWiFi()
{
  Serial.printf("\r\n[Wifi]: Connecting");
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.printf(".");
    delay(250);
  }
  digitalWrite(wifiLed, HIGH);
  Serial.printf("connected!\r\n[WiFi]: IP-Address is %s\r\n", WiFi.localIP().toString().c_str());
}

void setup(){
  
  Serial.begin(9600);
    
  setupWiFi();

  Blynk.begin (auth,WIFI_SSID,WIFI_PASS);
   

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
  DepthOfLiquid = ((Time * 0.034) / 2);
  Level = map(DepthOfLiquid,44,0,0,100);    
  Serial.println(DepthOfLiquid);
  
  display(Level);
  Blynk.virtualWrite(V0, DepthOfLiquid);
  Blynk.virtualWrite(V1, Level); 

}  
 
