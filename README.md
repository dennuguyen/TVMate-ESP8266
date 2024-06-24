# TV Mate ESP8266 Pairing

## ESP8266 Setup Instructions

1. Show circuit.
    - Pull GPIO0 to GND only when flashing.
1. Set client mode: `AT+CWMODE=1`
1. Join wifi network: `AT+CWJAP="ssid","password"`
1. Enable mDNS: `AT+MDNS=1,"esp8266","esp8266",80`
1. ESP8266 is now discoverable on the network.