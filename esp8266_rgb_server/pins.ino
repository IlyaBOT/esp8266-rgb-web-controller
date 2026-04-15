#ifndef ESP8266_RGB_SERVER_PINS_INO
#define ESP8266_RGB_SERVER_PINS_INO

// ESP8266 pin notes:
// - GPIO6..GPIO11 are wired to SPI flash and must not be used.
// - GPIO0, GPIO2 and GPIO15 are boot strapping pins, so avoid loading strips on them.
// Safe outputs for this project: GPIO4, GPIO12, GPIO13, GPIO14.
const uint8_t redPin = 12;    // GPIO12 / D6
const uint8_t greenPin = 13;  // GPIO13 / D7
const uint8_t bluePin = 14;   // GPIO14 / D5
const uint8_t addressPin = 4; // GPIO4 / D2

#endif
