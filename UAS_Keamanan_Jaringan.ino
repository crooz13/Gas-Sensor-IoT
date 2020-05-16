#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <PubSubClient.h>

int  i;
WiFiClient espClient;
PubSubClient client(espClient);

////////// deklarasi API

const char *hosts = "http://192.168.43.213/uaspakharnan/dbwrite.php";
String data, postData;

////////// koneksi ke broker
void reconnectmqttserver() {
while (!client.connected()) {
Serial.print("Attempting MQTT connection...");
String clientId = "ESP8266Client-";
 clientId += String(random(0xffff), HEX);
if (client.connect(clientId.c_str())) {
Serial.println("connected");
} else {
Serial.print("failed, rc=");
Serial.print(client.state());
Serial.println(" try again in 5 seconds");
delay(5000);
}
}
}

//////////// disini untuk data broker
int persentase = random(20, 30);

char msgmqtt1[255];
char msgmqtt2[255];
void callback(char* topic, byte* payload, unsigned int length) {
  String MQTT_DATA = "";
  for (int i=0;i<length;i++) {
   MQTT_DATA += (char)payload[i];}
}

void setup()
{
  i = 0;
Serial.begin(9600);
  WiFi.disconnect();
  delay(3000);
  Serial.println("START");
   WiFi.begin("Anjay","mantabjiwa1234");
  while ((!(WiFi.status() == WL_CONNECTED))){
    delay(300);
    Serial.print("..");

  }
  Serial.println("Connected");
  Serial.println("Your IP is");
  Serial.println((WiFi.localIP().toString()));
  client.setServer("192.168.43.213", 1883);
  client.setCallback(callback);

}


void loop()
{

HTTPClient http;    // http object of clas HTTPClient


// Convert integer variables to string
data = String(persentase);     

 
postData = "data=" + data;

// We can post values to PHP files as  example.com/dbwrite.php?name1=val1&name2=val2&name3=val3
// Hence created variable postDAta and stored our variables in it in desired format
// For more detials, refer:- https://www.tutorialspoint.com/php/php_get_post.htm

// Update Host URL here:-  
  
http.begin(hosts);              // Connect to host where MySQL databse is hosted
http.addHeader("Content-Type", "application/x-www-form-urlencoded");            //Specify content-type header

  
 
int httpCode = http.POST(postData);   // Send POST request to php file and store server response code in variable named httpCode
Serial.println("Hasil adalah, data = " + data);


// if connection eatablished then do this
if (httpCode == 200) { Serial.println("Values uploaded successfully."); Serial.println(httpCode); 
String webpage = http.getString();    // Get html webpage output and store it in a string
Serial.println(webpage + "\n"); 
}


// if failed to connect then return and restart

else { 
  Serial.println(httpCode); 
  Serial.println("Failed to upload values. \n"); 
  http.end(); 
  return; }
delay(10000);


    if (!client.connected()) {
    reconnectmqttserver();
    }
    client.loop();
    snprintf (msgmqtt1,255, "persentase %d ",persentase);
    client.publish("test", msgmqtt1);

}
