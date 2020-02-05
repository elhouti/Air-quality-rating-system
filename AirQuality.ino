#include <SoftwareSerial.h>
#include <maker_mq5.h>

SoftwareSerial ESP8266(10, 11);

String ssid = "wifi_host";
String pwd = "wifi_mdp";
String remoteServerIP = "@remote_server";
int remoteServerPort = 3000;
const int BEEPER_PIN = 8;
int counter=0;
bool stop=false;

String resp;
float t=0;

void setup() {
  Serial.begin(115200);
  initESP8266();
  initBeeper();
  delay(3000);
  pinMode(12, INPUT);
}

void loop() {
  t = analogRead(A0);
  if(t>200)beep();
  Serial.print(F("Airquality = "));
  Serial.println(t);
  sendESP8266(t);
}

void initESP8266() { 
  ESP8266.begin(115200);   
  Serial.println("Initialisation Wifi ...");

  ESP8266.println("AT+RST");
  readESP8266(2000);
  Serial.println("Reset done.");
  
  ESP8266.println("AT+CWMODE=3");
  readESP8266(5000);
  Serial.println("Mode config done.");
  
  ESP8266.println("AT+CWJAP=\""+ ssid + "\",\"" + pwd +"\"");
  readESP8266(10000);
  Serial.println("Wifi config done.");
  
  ESP8266.println("AT+CIFSR");
  readESP8266(1000);
  Serial.println("Ip @ done.");
  
  ESP8266.println("AT+CIPSERVER=1,80");
  readESP8266(1000);
  Serial.println("Server config at port 80 done.");
  
  Serial.println("Initialisation finished.");
}



void readESP8266(const int timeout) {
  resp = "";
  long int time = millis();
  while((time + timeout) > millis()) {
    while(ESP8266.available()) {
      resp += ESP8266.readString();
      Serial.println(resp);
      if(resp.indexOf("404")>0){
        stop=!stop;
        if(stop==true)digitalWrite(BEEPER_PIN, LOW);
      }
    }
  }
  Serial.println(resp);   
}

void sendESP8266(float val) {
  ESP8266.println("AT+CIPMUX=1");
  delay(1000);
  readESP8266(1000);
  ESP8266.println("AT+CIPSTART=4,\"TCP\",\"" + remoteServerIP + "\"," + String(remoteServerPort));
  delay(1000);
  readESP8266(1000);

  String req = "GET /?field1="+String(val)+" HTTP/1.1";
  ESP8266.println("AT+CIPSEND=4," + String(req.length() + 4));
  delay(1000);
  readESP8266(1000);
  
  ESP8266.println(req);
  delay(1000);
  readESP8266(2000);
  
  ESP8266.println("");
  delay(1000);
  readESP8266(2000);
}

// ----- Beeper Fucntions -----

void initBeeper() {
  Serial.println("Beeper Initialisation...");
  pinMode(BEEPER_PIN, OUTPUT);
  digitalWrite(BEEPER_PIN, LOW);
  Serial.println("Beeper Initialisation done.");
}

void beep() {
  if(!stop){
    digitalWrite(BEEPER_PIN, HIGH);
  }
}

// ------------------------------
