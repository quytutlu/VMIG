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
String TrangThai;
const unsigned long postingInterval = 500;  // delay between updates, in milliseconds

void setup() {
  randomSeed(analogRead(0));
  pinMode(7, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(3, OUTPUT);
  digitalWrite(7,1);
  digitalWrite(6,1);
  digitalWrite(5,1);
  digitalWrite(4,1);
  digitalWrite(3,1);
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
    //Serial.println();
    //Serial.println("disconnecting.");
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
    //Serial.println(data.substring(i,i+4));
    TrangThai=data.substring(i,i+4);
    // điền mã thiết bị trên hệ thống smarthometl.com
    //nếu mã thiết bị + 0 thì sẽ là tắt
    //nếu mã thiết bị +1 thì sẽ là bật
    // ví dụ: mã 1 thiết bị trên hệ thống smarthometl.com của bạn là ixp thì nếu bạn muốn tắt bóng đèn thì điền vào if(TrangThai=="ixp0") tương tự 
    //nếu muốn bật thiết bị thì điền vào if(TrangThai=="ixp1")
    //Đối với Arduino thì chỉ có thể điều khiển được 5 thiết bị vì thế bạn hãy cấu hình trên hệ thống smarthometl.com chỉ 5 thiết bị thôi nhé!
    if (TrangThai == "hqr1")
    {
      digitalWrite(7, LOW);    //Bật thiết bị 1 tạicổng số 7
      continue;
    }
    if (TrangThai == "hqr0")
    {
      digitalWrite(7, HIGH);    //Tắt thiết bị 1 tạicổng số 7
      continue;
    }
    
    if (TrangThai == "krq1")
    {
      digitalWrite(6, LOW);  //Bật thiết bị 2 tạicổng số 6
      continue;
    }
    if (TrangThai == "krq0")
    {
      digitalWrite(6, HIGH);  //Tắt thiết bị 2 tạicổng số 6
      continue;
    }
    
    if (TrangThai == "cfj1")
    {
      digitalWrite(5, LOW);    //Bật thiết bị 3 tạicổng số 5
      continue;
    }
    if (TrangThai == "cfj0")
    {
      digitalWrite(5, HIGH);  //Tắt thiết bị 3 tạicổng số 5
      continue;
    }
    
    if (TrangThai == "dpf1")
    {
      digitalWrite(4, LOW);  //Bật thiết bị 4 tạicổng số 4
      continue;
    }
    if (TrangThai == "dpf0")
    {
      digitalWrite(4, HIGH);  //Tắt thiết bị 4 tạicổng số 4
      continue;
    }
    
    if (TrangThai == "")
    {
      digitalWrite(3, LOW);  //Bật thiết bị 5 tạicổng số 3
      continue;
    }
    if (TrangThai == "")
    {
      digitalWrite(3, HIGH);  //Tắt thiết bị 5 tạicổng số 3
      continue;
    }
  }
  return;
}
void httpRequest() {
  if (client.connect(server, 80)) {
    Serial.println("connecting...");
    url = "GET /index.php?cmd=laytrangthaiarduino&id=149";// điền id của bạn trên hệ thống smarthometl.com vào đằng sau dấu =
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
    //Serial.println("disconnecting.");
    client.stop();
  }
}




