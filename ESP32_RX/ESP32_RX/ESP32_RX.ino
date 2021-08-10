#include <Wire.h>
#include "SSD1306.h"//ディスプレイ用ライブラリを読み込み
#include <WiFi.h>
#include <WiFiUdp.h>
#include <ArduinoJson.h>
#include <HTTPClient.h>
#include <Ticker.h>


WiFiUDP udp;
SSD1306  display(0x3c, 21, 22); //set SSD1306 instance（I2C address,SDA,SCL）

const char* ssid = "Lphone8"; //router SSID
const char* password = "helloworld1234"; //router pass
//covid19 api
const String endpoint = "https://covid-193.p.rapidapi.com/statistics";
const String key = "yourAPIKEY"; 

const int to_udp_port = 55556; //送信相手のポート番号

//NTP structure
struct tm timeInfo;
char date[20],hour_minute[20];

boolean connected = false;
boolean isDisp_ok = false; //ディスプレイ表示フラグ
int serious_value=0;
 
void setup() {  
  Serial.begin(115200);
  configTime(9 * 3600L, 0, "ntp.nict.jp", "time.google.com", "ntp.jst.mfeed.ad.jp");
  display.init();
  delay(1000);
  connectToWiFi();
  while(!connected){
    
    delay(1);
  }
  
 
 }
 
void loop() {
  //receiveUDP();
  //warn();
  //getCOVID_data();
  if(connected){display_time();}
  display.invertDisplay();
  delay(1000);
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

void display_time(){
  getLocalTime(&timeInfo);
  sprintf(date, " %04d/%02d/%02d",timeInfo.tm_year + 1900, timeInfo.tm_mon + 1, timeInfo.tm_mday);
  sprintf(hour_minute, "%02d:%02d",timeInfo.tm_hour, timeInfo.tm_min);
  
  display.clear();
  //date
  display.setFont(ArialMT_Plain_10);
  display.drawString(0, 0, date);
  //time
  display.setFont(ArialMT_Plain_10);
  display.drawString(100, 0, hour_minute);
  display.drawLine(2, 12, 126,12); //x0,y0,x1,y1
  display.display();
  }
 
void getCOVID_data(){
  //get No. of positive cases in Japan 
  //https://api.rakuten.net/api-sports/api/covid-193  
} 

void connectToWiFi(){
  Serial.println("Connecting to WiFi network: " + String(ssid));
   display.init();    //ディスプレイを初期化
   display.setFont(ArialMT_Plain_10);    //フォントを設定
   display.drawStringMaxWidth(0, 0, 128,  "Connecting to WiFi network: " + String(ssid));
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
      display.init();  //ディスプレイを初期化
      display.setFont(ArialMT_Plain_10);    //フォントを設定
      display.drawString(0, 0, "WiFi lost connection");    //(0,0)の位置に
      display.display(); 
      connected = false;
      break;
    default:
      break;
  }
}
