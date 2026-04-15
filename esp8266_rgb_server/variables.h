#pragma once

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

constexpr uint16_t numLeds = 10;
constexpr uint16_t pwmRange = 255;
constexpr uint16_t pwmFrequency = 1000;
constexpr uint8_t blueCalibrationLimit = 128;
constexpr uint8_t wifiRgbBreathMaxBlue = 63;
constexpr unsigned long wifiBreathPeriodMs = 4000;
constexpr unsigned long wifiConnectAnimationStepMs = 20;
constexpr unsigned long wifiConnectLogIntervalMs = 500;

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
constexpr long timeoutTime = 2000;
constexpr unsigned long hueStepIntervalMs = 60;
