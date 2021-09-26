#include <Wire.h>
#include "SSD1306.h"//ディスプレイ用ライブラリを読み込み
#include <WiFi.h>
#include <WiFiUdp.h>
#include <ArduinoJson.h>
#include <HTTPClient.h>
#include <Ticker.h>


WiFiUDP udp;
SSD1306  display(0x3c, 21, 22); //set SSD1306 instance（I2C address,SDA,SCL）

#define BUZZER_PIN 25 //buzzer pin number  GND<->buzzer pin
#define C4 261.6
#define C#4 277.18
#define D4 293.665
#define D#4 311.127
#define E4 329.63
#define F4 349.228
#define F#4 369.994
#define G4 391.995
#define G#4 415.305
#define A4 440
#define A#4 466.164
#define B4 493.883
#define C5 523.251


//covid19 api
const String endpoint = "https://covid-193.p.rapidapi.com/statistics";
const String key = "yourAPIKEY"; 

const int port = 55555; //ポート番号

//NTP structure
struct tm timeInfo;
char date[20],hour_minute[20];

boolean connected = false;
boolean isDisp_ok = false; //ディスプレイ表示フラグ

int packetSize = 0;
int serious_value=0;


 ///////////////////core 1 task　時計表示、接続
void setup() {  
  Serial.begin(115200);
  ledcSetup(1,12000, 8);
  ledcAttachPin(BUZZER_PIN,1);
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
  if(connected){
    display_time();
    receiveUDP();
    if(packetSize>0){
      display.setFont(ArialMT_Plain_16);
      display.drawString(20,25,"ALERT!!!");
      delay(10);
      buzzer();
    }
    display.display();
  }
  delay(1);
  }
  
//*****functions**************
void receiveUDP(){
  if(1){
      char packetBuffer[100];
      int packetSize = udp.parsePacket();
 
  // get packet
     if (packetSize){
       int len = udp.read(packetBuffer, packetSize);
       if (len > 0){
      packetBuffer[len] = '\0'; // end
     }
 
    // print&display
    Serial.print("serious_value=");
    Serial.println(packetBuffer);
    display.setFont(ArialMT_Plain_16);
    display.drawString(20,25,"ALERT!!!");
    display.display();
    buzzer();
    delay(1000);
    display.clear();
    display.setFont(ArialMT_Plain_16);
    display.drawString(20,25,"Serious violations="+String(packetBuffer));
    display.display();
    Serial.print("packetSize=");
    Serial.println(packetSize);
    delay(1000);
     }
}}


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
  udp.begin(port);   ///////UDPの待受ポート開放
}

void buzzer(){
  ledcWriteTone(1,C4);
  delay(500);
  ledcWriteTone(1,0); //mute
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
