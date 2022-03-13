#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <SoftwareSerial.h>
#include <string.h>
 SoftwareSerial mySerial(D0, D1); // RX, TX

const char* ssid = "Orange-DE9B";
const char* password = "BBY7NQ07BJ7";

const char* mqtt_server = "192.168.1.104";

char st1[] = "power";
char st2[] = "one";
char received[5];
int i,n;

WiFiClient EspClient;
PubSubClient client(EspClient);

void recieve_string(char *b){
 char d;
 //Serial.println("wait for len");

 while(Serial.available() == 0);
 d = Serial.read();
 
 //Serial.println(d);
 //Serial.println("wait for st");
 for (unsigned char z = 0;z <= (d-1);z++)
     {
      while(Serial.available() == 0);
       b[z] = Serial.read();
      //Serial.println(b[z]);
     }
 //Serial.println("end of receiving");
 b[d] = '\0';
}

void send_string(unsigned char *a ,char c)
{
  mySerial.write(c);
  delay(50);
  for(char z=0;z <= (c-1);z++)
  {
  mySerial.write(a[z]);
  }
}

void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("WiFi connected - ESP IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(String topic, byte* message, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;
  
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  if(topic=="Room/Jtv"){
     /* if(messageTemp == "power"){*/
        send_string(message , length);
     // }
     /* else if(messageTemp == "one"){
        send_string(st2 , 3);
      }*/
      }
  Serial.println();
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
     if (client.connect("ESP8266bath")) {
      Serial.println("connected");  
      client.subscribe("Room/Jtv");
    }
    else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

}

void loop() {
 if (!client.connected()) {
    reconnect();
  }
  
  if(!client.loop())
  {
    client.connect("ESP8266bath1");
  }
  
 /* recieve_string(received);
  
  if ((strcmp(received , st1)) == 0)
    {
      client.publish("bathroom/first", st1); 
    } 
  else if((strcmp(received , st2)) == 0)
    {
      client.publish("bathroom/first", st2);
    }
    */
  }
