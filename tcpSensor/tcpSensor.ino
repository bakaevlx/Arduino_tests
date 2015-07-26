#include <SPI.h>
#include <Ethernet.h>

//Arduino network info
byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};
IPAddress ip(169, 254, 120, 176);

// TCP
EthernetServer server(8888);  // local port
String inputStr;

// Requester (laptop) IP's
unsigned int replyToPort = 9990;
byte dnsArr[] = {169, 254, 119, 255};
byte gatewayArr[] = {169, 254, 119, 255};
byte subnetArr[] = {255, 255, 0, 0};

void setup() {

  // start Ethernet
  Ethernet.begin(mac, ip, dnsArr, gatewayArr, subnetArr);
  // TCP
  server.begin();

  // enable serial data print
  Serial.begin(9600);
  Serial.println("TCP setup finished");
}

void loop() {

  // Create a client connection
  EthernetClient client = server.available();

  if (client) {
    while (client.connected()) {
      if (client.available()) {

        // read request char by char
        char c = client.read();
        inputStr += c;

        //if request has ended
        if (c == '\n') {

          Serial.print("Received: ");
          Serial.print(inputStr); // ends with a line break

          // read input on analog pin 0
          int sensorValue = analogRead(A0);
          Serial.print("SensorValue=");
          Serial.println(sensorValue);

          // send a response
          client.println(sensorValue);

          delay(1);
          client.stop();
          Serial.println("Send finsihed");

          //clear string for next read
          inputStr = "";

        }
      }
    }
  }
}
