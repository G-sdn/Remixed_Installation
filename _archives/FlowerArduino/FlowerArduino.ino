//Planet_Kronos_x8 The Kazimier Dezember 2015
//The MIT License (MIT)

#include <SPI.h>
#include <Ethernet.h>
#include <EthernetUdp.h>
#include <OSCBundle.h>

// you can find this written on the board of some Arduino Ethernets or shields
byte mac[] = { 0x90, 0xA2, 0xDA, 0x0E, 0x9B, 0x7F };

// NOTE: Alternatively, you can assign a fixed IP to configure your
//       Ethernet shield.
byte ip[] = { 192, 168, 0, 100 };


int serverPort = 10000;
int destPort = 9000;
byte destIp[] = { 192, 168, 0, 121 };


//Create UDP message object
EthernetUDP Udp;


void setup() {
  Ethernet.begin(mac, ip);
  Udp.begin(serverPort);
  // Serial.begin(9600);

  pinMode(enA, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  // Set initial rotation direction
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
}

void loop() {
  // analogWrite(enA, 255);
  OSCBundle bundleIN;
  int size;
  if ((size = Udp.parsePacket()) > 0) {
    while (size--)
      bundleIN.fill(Udp.read());
    if (!bundleIN.hasError()) {
      bundleIN.route("/actuator/speed", changeSpeed);
      bundleIN.route("/actuator/rotation", changeRotation);
    }
  }
}

void changeSpeed(OSCMessage &msg, int addrOffset) {
  if (msg.isInt(0))
    {
      pinMode(LED_BUILTIN, OUTPUT);
      digitalWrite(LED_BUILTIN, (msg.getInt(0) > 0)? HIGH: LOW);
    }
}
