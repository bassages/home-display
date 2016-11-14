#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <U8g2lib.h>
#include <ArduinoJson.h>
#include <FS.h>

// OLED pages
#define HUMIDITY 1
#define TEMPERATURE 2

// 160MHz == 1sec
#define ONE_SECOND 160000000L

// SLC & SDA are pin GPIO4 and GPIO5 on the dev board.
U8G2_SSD1306_128X64_NONAME_1_SW_I2C u8g2(U8G2_R0, /* clock=*/ SCL, /* data=*/ SDA, /* reset=*/ U8X8_PIN_NONE);   // All Boards without Reset of the Display

// Config file
const char* configFileFullPath = "/home-display.conf";

// Accesspoint settings
const byte DNS_PORT = 53;
IPAddress accesspointIp(192, 168, 1, 1);
DNSServer dnsServer;
const char* accesspointSsid = "home-display";
// When the password is too short (less than 8 characters) the WiFi.softAP(ssid, password) function desn't work. There is no warning during compilation.
const char* accesspointPassword = "12345678";

// Webserver settings
const char* host = "home-display";

// Create an instance of the webserver, specify the port to listen on as an argument
ESP8266WebServer webserver(80);

// Latest values
double temperature = 0.0;
double humidity = 0.0;

int pageToShow = TEMPERATURE;

void setup() {
  Serial.begin(115200);

  setupFileSystem();
  // SPIFFS.remove(configFileFullPath); // Remove config file to test the setup flow...
  setupOled();
  setupNetwork();
}

void loop() {
  dnsServer.processNextRequest();
  webserver.handleClient();
}

void setupPageController() {
  noInterrupts();
  timer0_isr_init();
  timer0_attachInterrupt(togglePages);
  timer0_write(ESP.getCycleCount() + ONE_SECOND);
  interrupts();  
}

void setupOled() {
  u8g2.begin();
  u8g2.enableUTF8Print();
}

void setupFileSystem() {
  if (!SPIFFS.begin()) {
    Serial.println("Failed to mount file system");
    return;
  }
}

void setupNetwork() {
  String ssid = "";
  String password = "";

  // Load WiFi connection configuration
  if (!loadConfig(&ssid, &password)) {
    Serial.println("No WiFi configuration available.");
  } else {
    connectToWifi(ssid, password);
  }
  
  if (WiFi.status() == WL_CONNECTED) {
    Serial.print("Sucessfully connected to ");
    Serial.println(ssid);
    
    startWebserver();
    setupPageController();
    
  } else {
    
    Serial.println("Failed to connect to WiFi. Go into AccessPoint mode.");
    
    setupAccessPoint();
    startCaptivePortalWebserver();
    showConfigureNetwork(accesspointSsid);
  }

  Serial.print("local-ip: ");
  Serial.println(WiFi.localIP());
  
  Serial.print("accesspoint-ip: ");
  Serial.println(WiFi.softAPIP());

  Serial.print("host: ");
  Serial.println(host);
}

void setupAccessPoint() {
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(accesspointIp, accesspointIp, IPAddress(255, 255, 255, 0));
  WiFi.softAP(accesspointSsid, accesspointPassword);
  
  // if DNSServer is started with "*" for domain name, it will reply with provided IP to all DNS request
  dnsServer.start(DNS_PORT, "*", accesspointIp);
}

void connectToWifi(String ssid, String password) {
  showConnectingToNetwork(ssid);

  WiFi.hostname(host);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid.c_str(), password.c_str());

  // Give 10 seconds to connect
  unsigned long startTime = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - startTime < 10000) {
    Serial.write('.');
    //Serial.print(WiFi.status());
    delay(500);
  }
}

void startWebserver() {
  webserver.on("/", handleRoot);
  webserver.on("/klimaat", handleKlimaatUpdate);
  webserver.on("/configuration", handleConfiguration);
  webserver.onNotFound(handleNotFound);
  
  webserver.begin();
  Serial.println("Webserver started.");
}

void startCaptivePortalWebserver() {
  webserver.onNotFound(handleConfiguration);
  webserver.begin();
  Serial.println("CaptivePortalWebserver started.");
}

boolean dataAvailable() {
  return !temperature == 0.0 && !humidity == 0.0;
}

void togglePages(void) {
  if (dataAvailable()) {
    if (pageToShow == TEMPERATURE) {
      showPageTemperature();
      pageToShow = HUMIDITY;
    } else if (pageToShow == HUMIDITY) {
      showPageHumidity();
      pageToShow = TEMPERATURE;
    }
    // Change page every few seconds
    timer0_write(ESP.getCycleCount() + (ONE_SECOND * 3));
  } else {
    showWaitingForData();
    timer0_write(ESP.getCycleCount() + (ONE_SECOND / 2));
  }
}

