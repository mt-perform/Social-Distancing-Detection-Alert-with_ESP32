#include <Wire.h>
#include "SSD1306.h"//ディスプレイ用ライブラリを読み込み
 
SSD1306  display(0x3c, 21, 22); //SSD1306インスタンスの作成（I2Cアドレス,SDA,SCL）
 
void setup() {  

  display.init();    //ディスプレイを初期化
  display.setFont(ArialMT_Plain_24);    //フォントを設定
  display.drawString(0, 0, "1234567890");    //(0,0)の位置にHello Worldを表示
  display.drawString(0, 25, "1234567890"); 
  display.display();   //指定された情報を描画
 }
 
void loop() {

  
 
  }
