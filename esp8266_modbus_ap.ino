/*
   ESP8266 ile Modbus Slave modda Access Point Mod ile calistirilmasi.
   21.11.2021
   electrocoder@gmail.com
   sahin mersin
   Mese Bilisim Yazilim Mekatronik
*/

#include <ESP8266WiFi.h>
#include <WiFiManager.h>          //https://github.com/tzapu/WiFiManager
#include <Ticker.h>

const char* ssid = "Sahin";
const char* password = "**********";


Ticker ticker;

#define AP_BUTTON_PIN 0 // GPIO0

WiFiManager wifiManager;

int ModbusTCP_port = 502;

//////// Required for Modbus TCP / IP /// Requerido para Modbus TCP/IP /////////
#define maxInputRegister 20
#define maxHoldingRegister 20

#define MB_FC_NONE 0
#define MB_FC_READ_REGISTERS 3 //implemented
#define MB_FC_WRITE_REGISTER 6 //implemented
#define MB_FC_WRITE_MULTIPLE_REGISTERS 16 //implemented
//
// MODBUS Error Codes
//
#define MB_EC_NONE 0
#define MB_EC_ILLEGAL_FUNCTION 1
#define MB_EC_ILLEGAL_DATA_ADDRESS 2
#define MB_EC_ILLEGAL_DATA_VALUE 3
#define MB_EC_SLAVE_DEVICE_FAILURE 4
//
// MODBUS MBAP offsets
//
#define MB_TCP_TID 0
#define MB_TCP_PID 2
#define MB_TCP_LEN 4
#define MB_TCP_UID 6
#define MB_TCP_FUNC 7
#define MB_TCP_REGISTER_START 8
#define MB_TCP_REGISTER_NUMBER 10

byte ByteArray[260];
unsigned int MBHoldingRegister[maxHoldingRegister];

//////////////////////////////////////////////////////////////////////////

WiFiServer MBServer(ModbusTCP_port);

//

void tick() {

  //toggle state
  int state = digitalRead(BUILTIN_LED);  // get the current state of GPIO1 pin
  digitalWrite(BUILTIN_LED, !state);     // set pin to the opposite state

}


void configModeCallback (WiFiManager *myWiFiManager) {

  //gets called when WiFiManager enters configuration mode
  Serial.println("Entered config mode");
  Serial.println(WiFi.softAPIP());
  //if you used auto generated SSID, print it
  Serial.println(myWiFiManager->getConfigPortalSSID());
  //entered config mode, make led toggle faster
  ticker.attach(0.2, tick);

}


void checkButton() {

  // AP butonu ile Access Point moduna gecilirse Wifi uzerinden Update yapilabilir.
  // check for button press
  if ( digitalRead(AP_BUTTON_PIN) == 0 ) {
    // poor mans debounce/press-hold, code not ideal for production
    delay(50);
    if ( digitalRead(AP_BUTTON_PIN) == 0 ) {
      Serial.println("Button Pressed");
      // still holding button for 3000 ms, reset settings, code not ideaa for production
      delay(3000); // reset delay hold
      if ( digitalRead(AP_BUTTON_PIN) == 0 ) {
        Serial.println("Button Held");
        Serial.println("Erasing Config, restarting");
        wifiManager.resetSettings();
        ESP.reset();
      }
    }
  }

  delay(10);

}


void setup() {

  Serial.begin(115200);
  pinMode(BUILTIN_LED, OUTPUT);
  pinMode(AP_BUTTON_PIN, INPUT_PULLUP);
  ticker.attach(0.6, tick);
  delay(100);
  wifiManager.setAPCallback(configModeCallback);
  delay(100);
  Serial.println(".");

  if (!wifiManager.autoConnect("MeseIOT", "MeseIOT**")) {
    Serial.println("failed to connect and hit timeout");
    //reset and try again, or maybe put it to deep sleep
    ESP.reset();
    delay(1000);
  }

  //if you get here you have connected to the WiFi
  Serial.println("connected...yeey :)");
  ticker.detach();
  //keep LED on
  digitalWrite(BUILTIN_LED, LOW);

  //
  MBServer.begin();
  Serial.println("Connected ");
  Serial.print("ESP8266 Slave Modbus TCP/IP ");
  Serial.print(WiFi.localIP()); Serial.print(":"); Serial.println(String(ModbusTCP_port));
  Serial.println("Modbus TCP/IP Online");

}


void loop() {

  // buttonCheck AP Mode Update 0 GPIO0
  // AP butonu ile Access Point moduna gecilirse Wifi uzerinden Update yapilabilir.
  checkButton();
  // buttonCheck AP Mode Update 0 GPIO0

  // Check if a client has connected // Modbus TCP/IP
  WiFiClient client = MBServer.available();
  if (!client) {
    return;
  }

  boolean flagClientConnected = 0;
  byte byteFN = MB_FC_NONE;
  int Start;
  int WordDataLength;
  int ByteDataLength;
  int MessageLength;

  // Modbus TCP/IP
  while (client.connected()) {

    if (client.available()) {

      flagClientConnected = 1;
      int i = 0;

      while (client.available()) {

        ByteArray[i] = client.read();
        i++;

      }

      client.flush();

      ///// code here --- codigo aqui

      ///////// Holding Register [0] A [9] = 10 Holding Registers Escritura
      ///////// Holding Register [0] A [9] = 10 Holding Registers Writing

      MBHoldingRegister[0] = random(0, 12);  // 40001
      MBHoldingRegister[1] = random(0, 12);  // 40002
      MBHoldingRegister[2] = random(0, 12);  // 40003
      MBHoldingRegister[3] = random(0, 12);  // 40004
      MBHoldingRegister[4] = random(0, 12);  // 40005
      MBHoldingRegister[5] = random(0, 12);  // 40006
      MBHoldingRegister[6] = random(0, 12);  // 40007
      MBHoldingRegister[7] = random(0, 12);  // 40008
      MBHoldingRegister[8] = random(0, 12);  // 40009
      MBHoldingRegister[9] = random(0, 12);  // 40010

      ///////// Holding Register [10] A [19] = 10 Holding Registers Lectura
      ///// Holding Register [10] A [19] = 10 Holding Registers Reading

      int Temporal[10];

      Temporal[0] = MBHoldingRegister[10];  // 40011
      Temporal[1] = MBHoldingRegister[11];  // 40012
      Temporal[2] = MBHoldingRegister[12];  // 40013
      Temporal[3] = MBHoldingRegister[13];  // 40014
      Temporal[4] = MBHoldingRegister[14];  // 40015
      Temporal[5] = MBHoldingRegister[15];  // 40016
      Temporal[6] = MBHoldingRegister[16];  // 40017
      Temporal[7] = MBHoldingRegister[17];  // 40018
      Temporal[8] = MBHoldingRegister[18];  // 40019
      Temporal[9] = MBHoldingRegister[19];  // 40020

      /// Enable Output 14
      digitalWrite(2, MBHoldingRegister[10] );

      //// debug
      for (int i = 0; i < 10; i++) {

        Serial.print("[");
        Serial.print(i + 10);
        Serial.print("] ");
        Serial.print(Temporal[i]);
        Serial.print("  ");

      }

      Serial.println("");

      //// end code - fin


      //// routine Modbus TCP
      byteFN = ByteArray[MB_TCP_FUNC];
      Start = word(ByteArray[MB_TCP_REGISTER_START], ByteArray[MB_TCP_REGISTER_START + 1]);
      WordDataLength = word(ByteArray[MB_TCP_REGISTER_NUMBER], ByteArray[MB_TCP_REGISTER_NUMBER + 1]);
    }

    // Handle request
    switch (byteFN) {

      case MB_FC_NONE:
        break;

      case MB_FC_READ_REGISTERS: // 03 Read Holding Registers
        ByteDataLength = WordDataLength * 2;
        ByteArray[5] = ByteDataLength + 3; //Number of bytes after this one.
        ByteArray[8] = ByteDataLength; //Number of bytes after this one (or number of bytes of data).
        for (int i = 0; i < WordDataLength; i++) {
          ByteArray[ 9 + i * 2] = highByte(MBHoldingRegister[Start + i]);
          ByteArray[10 + i * 2] = lowByte(MBHoldingRegister[Start + i]);
        }
        MessageLength = ByteDataLength + 9;
        client.write((const uint8_t *)ByteArray, MessageLength);
        byteFN = MB_FC_NONE;
        break;

      case MB_FC_WRITE_REGISTER: // 06 Write Holding Register
        MBHoldingRegister[Start] = word(ByteArray[MB_TCP_REGISTER_NUMBER], ByteArray[MB_TCP_REGISTER_NUMBER + 1]);
        ByteArray[5] = 6; //Number of bytes after this one.
        MessageLength = 12;
        client.write((const uint8_t *)ByteArray, MessageLength);
        byteFN = MB_FC_NONE;
        break;

      case MB_FC_WRITE_MULTIPLE_REGISTERS: //16 Write Holding Registers
        ByteDataLength = WordDataLength * 2;
        ByteArray[5] = ByteDataLength + 3; //Number of bytes after this one.
        for (int i = 0; i < WordDataLength; i++) {
          MBHoldingRegister[Start + i] = word(ByteArray[ 13 + i * 2], ByteArray[14 + i * 2]);
        }
        MessageLength = 12;
        client.write((const uint8_t *)ByteArray, MessageLength);
        byteFN = MB_FC_NONE;
        break;
    }
  }

}
