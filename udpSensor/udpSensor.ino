#include <SPI.h>
#include <Ethernet.h>
#include <EthernetUdp.h>

//Arduino network info
byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};
IPAddress ip(169, 254, 120, 176);
unsigned int localPort = 8888;

// UDP response -- these are the requester (laptop) IP's
unsigned int replyToPort = 9990;
byte dnsArr[] = {169, 254, 119, 255};
byte gatewayArr[] = {169, 254, 119, 255};
byte subnetArr[] = {255, 255, 0, 0};

// buffers for receiving and sending data
char packetBuffer[UDP_TX_PACKET_MAX_SIZE];  // buffer to hold incoming packet,
//char  ReplyBuffer[] = "acknowledged";       // a string to send back

// An EthernetUDP instance to let us send and receive packets over UDP
EthernetUDP Udp;

void setup() {
  // start the Ethernet
  Ethernet.begin(mac, ip, dnsArr, gatewayArr, subnetArr);
//  Ethernet.begin(mac, ip);
  // start UDP
  Udp.begin(localPort);
  Serial.begin(9600);
  Serial.print("Setup finished. Local UDP port=");
  Serial.println(localPort);
}

void loop() {
  // if there's data available, read a packet
  int packetSize = Udp.parsePacket();
  if (packetSize) {
    // read the packet into packetBufffer
    Udp.read(packetBuffer, UDP_TX_PACKET_MAX_SIZE);
    Serial.print("Message=");
    Serial.println(packetBuffer);

    Serial.println("Delay...");
    delay(500);  // ms
    Serial.println("Delay finished");

    // read the input on analog pin 0:
    int sensorValue = analogRead(A0);
//    float voltage = sensorValue * (5.0 / 1023.0);

    // send UDP reply to the IP address that sent us the packet
    IPAddress replyToIp  = Udp.remoteIP();
    Serial.print("Sending response: ");
    Serial.print(replyToIp);
    Serial.print(":");
    Serial.println(replyToPort);

    Udp.beginPacket(replyToIp, replyToPort);
    Udp.print(sensorValue);
    int udpStatus = Udp.endPacket();
    Serial.print("UDP status=");
    Serial.println(udpStatus);

  }
  delay(10);
}
