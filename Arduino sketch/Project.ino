#include <WiFiNINA.h>
#include <WiFiUdp.h>
#include "arduino_secrets.h"
#define SERVER_ADDRESS "192.168.1.109"

const char ssid[] = SECRET_SSID;
const char pass[] = SECRET_PASS;
int status = WL_IDLE_STATUS;
WiFiClient client;

WiFiUDP Udp;
unsigned int localPort = 2390;
IPAddress timeServer(162, 159, 200, 123);
const int NTP_PACKET_SIZE = 48;
byte packetBuffer[NTP_PACKET_SIZE];
unsigned long epoch;

void setup() {
  connectToSerial();
  connectToWiFi();
  connectToServer();
  Udp.begin(localPort);
  Serial.println();
}

void connectToSerial() {
  Serial.begin(9600);
  while (!Serial) {
    ;
  }
}

void connectToWiFi() {
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    while (true);
  }

  String fv = WiFi.firmwareVersion();
  if (fv < WIFI_FIRMWARE_LATEST_VERSION) {
    Serial.println("Please upgrade the firmware");
  }

  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);
    status = WiFi.begin(ssid, pass);

    delay(10000);
  }

  Serial.println("You're connected to the network ");
  printCurrentNet();
  printWifiData();
}

void printCurrentNet() {
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  byte bssid[6];
  WiFi.BSSID(bssid);
  Serial.print("BSSID: ");
  printMacAddress(bssid);

  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.println(rssi);

  byte encryption = WiFi.encryptionType();
  Serial.print("Encryption Type: ");
  Serial.println(encryption, HEX);
  Serial.println();
}

void printWifiData() {
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  byte mac[6];
  WiFi.macAddress(mac);
  Serial.print("MAC address: ");
  printMacAddress(mac);
}

void printMacAddress(byte mac[]) {
  for (int i = 5; i >= 0; i--) {
    if (mac[i] < 16) {
      Serial.print("0");
    }
    Serial.print(mac[i], HEX);
    if (i > 0) {
      Serial.print(":");
    }
  }
  Serial.println();
}

void connectToServer() {
    if (client.connect(SERVER_ADDRESS, 80)) {
      Serial.println("Connected to server.");
    } else {
        if (WiFi.status() != WL_CONNECTED) {
          checkNetwork();
        }
        Serial.println("Error connecting to server.");
        delay(1000);
        connectToServer();
  }
}

void checkNetwork() {
  status = WiFi.status();
  while (status != WL_CONNECTED) {
      Serial.print("Attempting to connect to WiFi.. Status: ");
      Serial.println(status);
      status = WiFi.begin(ssid, pass);
      delay(10000);
  }
}

float getTemperature() {

  int reading = analogRead(A0);

  float voltage = reading * 5.0;
  voltage /= 1024.0;

  float temperatureC = (voltage - 0.5) * 100 ;

  return temperatureC;
}

void printTemperatue(float value) {
  Serial.print(value);
  Serial.println(" degrees");
}

void postTemperatureData(float value) {
  printTemperatue(value);
  String value_str(value);
  String epoch_str(epoch);
  client.println("POST /gateway.php HTTP/1.1");
  client.print("Host: ");
  client.println(SERVER_ADDRESS);
  client.println("Content-Type: application/x-www-form-urlencoded");
  client.print("Content-Length: "); client.println(String(value_str.length() + epoch_str.length() + 14));
  client.println();
  client.print("value="); client.print(value_str); client.print("&time="); client.print(epoch_str); client.println();

  delay(250);
  while (client.available()) {
    char c = client.read();
    Serial.write(c);
  }
}

void printWifiStatus() {
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}

unsigned long sendNTPpacket(IPAddress& address) {
  memset(packetBuffer, 0, NTP_PACKET_SIZE);

  packetBuffer[0] = 0b11100011;
  packetBuffer[12]  = 49;
  packetBuffer[13]  = 0x4E;
  packetBuffer[14]  = 49;
  packetBuffer[15]  = 52;

  Udp.beginPacket(address, 123);
  Udp.write(packetBuffer, NTP_PACKET_SIZE);
  Udp.endPacket();
}

void updateDateAndTime() {
  sendNTPpacket(timeServer);
  delay(1000);
  
  if (Udp.parsePacket()) {
    Udp.read(packetBuffer, NTP_PACKET_SIZE);

    unsigned long highWord = word(packetBuffer[40], packetBuffer[41]);
    unsigned long lowWord = word(packetBuffer[42], packetBuffer[43]);
    unsigned long secsSince1900 = highWord << 16 | lowWord;
    const unsigned long seventyYears = 2208988800UL;
    epoch = secsSince1900 - seventyYears;
  } else {
    Serial.println("No packet received.");
    epoch = 0;
  }
}

void loop() {
  checkNetwork();
  connectToServer();

  updateDateAndTime();
  postTemperatureData(getTemperature());
  Serial.println();
  
  delay(3750);
}
