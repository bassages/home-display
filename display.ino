void showWaitingForData() {
    u8g2.firstPage();
    do {
      u8g2.setFont(u8g2_font_profont12_tr);
      
      u8g2.setCursor(4, 12);
      u8g2.print("Wait for data...");
      
      u8g2.setCursor(4, 24);
      u8g2.print("ip:");

      u8g2.setCursor(4, 36);
      u8g2.print(WiFi.localIP());

      u8g2.setCursor(4, 48);
      u8g2.print("host:");

      u8g2.setCursor(4, 60);
      u8g2.print(host);
      
    } while ( u8g2.nextPage() );
}

void showConnectingToNetwork(String ssid) {
  u8g2.firstPage();
  do {
    u8g2.setFont(u8g2_font_profont12_tr);
    u8g2.setCursor(4, 12);
    u8g2.print("Connecting to:");
    u8g2.setCursor(4, 24);
    u8g2.print(ssid);
  } while ( u8g2.nextPage() );
}

void showConfigureNetwork(String ssid) {
  u8g2.firstPage();
  do {
    u8g2.setFont(u8g2_font_profont12_tr);

    u8g2.setCursor(4, 12);
    u8g2.print("1. Connect to:");

    String line = "SSID: ";
    line += accesspointSsid;
    u8g2.setCursor(4, 24);
    u8g2.print(line);

    line = "pwd:  ";
    line += accesspointPassword;
    u8g2.setCursor(4, 36);
    u8g2.print(line);

    u8g2.setCursor(4, 50);
    u8g2.print("2. Browse to:");

    line = "http://";
    line += host;
    u8g2.setCursor(4, 62);
    u8g2.print(line);

  } while ( u8g2.nextPage() );
}

void showPageTemperature() {
    u8g2.firstPage();
    do {
      u8g2.setFont(u8g2_font_profont15_tr);

      u8g2.setCursor(30, 14);
      u8g2.print("Temperatuur");

      u8g2.setFont(u8g2_font_crox5hb_tr);
      u8g2.setCursor(30, 43);
      u8g2.print(temperature, 1);
      
      u8g2.setFont(u8g2_font_profont12_tr);
      u8g2.setCursor(82, 31);
      u8g2.print("o");

      u8g2.setFont(u8g2_font_crox5hb_tr);
      u8g2.setCursor(86, 43);
      u8g2.print("C");

      u8g2.setFont(u8g2_font_unifont_t_symbols);
      u8g2.setCursor(50, 66);
      u8g2.print("○ ●"); //◻ ◼ ?
      
    } while ( u8g2.nextPage() );
}

void showPageHumidity() {
  u8g2.firstPage();
  do {
    u8g2.setFont(u8g2_font_profont15_tr);
    u8g2.setCursor(10, 14);
    u8g2.print("Luchtvochtigheid");

    u8g2.setFont(u8g2_font_crox5hb_tr);
    u8g2.setCursor(30, 43);
    u8g2.print(humidity, 1);
    
    u8g2.setFont(u8g2_font_crox5hb_tr);
    u8g2.setCursor(80, 44);
    u8g2.print("%");

    u8g2.setFont(u8g2_font_unifont_t_symbols);
    u8g2.setCursor(50, 66);
    u8g2.print("● ○");

  } while ( u8g2.nextPage() );
}

