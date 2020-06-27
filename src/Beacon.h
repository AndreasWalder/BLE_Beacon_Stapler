BLEBeacon setBeacon(BLEBeacon Beacon) {
  ReverseString(BEACON_UUID);
  Beacon.setManufacturerId(ManufacturerId); 
  Beacon.setProximityUUID(BLEUUID(BEACON_UUID));
  Beacon.setMajor(Major);
  Beacon.setMinor(Minor);
  Beacon.setSignalPower(SignalPower);
  
  BLEAdvertisementData oAdvertisementData = BLEAdvertisementData();
  BLEAdvertisementData oScanResponseData = BLEAdvertisementData();
  
  oAdvertisementData.setFlags(0x06); // BR_EDR_NOT_SUPPORTED 0x04
  
  std::string strServiceData = "";
  strServiceData += (char)26;     // Len
  strServiceData += (char)0xFF;   // Type
  strServiceData += Beacon.getData(); 
  oAdvertisementData.addData(strServiceData);

  /*
   uint16_t seq = (uint16_t)(5);
  uint16_t temp = (uint16_t)(10);
  uint16_t humid = (uint16_t)(20);
  uint16_t pressx = (uint16_t)(30);
  
   std::string strServiceData = "";
   strServiceData += (char) 0x0a ;    // Länge
   strServiceData += (char) 0xff ;    // AD Typ 0xFF: Herstellerspezifische Daten
   strServiceData += (char) 0xff ;    // Testherstellungs- ID Low Byte
   strServiceData += (char) 0xff ;    // Teste Herstellungs-ID High-Byte
   strServiceData += (char) seq;      // Sequenznummer
   strServiceData +=  (char)(temp & 0xff);          // Unteres Temperaturbyte
   strServiceData += (char)((temp >> 8) & 0xff);   // Hohes Temperaturbyte
   strServiceData += (char)(humid & 0xff);         // Niedrigeres Feuchtigkeitsbyte
   strServiceData += (char)((humid >> 8) & 0xff);  // Hohes Byte Feuchtigkeit
   strServiceData += (char)(pressx & 0xff);        // Unteres Byte des atmosphärischen Drucks
   strServiceData += (char)((pressx >> 8) & 0xff); // High Byte Atmosphärendruck
   strServiceData += oBeacon.getData(); 
   oAdvertisementData.addData(strServiceData);
   */
  
  pAdvertising->setAdvertisementData(oAdvertisementData);
  pAdvertising->setScanResponseData(oScanResponseData);
  return Beacon;
}