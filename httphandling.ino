void handleRoot() {
  webserver.sendHeader("Location", String("/configuration"), true);
  webserver.send(302, "text/plain", "");
}

void handleKlimaatUpdate() {
  DynamicJsonBuffer jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(webserver.arg(0));

  // Test if parsing succeeds.
  if (!root.success()) {
    Serial.println("parseObject() failed");
    webserver.send(500, "text/plain", "Failed to process request");
    return;
  }

  temperature = root["temperatuur"];
  humidity = root["luchtvochtigheid"];

  Serial.print("Recieved klimaat: ");
  Serial.println(webserver.arg(0));
  
  webserver.send(200, "text/plain", "Thanks for the klimaat");
}

void handleConfiguration() {
  if (webserver.method() == HTTP_GET) {
     handleConfigurationGet();
  } else if (webserver.method() == HTTP_POST) {
    handleConfigurationPost();
  }
}

void handleConfigurationPost() {
  String ssidParam = webserver.arg("ssid");
  ssidParam.trim();
  
  String passwordParam = webserver.arg("password");
  passwordParam.trim();
  
  Serial.print("Updated SSID: ");
  Serial.println(ssidParam);

  Serial.print("Updated password: ");
  Serial.println(passwordParam);

  if (ssidParam.length() == 0) {
    webserver.send(400, "text/html", "Failed to save configuration: ssid is mandatory");
  } else if (passwordParam.length() == 0) {
    webserver.send(400, "text/html", "Failed to save configuration: password is mandatory");
  } else {
    bool sucessfullySaved = saveConfig(ssidParam, passwordParam);
    if (sucessfullySaved) {
      webserver.send(200, "text/html", "<!DOCTYPE html>"
                                     "<html>"
                                     "<head>"
                                     "<title>Home Display - Configuration</title>"
                                     + getStyleTag() +
                                     "</head>"
                                     "<body>"
                                     "<h2>Configuration saved succesfully</h2>"
                                     "<p>home-display will restart now to apply the new settings...</p>"
                                     "</body></html>");
      ESP.restart();
    } else {
      webserver.send(500, "text/html", "Failed to save configuration");
    }
  }  
}

void handleConfigurationGet() {
  webserver.send(200, "text/html", "<!DOCTYPE html>"
                                 "<html>"
                                 "<head>"
                                 "<title>Home Display - Configuration</title>"
                                 + getStyleTag() +
                                 "</head>"
                                 "<body>"
                                 "<h2>WiFi configuration</h2>"
                                 "<p>home-display needs a WiFi network connection, <br/>please provide the SSID and password of the network in the form below.</p>"
                                 "<form action=\"configuration\" method=\"post\">"
                                 "<table>"
                                 "<tr>"
                                 "<td>SSID * </td><td><input type=\"text\" name=\"ssid\" size=\"30\" maxlength=\"50\" required></td>"
                                 "</tr>"
                                 "<tr>"
                                 "<td>Password * </td><td><input type=\"password\" name=\"password\" size=\"30\" maxlength=\"50\" required></td>"
                                 "</tr>"
                                 "<tr>"
                                 "<td colspan=\"2\" align=\"right\"><input type=\"submit\" value=\"Save and (re)connect\"></td>"
                                 "</tr>"
                                 "</table>"
                                 "</form>"
                                 "</body></html>");
}

void handleNotFound(){
  String message = "Not Found\n\n";
  message += "URI: ";
  message += webserver.uri();
  message += "\nMethod: ";
  message += (webserver.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += webserver.args();
  message += "\n";
  for (uint8_t i=0; i<webserver.args(); i++){
    message += " " + webserver.argName(i) + ": " + webserver.arg(i) + "\n";
  }
  webserver.send(404, "text/plain", message);
}

String getStyleTag() {
  return String("<style>body {font-family: \"Helvetica Neue\",Helvetica,Arial,sans-serif; font-size: 14px; line-height: 1.5;}</style>");
}
