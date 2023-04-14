#include <Servo.h>
#include <SPI.h>
#include <Ethernet.h>
#include <EthernetUdp.h>
#include <OSCBundle.h>
#include <OSCMessage.h>
#include <Artnet.h>
#include <Adafruit_NeoPixel.h>

// ***⌄⌄ Actuator variables ⌄⌄***
#define enA 9
#define in1 6
#define in2 7

// ***⌄⌄ Ethernet variables ⌄⌄***
byte mac[] = { 0x90, 0xA2, 0xDA, 0x0E, 0x9E, 0x7F };
byte ip[] = { 192, 168, 0, 103 };
byte broadcastIp[] = { 192, 168, 0, 101 };
int inPort = 10000;
int outPort = 10001;
EthernetUDP Udp;

// ***⌄⌄ Servo variables ⌄⌄***
int openAngle;
int petalSpeed;
Servo Petals;

// ***⌄⌄ LED variables ⌄⌄***
// Neopixel settings
const int numLeds = 159; //Number of LEDs
const int channelsPerLed = 3;
const int numberOfChannels = numLeds * channelsPerLed; // Total number of channels you want to receive (1 led = 3 channels)
const byte dataPin = 3;
Adafruit_NeoPixel leds = Adafruit_NeoPixel(numLeds, dataPin, NEO_GRB + NEO_KHZ800);
Artnet artnet; // Artnet variables
const int startUniverse = 1; // CHANGE FOR YOUR SETUP most software this is 1, some software send out artnet first universe as 0.
// Check if we got all universes
const int maxUniverses = numberOfChannels / 512 + ((numberOfChannels % 512) ? 1 : 0);
bool universesReceived[maxUniverses];
bool sendFrame = 1;
int previousDataLength = 0;

void setup() {
  Serial.begin(9600);
  Serial.println("Begin");

  // ***⌄⌄ Ethernet setup ⌄⌄***
  Ethernet.begin(mac, ip); //Define arduino
  Udp.begin(inPort); //Start UDP server port

  // ***⌄⌄ Artnet setup ⌄⌄***
  artnet.begin(mac, ip);
  leds.begin();
  artnet.setBroadcast(broadcastIp);
  artnet.setArtDmxCallback(onDmxFrame); // this will be called for each packet received
  initTest(); //Test LEDs

  // ***⌄⌄ Actuator setup ⌄⌄***
  pinMode(enA, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  // set initial direction
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);

  //***⌄⌄ Servo setup ⌄⌄***
  Petals.attach(5); // Attach all Servos to PWM Pin 5 on Arduino Uno
  Petals.write(0); // Set all Motors to their zero position
  openAngle = 100; // For purpose of this basic Open - Close code; initiallize flower fully open angle
  petalSpeed = 200;  // ms delay between 1 degree movements. For purpose of this basic Open - Close code; initiallize flower Opening and Closing speed
}

void loop() {
  // OSCMessage msgIN;
  // OSCMessage msgOUT("/echo");

  artnet.read();

  // int size;
  // if ((size = Udp.parsePacket()) > 0) {
  //   Serial.println("Received Packet");
  //   while (size--)
  //     msgIN.fill(Udp.read());

  //   if (!msgIN.hasError()) {
  //     char buf[32];
  //     msgIN.getAddress(buf);
  //     Serial.println(buf);

  //     // Send echo back to Touchdesigner
  //     msgOUT.add(buf).add(msgIN.getInt(0));
  //     Udp.beginPacket(broadcastIp, outPort);
  //     msgOUT.send(Udp);
  //     Udp.endPacket();
  //     msgOUT.empty();

      // msgIN.route("/test", printOSC);
      // // msgIN.route("/servo/onOff", movePetal);
      // msgIN.route("/actuator/direction", changeRotation);
      // msgIN.route("/actuator/speed", changeSpeed);
    // }
  // }
}

void printOSC(OSCMessage &msg, int addrOffset) {
  int value = msg.getInt(0);
  Serial.print("Test value: ");
  Serial.println(value);
}

void changeSpeed(OSCMessage &msg, int addrOffset) {
  if (msg.isInt(0)) {
    int value;
    value = msg.getInt(0);  //get an integer from the OSC message
    Serial.print("Speed value: ");
    Serial.println(value);
    analogWrite(enA, value);
  }
}

void changeRotation(OSCMessage &msg, int addrOffset) {
  if (msg.isInt(0)) {
    int value;
    value = msg.getInt(0);  //get an integer from the OSC message
    Serial.print("Rotation value: ");
    Serial.println(value);
    if (value) {
      digitalWrite(in1, LOW);
      digitalWrite(in2, HIGH);
    } else {
      digitalWrite(in1, HIGH);
      digitalWrite(in2, LOW);
    }
  }
}

void onDmxFrame(uint16_t universe, uint16_t length, uint8_t sequence, uint8_t* data, IPAddress remoteIP)
{
  sendFrame = 1;
  // set brightness of the whole strip
  if (universe == 1)
  {
    leds.show();
  }

  // Store which universe has got in
  if ((universe - startUniverse) < maxUniverses)
    universesReceived[universe - startUniverse] = 1;

  for (int i = 0 ; i < maxUniverses ; i++)
  {
    if (universesReceived[i] == 0)
    {
      Serial.println("Broke");
      sendFrame = 0;
      break;
    }
  }

  // read universe and put into the right part of the display buffer
  for (int i = 0; i < length / channelsPerLed; i++)
  {
    int led = i + (universe - startUniverse) * (previousDataLength / channelsPerLed);
    if (led < numLeds) {
      if (channelsPerLed == 4)
        leds.setPixelColor(led, data[i * channelsPerLed], data[i * channelsPerLed + 1], data[i * channelsPerLed + 2], data[i * channelsPerLed + 3]);
      if (channelsPerLed == 3)
        leds.setPixelColor(led, data[i * channelsPerLed], data[i * channelsPerLed + 1], data[i * channelsPerLed + 2]);
    }
  }
  previousDataLength = length;

  if (sendFrame)
  {
    leds.show();
    // Reset universeReceived to 0
    memset(universesReceived, 0, maxUniverses);
  }
}

void initTest()
{
  for (int i = 0 ; i < numLeds ; i++)
    leds.setPixelColor(i, 127, 0, 0);
  leds.show();
  delay(500);
  for (int i = 0 ; i < numLeds ; i++)
    leds.setPixelColor(i, 0, 127, 0);
  leds.show();
  delay(500);
  for (int i = 0 ; i < numLeds ; i++)
    leds.setPixelColor(i, 0, 0, 127);
  leds.show();
  delay(500);
  for (int i = 0 ; i < numLeds ; i++)
    leds.setPixelColor(i, 0, 0, 0);
  leds.show();
}