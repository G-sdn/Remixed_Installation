//Planet_Kronos_x8 The Kazimier Dezember 2015
//The MIT License (MIT)

#include <SPI.h>
#include <Ethernet.h>
#include <EthernetUdp.h>
#include <OSCBundle.h>

#define enA 10
#define in1 12
#define in2 11

int incomingByte = 0; 

void setup() {
  Serial.begin(9600);


  pinMode(enA, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);

  // Set initial rotation direction
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
}

void loop() {

  if (Serial.available() > 0) {
    incomingByte = Serial.read();  // read the incoming byte:
    Serial.print(" I received:");
    Serial.println(incomingByte);


    // OSCBundle bundleIN;
    // int size;
    // if ((size = Udp.parsePacket()) > 0) {
    //   while (size--)
    //     bundleIN.fill(Udp.read());
    //   if (!bundleIN.hasError()) {
    //     bundleIN.route("/actuator/speed", changeSpeed);
    //     bundleIN.route("/actuator/rotation", changeRotation);
    //   }
    // }
  }

  // void changeSpeed(OSCMessage & msg, int addrOffset) {
  //   if (msg.isInt(0)) {
  //     int value;
  //     value = msg.getInt(0);  //get an integer from the OSC message
  //     analogWrite(enA, value);
  //   }
  // }

  // void changeRotation(OSCMessage & msg, int addrOffset) {
  //   if (msg.isInt(0)) {
  //     int value;
  //     value = msg.getInt(0);  //get an integer from the OSC message
  //     if (value) {
  //       digitalWrite(in1, LOW);
  //       digitalWrite(in2, HIGH);
  //     } else {
  //       digitalWrite(in1, HIGH);
  //       digitalWrite(in2, LOW);
  //     }
  //   }
  }
