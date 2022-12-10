/*
 * Connect the RN2xx3 as follows:
 * RN2xx3 -- ESP8266
 * Uart TX -- D1
 * Uart RX -- D2
 * Vcc -- 3.3V
 * Gnd -- Gnd
 *
 */
#include <rn2xx3.h>
#include <SoftwareSerial.h>

#define RESET 15
SoftwareSerial mySerial(D1, D2);

// using LoRa WAN
rn2xx3 myLora(mySerial);

void setup()
{
  // LED pin is GPIO2 which is the ESP8266's built in LED
  pinMode(2, OUTPUT);

  // Open serial communications and wait for port to open:
  Serial.begin(19200);
  mySerial.begin(57600); // sofware serial for LoRa

  delay(1000); // wait for the arduino ide's serial console to open

  Serial.println("Startup");

  initialize_radio();

  // transmit a startup message
  myLora.tx("TTN Mapper on ESP8266 node");

  initWifi();

  delay(2000);
}

void initialize_radio()
{
  // reset RN2xx3
  pinMode(RESET, OUTPUT);
  digitalWrite(RESET, LOW);
  delay(100);
  digitalWrite(RESET, HIGH);

  delay(100); // wait for the RN2xx3's startup message
  mySerial.flush();

  // check communication with radio
  String hweui = myLora.hweui();
  while (hweui.length() != 16)
  {
    Serial.println("Communication with RN2xx3 unsuccessful. Power cycle the board.");
    Serial.println(hweui);
    delay(10000);
    hweui = myLora.hweui();
  }

  Serial.println("RN2xx3 firmware version:");
  Serial.println(myLora.sysver());

  // configure your keys and join the network
  Serial.println("Trying to join TTN");
  bool join_result = false;

  // TTN EUIs
  join_result = myLora.initOTAA("0123456789ABCDEF", "A9C2ECFFBBE76304EEC5DC7470AC2E93", "70B3D57ED005765E");

  while (!join_result)
  {
    Serial.println("Unable to join. Are your keys correct, and do you have TTN coverage?");
    delay(60000); // delay a minute before retry
    join_result = myLora.init();
  }
  Serial.println("Successfully joined TTN");
}
char msg[35];
// the loop routine runs over and over again forever:
void loop()
{

  uint8_t lenght = scanSSIDs(msg);
  String rxMsg = getRx();
  if (rxMsg.length() > 0)
    myLora.txBytes((byte *)msg, lenght); // one byte, blocking function

  else
  {
    // use this becuse recive data package somewhere goes wrong
    delay(19000);
    myLora.txBytes((byte *)msg, lenght);
  }
  delay(1000);
}