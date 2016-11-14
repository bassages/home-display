/**
 * @brief Read WiFi connection information from file system.
 * @param ssid String pointer for storing SSID.
 * @param pass String pointer for storing PSK.
 * @return True or False.
 * 
 * The config file have to containt the WiFi SSID in the first line
 * and the WiFi PSK in the second line.
 * Line seperator can be \r\n (CR LF) \r or \n.
 */
bool loadConfig(String *ssid, String *pass) {
  // open file for reading.
  File configFile = SPIFFS.open(configFileFullPath, "r");
  if (!configFile) {
    Serial.print("Failed to open ");
    Serial.println(configFileFullPath);
    return false;
  }

  // Read content from config file.
  String content = configFile.readString();
  configFile.close();
  
  content.trim();

  // Check if ther is a second line available.
  int8_t pos = content.indexOf("\r\n");
  uint8_t le = 2;
  // check for linux and mac line ending.
  if (pos == -1) {
    le = 1;
    pos = content.indexOf("\n");
    if (pos == -1) {
      pos = content.indexOf("\r");
    }
  }

  // If there is no second line: Some information is missing.
  if (pos == -1) {
    Serial.print("Invalid content in file ");
    Serial.println(configFileFullPath);
    Serial.println(content);
    return false;
  }

  // Store SSID and PASSWORD into string vars.
  *ssid = content.substring(0, pos);
  *pass = content.substring(pos + le);

  ssid->trim();
  pass->trim();

  return true;
}

/**
 * @brief Save WiFi SSID and PSK to configuration file.
 * @param ssid SSID as string pointer.
 * @param pass PSK as string pointer,
 * @return True or False.
 */
bool saveConfig(String ssid, String password) {
  // Open config file for writing.
  File configFile = SPIFFS.open(configFileFullPath, "w");
  if (!configFile) {
    Serial.print("Failed to open ");
    Serial.print(configFileFullPath);
    Serial.println(" for writing");
    return false;
  }
  // Save SSID and password
  configFile.println(ssid);
  configFile.println(password);
  configFile.close();

  return true;
}
