#include <SoftwareSerial.h>       //Including the software serial library
#include <ESP8266_Lib.h>
#include <BlynkSimpleShieldEsp8266.h>
#include <PulseSensorPlayground.h>   
#include <LiquidCrystal.h>

#define USE_ARDUINO_INTERRUPTS true
#define BLYNK_PRINT Serial 

PulseSensorPlayground pulseSensor;
LiquidCrystal lcd(12, 11, 5, 4, 3, 2); 

ESP8266 wifi(&esp8266);

const int PulseWire = 0;       // PulseSensor PURPLE WIRE connected to ANALOG PIN 0
const int LED13 = 13;          // The on-board Arduino LED, close to PIN 13.
int Threshold = 550;           // Determine which Signal to "count as a beat" and which to ignore.
                               // Use the "Gettting Started Project" to fine-tune Threshold Value beyond default setting.
                               // Otherwise leave the default "550" value. 


const int sensor=A5; // Assigning analog pin A5 to variable 'sensor'
float tempc; //variable to store temperature in degree Celsius
float tempf; //variable to store temperature in Fahreinheit
float vout; //temporary variable to hold sensor reading


char auth[] = "XMCOJwKfySl3TPH6IsyqyNOiRc8s3Bl_"; 
char ssid[] = "farhah_atiq";    /* Boleh Pakai */
char pass[] = "farhah5438";           /* Boleh Pakai */

void setup() 
{
  Serial.begin(115200);          // For Serial Monitor
  esp8266.begin(9600);
  
  Serial.print("System initial..");
  Serial.println();

  Blynk.begin(auth, wifi, ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
  delay(500);
  Serial.print(".");
  }

  // Configure the PulseSensor object, by assigning our variables to it. 
  pulseSensor.analogInput(PulseWire);   
  pulseSensor.blinkOnPulse(LED13);       //auto-magically blink Arduino's LED with heartbeat.
  pulseSensor.setThreshold(Threshold);   

  // Double-check the "pulseSensor" object was created and "began" seeing a signal. 
   if (pulseSensor.begin()) {
    Serial.println("  We created a pulseSensor Object !");  //This prints one time at Arduino power-up,  or on Arduino reset.  
  }
  
pinMode(sensor,INPUT); // Configuring sensor pin as input
lcd.begin(16, 2);         // initializes the 16x2 LCD
lcd.setCursor(2,0);       // sets the cursor at column 2 row 0
lcd.print("TEMP :"); // prints temperature
lcd.setCursor(2,1);       // sets the cursor at column 2 row 1
lcd.print("BPM :"); // prints temperature
}

void loop() 
{
  Blynk.run();
  int myBPM = pulseSensor.getBeatsPerMinute();  // Calls function on our pulseSensor object that returns BPM as an "int".
                                               // "myBPM" hold this BPM value now. 

  if (pulseSensor.sawStartOfBeat()) {            // Constantly test to see if "a beat happened". 
  Serial.println("  ♥  A HeartBeat Happened ! "); // If test is "true", print a message "a heartbeat happened".
  Serial.print("BPM: ");                        // Print phrase "BPM: " 
  Serial.println(myBPM);                        // Print the value inside of myBPM. 
  }

  delay(20);                    // considered best practice in a simple sketch.

  vout=analogRead(sensor); //Reading the value from sensor
  vout=(vout*5.0)/1024.0;
  tempc=(vout-0.5)*100; // Storing value in Degree Celsius
  tempf=(tempc*9.0/5.0)+32.0; // Converting to Fahrenheit

  Serial.print("in DegreeC=");
  Serial.print("\t");
  Serial.print(tempc);
  Serial.print(" ");
  Serial.print("in Fahrenheit=");
  Serial.print("\t");
  Serial.print(tempf);
  Serial.println();
  delay(500); //Delay of 1 second for ease of viewing

  lcd.setCursor(8,0);   // sets cursor at column 0 row 0
  lcd.print(tempc);      // prints temperature in degree Celsius
  lcd.print((char)223); // prints degree sign
  lcd.print("C");       // prints letter c
  lcd.setCursor(8,0);   // sets cursor at column 8 row 1
  lcd.print(tempf);     // prints temperature in degree Fahrenheit
  lcd.print((char)223); // prints degree sign
  lcd.print("F");       // prints letter F
  lcd.setCursor(8,1);   // sets cursor at column 8 row 1
  lcd.print(myBPM); 
  lcd.print((char)223);
  delay(1000);          // 1 second delay

  Blynk.virtualWrite (V1, tempc);
  Blynk.virtualWrite (V2, myBPM);
}
