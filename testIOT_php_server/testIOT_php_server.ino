#include <ESP8266WiFi.h>
#include <ArduinoJson.h>

const char* ssid = "TP-Gau";
const char* password = "Minhhanh7166";

const char* host     = "vast-caverns-86157.herokuapp.com"; // Your domain  
String path          = "/light.json";  
//const int pin        = 5;

void setup() {  
  Serial.begin(115200);
  
  delay(10);
  Serial.println(WiFi.localIP());
  pinMode(D1, OUTPUT); 
  digitalWrite(D1, LOW);
  

  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid,password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("WiFi connected");  
 // Serial.println("IP address: " + WiFi.localIP());
}

void loop() {  

  Serial.print("connecting to ");
  Serial.println(host);
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    delay(500);
    Serial.println("connection failed");
    return;
  }

  client.print(String("GET ") + path + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: keep-alive\r\n\r\n");

  delay(500); // wait for server to respond

  // read response
  String section="header";
  while(client.available()){
    delay(500);
    String line = client.readStringUntil('\r');
    // Serial.print(line);
    // we’ll parse the HTML body here
    if (section=="header") { // headers..
      Serial.print(".");
      if (line=="\n") { // skips the empty space at the beginning 
        section="json";
      }
    }
    else if (section=="json") {  // print the good stuff
      section="ignore";
      String result = line.substring(1);

      // Parse JSON
      int size = result.length() + 1;
      char json[size];
      result.toCharArray(json, size);
      StaticJsonBuffer<200> jsonBuffer;
      JsonObject& json_parsed = jsonBuffer.parseObject(json);
      if (!json_parsed.success())
      {
        Serial.println("parseObject() failed");
        return;
      }
  const char* x= json_parsed["brightness"];
  int c = (int)x;
  String str(x);
  Serial.println(str.toInt());
  Serial.println(x);
  Serial.println(c);
      // Make the decision to turn off or on the LED
      if (strcmp(json_parsed["brightness"],x) == 0) {
        digitalWrite(D1, HIGH); 
        analogWrite(D1,str.toInt());
        Serial.println("LED ON");
      }
      else if(strcmp(json_parsed["light"], "on") == 0){
         analogWrite(D1,str.toInt());
          Serial.println("LED ON1");
        }
        
      else {
        digitalWrite(D1, LOW);
        Serial.println("led off");
      }
    }
  }
  Serial.print("closing connection. ");
}

