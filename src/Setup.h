void IncludeSetup() {
  Serial.begin(115200);

  pinMode(PushButtonPin, PULLUP);
  pinMode(extAlarmPin, PULLUP);

  // ----------------------OLET----------------------------------------------
   Heltec.begin(true /*DisplayEnable Enable*/, false /*LoRa Disable*/, true /*Serial Enable*/);
   pinMode(25, OUTPUT); //OLET Beleuchtung
   digitalWrite(25,HIGH); //OLET Beleuchtung an
   Heltec.display->clear(); //OLET löschen
   //Heltec.display->resetOrientation();
   Heltec.display->setFont(ArialMT_Plain_10);         //Schriftgröße 10
   Heltec.display->setTextAlignment(TEXT_ALIGN_LEFT); //Schriftausrichtung
   Heltec.display->drawXbm(10, 8, Liebherr_Logo_width, Liebherr_Logo_height, Liebherr_Logo_bits);
   Heltec.display->drawString(20, 30, "QM-RS485-Check");
   Heltec.display->drawString(40, 44, "(R) 2020");
   Heltec.display->display(); //OLET anzeigen
   delay(400);
   Heltec.display->clear(); //OLET löschen
   Heltec.display->display(); //OLET anzeigen

  Heltec.begin(true /*DisplayEnable Enable*/, false /*LoRa Disable*/, true /*Serial Enable*/);
  
  Heltec.display->setFont(ArialMT_Plain_10);
  Heltec.display->clear();

  InitBattery();

  runner.init();
  Serial.println("Initialized scheduler");

  runner.addTask(t1);
  runner.addTask(t2);
  runner.addTask(t3);

  t1.enable();
  Serial.println("Enabled task1");
  t2.enable();
  Serial.println("Enabled task2");
  t3.enable();
  Serial.println("Enabled task3");
}

void InitBattery()
{
    #if (defined(HELTEC_V2_1))
  adc_chars = (esp_adc_cal_characteristics_t *)calloc(1, sizeof(esp_adc_cal_characteristics_t));
  esp_adc_cal_value_t val_type = esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_6, ADC_WIDTH_BIT_12, DEFAULT_VREF, adc_chars);
  adc1_config_width(ADC_WIDTH_BIT_12);
  adc1_config_channel_atten(ADC1_CHANNEL_1, ADC_ATTEN_DB_6);
#else
  // Use this for older V2.0 with VBatt reading wired to GPIO13
  adc_chars = (esp_adc_cal_characteristics_t *)calloc(1, sizeof(esp_adc_cal_characteristics_t));
  esp_adc_cal_value_t val_type = esp_adc_cal_characterize(ADC_UNIT_2, ADC_ATTEN_DB_6, ADC_WIDTH_BIT_12, DEFAULT_VREF, adc_chars);
  adc2_config_channel_atten(ADC2_CHANNEL_4, ADC_ATTEN_DB_6);
#endif

#if defined(__DEBUG) && __DEBUG > 0
  Serial.printf("ADC Calibration: ");
  if (val_type == ESP_ADC_CAL_VAL_EFUSE_VREF)
  {
    Serial.printf("eFuse Vref\n");
  }
  else if (val_type == ESP_ADC_CAL_VAL_EFUSE_TP)
  {
    Serial.printf("Two Point\n");
  }
  else
  {
    Serial.printf("Default[%dmV]\n", DEFAULT_VREF);
  }
#else
  if (val_type)
    ; // Suppress warning
#endif

#if defined(__DEBUG) && __DEBUG >= 1
  Serial.printf("ADC Calibration: ");
  if (val_type == ESP_ADC_CAL_VAL_EFUSE_VREF)
  {
    Serial.printf("eFuse Vref\n");
  }
  else if (val_type == ESP_ADC_CAL_VAL_EFUSE_TP)
  {
    Serial.printf("Two Point\n");
  }
  else
  {
    Serial.printf("Default[%dmV]\n", DEFAULT_VREF);
  }
#else
  if (val_type)
    ;                                  // Suppress warning
#endif

  // Prime the Sample register
  for (uint8_t i = 0; i < VBATT_SMOOTH; i++)
  {
    Sample();
  }

  pinMode(VBATT_GPIO, OUTPUT);
  digitalWrite(VBATT_GPIO, LOW); // ESP32 Lora v2.1 reads on GPIO37 when GPIO21 is low
  delay(ADC_READ_STABILIZE);     // let GPIO stabilize
}