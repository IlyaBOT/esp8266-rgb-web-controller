#include "libs.ino"
#include "pins.ino"
#include "variables.ino"

#ifndef DEBUG_MODE
#define DEBUG_MODE 1
#endif

#if DEBUG_MODE
#define DEBUG_BEGIN(baud) Serial.begin(baud)
#define DEBUG_PRINT(value) Serial.print(value)
#define DEBUG_PRINTLN(value) Serial.println(value)
#define DEBUG_PRINTLN_EMPTY() Serial.println()
#define DEBUG_WRITE(value) Serial.write(value)
#else
#define DEBUG_BEGIN(baud) do {} while (0)
#define DEBUG_PRINT(value) do {} while (0)
#define DEBUG_PRINTLN(value) do {} while (0)
#define DEBUG_PRINTLN_EMPTY() do {} while (0)
#define DEBUG_WRITE(value) do {} while (0)
#endif

#define COLOR_ORDER RGB

void applyRequestedColor();
void colorToRGB();
void countHUE();
long HSBtoRGB(float hueValue, float satValue, float brightnessValue);
void parseColorRequest();
void showWifiConnectingEffect();
void setArgbColor(uint8_t redValue, uint8_t greenValue, uint8_t blueValue);
void setRgbColor(uint8_t redValue, uint8_t greenValue, uint8_t blueValue);

void setup() {
  DEBUG_BEGIN(115200);
  header.reserve(512);

  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  pinMode(addressPin, OUTPUT);

  analogWriteRange(pwmRange);
  analogWriteFreq(pwmFrequency);
  setRgbColor(0, 0, 0);

  FastLED.addLeds<WS2812B, addressPin, COLOR_ORDER>(leds, numLeds);
  FastLED.clear(true);

  WiFi.mode(WIFI_STA);
  DEBUG_PRINT("Connecting to ");
  DEBUG_PRINTLN(ssid);
  WiFi.begin(ssid, password);

  unsigned long lastWifiStatusPrint = 0;
  while (WiFi.status() != WL_CONNECTED) {
    const unsigned long now = millis();
    showWifiConnectingEffect();
    if (now - lastWifiStatusPrint >= wifiConnectLogIntervalMs) {
      lastWifiStatusPrint = now;
      DEBUG_PRINT(".");
    }
    delay(wifiConnectAnimationStepMs);
  }

  setArgbColor(0, 0, 0);
  setRgbColor(0, 0, 0);

  DEBUG_PRINTLN_EMPTY();
  DEBUG_PRINTLN("WiFi connected.");
  DEBUG_PRINTLN("IP address: ");
  DEBUG_PRINTLN(WiFi.localIP());
  server.begin();
}

void loop() {
  countHUE();

  const int currentMode = modeString.toInt();
  if (currentMode == 2) {
    setArgbColor(red, green, blue);
  } else if (currentMode == 3) {
    setRgbColor(red, green, blue);
  } else if (currentMode == 4) {
    setArgbColor(red, green, blue);
    setRgbColor(red, green, blue);
  }

  WiFiClient client = server.available();
  if (!client) {
    return;
  }

  currentTime = millis();
  previousTime = currentTime;
  DEBUG_PRINTLN("New Client.");
  String currentLine = "";

  while (client.connected() && currentTime - previousTime <= timeoutTime) {
    currentTime = millis();
    if (!client.available()) {
      delay(1);
      continue;
    }

    char c = client.read();
    DEBUG_WRITE(c);
    header += c;

    if (c == '\n') {
      if (currentLine.length() == 0) {
        client.println("HTTP/1.1 200 OK");
        client.println("Content-type:text/html");
        client.println("Connection: close");
        client.println();
        client.print(FPSTR(HTML_PAGE));
        client.println();

        if (header.indexOf("GET /?r") >= 0) {
          parseColorRequest();
          if (modeString.toInt() == 1) {
            applyRequestedColor();
          }
        }
        break;
      }
      currentLine = "";
    } else if (c != '\r') {
      currentLine += c;
    }
  }

  header = "";
  client.stop();
  DEBUG_PRINTLN("Client disconnected.");
  DEBUG_PRINTLN_EMPTY();
}

void parseColorRequest() {
  pos1 = header.indexOf('r');
  pos2 = header.indexOf('g');
  pos3 = header.indexOf('b');
  pos4 = header.indexOf('m');
  pos5 = header.indexOf('&');

  if (pos1 < 0 || pos2 < 0 || pos3 < 0 || pos4 < 0 || pos5 < 0) {
    return;
  }

  redString = header.substring(pos1 + 1, pos2);
  greenString = header.substring(pos2 + 1, pos3);
  blueString = header.substring(pos3 + 1, pos4);
  modeString = header.substring(pos4 + 1, pos5);

  DEBUG_PRINTLN(redString.toInt());
  DEBUG_PRINTLN(greenString.toInt());
  DEBUG_PRINTLN(blueString.toInt());
  DEBUG_PRINTLN(modeString.toInt());
}

void applyRequestedColor() {
  setRgbColor(redString.toInt(), greenString.toInt(), blueString.toInt());
  setArgbColor(redString.toInt(), greenString.toInt(), blueString.toInt());
}

void countHUE() {
  if (millis() - lastHueUpdate < hueStepIntervalMs) {
    return;
  }

  lastHueUpdate = millis();
  const float colorNumber = counterHue % 256;
  hue = (colorNumber / 255.0f) * 360.0f;
  colorToRGB();
  counterHue++;
}

void colorToRGB() {
  const long rgb = HSBtoRGB(hue, saturation, brightness);
  red = (rgb >> 16) & 255;
  green = (rgb >> 8) & 255;
  blue = rgb & 255;
}

void setArgbColor(uint8_t redValue, uint8_t greenValue, uint8_t blueValue) {
  for (uint16_t i = 0; i < numLeds; i++) {
    leds[i] = CRGB(redValue, greenValue, blueValue);
  }
  FastLED.show();
}

void showWifiConnectingEffect() {
  const uint8_t phase = static_cast<uint8_t>((millis() % wifiBreathPeriodMs) * 256UL / wifiBreathPeriodMs);
  const uint8_t breathBlueValue = sin8(static_cast<uint8_t>(phase + 192));
  const uint8_t rgbBreathBlueValue = static_cast<uint8_t>((static_cast<uint16_t>(breathBlueValue) * wifiRgbBreathMaxBlue + 127) / 255);

  setArgbColor(0, 0, breathBlueValue);
  setRgbColor(0, 0, rgbBreathBlueValue);
}

void setRgbColor(uint8_t redValue, uint8_t greenValue, uint8_t blueValue) {
  analogWrite(redPin, redValue);
  analogWrite(greenPin, greenValue);
  analogWrite(bluePin, map(blueValue, 0, 255, 0, blueCalibrationLimit));
}

long HSBtoRGB(float hueValue, float satValue, float brightnessValue) {
  float redValue = 0.0f;
  float greenValue = 0.0f;
  float blueValue = 0.0f;

  if (satValue == 0.0f) {
    redValue = brightnessValue;
    greenValue = brightnessValue;
    blueValue = brightnessValue;
  } else {
    if (hueValue >= 360.0f) {
      hueValue = 0.0f;
    }

    const int slice = hueValue / 60.0f;
    const float hueFrac = (hueValue / 60.0f) - slice;
    const float aa = brightnessValue * (1.0f - satValue);
    const float bb = brightnessValue * (1.0f - satValue * hueFrac);
    const float cc = brightnessValue * (1.0f - satValue * (1.0f - hueFrac));

    switch (slice) {
      case 0:
        redValue = brightnessValue;
        greenValue = cc;
        blueValue = aa;
        break;
      case 1:
        redValue = bb;
        greenValue = brightnessValue;
        blueValue = aa;
        break;
      case 2:
        redValue = aa;
        greenValue = brightnessValue;
        blueValue = cc;
        break;
      case 3:
        redValue = aa;
        greenValue = bb;
        blueValue = brightnessValue;
        break;
      case 4:
        redValue = cc;
        greenValue = aa;
        blueValue = brightnessValue;
        break;
      case 5:
        redValue = brightnessValue;
        greenValue = aa;
        blueValue = bb;
        break;
      default:
        redValue = 0.0f;
        greenValue = 0.0f;
        blueValue = 0.0f;
        break;
    }
  }

  const long ired = redValue * 255.0f;
  const long igreen = greenValue * 255.0f;
  const long iblue = blueValue * 255.0f;
  return long((ired << 16) | (igreen << 8) | iblue);
}
