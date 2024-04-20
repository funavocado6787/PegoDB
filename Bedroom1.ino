#include <ESP8266WiFi.h> 
#include <Servo.h>
#include <FirebaseArduino.h> 
#define FIREBASE_HOST "myhome-e0693.firebaseio.com" 
#define FIREBASE_AUTH "" 
#define WIFI_SSID "Gomiphone" 
#define WIFI_PASSWORD "88888888" 

int Connect=0;
String fan, tam_fan,
       lamp, tam_lamp,
       light, tam_light,
       window, tam_window,
       doorbed, tam_doorbed;
String Status;

#define SERVO1 D1 // Cửa sổ BedRoom1
#define SERVO2 D2 // Cửa sổ BedRoom1
#define SERVO3 D3 // Cửa chính BedRoom1
Servo gServo1;
Servo gServo2;
Servo gServo3;
int lightstatus = D4;
int lampstatus = D5;  
int fanstatus = D6;
 
void setup()
{
  Serial.begin(115200);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD); 
  Serial.print("Dang ket noi ");
  Serial.print(WIFI_SSID);
  while (WiFi.status() != WL_CONNECTED) 
  {
    Serial.print(".");
    delay(100);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);; 
  Firebase.setString("Home/BedRoom1/fan", "OFF"); 
  Firebase.setString("Home/BedRoom1/lamp", "OFF");
  Firebase.setString("Home/BedRoom1/light", "OFF");
  Firebase.setString("Home/BedRoom1/window", "OFF");
  Firebase.setString("Home/BedRoom1/doorbed", "OFF");
  gServo1.attach(SERVO1);
  Serial.begin(115200);
  gServo2.attach(SERVO2);
  Serial.begin(115200); 
  gServo3.attach(SERVO3);
  Serial.begin(115200);
  gServo1.write(180);
  gServo2.write(0);
  gServo3.write(0);  
  pinMode(lightstatus, OUTPUT);
  pinMode(lampstatus, OUTPUT);
}
     
void CHECK_CHANGE()
{
  if( fan != tam_fan || lamp != tam_lamp || 
      light != tam_light || window != tam_window || 
      doorbed != tam_doorbed
    )
    {
      tam_fan = fan;
      tam_lamp = lamp;
      tam_light = light;
      tam_window = window;
      tam_doorbed = doorbed;
      
      Status = fan + ", " + lamp + ", " + light + ", " + window + ", " + doorbed;
     Serial.print(Status);
     Serial.print("\n");
     }
     else
     {}  
}

void loop() 
{
  if (Firebase.failed()) 
  { 
    Serial.print("setting /number failed:"); 
    Serial.println(Firebase.error());   
    return; 
  }
  if (WiFi.status() != WL_CONNECTED) 
  {
    fan = "OFF";
    lamp = "OFF";
    light = "OFF";
    window = "OFF";
    doorbed = "OFF";
    CHECK_CHANGE();
    Connect = 1;
  }
  else
  {
    if(Connect == 1)
    {
      Firebase.setString("Home/BedRoom1/fan", "OFF"); 
      Firebase.setString("Home/BedRoom1/lamp", "OFF");
      Firebase.setString("Home/BedRoom1/light", "OFF");
      Firebase.setString("Home/BedRoom1/window", "OFF");
      Firebase.setString("Home/BedRoom1/doorbed", "OFF");
      Connect = 0;
    }
  }
  fan = Firebase.getString("Home/BedRoom1/fan"); 
  lamp = Firebase.getString("Home/BedRoom1/lamp");
  light = Firebase.getString("Home/BedRoom1/light");
  window = Firebase.getString("Home/BedRoom1/window");
  doorbed = Firebase.getString("Home/BedRoom1/doorbed");
  CHECK_CHANGE();

  if (window == "ON" || window == "on") 
  {    
    Serial.println("MO CUA SO BEDROOM1");
    gServo1.write(180);
    gServo2.write(0);
  }
  else if (window == "OFF" || window == "off") 
  {            
    Serial.println("DONG CUA SO BEDROOM1");
    gServo1.write(0);
    gServo2.write(180);
  }
  if (light == "ON" || light == "on") 
  { 
    Serial.println("MO DEN BEDROOM1");   
   // digitalWrite(LED_BUILTIN, LOW); 
    digitalWrite(lightstatus, HIGH); 
  }
  else if (light == "OFF" || light == "off") 
  {
    Serial.println("TAT DEN BEDROOM1");             
    //digitalWrite(LED_BUILTIN, HIGH); 
    digitalWrite(lightstatus, LOW);
  }

  if (lamp == "ON" || lamp == "on") 
  {
    Serial.println("MO DEN NGU BEDROOM1");     
    digitalWrite(lampstatus, HIGH); 
  }
  else if (lamp == "OFF" || lamp == "off") 
  {
    Serial.println("TAT DEN NGU BEDROOM1");
    Serial.println("\n");             
    digitalWrite(lampstatus, LOW);
  }
  
  if (doorbed == "ON" || doorbed == "on") 
  {
    Serial.println("MO CUA BEDROOM1");     
    gServo3.write(0);
  }
  else if (doorbed == "OFF" || doorbed == "off") 
  {
    Serial.println("DONG CUA BEDROOM1");
    Serial.println("\n");            
    gServo3.write(180);
  }
  
  if (fan == "ON" || fan == "on") 
  {    
    digitalWrite(fanstatus, HIGH); 
  }
  else if (fan == "OFF" || fan == "off") 
  {            
    digitalWrite(fanstatus, LOW);
  }
}
