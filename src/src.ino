#include <ESP8266WiFi.h>
#include "PubSubClient.h"
#include <DHT11.h>

byte server1[] = {192, 168, 0, 10}; // MQTT 브로커
int port = 1883;

char ssid[] = "GSM_IoT_2G";
char password[] = "12341234";
DHT11 dht11(4); // 4번핀

WiFiClient client;

void msgReceived(char *topic, byte *payload, unsigned int uLen) {
  char pBuffer[uLen + 1]; // 데이터 담는 배열
  // c언어는 null문자를 만나면 문자열 끝
  int i;
  for (i = 0; i < uLen; i++) {
    pBuffer[i] = payload[i];
  }
  pBuffer[i] = '\0';
  Serial.println(pBuffer);
  if (pBuffer[0] == '1') {
    digitalWrite(14, HIGH);
  } else if (pBuffer[0] == '2') {
    digitalWrite(14, LOW);
  }
}
PubSubClient mqttClient(server1, port, msgReceived, client);

void setup() {
  // put your setup code here, to run once:
  pinMode(14, OUTPUT);
  Serial.begin(9600);
  while (!Serial);
  Serial.println("1");

  delay(1000);
  Serial.println();
  Serial.println();
  Serial.println("Connection to~");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(",");
  }
  Serial.println("");
  Serial.println("Wi-Fi AP Connected!");

  Serial.println(WiFi.localIP());
  if (mqttClient.connect("Arduino")) { // MQTT브로커에 접속 시도
    Serial.println("MQTT Broker Connected!");
    mqttClient.subscribe("led");
  }
}

void loop() {
  mqttClient.loop();
  float tmp, hum;
  int err = dht11.read(tmp, hum); // 데이터 읽기
  if (err == 0) {
    char message[64] = "", pTmpBuf[50], pHumBuf[50];
    dtostrf(tmp, 5, 2, pTmpBuf);
    dtostrf(hum, 5, 2, pHumBuf);
    sprintf(message, "{\"temp\":5s, \"hum\" : %s}", pTmpBuf, pHumBuf);
    mqttClient.publish("dht11", message);
  }
  delay(3000);
}
