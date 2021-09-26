from socket import socket, AF_INET, SOCK_DGRAM

class ESP32_TX:
    def __init__(self,text):
        self.value=text
        self.ADDRESS = "192.168.0.116" # ESP32 address
        self.PORT = 55555
    def print_value(self):
        print(self.value)

    def esp32_udp(self,len_s):
        s = socket(AF_INET, SOCK_DGRAM)
 
        msg = str(len_s)
        s.sendto(msg.encode(), (self.ADDRESS, self.PORT))
        s.close()

#デバッグ用プログラム　ESP32とソケット通信のテストに使います


#ESP32_TX("TX").esp32_udp("52226") 
#ESP32_TX("TX").esp32_udp("hello")
#ESP32_TX("TX").esp32_udp(52226)
#ESP32_TX("").esp32_udp(522+26) #548
ESP32_TX("TX").esp32_udp(16)
