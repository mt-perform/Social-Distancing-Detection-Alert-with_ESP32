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

uint8_t receive_position_data = 0;
uint8_t receive_direction = 0;
uint8_t receive_color_data = 0;
uint8_t old_line_data = 0;
uint8_t old_color_data = 0;
boolean isDisp_ok = false; //ディスプレイ表示フラグ
boolean isSet_send_data = false;
boolean isSend_rect_move = false; //図形動作開始フラグ
int16_t send_position = 0; //送信図形の座標位置
uint8_t send_direction = 0; //送信図形の動作方向変数
uint8_t send_color_num = 0; //送信図形のカラー番号
uint32_t now_time = 0;
int16_t interval = 100; //UDPデータ送信間隔
uint8_t rect_width = 63; //図形の幅
uint8_t rect_height = 100; //図形の高さ
 
void setup() {  
   Serial.begin(115200);
  delay(1000);
  connectToWiFi();
  while(!connected){
    //接続前処理内容
    delay(1);
  }
  
 
 }
 
void loop() {
  receiveUDP();
  
 
  }

//************************************
void receiveUDP(){
  if(1){
    int packetSize = udp.parsePacket();
    if(packetSize > 0){
      receive_position_data = udp.read();
      receive_direction = udp.read();
      receive_color_data = udp.read();
      //Serial.println(receive_position_data);
      //Serial.printf("receive_position_data=%d, receive_color_data=%d\r\n", receive_position_data, receive_color_data);
      isDisp_ok = true;
    }
  }
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
