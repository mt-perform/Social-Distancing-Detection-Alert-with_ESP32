#include <WiFi.h>
#include <WiFiUdp.h>

const char* ssid = "Lphone8"; //router SSID
const char* password = "helloworld1234"; //router pass
 
const int my_server_udp_port = 55555; //開放する自ポート

WiFiUDP udp;
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
    //受信後処理内容
    delay(1);
  }
  
  delay(500); 
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
  WiFi.disconnect(true, true);
  delay(1000);
  WiFi.onEvent(WiFiEvent);
  WiFi.begin(ssid, password);
  Serial.println("Waiting for WIFI connection...");
}
 
void WiFiEvent(WiFiEvent_t event){
  IPAddress myIP = WiFi.localIP();
  switch(event) {
    case SYSTEM_EVENT_STA_GOT_IP:
      Serial.println("WiFi connected!");
      Serial.print("My IP address: ");
      Serial.println(myIP);
      //udp.begin関数は自サーバーの待ち受けポート開放する関数である
      udp.begin(myIP, my_server_udp_port);
      delay(1000);
      connected = true;
      break;
    case SYSTEM_EVENT_STA_DISCONNECTED:
      Serial.println("WiFi lost connection");
      connected = false;
      break;
    default:
      break;
  }
}
