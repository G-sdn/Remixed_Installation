
#include <SPI.h>
#include <Ethernet.h>
#include <EthernetUdp.h>
#include <OSCBundle.h>
#include <VarSpeedServo.h>

//  *** ETHERNET SETUP ***
byte mac[] = { 0x90, 0xA2, 0xDA, 0x0E, 0x9E, 0x6F };
byte ip[] = { 192, 168, 0, 103 };
int ServoInPort = 10000;

//  *** SERVO SETUP ***
int flowerclose = 90;  // Baseline angle of when all petals are at an upright, closed position. THIS SHOULD BE 90 DEGREES TO AVOID
int floweropen = 100;  // Baseline angle of when all petals are fully open.

int twch = 95;    // Closed twitch value
int twch2 = 102;  // Open Flower Twitch Value

int FourMin = 240000;  // 4 Minute Delay
int uniDelay = 500;
int casDelay = 10;

int trig = 6;

int v = 10;        // Uniform Bloom Open Close Movement SPEED
int t = 10;        // Twitch SPEED
int c = 15;        // Cascade Open SPEED
bool wait = true;  // wait for servo to finish movement before next line of command

VarSpeedServo petal1;
VarSpeedServo petal2;
VarSpeedServo petal3;
VarSpeedServo petal4;
VarSpeedServo petal5;
VarSpeedServo petal6;

EthernetUDP UdpServo;

void setup() {

  Serial.begin(9600);

  Ethernet.begin(mac, ip);
  UdpServo.begin(ServoInPort);

  Serial.println("Serial begin");

  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());

  petal1.attach(3);
  petal2.attach(5);
  petal3.attach(6);
  petal4.attach(9);
  petal5.attach(10);
  petal6.attach(11);

  /* Reset all servos back to closed position at slow speed */
  if (petal1.read() != flowerclose) {
    petal1.write(flowerclose, 10);
  }
  if (petal2.read() != flowerclose) {
    petal2.write(flowerclose, 10);
  }
  if (petal3.read() != flowerclose) {
    petal3.write(flowerclose, 10);
  }
  if (petal4.read() != flowerclose) {
    petal4.write(flowerclose, 10);
  }
  if (petal5.read() != flowerclose) {
    petal5.write(flowerclose, 10);
  }
  if (petal6.read() != flowerclose) {
    petal6.write(flowerclose, 10);
  }
}

void loop() {

  OSCMessage msgIN;
  int servoPacketSize;

  if ((servoPacketSize = UdpServo.parsePacket()) > 0) {
    while (servoPacketSize--)
      msgIN.fill(UdpServo.read());
    if (!msgIN.hasError()) {
      msgIN.route("/test", printOSC);
      msgIN.route("/servo/twitch", twitch);
      msgIN.route("/servo/UniformFlutterOpen", uniFlutterOpen);
      msgIN.route("/servo/UniformFlutterClose", uniFlutterClose);
      msgIN.route("/servo/CascadeFlutterOpen", casFlutterOpen);
      msgIN.route("/servo/CascadeFlutterClose", casFlutterClose);
      msgIN.route("/servo/UniformOpen2", uniOpen2);
      msgIN.route("/servo/UniformClose2", uniClose2);
    }
  }
}

void printOSC(OSCMessage& msg, int addrOffset) {
  Serial.println("Got OSC message");
}
//TWITCH
void twitch(OSCMessage& msg, int addrOffset) {
  if (msg.isInt(0)) {
    petal1.write(twch, t, wait);
    petal1.write(flowerclose, t, wait);
    petal3.write(twch, t, wait);
    delay(1000);
    petal3.write(flowerclose, t, wait);
    petal4.write(twch, t, wait);
    petal4.write(flowerclose, t, wait);
    delay(1000);
    petal2.write(twch, t, wait);
    petal2.write(flowerclose, t, wait);
    delay(1000);
    petal5.write(twch, t, wait);
    petal5.write(flowerclose, t, wait);
    petal1.write(twch, t, wait);
    petal1.write(flowerclose, t, wait);
    delay(1000);

    petal4.write(twch, t, wait);
    petal4.write(flowerclose, t, wait);
    petal6.write(twch, t, wait);
    delay(1000);
    petal6.write(flowerclose, t, wait);
    petal2.write(twch, t, wait);
    petal2.write(flowerclose, t, wait);
    petal1.write(twch, t, wait);
    petal1.write(flowerclose, t, wait);
    delay(1000);
    petal5.write(twch, t, wait);
    petal5.write(flowerclose, t, wait);
    petal3.write(twch, t, wait);
    petal3.write(flowerclose, t, wait);
    delay(1000);

    petal1.write(twch, t, wait);
    petal1.write(flowerclose, t, wait);
    petal3.write(twch, t, wait);
    petal3.write(flowerclose, t, wait);
    delay(1000);
    petal4.write(twch, t, wait);
    petal4.write(flowerclose, t, wait);
    delay(1000);
    petal2.write(twch, t, wait);
    petal2.write(flowerclose, t, wait);
    delay(1000);
    petal5.write(twch, t, wait);
    petal5.write(flowerclose, t, wait);
    petal1.write(twch, t, wait);
    petal1.write(flowerclose, t, wait);
    delay(1000);

    Serial.println("Twitch Complete");
  }
}
// UNIFORM FLUTTER OPEN
void uniFlutterOpen(OSCMessage& msg, int addrOffset) {
  if (msg.isInt(0)) {

    petal1.write(100, v);
    petal2.write(100, v);
    petal3.write(100, v);
    petal4.write(100, v);
    petal5.write(100, v);
    petal6.write(100, v);
    petal6.wait();

    delay(uniDelay);

    petal1.write(flowerclose, v);
    petal2.write(flowerclose, v);
    petal3.write(flowerclose, v);
    petal4.write(flowerclose, v);
    petal5.write(flowerclose, v);
    petal6.write(flowerclose, v);
    petal6.wait();

    delay(uniDelay);

    petal1.write(110, v);
    petal2.write(110, v);
    petal3.write(110, v);
    petal4.write(110, v);
    petal5.write(110, v);
    petal6.write(110, v);
    petal6.wait();

    delay(uniDelay);

    petal1.write(100, v);
    petal2.write(100, v);
    petal3.write(100, v);
    petal4.write(100, v);
    petal5.write(100, v);
    petal6.write(100, v);
    petal6.wait();

    delay(uniDelay);

    petal1.write(120, v);
    petal2.write(120, v);
    petal3.write(120, v);
    petal4.write(120, v);
    petal5.write(120, v);
    petal6.write(120, v);
    petal6.wait();

    delay(uniDelay);

    petal1.write(110, v);
    petal2.write(110, v);
    petal3.write(110, v);
    petal4.write(110, v);
    petal5.write(110, v);
    petal6.write(110, v);
    petal6.wait();

    delay(uniDelay);

    petal1.write(130, v);
    petal2.write(130, v);
    petal3.write(130, v);
    petal4.write(130, v);
    petal5.write(130, v);
    petal6.write(130, v);
    petal6.wait();

    delay(uniDelay);

    petal1.write(120, v);
    petal2.write(120, v);
    petal3.write(120, v);
    petal4.write(120, v);
    petal5.write(120, v);
    petal6.write(120, v);
    petal6.wait();

    delay(uniDelay);

    petal1.write(140, v);
    petal2.write(140, v);
    petal3.write(140, v);
    petal4.write(140, v);
    petal5.write(140, v);
    petal6.write(140, v);
    petal6.wait();

    delay(uniDelay);

    petal1.write(130, v);
    petal2.write(130, v);
    petal3.write(130, v);
    petal4.write(130, v);
    petal5.write(130, v);
    petal6.write(130, v);
    petal6.wait();

    delay(uniDelay);

    petal1.write(150, v);
    petal2.write(150, v);
    petal3.write(150, v);
    petal4.write(150, v);
    petal5.write(150, v);
    petal6.write(150, v);
    petal6.wait();

    delay(uniDelay);

    Serial.println("Uniform Flutter Open Complete");
  }
}
// UNIFORM FLUTTER CLOSE
void uniFlutterClose(OSCMessage& msg, int addrOffset) {
  if (msg.isInt(0)) {


    petal1.write(150, v);
    petal2.write(150, v);
    petal3.write(150, v);
    petal4.write(150, v);
    petal5.write(150, v);
    petal6.write(150, v);
    petal6.wait();

    delay(uniDelay);

    petal1.write(130, v);
    petal2.write(130, v);
    petal3.write(130, v);
    petal4.write(130, v);
    petal5.write(130, v);
    petal6.write(130, v);
    petal6.wait();

    delay(uniDelay);

    petal1.write(140, v);
    petal2.write(140, v);
    petal3.write(140, v);
    petal4.write(140, v);
    petal5.write(140, v);
    petal6.write(140, v);
    petal6.wait();

    delay(uniDelay);

    petal1.write(120, v);
    petal2.write(120, v);
    petal3.write(120, v);
    petal4.write(120, v);
    petal5.write(120, v);
    petal6.write(120, v);
    petal6.wait();

    delay(uniDelay);

    petal1.write(130, v);
    petal2.write(130, v);
    petal3.write(130, v);
    petal4.write(130, v);
    petal5.write(130, v);
    petal6.write(130, v);
    petal6.wait();

    delay(uniDelay);

    petal1.write(110, v);
    petal2.write(110, v);
    petal3.write(110, v);
    petal4.write(110, v);
    petal5.write(110, v);
    petal6.write(110, v);
    petal6.wait();

    delay(uniDelay);

    petal1.write(120, v);
    petal2.write(120, v);
    petal3.write(120, v);
    petal4.write(120, v);
    petal5.write(120, v);
    petal6.write(120, v);
    petal6.wait();

    delay(uniDelay);

    petal1.write(100, v);
    petal2.write(100, v);
    petal3.write(100, v);
    petal4.write(100, v);
    petal5.write(100, v);
    petal6.write(100, v);
    petal6.wait();

    delay(uniDelay);

    petal1.write(110, v);
    petal2.write(110, v);
    petal3.write(110, v);
    petal4.write(110, v);
    petal5.write(110, v);
    petal6.write(110, v);
    petal6.wait();

    delay(uniDelay);

    petal1.write(flowerclose, v);
    petal2.write(flowerclose, v);
    petal3.write(flowerclose, v);
    petal4.write(flowerclose, v);
    petal5.write(flowerclose, v);
    petal6.write(flowerclose, v);
    petal6.wait();

    delay(uniDelay);

    Serial.println("Uniform Flutter Close Complete");
  }
}
// CASCADE FLUTTER OPEN
void casFlutterOpen(OSCMessage& msg, int addrOffset) {
  if (msg.isInt(0)) {

    petal1.write(100, c, wait);
    petal2.write(100, c, wait);
    petal3.write(100, c, wait);
    petal4.write(100, c, wait);
    petal5.write(100, c, wait);
    petal6.write(100, c, wait);

    delay(casDelay);

    petal1.write(flowerclose, c, wait);
    petal2.write(flowerclose, c, wait);
    petal3.write(flowerclose, c, wait);
    petal4.write(flowerclose, c, wait);
    petal5.write(flowerclose, c, wait);
    petal6.write(flowerclose, c, wait);

    delay(casDelay);

    petal1.write(110, c, wait);
    petal2.write(110, c, wait);
    petal3.write(110, c, wait);
    petal4.write(110, c, wait);
    petal5.write(110, c, wait);
    petal6.write(110, c, wait);

    delay(casDelay);

    petal1.write(100, c, wait);
    petal2.write(100, c, wait);
    petal3.write(100, c, wait);
    petal4.write(100, c, wait);
    petal5.write(100, c, wait);
    petal6.write(100, c, wait);

    delay(casDelay);

    petal1.write(120, c, wait);
    petal2.write(120, c, wait);
    petal3.write(120, c, wait);
    petal4.write(120, c, wait);
    petal5.write(120, c, wait);
    petal6.write(120, c, wait);

    delay(casDelay);

    petal1.write(110, c, wait);
    petal2.write(110, c, wait);
    petal3.write(110, c, wait);
    petal4.write(110, c, wait);
    petal5.write(110, c, wait);
    petal6.write(110, c, wait);

    delay(casDelay);

    petal1.write(130, c, wait);
    petal2.write(130, c, wait);
    petal3.write(130, c, wait);
    petal4.write(130, c, wait);
    petal5.write(130, c, wait);
    petal6.write(130, c, wait);

    delay(casDelay);

    petal1.write(120, c, wait);
    petal2.write(120, c, wait);
    petal3.write(120, c, wait);
    petal4.write(120, c, wait);
    petal5.write(120, c, wait);
    petal6.write(120, c, wait);

    delay(casDelay);

    petal1.write(140, c, wait);
    petal2.write(140, c, wait);
    petal3.write(140, c, wait);
    petal4.write(140, c, wait);
    petal5.write(140, c, wait);
    petal6.write(140, c, wait);

    delay(casDelay);

    petal2.write(130, c, wait);
    petal3.write(130, c, wait);
    petal4.write(130, c, wait);
    petal5.write(130, c, wait);
    petal6.write(130, c, wait);

    delay(casDelay);

    petal2.write(150, c, wait);
    petal3.write(150, c, wait);
    petal4.write(150, c, wait);
    petal5.write(150, c, wait);
    petal6.write(150, c, wait);

    delay(casDelay);

    Serial.println("Cascade Flutter Open Complete");
  }
}
// CASCADE FLUTER CLOSE
void casFlutterClose(OSCMessage& msg, int addrOffset) {
  if (msg.isInt(0)) {

    petal2.write(150, c, wait);
    petal3.write(150, c, wait);
    petal4.write(150, c, wait);
    petal5.write(150, c, wait);
    petal6.write(150, c, wait);

    delay(casDelay);

    petal2.write(130, c, wait);
    petal3.write(130, c, wait);
    petal4.write(130, c, wait);
    petal5.write(130, c, wait);
    petal6.write(130, c, wait);

    delay(casDelay);

    petal1.write(140, c, wait);
    petal2.write(140, c, wait);
    petal3.write(140, c, wait);
    petal4.write(140, c, wait);
    petal5.write(140, c, wait);
    petal6.write(140, c, wait);

    delay(casDelay);

    petal1.write(120, c, wait);
    petal2.write(120, c, wait);
    petal3.write(120, c, wait);
    petal4.write(120, c, wait);
    petal5.write(120, c, wait);
    petal6.write(120, c, wait);

    delay(casDelay);

    petal1.write(130, c, wait);
    petal2.write(130, c, wait);
    petal3.write(130, c, wait);
    petal4.write(130, c, wait);
    petal5.write(130, c, wait);
    petal6.write(130, c, wait);

    delay(casDelay);

    petal1.write(110, c, wait);
    petal2.write(110, c, wait);
    petal3.write(110, c, wait);
    petal4.write(110, c, wait);
    petal5.write(110, c, wait);
    petal6.write(110, c, wait);

    delay(casDelay);

    petal1.write(120, c, wait);
    petal2.write(120, c, wait);
    petal3.write(120, c, wait);
    petal4.write(120, c, wait);
    petal5.write(120, c, wait);
    petal6.write(120, c, wait);

    delay(casDelay);

    petal1.write(100, c, wait);
    petal2.write(100, c, wait);
    petal3.write(100, c, wait);
    petal4.write(100, c, wait);
    petal5.write(100, c, wait);
    petal6.write(100, c, wait);

    delay(casDelay);

    petal1.write(110, c, wait);
    petal2.write(110, c, wait);
    petal3.write(110, c, wait);
    petal4.write(110, c, wait);
    petal5.write(110, c, wait);
    petal6.write(110, c, wait);

    delay(casDelay);

    petal1.write(flowerclose, c, wait);
    petal2.write(flowerclose, c, wait);
    petal3.write(flowerclose, c, wait);
    petal4.write(flowerclose, c, wait);
    petal5.write(flowerclose, c, wait);
    petal6.write(flowerclose, c, wait);

    delay(casDelay);

    Serial.println("Cascade Flutter Close Complete");
  }
}
// UNIFORM TO FLOWER CLOSE OPEN SEQUENCE
void uniOpen2(OSCMessage& msg, int addrOffset) {
  if (msg.isInt(0)) {

    petal1.write(100, v);
    petal2.write(100, v);
    petal3.write(100, v);
    petal4.write(100, v);
    petal5.write(100, v);
    petal6.write(100, v);
    petal6.wait();

    delay(uniDelay);

    petal1.write(flowerclose, v);
    petal2.write(flowerclose, v);
    petal3.write(flowerclose, v);
    petal4.write(flowerclose, v);
    petal5.write(flowerclose, v);
    petal6.write(flowerclose, v);
    petal6.wait();

    delay(uniDelay);

    petal1.write(110, v);
    petal2.write(110, v);
    petal3.write(110, v);
    petal4.write(110, v);
    petal5.write(110, v);
    petal6.write(110, v);
    petal6.wait();

    delay(uniDelay);

    petal1.write(flowerclose, v);
    petal2.write(flowerclose, v);
    petal3.write(flowerclose, v);
    petal4.write(flowerclose, v);
    petal5.write(flowerclose, v);
    petal6.write(flowerclose, v);
    petal6.wait();

    delay(uniDelay);

    petal1.write(120, v);
    petal2.write(120, v);
    petal3.write(120, v);
    petal4.write(120, v);
    petal5.write(120, v);
    petal6.write(120, v);
    petal6.wait();

    delay(uniDelay);

    petal1.write(flowerclose, v);
    petal2.write(flowerclose, v);
    petal3.write(flowerclose, v);
    petal4.write(flowerclose, v);
    petal5.write(flowerclose, v);
    petal6.write(flowerclose, v);
    petal6.wait();

    delay(uniDelay);

    petal1.write(130, v);
    petal2.write(130, v);
    petal3.write(130, v);
    petal4.write(130, v);
    petal5.write(130, v);
    petal6.write(130, v);
    petal6.wait();

    delay(uniDelay);

    petal1.write(flowerclose, v);
    petal2.write(flowerclose, v);
    petal3.write(flowerclose, v);
    petal4.write(flowerclose, v);
    petal5.write(flowerclose, v);
    petal6.write(flowerclose, v);
    petal6.wait();

    delay(uniDelay);

    petal1.write(140, v);
    petal2.write(140, v);
    petal3.write(140, v);
    petal4.write(140, v);
    petal5.write(140, v);
    petal6.write(140, v);
    petal6.wait();

    delay(uniDelay);

    petal1.write(flowerclose, v);
    petal2.write(flowerclose, v);
    petal3.write(flowerclose, v);
    petal4.write(flowerclose, v);
    petal5.write(flowerclose, v);
    petal6.write(flowerclose, v);
    petal6.wait();

    delay(uniDelay);

    petal1.write(150, v);
    petal2.write(150, v);
    petal3.write(150, v);
    petal4.write(150, v);
    petal5.write(150, v);
    petal6.write(150, v);
    petal6.wait();

    delay(uniDelay);

    Serial.println("UUNIFORM TO FLOWER CLOSE OPEN SEQUENCE Complete");
  }
}

// UNIFORM TO FLOWER CLOSE - CLOSE SEQUENCE
void uniClose2(OSCMessage& msg, int addrOffset) {
  if (msg.isInt(0)) {

    delay(uniDelay);

    petal1.write(flowerclose, v);
    petal2.write(flowerclose, v);
    petal3.write(flowerclose, v);
    petal4.write(flowerclose, v);
    petal5.write(flowerclose, v);
    petal6.write(flowerclose, v);
    petal6.wait();

    delay(uniDelay);

    petal1.write(140, v);
    petal2.write(140, v);
    petal3.write(140, v);
    petal4.write(140, v);
    petal5.write(140, v);
    petal6.write(140, v);
    petal6.wait();

    delay(uniDelay);

    petal1.write(flowerclose, v);
    petal2.write(flowerclose, v);
    petal3.write(flowerclose, v);
    petal4.write(flowerclose, v);
    petal5.write(flowerclose, v);
    petal6.write(flowerclose, v);
    petal6.wait();

    delay(uniDelay);

    petal1.write(130, v);
    petal2.write(130, v);
    petal3.write(130, v);
    petal4.write(130, v);
    petal5.write(130, v);
    petal6.write(130, v);
    petal6.wait();

    delay(uniDelay);

    petal1.write(flowerclose, v);
    petal2.write(flowerclose, v);
    petal3.write(flowerclose, v);
    petal4.write(flowerclose, v);
    petal5.write(flowerclose, v);
    petal6.write(flowerclose, v);
    petal6.wait();

    delay(uniDelay);

    petal1.write(120, v);
    petal2.write(120, v);
    petal3.write(120, v);
    petal4.write(120, v);
    petal5.write(120, v);
    petal6.write(120, v);
    petal6.wait();

    delay(uniDelay);

    petal1.write(flowerclose, v);
    petal2.write(flowerclose, v);
    petal3.write(flowerclose, v);
    petal4.write(flowerclose, v);
    petal5.write(flowerclose, v);
    petal6.write(flowerclose, v);
    petal6.wait();

    delay(uniDelay);


    petal1.write(110, v);
    petal2.write(110, v);
    petal3.write(110, v);
    petal4.write(110, v);
    petal5.write(110, v);
    petal6.write(110, v);
    petal6.wait();

    delay(uniDelay);

    petal1.write(flowerclose, v);
    petal2.write(flowerclose, v);
    petal3.write(flowerclose, v);
    petal4.write(flowerclose, v);
    petal5.write(flowerclose, v);
    petal6.write(flowerclose, v);
    petal6.wait();

    delay(uniDelay);

    petal1.write(100, v);
    petal2.write(100, v);
    petal3.write(100, v);
    petal4.write(100, v);
    petal5.write(100, v);
    petal6.write(100, v);
    petal6.wait();

    delay(uniDelay);

    Serial.println("UUNIFORM TO FLOWER CLOSE CLOSE SEQUENCE Complete");
  }
}
