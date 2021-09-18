#include <WiFiClientSecure.h>
#include <ArduinoHttpClient.h>

// Discord setup
const char server[] = "discord.com";
const int port = 443;
const String discord_tts = "false";
const char fingerprint[] PROGMEM = "31 3B 70 94 D5 DF 90 78 9C A0 74 26 20 24 E4 3D 92 A7 57 9D";

// HTTPS Setup
WiFiClientSecure client;
HttpClient http_client = HttpClient(client, server, port);

void DSend(String content, String discord_webhook) {
  Serial.println("\n[HTTP] Connecting to Discord...");
  Serial.println("[HTTP] Message: " + content);
  Serial.println("[HTTP] URL: " + discord_webhook);

  http_client.post(discord_webhook, "application/json", "{\"content\":\"" + content + "\", \"tts\":" + discord_tts + "}");

  int statusCode = http_client.responseStatusCode();
  const String response = http_client.responseBody();

  if (statusCode == 204) {
    Serial.println("[HTTP] Status: message sent successfully!");
  }else{
    Serial.print("[HTTP] Error - Code: ");
    Serial.print(statusCode);
    Serial.println("\n[HTTP] Response: " + response);
  }
}
