from socket import socket, AF_INET, SOCK_DGRAM

class ESP32_TX:
    def __init__(self,text):
        self.value=text
        self.ADDRESS = "192.168.2.3" # ESP32 address
        self.PORT = 5555
    def print_value(self):
        print(self.value)

    def esp32_udp(self,len_s):
        s = socket(AF_INET, SOCK_DGRAM)
 
        while True:
            msg = input("> ")
            s.sendto(msg.encode(), (self.ADDRESS, self.PORT))
 
            s.close()