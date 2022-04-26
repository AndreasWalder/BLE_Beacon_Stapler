// Poll the proper ADC for VBatt on Heltec Lora 32 with GPIO21 toggled
uint16_t ReadVBatt()
{
  uint16_t reading = 666;

  digitalWrite(VBATT_GPIO, LOW); // ESP32 Lora v2.1 reads on GPIO37 when GPIO21 is low
  delay(ADC_READ_STABILIZE);     // let GPIO stabilize
#if (defined(HELTEC_V2_1))
  pinMode(ADC1_CHANNEL_1, OPEN_DRAIN); // ADC GPIO37
  reading = adc1_get_raw(ADC1_CHANNEL_1);
  pinMode(ADC1_CHANNEL_1, INPUT); // Disconnect ADC before GPIO goes back high so we protect ADC from direct connect to VBATT (i.e. no divider)
#else
  pinMode(ADC2_CHANNEL_4, OPEN_DRAIN); // ADC GPIO13
  adc2_get_raw(ADC2_CHANNEL_4, ADC_WIDTH_BIT_12, &reading);
  pinMode(ADC2_CHANNEL_4, INPUT); // Disconnect ADC before GPIO goes back high so we protect ADC from direct connect to VBATT (i.e. no divider
#endif

  uint16_t voltage = esp_adc_cal_raw_to_voltage(reading, adc_chars);
  voltage *= VOLTAGE_DIVIDER;

  return voltage;
}

//  Use a buffer to average/sample ADC
uint16_t Sample()
{
  static uint8_t i = 0;
  static uint16_t samp[VBATT_SMOOTH];
  static int32_t t = 0;
  static bool f = true;
  if (f)
  {
    for (uint8_t c = 0; c < VBATT_SMOOTH; c++)
    {
      samp[c] = 0;
    }
    f = false;
  }             // Initialize the sample array first time
  t -= samp[i]; // doing a rolling recording, so remove the old rolled around value out of total and get ready to put new one in.
  if (t < 0)
  {
    t = 0;
  }

  // ADC read
  uint16_t voltage = ReadVBatt();

  samp[i] = voltage;
#if defined(__DEBUG) && __DEBUG > 0
  Serial.printf("ADC Raw Reading[%d]: %d", i, voltage);
#endif
  t += samp[i];

  if (++i >= VBATT_SMOOTH)
  {
    i = 0;
  }
  uint16_t s = round(((float)t / (float)VBATT_SMOOTH));
#if defined(__DEBUG) && __DEBUG > 0
  Serial.printf("   Smoothed of %d/%d = %d\n", t, VBATT_SMOOTH, s);
#endif

  return s;
}

void drawBattery(uint16_t voltage, bool sleep)
{
  Heltec.display->setColor(BLACK);
  Heltec.display->fillRect(99, 0, 29, 24);

  Heltec.display->setColor(WHITE);
  Heltec.display->drawRect(104, 0, 12, 6);
  Heltec.display->fillRect(116, 2, 1, 2);

  uint16_t v = voltage;
  if (v < MINBATT)
  {
    v = MINBATT;
  }
  if (v > MAXBATT)
  {
    v = MAXBATT;
  }
  double pct = map(v, MINBATT, MAXBATT, 0, 100);
  uint8_t bars = round(pct / 10.0);
  Heltec.display->fillRect(105, 1, bars, 4);
  Heltec.display->setFont(ArialMT_Plain_10);
  Heltec.display->setTextAlignment(TEXT_ALIGN_RIGHT);
  // Draw small "z" when using sleep
  if (sleep > 0)
  {
    Heltec.display->drawHorizontalLine(121, 0, 4);
    Heltec.display->drawHorizontalLine(121, 5, 4);
    Heltec.display->setPixel(124, 1);
    Heltec.display->setPixel(123, 2);
    Heltec.display->setPixel(122, 3);
    Heltec.display->setPixel(121, 4);
  }
  Heltec.display->drawString(127, 5, String((int)round(pct)) + "%");
  Heltec.display->drawString(127, 14, String(round(voltage / 10.0) / 100.0) + "V");

  Heltec.display->drawRect(10, 48, 100, 15);


#if defined(__DEBUG) && __DEBUG > 0
  static uint8_t c = 0;
  if ((c++ % 10) == 0)
  {
    c = 1;
    Serial.printf("VBAT: %dmV [%4.1f%%] %d bars\n", voltage, pct, bars);
  }
#endif
}