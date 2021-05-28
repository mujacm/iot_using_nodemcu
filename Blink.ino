

#include <ESP8266HTTPClient.h> //http req
#include <ESP8266WiFi.h> //connect module to wifi
#include <ArduinoJson.h>

char jsonOutput[128];



void setup() {
  Serial.begin(115200);                 //Serial connection
  WiFi.begin("usrname","password");   //WiFi connection // first is your wifi name // second is your wifi password

  while (WiFi.status() != WL_CONNECTED) {  //Wait for the WiFI connection completion

    delay(500);
    Serial.println("Waiting for connection");

  }
  pinMode(D5, OUTPUT);     // Initialize the D5 pin as an output
}

// the loop function runs over and over again forever
void loop() {
  
  digitalWrite(D5, LOW);   // Turn the LED on (Note that LOW is the voltage level
 
  if(WiFi.status() == WL_CONNECTED) // IF WIFI IS CONNECTED
  {
    
    HTTPClient http;    //Declare object of class HTTPClient
    const size_t CAPACITY = JSON_OBJECT_SIZE(1);
    StaticJsonDocument <CAPACITY> doc;
    JsonObject object = doc.to<JsonObject>();
    object["msg"] = "hello"; // we send this data to the server
    serializeJson(doc,jsonOutput);
    
    http.begin("yourlocalhosturl");      //Specify request destination
    http.addHeader("Content-Type", "application/json");
    int httpCode = http.POST(String(jsonOutput));   //Send the request
    String payload = http.getString();   //Get the response payload
    StaticJsonDocument <256> ret;
    deserializeJson(ret,payload);
    const char* message = ret["msg"];
    const char* postData = ret["data"];
    int state = ret["ledState"]; // object field in which stores states of LED 
    
    Serial.println(httpCode);   //Print HTTP return code
    Serial.print(message);  
    Serial.print(postData); 
    if(state == 1)
    {
      digitalWrite(D5,HIGH);
      delay(1000);
    }
    else
    {
      Serial.print("no change");
    }
 
    http.end();  //Close connection
    
  }
  else 
  {
    Serial.print("not connected.");
  }
  
  delay(20000); 
}
