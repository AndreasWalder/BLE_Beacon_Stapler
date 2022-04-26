// ---------------------------------------------SETTINGS--------------------------------------------------------------------------------------

//Batterie Auswertung Einstellungen
#define MAXBATT 4200             // The default Lipo is 4200mv when the battery is fully charged.
#define LIGHT_SLEEP_VOLTAGE 3750 // Point where start light sleep
#define LOWBATT 3600             // Point where give massage "Load Batt" unter 25%
#define MINBATT 3400             // The default Lipo is 3200mv when the battery is empty...this WILL be low on the 3.3v rail specs!!!

#define VOLTAGE_DIVIDER 3.20                     // Lora has 220k/100k voltage divider so need to reverse that reduction via (220k+100k)/100k on vbat GPIO37 or ADC1_1 (early revs were GPIO13 or ADC2_4 but do NOT use with WiFi.begin())
#define DEFAULT_VREF 1100                        // Default VREF use if no e-fuse calibration
#define VBATT_SAMPLE 500                         // Battery sample rate in ms
#define VBATT_SMOOTH 50                          // Number of averages in sample
#define ADC_READ_STABILIZE 5                     // in ms (delay from GPIO control and ADC connections times)
#define LO_BATT_SLEEP_TIME 10 * 60 * 1000 * 1000 // How long when low batt to stay in sleep (us)
#define HELTEC_V2_1 1                            // Set this to switch between GPIO13(V2.0) and GPIO37(V2.1) for VBatt ADC.
#define VBATT_GPIO 21                            // Heltec GPIO to toggle VBatt read connection ... WARNING!!! This also connects VEXT to VCC=3.3v so be careful what is on header.  Also, take care NOT to have ADC read connection in OPEN DRAIN when GPIO goes HIGH

void drawBattery(uint16_t, bool = false);
esp_adc_cal_characteristics_t *adc_chars;

uint16_t BatteryVoltage;

int i = 100;

void drawBar2(int x, int y, int w, int h, float value);
void InitBattery();
void BatteryCheck();

const int Analog_channel_pin1= 36;
int ADC_VALUE1 = 0;
int voltage_value1 = 0; 
const int Analog_channel_pin2= 38;
int ADC_VALUE2 = 0;
int voltage_value2 = 0; 
// ---------------------------------------------------------------------------------------------------------------------------------------------


// --------------------------------------------Check-QM-Settings-----------------------------------------------------------------------------------------
const int PushButtonPin = GPIO_NUM_2;    // the number of the pushbutton pin
int PushButtonState;         // the current reading from the input pin


// constants won't change. They're used here to set pin numbers:
const int extAlarmPin = GPIO_NUM_21;    // the number of the pushbutton pin

// Variables will change:
int extAlarmState;             // the current reading from the input pin
int lastExtAlarmState = LOW;   // the previous reading from the input pin

// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 50;    // the debounce time; increase if the output flickers

bool ExtAlarmOK;

bool RS485OK;


void IRAM_ATTR handler(void* arg);
void button_task(void* arg);
void Auswahl();
void FunctionCheck1();
void FunctionCheck2();
void FunctionCheck3();
void AuswahlNext();
//-----------------------------------------------------------------------------------------------------------------------------------------------

Scheduler runner;
void t1Callback();
void t2Callback();
void t3Callback();

Task t1(500, TASK_FOREVER, &t1Callback); //CheckBattery alle 20ms
Task t2(500, TASK_FOREVER, &t2Callback); //CheckStatus alle 1000ms
Task t3(300, TASK_FOREVER, &t3Callback); //CheckStatus alle 2000ms

void SleepCheck();
//---------------------------------------------------------------------------------------------------------------------------------------------