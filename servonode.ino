#include <PubSubClient.h>
#include <ESP8266WiFi.h>
#include <Servo.h>

#define MQTT_SERVER "192.168.43.154"

const char* ssid = "akashi";
const char* password = "homework";

void callback(char* topic, byte* payload, unsigned int length);
void reconnect();
String macToStr(const uint8_t* mac);

char* servoTopic = "/test/servo";
char* response = "/test/response";

const char del = '@';
const char hdel = '*';
byte delpoint=0;
byte hdelpoint=0;


boolean flag=false;
int thighdeg=0;
int kneedeg=0;
int height=0;

Servo thigh;
Servo knee;

#define thighpin 5
#define kneepin 4

WiFiClient wifiClient;
PubSubClient client(MQTT_SERVER, 1883, callback, wifiClient);

void setup() {

  Serial.begin(115200);
  delay(100);
  thigh.attach(thighpin);
  knee.attach(kneepin);


  WiFi.begin(ssid, password);
  reconnect();

  delay(2000);
  client.publish(response, "setup complete");
}



void loop(){

  if (!client.connected() && WiFi.status() == 3) {reconnect();}

  client.loop();

  delay(20); 
 
  if(flag)
  {
   //thigh.write(thighdeg);
   //knee.write(kneedeg);
   char tmp[4];
   char mes[30];
   strcpy(mes,"THIGH :");
   itoa(thighdeg,tmp,10);
   strcat(mes,tmp);
   client.publish(response,mes);
   strcpy(mes,"KNEE :");
   itoa(kneedeg,tmp,10);
   strcat(mes,tmp);
   client.publish(response,mes);
   Serial.print(1);
   flag=false;
  }
}


void callback(char* topic, byte* payload, unsigned int length) {

  String topicStr = topic; 
  Serial.println("Callback update.");
  Serial.print("Topic: ");
  Serial.println(topicStr);
  for(int i=0;i<36;i++)
    {
      Serial.print((char)payload[i]);
      if(payload[i]==del)
      delpoint=i;
      if(payload[i]==hdel)
      hdelpoint=i;
    }
  Serial.println(length);
  putchar('\n');
  thighdeg=(payload[delpoint-3]-48)*100+(payload[delpoint-2]-48)*10+payload[delpoint-1]-48;
  kneedeg=(payload[delpoint+1]-48)*100+(payload[delpoint+2]-48)*10+payload[delpoint+3]-48;
  height=(payload[hdelpoint+1]-48)*100+(payload[hdelpoint+2]-48)*10+payload[hdelpoint+3]-48;
  Serial.printf("thigh deg: %d\n",thighdeg);
  Serial.printf("knee deg: %d\n",kneedeg);
  Serial.printf("height: %d\n",height);
  flag=true;
  delay(10);
}
    




void reconnect() {

  if(WiFi.status() != WL_CONNECTED){
    Serial.print("Connecting to ");
    Serial.println(ssid);

    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
    Serial.println("");
    Serial.println("WiFi connected");  
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
  }

  if(WiFi.status() == WL_CONNECTED){
    while (!client.connected()) {
      Serial.print("Attempting MQTT connection...");

      String clientName;
      clientName += "esp8266-";
      uint8_t mac[6];
      WiFi.macAddress(mac);
      clientName += macToStr(mac);

      if (client.connect((char*) clientName.c_str())) {
        Serial.print("\tMTQQ Connected");
        client.subscribe(servoTopic);
      }

      else{Serial.println("\tFailed."); abort();}
    }
  }
}

String macToStr(const uint8_t* mac){

  String result;

  for (int i = 0; i < 6; ++i) {
    result += String(mac[i], 16);

    if (i < 5){
      result += ':';
    }
  }

  return result;
}
