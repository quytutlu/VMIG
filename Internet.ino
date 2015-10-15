#include <SPI.h>
#include <Ethernet.h>
byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
boolean received = false;
EthernetClient client;
String str;
String url;
bool flag = false;
IPAddress server(52, 68, 172, 187);
unsigned long lastConnectionTime = 0;
boolean lastConnected = false;
int Pins[8]={2,3,4,5,6,7,8,9};
String ID="5";
const unsigned long postingInterval = 500;  // delay between updates, in milliseconds

void setup() {
  randomSeed(analogRead(0));
  for(int i=0;i<8;i++){
    pinMode(Pins[i],OUTPUT);
    digitalWrite(Pins[i],1);
  }
  Serial.begin(9600);
  Ethernet.begin(mac);
}

void loop() {
  str = "";
  flag = false;
  while (client.available()) {
    char c = client.read();
    if (c == '{'||c=='}')
    {
      flag = true;
      continue;
    }
    if (flag)
    {
      str += c;
    }
    received = true;
  }
  ParserData(str);
  if (received)
  {
    client.stop();
    received = false;
  }

  if (!client.connected() && lastConnected) {
    client.stop();
  }

  if (!client.connected() && (millis() - lastConnectionTime > postingInterval)) {
    httpRequest();
  }
  lastConnected = client.connected();
  delay(500);
}
void ParserData(String data){
  for(int i=0;i<data.length();i+=4){
    digitalWrite(Pins[i/4],data.substring(i+3,i+4)=="1"?0:1);
  }
}
void httpRequest() {
  if (client.connect(server, 80)) {
    Serial.println("connecting...");
    url = "GET /index.php?cmd=laytrangthaiarduino&id=";
    url+=ID;
    url += " HTTP/1.1";
    client.println(url);
    client.println("Host: 52.68.172.187");
    client.println("Connection: keep-open");
    client.println("Connection: close");
    client.println();
    lastConnectionTime = millis();
  }
  else {
    Serial.println("connection failed");
    client.stop();
  }
}
