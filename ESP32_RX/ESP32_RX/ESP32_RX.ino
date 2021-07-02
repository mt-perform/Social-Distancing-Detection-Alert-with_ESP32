#include <WiFi.h>

const char ssid[] = "Lphone8"; // TODO
const char pass[] = "helloworld0123";  // TODO
const int port = 11411; 

const IPAddress local_ip(***, ***, ***, ***);  //TODO
const IPAddress server_ip(***, ***, ***, ***);  //TODO
const IPAddress subnet(255, 255, 255, 0); 

WiFiClient client;

void setup() {
  Serial.begin(115200);

  WiFi.softAP(ssid, pass);       
  delay(100);               
  WiFi.softAPConfig(local_ip, local_ip, subnet);

  Serial.print("AP IP address: ");
  IPAddress myIP = WiFi.softAPIP();
  Serial.println(myIP);

  WiFi.begin(ssid, pass);
  while( WiFi.status() != WL_CONNECTED) {
    delay(500);  
  }  

  Serial.print("Local port: ");
  Serial.println(port);
  client.connect(server_ip, port);
}

void loop() {
  if (client.available()) {
    int val = client.read();
    Serial.printf("%c\n", val);
  }
  if (Serial.available()) {
    Serial.read();
    char write_data[1];
    write_data[0] = 'a';
    Serial.printf("write\n");
    client.write(write_data, 1);
  }
}
