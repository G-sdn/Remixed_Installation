#include <Ethernet.h>
#include <EthernetUdp.h>

#define enA 9
#define in1 6
#define in2 7

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};
IPAddress ip(192, 168, 0, 103);
IPAddress hostIP(192,168,0,2);

unsigned int localPort = 8888;  

char packetBuffer[UDP_TX_PACKET_MAX_SIZE]; 
char ReplyBuffer[] = "acknowledged";       

EthernetUDP Udp;
EthernetClient client;

void setup() {

  pinMode(enA, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  // Set initial rotation direction
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);

  // start the Ethernet
  Ethernet.begin(mac, ip);

  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  // start UDP
  Udp.begin(localPort);

  // analogWrite(enA, 255);
  // digitalWrite(in1, HIGH);
  // digitalWrite(in2, LOW);
  // delay(1000);
  // digitalWrite(in1, LOW);
  // digitalWrite(in2, HIGH);
  // delay(1000);
  // analogWrite(enA, 0);
}

void loop() {
  // if there's data available, read a packet
  int packetSize = Udp.parsePacket();
  if (packetSize) {
    // Serial.print("Received packet from ");
    // IPAddress remote = Udp.remoteIP();
    // for (int i=0; i < 4; i++) {
    //   Serial.print(remote[i], DEC);
    //   if (i < 3) {
    //     Serial.print(".");
    //   }
    // }

    // read the packet into packetBufffer
    Udp.read(packetBuffer, UDP_TX_PACKET_MAX_SIZE);
    Serial.println("Contents:");
    Serial.println(packetBuffer);

    char* separator = strchr(packetBuffer, ':');
    if (separator != 0)
    {
        // Actually split the string in 2: replace ':' with 0
        *separator = 0;
        String command = packetBuffer;
        ++separator;
        int value = atoi(separator);

        // Do something with servoId and position
        Serial.println(command);
        Serial.println(value);

        if(command == "speed"){
          changeSpeed(value);
        } else if (command == "direction"){
          changeDirection(value);
        } else {
          Serial.println("No valid command received");
        }
    }

    // send a reply to the IP address and port that sent us the packet we received
    Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
    Udp.write(ReplyBuffer);
    Udp.endPacket();
  }
}

void changeSpeed(int value){
  analogWrite(enA, value);
}

void changeDirection(int value){
  if(value == 0){
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
  } else if (value == 1){
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
  }
}
