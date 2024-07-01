#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WebSocketsServer.h>

#include <IRremote.hpp>

#define IR_SEND_PIN 2

const char* SSID = "WiFi-42F4";
const char* PASSWORD = "aEtRsT2$";
const uint16_t PORT = 80;

WebSocketsServer webSocket = WebSocketsServer(PORT);

void webSocketEvent(uint8_t num, WStype_t type, uint8_t* payload, size_t length) {
    switch (type) {
        case WStype_DISCONNECTED:
            Serial.printf("[%u] Disconnected!\n", num);
            break;
        case WStype_CONNECTED: {
            IPAddress ip = webSocket.remoteIP(num);
            Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);
        } break;
        case WStype_TEXT: {
            Serial.printf("[%u] get Text: %s\n", num, payload);
            const char* payload_str = (const char*)payload;
            if (strcmp(payload_str, "ping") == 0) {
                // Handle ping pong.
                webSocket.sendTXT(num, "pong");
            } else {
                // Fire IR command.
                uint32_t command = (uint32_t)strtol(payload_str, NULL, 16);
                IrSender.sendNEC(0x00, command, 0);
            }
        } break;
        case WStype_BIN:
            Serial.printf("[%u] get binary length: %u\n", num, length);
            hexdump(payload, length);
            break;
        default:
            break;
    }
}

void setup() {
    Serial.begin(115200);
    pinMode(IR_SEND_PIN, OUTPUT);

    // Connect to WiFi network.
    WiFi.mode(WIFI_STA);
    WiFi.begin(SSID, PASSWORD);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.printf(".");
    }
    Serial.printf("Connected to WiFi: %s\n", SSID);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());

    // Begin mDNS.
    if (!MDNS.begin("esp8266")) {
        Serial.println("ERROR: Could not set up mDNS responder.");
        while (1);
    }
    Serial.println("mDNS responder started.");
    MDNS.addService("esp8266", "tcp", PORT);

    // Service txt is required for Bonsoir to discover the device.
    MDNS.addServiceTxt("esp8266", "tcp", "vendor", "Espressif");
    MDNS.addServiceTxt("esp8266", "tcp", "version", "1.0");
    MDNS.addServiceTxt("esp8266", "tcp", "module", "AT");

    // Establish connection with websockets.
    webSocket.begin();
    webSocket.onEvent(webSocketEvent);
    Serial.println("Websocket server started.");

    // Begin IR sending protocol.
    IrSender.begin(IR_SEND_PIN);
    disableLEDFeedback();
}

void loop() {
    MDNS.update();
    webSocket.loop();
}