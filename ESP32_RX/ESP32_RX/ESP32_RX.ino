#include <Wire.h>
#include "SSD1306.h"//ディスプレイ用ライブラリを読み込み
#include <WiFi.h>
#include <WiFiUdp.h>

WiFiUDP udp;
SSD1306  display(0x3c, 21, 22); //SSD1306インスタンスの作成（I2Cアドレス,SDA,SCL）

const char* ssid = "Lphone8"; //router SSID
const char* password = "helloworld1234"; //router pass
 
const int to_udp_port = 55556; //送信相手のポート番号

boolean connected = false;

boolean isDisp_ok = false; //ディスプレイ表示フラグ
int serious_value=0;
 
void setup() {  
   Serial.begin(115200);
  delay(1000);
  connectToWiFi();
  while(!connected){
    delay(1);
  }
  
 
 }
 
void loop() {
  receiveUDP();
  warn();
  getCOVID_data();
  
 
  }

//*****functions**************
void receiveUDP(){
  if(1){
    int packetSize = udp.parsePacket();
    if(packetSize > 0){
      serious_value = udp.read();
      Serial.printf("serious_value=%d", serious_value);
      isDisp_ok = true;
    }
  }
}

void warn(){}
 
void getCOVID_data(){
  //get No. of positive cases in Japan 
  }
 
 
void connectToWiFi(){
  Serial.println("Connecting to WiFi network: " + String(ssid));
   display.init();    //ディスプレイを初期化
   display.setFont(ArialMT_Plain_10);    //フォントを設定
   display.drawString(0, 0, "Connecting to WiFi network: " + String(ssid));    //(0,0)の位置に
   display.display();   //指定された情報を描画
      
  WiFi.disconnect(true, true);
  delay(1000);
  WiFi.onEvent(WiFiEvent);
  WiFi.begin(ssid, password);
  Serial.println("Waiting for WIFI connection...");
  display.init();    //ディスプレイを初期化
  display.drawString(0, 0, "Waiting for WIFI connection...");    //(0,0)の位置に
  display.display(); 
}
 
void WiFiEvent(WiFiEvent_t event){
  IPAddress myIP = WiFi.localIP();
  switch(event) {
    case SYSTEM_EVENT_STA_GOT_IP:
      Serial.println("WiFi connected!");
      Serial.print("My IP address: ");
      Serial.println(myIP);
      display.init();    //ディスプレイを初期化
      display.setFont(ArialMT_Plain_10);    //フォントを設定
      display.drawString(0, 0, "WiFi connected!");    //(0,0)の位置に
      display.display();   //指定された情報を描画
      
      delay(1000);
      connected = true;
      break;
    case SYSTEM_EVENT_STA_DISCONNECTED:
      Serial.println("WiFi lost connection");
      display.init();    //ディスプレイを初期化
      display.setFont(ArialMT_Plain_10);    //フォントを設定
      display.drawString(0, 0, "WiFi lost connection");    //(0,0)の位置に
      display.display(); 
      connected = false;
      break;
    default:
      break;
  }
}
