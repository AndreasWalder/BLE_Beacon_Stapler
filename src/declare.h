#include <Arduino.h>
#include <esp_adc_cal.h>
#include <driver/adc.h>
#include "heltec.h"
#include "TaskScheduler.h" //Task Scheduler
#include "Settings.h" //Einstellungen
#include "CheckBattery.h" //Batterie Auswertung und Anzeige %
#include "LiebherrLogo.h" //Liebherr Logo
#include "Setup.h" //Arduino Setup