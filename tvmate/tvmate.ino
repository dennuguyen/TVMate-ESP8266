#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WebSocketsServer.h>

const char* ssid = "";
const char* password = "";

const int port = 80;

WebSocketsServer webSocket = WebSocketsServer(port);  // websocket init with port 81

void webSocketEvent(uint8_t num, WStype_t type, uint8_t* payload, size_t length) {
    switch (type) {
        case WStype_DISCONNECTED:
            Serial.printf("[%u] Disconnected!\n", num);
            break;
        case WStype_CONNECTED: {
            IPAddress ip = webSocket.remoteIP(num);
            Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);

            // send message to client
            webSocket.sendTXT(num, "Connected");
        } break;
        case WStype_TEXT:
            Serial.printf("[%u] get Text: %s\n", num, payload);

            // send message to client
            // webSocket.sendTXT(num, "message here");

            // send data to all connected clients
            // webSocket.broadcastTXT("message here");
            break;
        case WStype_BIN:
            Serial.printf("[%u] get binary length: %u\n", num, length);
            hexdump(payload, length);

            // send message to client
            // webSocket.sendBIN(num, payload, length);
            break;
    }
}

void setup() {
    Serial.begin(115200);

    // Connect to WiFi network.
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    Serial.println("");

    // Wait for connection.
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.print("Connected to WiFi: ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());

    if (!MDNS.begin("esp8266")) {
        Serial.println("ERROR: Could not set up mDNS responder.");
        while (1);
    }

    Serial.println("mDNS responder started.");
    MDNS.addService("esp8266", "tcp", port);

    // Service txt is required for Bonsoir to discover the device.
    MDNS.addServiceTxt("esp8266", "tcp", "vendor", "Espressif");
    MDNS.addServiceTxt("esp8266", "tcp", "version", "1.0");
    MDNS.addServiceTxt("esp8266", "tcp", "module", "AT");

    webSocket.begin();
    webSocket.onEvent(webSocketEvent);
    Serial.println("Websocket server started.");
}

void loop() {
    MDNS.update();
    webSocket.loop();
}