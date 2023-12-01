#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <time.h>

#define MY_NTP_SERVER "at.pool.ntp.org"
#define MY_TZ "AST4ADT,M3.2.0/02:00,M11.1.0/02:00"
/*-------------CONFIG--------------------*/
const char *WIFI_SSID = "TP-LINK_944A";
const char *WIFI_Password = "33964355";
const String Relay_Password = "cemas";
const int Relay_PIN = 0;
/*-------------CONFIG--------------------*/

ESP8266WebServer server(80);
const long offsetTime = -14400; // Time Zone * 60 * 60
String State = "OFF";

/* Globals */
time_t now; // this are the seconds since Epoch (1970) - UTC
tm tm;

void setup(void)
{
      pinMode(Relay_PIN, OUTPUT);
      digitalWrite(Relay_PIN, LOW);

      Serial.begin(115200);

      Serial.println("\nNTP TZ DST - bare minimum");
      configTime(MY_TZ, MY_NTP_SERVER);

      Serial.println("Time configured. Waiting for time sync...");

      // Wait for time to be synchronized
      while (!time(nullptr))
      {
            delay(1000);
            Serial.println("Waiting for time to be synchronized...");
      }

      Serial.println("Time synchronized!");

      WiFi.begin(WIFI_SSID, WIFI_Password);
      Serial.println("Here: connecting ");

      // Wait for connection
      while (WiFi.status() != WL_CONNECTED)
      {
            delay(500);
            Serial.print(".");
      }

      Serial.println("");
      Serial.print("Connected to ");
      Serial.println(WIFI_SSID);
      Serial.print("IP address: ");
      Serial.println(WiFi.localIP());

      if (MDNS.begin("esp8266"))
      {
            Serial.println("MDNS responder started");
      }

      server.on("/", handleRoot);

      server.begin();
      Serial.println("HTTP server started");
}

void loop(void)
{
  ringByNTPTime();
  server.handleClient();
}

void handleRoot()
{
      if (server.method() != HTTP_POST)
      {
            server.send(405, "text/plain", "Method Not Allowed");
      }
      else
      {
            Serial.println(server.arg(0));
            Serial.println(server.arg(1));

            if (server.arg(0) == Relay_Password)
            {
                  if (server.arg(1) == "ON")
                  {
                        State = "ON";
                        server.send(200, "text/plain", State);
                        digitalWrite(Relay_PIN, HIGH);
                  }
                  else if (server.arg(1) == "OFF")
                  {
                        State = "OFF";
                        server.send(200, "text/plain", State);
                        digitalWrite(Relay_PIN, LOW);
                  }
                  else if (server.arg(1) == "STATE")
                  {
                        server.send(200, "text/plain", State);
                  }
                  else
                  {
                        server.send(200, "text/plain", "UNKNOWN_COMMAND");
                  }
            }
            else
            {
                  server.send(401, "text/plain", "AUTH_FAIL");
            }
      }
}

void ringByNTPTime()
{
      time(&now);             // read the current time
      localtime_r(&now, &tm); // update the structure tm with the current time
      int hour = tm.tm_hour;
      int minutes = tm.tm_min;
      int weekDay = tm.tm_wday;
      String timeNowString = "" + String(hour) + "" + String(minutes);
      int caseSwitch = timeNowString.toInt();
      Serial.println(caseSwitch);
      Serial.println(weekDay);

      if ((weekDay > 0 && weekDay < 6) && (hour > 6 && hour < 16))
      {
            switch (caseSwitch)
            {
            case 72:
                  ring();
                  break;

            case 80:
                  ring();
                  break;

            case 850:
                  ring();
                  break;

            case 940:
                  ring();
                  break;

            case 100:
                  ring();
                  break;

            case 1050:
                  ring();
                  break;

            case 1140:
                  ring();
                  break;

            case 1220:
                  ring();
                  break;

            case 130:
                  ring();
                  break;

            case 1350:
                  ring();
                  break;

            case 1440:
                  Serial.println("Bye!!, end the day.");
                  ring();
                  break;

            default:
              Serial.println("Time between hours doorbell off");
              delay(1000);
              digitalWrite(Relay_PIN, LOW);
              break;
            }
      }
      else
      {
        Serial.println("In weekend and doorbell off");
        digitalWrite(Relay_PIN, LOW);
        delay(1000);
      }
}

void ring()
{
  Serial.println("doorbell on");
  digitalWrite(Relay_PIN, LOW);
  waitRing();
  digitalWrite(Relay_PIN, HIGH);
  Serial.println("doorbell off");
  waitMinute();
}

void waitRing()
{
      delay(6000); // delay for 6 seconds
}

void waitMinute()
{
      delay(70000); // delay for 1 minute
}