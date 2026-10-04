#include <WiFi.h>
#include <WebServer.h>

// Wokwi virtual WiFi network credentials
const char* ssid = "Wokwi-GUEST";
const char* password = "";

const int LED_PIN = 2; // Onboard/External LED on GPIO 2
WebServer server(80);

// HTML page content with ON/OFF buttons
String getHTMLPage(bool state) {
  String page = "<!DOCTYPE html><html><head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">";
  page += "<title>ESP32 Web Control</title>";
  page += "<style>body{font-family:Arial;text-align:center;margin-top:50px;}";
  page += ".btn{padding:15px 30px;font-size:18px;margin:10px;text-decoration:none;color:white;border-radius:5px;display:inline-block;}";
  page += ".on{background-color:#4CAF50;} .off{background-color:#f44336;}</style></head><body>";
  page += "<h2>ESP32 Web-Controlled LED</h2>";
  page += "<p>LED Status: <b>" + String(state ? "ON" : "OFF") + "</b></p>";
  page += "<a class=\"btn on\" href=\"/led/on\">TURN ON</a>";
  page += "<a class=\"btn off\" href=\"/led/off\">TURN OFF</a>";
  page += "</body></html>";
  return page;
}

void handleRoot() {
  server.send(200, "text/html", getHTMLPage(digitalRead(LED_PIN)));
}

void handleLEDOn() {
  digitalWrite(LED_PIN, HIGH);
  server.send(200, "text/html", getHTMLPage(true));
}

void handleLEDOff() {
  digitalWrite(LED_PIN, LOW);
  server.send(200, "text/html", getHTMLPage(false));
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi Connected!");
  Serial.print("Access the web app at: http://");
  Serial.println(WiFi.localIP());

  // Define route handlers
  server.on("/", handleRoot);
  server.on("/led/on", handleLEDOn);
  server.on("/led/off", handleLEDOff);

  server.begin();
}

void loop() {
  server.handleClient(); // Handle incoming HTTP requests
}
