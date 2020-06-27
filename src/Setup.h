#include <heltec.h>
void IncludeSetup() {
 
  // ----------------------OLET----------------------------------------------
   Heltec.begin(true /*DisplayEnable Enable*/, false /*LoRa Disable*/, true /*Serial Enable*/);
   pinMode(25, OUTPUT); //OLET Beleuchtung
   digitalWrite(25,HIGH); //OLET Beleuchtung an
   Heltec.display->clear(); //OLET löschen
   Heltec.display->setFont(ArialMT_Plain_10);         //Schriftgröße 10
   Heltec.display->setTextAlignment(TEXT_ALIGN_LEFT); //Schriftausrichtung
   Heltec.display->drawXbm(10, 8, Liebherr_Logo_width, Liebherr_Logo_height, Liebherr_Logo_bits);
   Heltec.display->drawString(20, 30, "Indoor Localization");
   Heltec.display->drawString(40, 44, "(R) 2020");
   Heltec.display->display(); //OLET anzeigen
   delay(5000);
   Heltec.display->clear(); //OLET löschen
   Heltec.display->display(); //OLET anzeigen
}