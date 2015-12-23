/*
 (C) Copyright 2015 by Marek Hakala <hakala.marek@gmail.com>

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

 http://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
    limitations under the License.
*/

// Interface configuration  
#define ET_IF_WEB // Web
#define ET_IF_SERIAL // Serial line

#include <SPI.h>
#include <Wire.h>
#include <Ethernet.h>

#include "DHT.h"
#include "WebServer.h"

// Network configuration
#define ET_DHCP // Enable DHCP
//#undef ET_DHCP // Enable Manual IP config

// Temperature sensor(DHT22) PIN configuration: A0
#define DHT_22_PIN A0

// Network
byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};

#ifndef ET_DHCP
  IPAddress ip(10,0,0,222);
  IPAddress gateway(10,0,0,138);	
  IPAddress subnet(255,255,255,0);
#endif

dht dht_sensor;
int mode = 0;
float humidity = 0.0;
float temperature = 0.0;
float temperature_fahreinheit = 0.0;

#ifdef ET_IF_WEB
WebServer webserver("", 80);

void web_home(WebServer &server, WebServer::ConnectionType type, char *, bool) {
  server.httpSuccess("application/xml; charset=utf-8");

  if (type != WebServer::HEAD) {
    P(xml1) = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
    "<?xml-stylesheet type=\"text/xsl\" href=\"et.xsl\"?>"
    "<response>"
    " <values count=\"3\">"
    "   <value type=\"temperature\" unit=\"celsius\">";
    P(xm2) = "   </value>"
    "   <value type=\"temperature\" unit=\"fahreinheit\">";
    P(xml3) = "   </value>"
    "   <value type=\"humidity\" unit=\"percentage\">";
    P(xml4) = "   </value>"
    " </values>"
    "</response>";
    
    server.printP(xml1);
    server.print(temperature);
    server.printP(xm2);
    server.print(temperature_fahreinheit);
    server.printP(xml3);
    server.print(humidity);
    server.printP(xml4);
  }
}

void web_xsl(WebServer &server, WebServer::ConnectionType type, char *, bool) {
  server.httpSuccess("application/xml; charset=utf-8");

  if (type != WebServer::HEAD) {
    P(xml) = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
    "<xsl:stylesheet version=\"1.0\" xmlns:xsl=\"http://www.w3.org/1999/XSL/Transform\">"
    " <xsl:template match=\"/\">"
    "  <html>"
    "   <head>"
    "    <title>Ethernet thermometer</title>"
    "    <link href=\"https://maxcdn.bootstrapcdn.com/bootstrap/3.3.6/css/bootstrap.min.css\" rel=\"stylesheet\" integrity=\"sha256-7s5uDGW3AHqw6xtJmNNtr+OBRJUlgkNJEo78P4b0yRw= sha512-nNo+yCHEyn0smMxSswnf/OnX6/KwJuZTlNZBjauKhTK0c+zT+q5JOCx0UFhXQ6rJR9jg6Es8gPuD2uZcYDLqSw==\" crossorigin=\"anonymous\" />"
    "   </head>"
    "   <body>"
    "    <div class=\"container\">"
    "     <div class=\"row text-center\">"
    "      <h1>Ethernet thermometer</h1>"
    "     </div>"
    "     <div class=\"row text-center\">"
    "      <hr />"
    "      <p><a href=\"/\" class=\"btn btn-xs btn-default\">Update</a></p>"
    "      <hr />"
    "      <table class=\"table table-striped table-bordered\">"
    "       <tr>"
    "        <th>Celsius</th>"
    "        <th>Fahrenheit</th>"
    "        <th>Humidity</th>"
    "       </tr>"
    "       <tr>"
    "        <td><xsl:value-of select=\"response/values/value[1]\"/> &#176;C</td>"
    "        <td><xsl:value-of select=\"response/values/value[2]\"/> &#176;F</td>"
    "        <td><xsl:value-of select=\"response/values/value[3]\"/> %</td>"
    "       </tr>"
    "      </table>"
    "     </div>"
    "    </div>"
    "   </body>"
    "  </html>"
    " </xsl:template>"
    "</xsl:stylesheet>";
    
    server.printP(xml);
  }
}
#endif

void setup() {
  // Setup serial line
  Serial.begin(9600); 
  Serial.println("Ethernet Thermometer 1.0");

  // Setup network
  SPI.begin();

#ifdef ET_DHCP
  Ethernet.begin(mac);
#else
  Ethernet.begin(mac, ip);
#endif

#ifdef ET_IF_SERIAL
  Serial.print("IP addr: ");
  Serial.println(Ethernet.localIP());
#endif

#ifdef ET_IF_WEB
  webserver.setDefaultCommand(&web_home);
  webserver.addCommand("et.xsl", &web_xsl);
#endif
}

void loop() {
  char buff[64];
  int len = 64;
  
  mode = (mode + 1) % 100;
  sensors_update();
  
#ifdef ET_IF_SERIAL
  serial_output();
#endif

#ifdef ET_IF_WEB
  webserver.processConnection(buff, &len);
#endif
}

float convert_c_to_f(float celsius) {
  return celsius * 1.8 + 32;
}

void sensors_update() {
  if (mode%30 == 0 && dht_sensor.read22(DHT_22_PIN) == DHTLIB_OK) {
    humidity = dht_sensor.humidity;
    temperature = dht_sensor.temperature;
    temperature_fahreinheit = convert_c_to_f(temperature);
  }
}

void serial_output() {
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.print(" *C \t");
  Serial.print(temperature_fahreinheit);
  Serial.print(" *F \t");
  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.print(" %\n");
}

