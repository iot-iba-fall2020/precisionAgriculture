#include <WiFiClient.h> 
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
#include "DHTesp.h"
DHTesp dht;

const char *ssid = "argo";  //ENTER YOUR WIFI ssid
const char *password = "argo123456";  //ENTER YOUR WIFI password

int moisture = 0;
float temprature = 0;
float humidity = 0;
bool isWatering = false;
bool isHeating = false;
int moterPIN = 13;

void setup() {
connectWifi();
dht.setup(4, DHTesp::DHT11);
}


void loop() {
getMoisture();
getTempratureAndHumidity();
SendSensorData();
digitalWrite(moterPIN, !isWatering);
delay(15000);  //Post Data at every 15 seconds
}
//function to connect to wifi
void connectWifi(){
  delay(1000);
  Serial.begin(115200);
  WiFi.mode(WIFI_OFF);        //Prevents reconnection issue (taking too long to connect)
  delay(1000);
  WiFi.mode(WIFI_STA);        //This line hides the viewing of ESP as wifi hotspot
  WiFi.begin(ssid, password);     //Connect to your WiFi router
  Serial.println("");
  Serial.print("Connecting");
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  //If connection successful show IP address in serial monitor 
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());  //IP address assigned to your ESP
}


//function to send sensor data 
void SendSensorData() {
  HTTPClient http;    //Declare object of class HTTPClient
  String Temprature,Humidity,Moisture,isWatering,lightIntensity,isHeating,batteryStatus, postData;
  Temprature=String(temprature);
  Humidity=String(humidity);
  Moisture=String(moisture);
  isWatering=String(-1);
  lightIntensity=String(-1);
  isHeating=String(-1);
  batteryStatus=String(-1);
  
  //Post Data
  postData = "temprature=" +  Temprature + "&humidity=" + Humidity+ "&moisture=" + Moisture+  "&isWatering=" + isWatering+ "&lightIntensity=" + lightIntensity+ "&isHeating=" + isHeating+  "&batteryStatus=" + batteryStatus;
  
  http.begin("http://192.168.4.1/log");              //change the ip to your computer ip address
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");    //Specify content-type header
 
  int httpCode = http.POST(postData);   //Send the request
  String payload = http.getString();    //Get the response payload

  //processs pay load to adjust watering and heating
  
  Serial.println(postData);
  Serial.println(httpCode);   //Print HTTP return code
  Serial.println(payload);    //Print request response payload

  http.end();  //Close connection
}


int getMoisture(){
  moisture = analogRead(0); 
}

int getTempratureAndHumidity(){
    delay(dht.getMinimumSamplingPeriod() + 500);
    humidity = dht.getHumidity();
    temprature = dht.getTemperature();
}
