/*
 *  HTTP over TLS (HTTPS) example sketch
 *
 *  This example demonstrates how to use
 *  WiFiClientSecure class to access HTTPS API.
 *  We fetch and display the status of
 *  esp8266/Arduino project continous integration
 *  build.
 *
 *  Created by Ivan Grokhotkov, 2015.
 *  This example is in public domain.
 */

#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>

const char* ssid = "ssid";
const char* password = "pass";

const char* host = "zooibivgswn.mlkcca.com";
const int httpsPort = 443;

boolean write_flag = false;
String data = "";
int i;

void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.print("connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());


  // Use WiFiClientSecure class to create TLS connection

  WiFiClientSecure client;
  Serial.print("connecting to ");
  Serial.println(host);
  if (!client.connect(host, httpsPort)) {
    Serial.println("connection failed");
    return;
  }

  String url = "/api?api=get&appid=zooibivgswn&id=ibixc59pkloi7hp&path=heatmap";
  Serial.print("requesting URL: ");
  Serial.println(url);

  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "User-Agent: BuildFailureDetectorESP8266\r\n" +
               "Connection: close\r\n\r\n");

  Serial.println("request sent");
  while (client.connected()) {
    String line = client.readString();
    if (line == "\r") {
      Serial.println("headers received");
      break;
    }
  }
  String line = client.readString();
  int lineLength = line.length();

  const char *lineChar = line.c_str();

  for(i = 0; i < lineLength; i++) {
    if(lineChar[i] == '{'){
      write_flag = true;
    }
    if(write_flag && lineChar[i] != '\\'){
      data = data + lineChar[i];
    }
  }
  data.replace("\"value\":\"","\"value\":");
  data.replace("\"}}}","}}}");
  data.replace("{\"err\":null,\"content\":{\"d\":","");
  data.replace("}}}}","}}");

  const char *json = data.c_str();
  StaticJsonBuffer<1000> jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(json);
  int value = root["value"]["v"];
  Serial.println("responce was:");
  Serial.println("==========");
  Serial.println(data);
  Serial.println("value.v is ...");
  Serial.println(value);
  Serial.println("==========");
  Serial.println("closing connection");
}

void loop() {
}