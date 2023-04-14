//Planet_Kronos_x8 The Kazimier Dezember 2015
//The MIT License (MIT)

#include <SPI.h>
#include <Ethernet.h>
#include <EthernetUdp.h>
#include <OSCBundle.h>

#define enA 10
#define in1 12
#define in2 11

const int Relay1 = 1;
const int Relay2 = 2;
const int Relay3 = 3;
const int Relay4 = 4;
const int Relay5 = 5;
const int Relay6 = 6;
const int Relay7 = 7;
const int Relay8 = 8;

const int RelayArray[] = {Relay1, Relay2, Relay3, Relay4, Relay5, Relay6, Relay7, Relay8};

const int D = 1000;

// you can find this written on the board of some Arduino Ethernets or shields
byte mac[] = { 0x90, 0xA2, 0xDA, 0x0E, 0x9B, 0x7F };

// NOTE: Alternatively, you can assign a fixed IP to configure your
//       Ethernet shield.
byte ip[] = { 192, 168, 0, 100 };

int serverPort = 10000;

//Create UDP message object
EthernetUDP Udp;

void setup() {
  Ethernet.begin(mac, ip);
  Udp.begin(serverPort);
  //Serial.begin(115200);

  pinMode(enA, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  // Set initial rotation direction
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);

  // pinMode(Relay1, OUTPUT);
  // pinMode(Relay2, OUTPUT);
  // pinMode(Relay3, OUTPUT);
  // pinMode(Relay4, OUTPUT);
  // pinMode(Relay5, OUTPUT);
  // pinMode(Relay6, OUTPUT);
  // pinMode(Relay7, OUTPUT);
  // pinMode(Relay8, OUTPUT);
  // digitalWrite(Relay1, HIGH);
  // digitalWrite(Relay2, HIGH);
  // digitalWrite(Relay3, HIGH);
  // digitalWrite(Relay4, HIGH);
  // digitalWrite(Relay5, HIGH);
  // digitalWrite(Relay6, HIGH);
  // digitalWrite(Relay7, HIGH);
  // digitalWrite(Relay8, HIGH);
}

void loop() {

  OSCBundle bundleIN;
  int size;
  if ((size = Udp.parsePacket()) > 0) {
    while (size--)
      bundleIN.fill(Udp.read());
    if (!bundleIN.hasError()) {
      bundleIN.route("/relay", toggleRelay);
      bundleIN.route("/actuator/speed", changeSpeed);
      bundleIN.route("/actuator/rotation", changeRotation);
    }
  }
}

void toggleRelay(OSCMessage &msg, int addrOffset) {

  char buf[1024];
  msg.getAddress(buf, addrOffset + 1);

  int index = atoi(buf);


  if (msg.isInt(0))  //only if theres a number
  {
    int value;
    value = msg.getInt(0);  //get an integer from the OSC message
    digitalWrite(RelayArray[index-1], value);
  }
}

void changeSpeed(OSCMessage &msg, int addrOffset) {
  if (msg.isInt(0))
    {
      int value;
      value = msg.getInt(0);  //get an integer from the OSC message
      analogWrite(enA, value);
    }
}

void changeRotation(OSCMessage &msg, int addrOffset) {
  if (msg.isInt(0))
    {
      int value;
      value = msg.getInt(0);  //get an integer from the OSC message
      if(value){
        digitalWrite(in1, LOW);
        digitalWrite(in2, HIGH);
      } else {
        digitalWrite(in1, HIGH);
        digitalWrite(in2, LOW);        
      }
    }
}
