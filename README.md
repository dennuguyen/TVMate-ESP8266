# TV Mate ESP8266 Pairing

## ESP8266 Circuit

## ESP8266 Flash Instructions

1. Pull GPIO0 pin to GND before boot.
1. Use USB-to-UART. Show circuit.
1. Use Arduino IDE to flash.
1. Remove GPIO0 pin to GND before boot for normal operating mode.

## ESP8266 Relevant AT Commands

1. Ensure ESP8266 is flashed with AT commands.
1. Set client mode: `AT+CWMODE=1`
1. Join wifi network: `AT+CWJAP="ssid","password"`
1. Enable mDNS: `AT+MDNS=1,"esp8266","esp8266",80`
1. ESP8266 is now discoverable on the network.

## Dependencies

- ESP8266

## Setup Instructions

1. 