#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <TimeLib.h>
#include <WiFiUdp.h>
#include <AM2320_asukiaaa.h>

//global variables
static const char ntpServerName[] = "0.pool.ntp.org";
int timeZone = 0;
ESP8266WebServer server(80);
const char* ssid = "AP Name";
const char* password = "AP password";
String webSite, javaScript, XML;
WiFiUDP Udp;
unsigned int localPort = 8888;
time_t timelast = 0;
time_t timetest, timenow, ttimenow, ttimemax, ttimemin, htimenow, htimemax, htimemin;
int templast, tnow, tmax, tmin, hnow, hmax, hmin, humnow, humlast;
bool start = false;
AM2320_asukiaaa mySensor;

//build the html component of the site
void buildWebsite() {
  buildJavascript();
  webSite = "<!DOCTYPE HTML>\n";
  webSite += javaScript;
  //Needed to make website nice to read on phone
  webSite += "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">";
  webSite += "<BODY onload='process()'>\n";
  webSite += "<b>Weather Thing</b></p>";
  
  //Table for display of data
  webSite += "<table stype=\"width:100%\" border=\"1\">";
  webSite += "<tr>";
  webSite += "<th>Reading</th>";
  webSite += "<th>Value</th>";
  webSite += "<th>Time</th>";
  webSite += "</tr>";
  
  webSite += "<tr>";
  webSite += "<td>TNow</td>";
  webSite += "<td>";
  webSite += "<A id='tnow'></A>\n";
  webSite += "degC";
  webSite += "<td>";
  webSite += "<A id='timenow'></A>\n";
  webSite += "</td>";
  webSite += "</tr>";

  webSite += "<tr>";
  webSite += "<td>HNow</td>";
  webSite += "<td>";
  webSite += "<A id='hnow'></A>\n";
  webSite += "%";
  webSite += "<td>";
  webSite += "<A id='timenow'></A>\n";
  webSite += "</td>";
  webSite += "</tr>";

  webSite += "<tr>";
  webSite += "<td>TMax</td>";
  webSite += "<td>";
  webSite += "<A id='tmax'></A>\n";
  webSite += "degC";
  webSite += "<td>";
  webSite += "<A id='ttimemax'></A>\n";
  webSite += "</td>";
  webSite += "</tr>";

  webSite += "<tr>";
  webSite += "<td>HMax</td>";
  webSite += "<td>";
  webSite += "<A id='hmax'></A>\n";
  webSite += "%";
  webSite += "<td>";
  webSite += "<A id='htimemax'></A>\n";
  webSite += "</td>";
  webSite += "</tr>";

  webSite += "<tr>";
  webSite += "<td>TMin</td>";
  webSite += "<td>";
  webSite += "<A id='tmin'></A>\n";
  webSite += "degC";
  webSite += "</td>";
  webSite += "<td>";
  webSite += "<A id='ttimemin'></A>\n";
  webSite += "</td>";
  webSite += "</tr>";

  webSite += "<tr>";
  webSite += "<td>HMin</td>";
  webSite += "<td>";
  webSite += "<A id='hmin'></A>\n";
  webSite += "%";
  webSite += "</td>";
  webSite += "<td>";
  webSite += "<A id='htimemin'></A>\n";
  webSite += "</td>";
  webSite += "</tr>";

  webSite += "</table>";
  webSite += "</BODY>\n";
  webSite += "</HTML>\n";
}

//build the javascript component of the website
void buildJavascript() {
  javaScript = "<SCRIPT>\n";
  javaScript += "var xmlHttp=createXmlHttpObject();\n";
  
  javaScript += "function createXmlHttpObject(){\n";
  javaScript += " if(window.XMLHttpRequest){\n";
  javaScript += "    xmlHttp=new XMLHttpRequest();\n";
  javaScript += " }else{\n";
  javaScript += "    xmlHttp=new ActiveXObject('Microsoft.XMLHTTP');\n";
  javaScript += " }\n";
  javaScript += " return xmlHttp;\n";
  javaScript += "}\n";

  javaScript += "function process(){\n";
  javaScript += " if(xmlHttp.readyState==0 || xmlHttp.readyState==4){\n";
  javaScript += "   xmlHttp.open('PUT','xml',true);\n";
  javaScript += "   xmlHttp.onreadystatechange=handleServerResponse;\n"; // no brackets?????
  javaScript += "   xmlHttp.send(null);\n";
  javaScript += " }\n";
  javaScript += " setTimeout('process()',1000);\n";
  javaScript += "}\n";

  javaScript += "function handleServerResponse(){\n";
  javaScript += " if(xmlHttp.readyState==4 && xmlHttp.status==200){\n";
  javaScript += "   xmlResponse=xmlHttp.responseXML;\n";

  javaScript += "   xmldoc = xmlResponse.getElementsByTagName('tnow');\n";
  javaScript += "   message = xmldoc[0].firstChild.nodeValue;\n";
  javaScript += "   document.getElementById('tnow').innerHTML=message;\n";

  javaScript += "   xmldoc = xmlResponse.getElementsByTagName('timenow');\n";
  javaScript += "   message = xmldoc[0].firstChild.nodeValue;\n";
  javaScript += "   document.getElementById('timenow').innerHTML=message;\n";

  javaScript += "   xmldoc = xmlResponse.getElementsByTagName('hnow');\n";
  javaScript += "   message = xmldoc[0].firstChild.nodeValue;\n";
  javaScript += "   document.getElementById('hnow').innerHTML=message;\n";
  
  javaScript += "   xmldoc = xmlResponse.getElementsByTagName('tmax');\n";
  javaScript += "   message = xmldoc[0].firstChild.nodeValue;\n";
  javaScript += "   document.getElementById('tmax').innerHTML=message;\n";

  javaScript += "   xmldoc = xmlResponse.getElementsByTagName('ttimemax');\n";
  javaScript += "   message = xmldoc[0].firstChild.nodeValue;\n";
  javaScript += "   document.getElementById('ttimemax').innerHTML=message;\n";

  javaScript += "   xmldoc = xmlResponse.getElementsByTagName('hmax');\n";
  javaScript += "   message = xmldoc[0].firstChild.nodeValue;\n";
  javaScript += "   document.getElementById('hmax').innerHTML=message;\n";

  javaScript += "   xmldoc = xmlResponse.getElementsByTagName('htimemax');\n";
  javaScript += "   message = xmldoc[0].firstChild.nodeValue;\n";
  javaScript += "   document.getElementById('htimemax').innerHTML=message;\n";

  javaScript += "   xmldoc = xmlResponse.getElementsByTagName('tmin');\n";
  javaScript += "   message = xmldoc[0].firstChild.nodeValue;\n";
  javaScript += "   document.getElementById('tmin').innerHTML=message;\n";

  javaScript += "   xmldoc = xmlResponse.getElementsByTagName('ttimemin');\n";
  javaScript += "   message = xmldoc[0].firstChild.nodeValue;\n";
  javaScript += "   document.getElementById('ttimemin').innerHTML=message;\n";

  javaScript += "   xmldoc = xmlResponse.getElementsByTagName('hmin');\n";
  javaScript += "   message = xmldoc[0].firstChild.nodeValue;\n";
  javaScript += "   document.getElementById('hmin').innerHTML=message;\n";

  javaScript += "   xmldoc = xmlResponse.getElementsByTagName('htimemin');\n";
  javaScript += "   message = xmldoc[0].firstChild.nodeValue;\n";
  javaScript += "   document.getElementById('htimemin').innerHTML=message;\n";

  javaScript += " }\n";
  javaScript += "}\n";
  javaScript += "</SCRIPT>\n";
}

//build the xml component of the website
void buildXML() {
  XML = "<?xml version='1.0'?>";
  XML += "<Stats>";
  XML += "<tnow>";
  XML += tnow;
  XML += "</tnow>";
  XML += "<timenow>";
  XML += formattime(timenow);
  XML += "</timenow>";
  XML += "<hnow>";
  XML += hnow;
  XML += "</hnow>";
  XML += "<tmax>";
  XML += tmax;
  XML += "</tmax>";
  XML += "<ttimemax>";
  XML += formattime(ttimemax);
  XML += "</ttimemax>";
  XML += "<hmax>";
  XML += hmax;
  XML += "</hmax>";
  XML += "<htimemax>";
  XML += formattime(htimemax);
  XML += "</htimemax>";
  XML += "<tmin>";
  XML += tmin;
  XML += "</tmin>";
  XML += "<ttimemin>";
  XML += formattime(ttimemin);
  XML += "</ttimemin>";
  XML += "<hmin>";
  XML += hmin;
  XML += "</hmin>";
  XML += "<htimemin>";
  XML += formattime(htimemin);
  XML += "</htimemin>";
  XML += "</Stats>";
}

//website handler
void handleWebsite() {
  buildWebsite();
  server.send(200, "text/html", webSite);
}

//xml handler
void handleXML() {
  buildXML();
  server.send(200, "text/xml", XML);
}

//NTP code to get time
const int NTP_PACKET_SIZE = 48; // NTP time is in the first 48 bytes of message
byte packetBuffer[NTP_PACKET_SIZE]; //buffer to hold incoming & outgoing packets

time_t getNtpTime()
{
  IPAddress ntpServerIP; // NTP server's ip address

  while (Udp.parsePacket() > 0) ; // discard any previously received packets
  Serial.println("Transmit NTP Request");
  // get a random server from the pool
  WiFi.hostByName(ntpServerName, ntpServerIP);
  Serial.print(ntpServerName);
  Serial.print(": ");
  Serial.println(ntpServerIP);
  sendNTPpacket(ntpServerIP);
  uint32_t beginWait = millis();
  while (millis() - beginWait < 1500) {
    int size = Udp.parsePacket();
    if (size >= NTP_PACKET_SIZE) {
      Serial.println("Receive NTP Response");
      Udp.read(packetBuffer, NTP_PACKET_SIZE);  // read packet into the buffer
      unsigned long secsSince1900;
      // convert four bytes starting at location 40 to a long integer
      secsSince1900 =  (unsigned long)packetBuffer[40] << 24;
      secsSince1900 |= (unsigned long)packetBuffer[41] << 16;
      secsSince1900 |= (unsigned long)packetBuffer[42] << 8;
      secsSince1900 |= (unsigned long)packetBuffer[43];
      return secsSince1900 - 2208988800UL + timeZone * SECS_PER_HOUR;
    }
  }
  Serial.println("No NTP Response :-(");
  return 0; // return 0 if unable to get the time
}

// send an NTP request to the time server at the given address
void sendNTPpacket(IPAddress &address)
{
  // set all bytes in the buffer to 0
  memset(packetBuffer, 0, NTP_PACKET_SIZE);
  // Initialize values needed to form NTP request
  // (see URL above for details on the packets)
  packetBuffer[0] = 0b11100011;   // LI, Version, Mode
  packetBuffer[1] = 0;     // Stratum, or type of clock
  packetBuffer[2] = 6;     // Polling Interval
  packetBuffer[3] = 0xEC;  // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  packetBuffer[12] = 49;
  packetBuffer[13] = 0x4E;
  packetBuffer[14] = 49;
  packetBuffer[15] = 52;
  // all NTP fields have been given values, now
  // you can send a packet requesting a timestamp:
  Udp.beginPacket(address, 123); //NTP requests are to port 123
  Udp.write(packetBuffer, NTP_PACKET_SIZE);
  Udp.endPacket();
}

//show the time value as a readable format
String formattime(time_t in)
{
  String timestamp = "";
  // nice formatted display of the time
  timestamp += year(in);
  timestamp += "-";
  if (month() <= 9) {
    timestamp += "0";
  }
  timestamp += month(in);
  timestamp += "-";
  if (day(in) <= 9) {
    timestamp += "0";
  }
  timestamp += day(in);
  timestamp += "T";
  if (hour(in) <= 9) {
    timestamp += "0";
  }
  timestamp += hour(in);
  timestamp += ":";
  if (minute(in) <= 9) {
    timestamp += "0";
  }
  timestamp += minute(in);
  timestamp += ":";
  if (second(in) <= 9) {
    timestamp += "0";
  }
  timestamp += second(in);
  //Serial.println(timestamp);
  return timestamp;
}


//Initialise all the junk
void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)delay(500);
  WiFi.mode(WIFI_STA);
  Serial.println("\n\nBOOTING ESP8266 ...");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("Station IP address: ");
  Serial.println(WiFi.localIP());
  server.on("/", handleWebsite);
  server.on("/xml", handleXML);
  server.begin();
  Serial.println("Starting UDP");
  Udp.begin(localPort);
  Serial.print("Local port: ");
  Serial.println(Udp.localPort());
  Serial.println("waiting for sync");
  setSyncProvider(getNtpTime);
  setSyncInterval(3600);
  Wire.begin();
  mySensor.setWire(&Wire);
  //reset the min and max values and timestamps
  if (mySensor.update() != 0) {
    Serial.println("Error: Cannot update sensor values.");
  } else {
  tnow = mySensor.temperatureC;
  hnow = mySensor.humidity;
  }
  Serial.print("Starting Temperature ");
  Serial.print(tnow);
  Serial.println("degC");
  Serial.print("Starting Humidity ");
  Serial.print(hnow);
  Serial.println("%");
  //sensor wait
  delay(500);
  tmax = tnow;
  hmax = hnow;
  tmin = tnow;
  hmin = hnow;
  templast = tnow;
  humlast = hnow;
  timenow = now();
  ttimemax = timenow;
  htimemax = timenow;
  ttimemin = timenow;
  htimemin = timenow;
  timetest = timenow;
}

//main program loop
void loop() {
  server.handleClient();
  if (timeStatus() != timeNotSet) {
    //update the display only if time has changed
    if (now() != timelast) {
      timelast = now();
      timenow = now();
    }
  }
  
  //Wait for the start of the hour to begin logging
  if (minute(timenow) == 0 and second(timenow) == 0) {
    start = true;
    Serial.println("Logging started on the hour");
  }
  
  //read the values for display + max and min checking
  if (mySensor.update() != 0) {
    Serial.println("Error: Cannot update sensor values.");
  } else {
  tnow = mySensor.temperatureC;
  hnow = mySensor.humidity;
  }
  //sensor wait
  delay(500);
    //if we get a new max we record it
  if (tnow > tmax) {
    ttimemax = timenow;
    tmax = tnow;
  }
  if (hnow > hmax) {
    htimemax = timenow;
    hmax = hnow;
  }

  //If we get a new min we record it
  if (tnow < tmin) {
    ttimemin = timenow;
    tmin = tnow;
  }
  if (hnow < hmin) {
    htimemin = timenow;
    hmin = hnow;
  } 

  //save the temp to the array every 15 minutes
  if ((timenow >= timetest) and start) {
    //Write to MySQL instance in cloud
    String poststring = "http://<site>/function.php?Action=insert&Temperature=";
    poststring += tnow;
    poststring += "&Humidity=";
    poststring += hnow;
    poststring += "&TimeStamp=";
    poststring += formattime(timenow);
    Serial.println(poststring);
    HTTPClient http;
    http.begin(poststring);
    int httpCode = http.GET();
    if (httpCode > 0) { //Check the returning code
      String payload = http.getString();   //Get the request response payload
      Serial.println(payload);             //Print the response payload
    }
    http.end();
    timetest = timenow + 900; //15 minutes is 900 seconds
    
  }
  //Reset for midnight
  if (formattime(timenow).substring(11) == "11:45:00") {
    delay(1000);
    ESP.restart();//reboot the unit as a housekeeping task it will restart logging on the hour
  }
}
