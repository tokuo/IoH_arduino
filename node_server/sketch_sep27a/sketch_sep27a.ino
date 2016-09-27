#include <SPI.h>
#include <Ethernet2.h>

byte mac[] = {0x90, 0xA2, 0xDA, 0x10, 0x6A, 0x19};
IPAddress ip(192,168,10,150);
//edge
char server[] = "192.168.10.105";
EthernetClient client;

void setup() {
  Serial.begin(9600);
  Ethernet.begin(mac,ip);
  if(client.connect(server,3000)){
    Serial.println("connected");
  }else{
    Serial.println("failed");
  }
}

void loop() {
  if(client.available()){
    char c = client.read();
    Serial.print(c);
  }

}
