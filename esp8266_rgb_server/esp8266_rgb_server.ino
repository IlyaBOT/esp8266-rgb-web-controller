#include <FastLED.h>
#include <ESP8266WiFi.h>

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

const char* ssid = "MyCoolWiFiNetwork";
const char* password = "youshallnotpass";

WiFiServer server(80);

static const char HTML_PAGE[] PROGMEM = R"rawliteral(<!doctype html><html><head><meta name="viewport" content="width=device-width,initial-scale=1"><meta charset="utf-8"><link rel="icon" href="https://i.imgur.com/vT4FIcd.png"><link rel="preconnect" href="https://fonts.googleapis.com"><link rel="preconnect" href="https://fonts.gstatic.com" crossorigin><link href="https://fonts.googleapis.com/css2?family=Noto+Sans:ital,wght@0,100..900;1,100..900&display=swap" rel="stylesheet"><link rel="stylesheet" href="https://stackpath.bootstrapcdn.com/bootstrap/4.3.1/css/bootstrap.min.css"><link rel="stylesheet" href="https://cdn.jsdelivr.net/gh/mdbassit/Coloris@latest/dist/coloris.min.css"><script src="https://cdn.jsdelivr.net/gh/mdbassit/Coloris@latest/dist/coloris.min.js"></script></head><body><style>body,h1,p{font-family:"Noto Sans",sans-serif}body{background:#666}h1{font-size:2.5rem}.panel{width:40rem;height:30rem;max-height:50rem;background:#2f2f2f;border-radius:16px;margin:3rem auto 0;padding:8px}.title{margin:2% auto 0;padding-bottom:4px;text-align:center;background-image:repeating-linear-gradient(90deg,red,magenta,#6e00ff,blue,aqua,lime,yellow,orange,red,magenta,#6e00ff,blue,aqua,lime,yellow,orange,red,magenta);background-size:1200% 1200%;-webkit-background-clip:text;-webkit-text-fill-color:transparent;animation:rainbow 60s ease infinite}.btn{display:block;max-width:16rem;margin:16px auto 0;color:#fff;background:#6e00ff;border-color:#6e00ff}.label{display:block;color:#fff;font-size:1.5rem}.label-color{float:left;padding-right:4px}.modes{text-align:center;padding-bottom:4px}.mode-label{display:block;color:#fff;font-size:2rem;font-weight:700}.btn-mode{display:inline-block;width:16rem}.btn-rainbow{width:fit-content;border:0;font-weight:700;color:#fff;text-shadow:-1px -1px 0 #000,1px -1px 0 #000,-1px 1px 0 #000,1px 1px 0 #000;background-image:repeating-linear-gradient(90deg,red,magenta,#6e00ff,blue,aqua,lime,yellow,orange,red,magenta,#6e00ff,blue,aqua,lime,yellow,orange,red,magenta);background-size:1200% 1200%;animation:rainbow 60s ease infinite}.divider{margin:0 0 8px;border:0;border-top:1px solid #666}.footer{margin:1rem auto 0;text-align:center;font-size:16px;color:#fff;text-shadow:-4px -4px 8px #000,4px -4px 8px #000,-4px 4px 8px #000,4px 4px 8px #000}.clr-field{float:right}@media(max-width:768px){.panel{width:30rem}h1{font-size:1.78rem}.title{font-size:20px}.btn{font-size:20px}.label{font-size:1.75rem;float:left;padding-right:4px}.colorPicker{max-width:3.5rem;margin-top:12px}.btn-rainbow{font-size:1rem}}@media(max-width:600px){.panel{width:25rem}h1{font-size:1.689rem;font-weight:700}.title{font-size:18px}.btn{width:16rem;font-size:16px;font-weight:700}.label{font-size:1.35rem;float:left;padding-right:1px}.mode-label{font-size:1.75rem}.colorPicker{max-width:3.5rem;margin-top:6px}.btn-mode{margin-top:0}.btn-rainbow{display:block;margin-top:12px;font-size:.99rem}}@media(max-width:480px){.panel{width:18rem}h1{font-size:1.189rem;font-weight:700}.title{font-size:16px}.btn{max-width:10rem;font-size:16px;font-weight:700}.label{font-size:1.15rem;float:left;padding-right:1px}.mode-label{font-size:1.25rem}.colorPicker,.clr-field{max-width:5.92rem}.btn-rainbow{display:block;margin-top:12px;font-size:.75rem}}@media(max-height:960px){.panel{margin-top:2rem}}@media(max-height:512px){.panel{margin-top:1rem}}@keyframes rainbow{0%{background-position:0 50%}50%{background-position:100% 25%}100%{background-position:0 50%}}</style><div class="panel"><div class="container"><div class="title"><h1>ESP8266 RGB Strip Controller</h1></div><p class="label label-color">Цвет RGB ленты:</p><input type="text" class="colorPicker" data-coloris><script>Coloris({format:"hex",theme:"polaroid",themeMode:"dark",alpha:false})</script><input class="btn btn-primary btn-lg change_color" style="width:16rem;margin-top:3.25rem" type="button" value="Сменить цвет" id="change_color"><br></div><hr class="divider"><div class="modes"><p class="mode-label">Режимы/Эффекты</p><a class="btn btn-primary btn-lg btn-mode" href="?r0g0b0m1&">Цвет</a><br><a class="btn btn-primary btn-lg btn-mode btn-rainbow" href="?r0g0b0m3&">Радуга на RGB</a><a class="btn btn-primary btn-lg btn-mode btn-rainbow" href="?r0g0b0m2&">Радуга на ARGB</a><a class="btn btn-primary btn-lg btn-mode btn-rainbow" href="?r0g0b0m4&">Радуга везде</a></div></div><script>let red=0,green=0,blue=0;const toRgb=v=>{const m=/^#?([a-f\d]{2})([a-f\d]{2})([a-f\d]{2})$/i.exec(v);return m?{r:parseInt(m[1],16),g:parseInt(m[2],16),b:parseInt(m[3],16)}:null};document.addEventListener("coloris:pick",e=>{const rgb=toRgb(e.detail.color);if(rgb){red=rgb.r;green=rgb.g;blue=rgb.b}});document.getElementById("change_color").addEventListener("click",()=>{window.location=`?r${red}g${green}b${blue}m1&`})</script><footer><div class="footer"><p>2024-2026 © IB WorkShop. Все права защищены.</p></div></footer></body></html>)rawliteral";

String header;
String redString = "0";
String greenString = "0";
String blueString = "0";
String modeString = "0";

int pos1 = 0;
int pos2 = 0;
int pos3 = 0;
int pos4 = 0;
int pos5 = 0;

// ESP8266 pin notes:
// - GPIO6..GPIO11 are wired to SPI flash and must not be used.
// - GPIO0, GPIO2 and GPIO15 are boot strapping pins, so avoid loading strips on them.
// Safe outputs for this project: GPIO4, GPIO12, GPIO13, GPIO14.
const uint8_t redPin = 12;    // GPIO12 / D6
const uint8_t greenPin = 13;  // GPIO13 / D7
const uint8_t bluePin = 14;   // GPIO14 / D5
const uint8_t addressPin = 4; // GPIO4 / D2
const uint16_t numLeds = 10;
#define COLOR_ORDER RGB

const uint16_t pwmRange = 255;
const uint16_t pwmFrequency = 1000;
const uint8_t blueCalibrationLimit = 128;
const uint8_t wifiRgbBreathMaxBlue = 63;
const unsigned long wifiBreathPeriodMs = 4000;
const unsigned long wifiConnectAnimationStepMs = 20;
const unsigned long wifiConnectLogIntervalMs = 500;

uint16_t counterHue = 0;

float saturation = 1.0f;
float brightness = 1.0f;
float hue = 0.0f;

uint8_t red = 255;
uint8_t green = 0;
uint8_t blue = 102;

CRGB leds[numLeds];

unsigned long currentTime = 0;
unsigned long previousTime = 0;
unsigned long lastHueUpdate = 0;
const long timeoutTime = 2000;
const unsigned long hueStepIntervalMs = 60;

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
