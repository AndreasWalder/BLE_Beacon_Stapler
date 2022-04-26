#include "declare.h" //Includes und Variablen

int a = 0;
int b = 0;
bool auswahlSelected = false;
SemaphoreHandle_t semaphore = nullptr;
unsigned long Zeit = 0;
unsigned long VorherigeMillis = 0;
const long interval1 = 1000;
const long interval2 = 180000;

String Geraet1 = "LK.. / LG.."; 
String Geraet2 = "MK..       ";
String Geraet3 = "L.. NIHON  "; 

int test = 0;
int auswahl = 0;
bool auswahlOK = true;
bool auswahlOK1 = false;
void setup() {
  IncludeSetup();
  semaphore = xSemaphoreCreateBinary();
	
	// Setup the button GPIO pin
	gpio_pad_select_gpio(GPIO_NUM_0);
	
	// Quite obvious, a button is a input
	gpio_set_direction(GPIO_NUM_0, GPIO_MODE_INPUT);
	
	// Trigger the interrupt when going from HIGH -> LOW ( == pushing button)
	gpio_set_intr_type(GPIO_NUM_0, GPIO_INTR_NEGEDGE);
	
	// Associate button_task method as a callback
	xTaskCreate(button_task, "button_task", 4096, NULL, 10, NULL);
	
	// Install default ISR service 
	gpio_install_isr_service(0);
	
	// Add our custom button handler to ISR
	gpio_isr_handler_add(GPIO_NUM_0, handler, NULL);
}

void loop() 
{
  runner.execute();
  if (auswahlOK1 == false)
         {
           Serial.println("Test auswahlSelected"); 
           auswahlOK = true;
           t2.enable();
            AuswahlNext();
         }   
}

void t1Callback() {
     BatteryCheck();
}

void t2Callback() {
  if (auswahlSelected == false)
  {
    if (BatteryVoltage < LOWBATT && BatteryVoltage > MINBATT)
    {
      InitBattery();
      i = 10;
      t1.enable();
      t2.disable();
      t3.enable();
      return;
    }
    Serial.println("Auswahl1");
    auswahlSelected = true;
    t1.disable();
    t2.disable();
    t3.disable();
    Serial.println("Auswahl2");
    Heltec.display->sleep();
    Heltec.display->wakeup();
    Heltec.display->init();
    Heltec.display->setColor(WHITE);
    Heltec.display->setFont(ArialMT_Plain_16);
    Heltec.display->setTextAlignment(TEXT_ALIGN_CENTER);
    Heltec.display->clear(); //OLET löschen
    Heltec.display->flipScreenVertically();
    Heltec.display->display(); //OLET anzeigen
    Heltec.display->drawString(55, 1, Geraet1 + " <");
    Heltec.display->drawString(46, 23, Geraet2);
    Heltec.display->drawString(55, 46, Geraet3);
    Heltec.display->display(); //OLET anzeigen  
    auswahl = 1;
    test = 2; 
    Auswahl();
  }
}

void t3Callback() { 
   SleepCheck();
}

void callback(){
}

void SleepCheck()
{
  Heltec.display->clear(); //OLET anzeigen  
  Heltec.display->flipScreenVertically();
  //Heltec.display->resetOrientation();
  drawBattery(BatteryVoltage, BatteryVoltage < LIGHT_SLEEP_VOLTAGE);
  Heltec.display->setFont(ArialMT_Plain_10);
  Heltec.display->setTextAlignment(TEXT_ALIGN_CENTER);
  Heltec.display->drawString(15, 3, "V1.0");

  Heltec.display->setFont(ArialMT_Plain_16);
  Heltec.display->setTextAlignment(TEXT_ALIGN_CENTER);
  Heltec.display->drawString(60, 27, "QM Check Box");

  i--;
  Serial.println(i);
  Heltec.display->fillRect(10, 48, i, 15);
  Heltec.display->display();

  if (i == 2)
  {
    Heltec.display->clear(); //OLET anzeigen  
    Heltec.display->flipScreenVertically();
    //Heltec.display->resetOrientation();
    Heltec.display->setColor(WHITE);
    Heltec.display->setFont(ArialMT_Plain_16);
    Heltec.display->setTextAlignment(TEXT_ALIGN_CENTER);
    Heltec.display->drawString(65, 14, "Akku sparren");
    Heltec.display->drawString(65, 30, "Legt sich schlafen!");
    Heltec.display->display();
    delay(3000);

    touchAttachInterrupt(T2, callback, 40);
    esp_sleep_enable_touchpad_wakeup();
    
    esp_deep_sleep_start();
    i = 100;
    return;
  }

  Heltec.display->display(); //OLET anzeigen  
}

void IRAM_ATTR handler(void* arg) {
  xSemaphoreGiveFromISR(semaphore, NULL);
}

void button_task(void* arg) {for(;;) {
    if(xSemaphoreTake(semaphore,portMAX_DELAY) == pdTRUE) {
      if (auswahlOK1 == true ) return;
      Serial.println("select button pushed!\n");
         t1.disable();
         t2.disable();
         t3.disable();
         Heltec.display->clear(); //OLET anzeigen  
         Heltec.display->sleep();
         Heltec.display->wakeup();
         Heltec.display->init();
         Heltec.display->init();
         Heltec.display->setColor(WHITE);
         Heltec.display->setFont(ArialMT_Plain_16);
         Heltec.display->setTextAlignment(TEXT_ALIGN_CENTER);
         Heltec.display->clear(); //OLET löschen
         Heltec.display->flipScreenVertically();
         Heltec.display->display(); //OLET anzeigen
         if (auswahlSelected == false)
         {
           Serial.println("Test auswahlSelected"); 
           auswahlOK = true;
           t2.enable();
           Auswahl();
         }

         
   
         if (test == 1)
         {
              Heltec.display->clear(); //OLET anzeigen  
              Heltec.display->init();
              Heltec.display->setColor(WHITE);
              Heltec.display->setFont(ArialMT_Plain_16);
              Heltec.display->setTextAlignment(TEXT_ALIGN_CENTER);
              Heltec.display->clear(); //OLET löschen
              Heltec.display->flipScreenVertically();
              Heltec.display->drawString(55, 1, Geraet1 + " <");
              Heltec.display->drawString(46, 23, Geraet2);
              Heltec.display->drawString(55, 46, Geraet3);
              auswahl = 1;           
              Serial.println("Auswahl1"); 
         }
         if (test == 2)
         {
              Heltec.display->clear(); //OLET anzeigen  
              Heltec.display->init();
              Heltec.display->setColor(WHITE);
              Heltec.display->setFont(ArialMT_Plain_16);
              Heltec.display->setTextAlignment(TEXT_ALIGN_CENTER);
              Heltec.display->clear(); //OLET löschen
              Heltec.display->flipScreenVertically();
              Heltec.display->drawString(55, 1, Geraet1);
              Heltec.display->drawString(46, 23, Geraet2 + " <");
              Heltec.display->drawString(55, 46, Geraet3);
              auswahl = 2;
              Serial.println("Auswahl2");
         }
         if (test == 3)
         {
              Heltec.display->clear(); //OLET anzeigen  
              Heltec.display->init();
              Heltec.display->setColor(WHITE);
              Heltec.display->setFont(ArialMT_Plain_16);
              Heltec.display->setTextAlignment(TEXT_ALIGN_CENTER);
              Heltec.display->clear(); //OLET löschen
              Heltec.display->flipScreenVertically();
              Heltec.display->drawString(55, 1, Geraet1);
              Heltec.display->drawString(46, 23, Geraet2);
              Heltec.display->drawString(55, 46, Geraet3 + " <");
              auswahl = 3;
              Serial.println("Auswahl3");
              test = 0;
         }

         test = test + 1;

        Heltec.display->display(); //OLET anzeigen
        sleep(1);

         Heltec.display->display(); //OLET anzeigen    

    }
  }
}

void AuswahlNext()
{
  auswahlOK1 = true;

  
  while (auswahlOK1)
  {
   

    Zeit = millis();
    if (Zeit - VorherigeMillis >= interval1) {
      VorherigeMillis = Zeit;
      BatteryCheck();
      SleepCheck();
    } 

    int PushButtonState = digitalRead(PushButtonPin);
 
          //button being held for half second 
          if (PushButtonState == LOW) 
          { 
              if (auswahl == 1)
              {
                  Serial.println("Auswahl: 1");
                  FunctionCheck1();
                  auswahlOK1 = false;
              }
              if (auswahl == 2)
              {
                  Serial.println("Auswahl: 2");
                  FunctionCheck2();
                  auswahlOK1 = false;
              }
              if (auswahl == 3)
              {
                  Serial.println("Auswahl: 3");
                  FunctionCheck3();
                  auswahlOK1 = false;
              }
              Heltec.display->display(); //OLET anzeigen  
              sleep(1);
          }
  }
}

void Auswahl()
{
  if (auswahl == 0) return;
  while (auswahlOK)
  {
    int PushButtonState = digitalRead(PushButtonPin);
 
          //button being held for half second 
          if (PushButtonState == LOW) 
          { 
              t1.disable();
              if (auswahl == 1)
              {
                  Serial.println("Auswahl: 1");
                  FunctionCheck1();
                  auswahlOK = false;
              }
              if (auswahl == 2)
              {
                  Serial.println("Auswahl: 2");
                  FunctionCheck2();
                  auswahlOK = false;
              }
              if (auswahl == 3)
              {
                  Serial.println("Auswahl: 3");
                  FunctionCheck3();
                  auswahlOK = false;
              }
              BatteryCheck();
              Heltec.display->display(); //OLET anzeigen  
              sleep(1);
          }

        Zeit = millis();
          
         if (Zeit - VorherigeMillis >= interval2) {
           VorherigeMillis = Zeit;
           Heltec.display->clear(); //OLET anzeigen  
           Heltec.display->flipScreenVertically();
            //Heltec.display->resetOrientation();
            Heltec.display->setColor(WHITE);
            Heltec.display->setFont(ArialMT_Plain_16);
            Heltec.display->setTextAlignment(TEXT_ALIGN_CENTER);
            Heltec.display->drawString(65, 14, "Akku sparren");
            Heltec.display->drawString(65, 30, "Legt sich schlafen!");
            Heltec.display->display();
            delay(3000);

            touchAttachInterrupt(T2, callback, 40);
            esp_sleep_enable_touchpad_wakeup();
            
            esp_deep_sleep_start();
            if (BatteryVoltage < LOWBATT && BatteryVoltage > MINBATT)
            {
              i = 10;
            } 
            else
            {
              i = 100;
            }
            
            return;
         }    
  }
}

void FunctionCheck1()
{
      t1.disable();
      t2.disable();
      t3.disable();
      if (BatteryVoltage < LOWBATT && BatteryVoltage > MINBATT)
      {
        i = 10;
      } 
      else
      {
        i = 100;
      }
      Serial.println("Messung Läuft");
      Heltec.display->sleep();
      delay(300);
      Heltec.display->wakeup();
      Heltec.display->init();
      Heltec.display->setColor(WHITE);
      Heltec.display->setFont(ArialMT_Plain_24);
      Heltec.display->setTextAlignment(TEXT_ALIGN_CENTER);
      Heltec.display->clear(); //OLET löschen
      Heltec.display->flipScreenVertically();
      //Heltec.display->resetOrientation();
      Heltec.display->display(); //OLET anzeigen
      Heltec.display->drawString(65, 5, "Messung");
      Heltec.display->drawString(68, 28, Geraet1);
      Heltec.display->drawString(65, 5, "Messung");
      Heltec.display->drawString(68, 28, Geraet1);
      Heltec.display->display(); //OLET anzeigen  
      for (size_t i = 20; i < 110; i++)
      {

         //Ergebnisse holen
         extAlarmState = digitalRead(extAlarmPin);

        ADC_VALUE1 = analogRead(Analog_channel_pin1);
        Serial.print("ADC VALUE1 = ");
        Serial.println(ADC_VALUE1);
        delay(5);
        voltage_value1 = (ADC_VALUE1 * 3.3 ) / (4095);
        voltage_value1 = voltage_value1 * 2.24;
        delay(5);

        ADC_VALUE2 = analogRead(Analog_channel_pin2);
        Serial.print("ADC VALUE2 = ");
        Serial.println(ADC_VALUE2);
        delay(5);
        voltage_value2 = (ADC_VALUE2 * 3.3 ) / (4095);
        voltage_value2 = voltage_value2 * 2.24;
        delay(5);

         

         Heltec.display->drawString(i, 38, ".");
         Heltec.display->display(); //OLET anzeigen 
         Serial.print("Ext Alarm:"); 
         Serial.println(extAlarmState);
         Serial.print("RS485 Messpunkt1:");
         Serial.println(voltage_value1);
         Serial.print("RS485 Messpunkt2:");
         Serial.println(voltage_value2);
         delay(5);
      }
      
      Heltec.display->sleep();
      delay(1000);
      Heltec.display->wakeup();
      Heltec.display->init();
      Heltec.display->setColor(WHITE);
      Heltec.display->setFont(ArialMT_Plain_24);
      Heltec.display->setTextAlignment(TEXT_ALIGN_CENTER);
      Heltec.display->clear(); //OLET löschen
      Heltec.display->flipScreenVertically();
      //Heltec.display->resetOrientation();
      Heltec.display->display(); //OLET anzeigen
      Heltec.display->drawString(62, 19, "Ergebnisse");
      Heltec.display->display(); //OLET anzeigen
      delay(500);
      Heltec.display->clear(); //OLET löschen

     if (extAlarmState == false) {
       if (ADC_VALUE1 == 0)
       {
          Heltec.display->drawString(69, 7, "keine");
          Heltec.display->drawString(67, 28, "Spannung");
          Heltec.display->display(); //OLET anzeigen   
          ExtAlarmOK = false;
          Heltec.display->display(); //OLET anzeigen
          delay(1500);
          Heltec.display->clear(); //OLET löschen
          Heltec.display->display(); //OLET anzeigen
          Serial.println("Weiter");
          InitBattery();
          t1.enable();
          t2.enable();
          t3.enable();
          Heltec.display->display(); //OLET anzeigen   
          ExtAlarmOK = false;
          return;
       }
       else
       {
       Heltec.display->drawString(69, 7, "Alarm");
       Heltec.display->drawString(71, 30, "OK");
       Heltec.display->display(); //OLET anzeigen   
       ExtAlarmOK = true;
       }
     } 
     else
     {
       Heltec.display->drawString(69, 7, "Alarm");
       Heltec.display->drawString(68, 30, "Fehler");
       Heltec.display->display(); //OLET anzeigen 
       ExtAlarmOK = false;      
     }
       
       delay(1000);
       Heltec.display->clear(); //OLET löschen
       Heltec.display->display(); //OLET anzeigen
       delay(500);
        
        /*
        Heltec.display->clear(); //OLET löschen
        Heltec.display->drawString(69, 7, String(voltage_value1));
        Heltec.display->drawString(68, 30, String(voltage_value2));
        Heltec.display->display(); //OLET anzeigen
        delay(1000);
        */

      if (voltage_value1 < 1.5 || voltage_value2 < 1.5) {   
        Heltec.display->clear(); //OLET löschen
        Heltec.display->drawString(69, 7, "RS485");
        Heltec.display->drawString(68, 30, "Fehler");
        Heltec.display->display(); //OLET anzeigen
        RS485OK = false;
      }
      else
      {
        Heltec.display->clear(); //OLET löschen
        Heltec.display->drawString(69, 7, "RS485");
        Heltec.display->drawString(71, 30, "OK");
        Heltec.display->display(); //OLET anzeigen
        RS485OK = true;     
      }
      
      delay(1000);
      Heltec.display->clear(); //OLET löschen
      Heltec.display->display(); //OLET anzeigen
      Heltec.display->drawString(69, 19, "FERTIG!");
      Heltec.display->display(); //OLET anzeigen
      delay(1500);
      Heltec.display->clear(); //OLET löschen
      Heltec.display->display(); //OLET anzeigen
      Serial.println("Weiter");
      InitBattery();
      t1.enable();
      t2.enable();
      t3.enable();
      auswahlSelected = false;
}

void FunctionCheck2()
{
      t1.disable();
      t2.disable();
      t3.disable();
      Serial.println("Messung Läuft");
      Heltec.display->sleep();
      delay(300);
      Heltec.display->wakeup();
      Heltec.display->init();
      Heltec.display->setColor(WHITE);
      Heltec.display->setFont(ArialMT_Plain_24);
      Heltec.display->setTextAlignment(TEXT_ALIGN_CENTER);
      Heltec.display->clear(); //OLET löschen
      Heltec.display->flipScreenVertically();
      //Heltec.display->resetOrientation();
      Heltec.display->display(); //OLET anzeigen
      Heltec.display->drawString(65, 5, "Messung");
      Heltec.display->drawString(68, 28, Geraet2);
      Heltec.display->drawString(65, 5, "Messung");
      Heltec.display->drawString(68, 28, Geraet2);
      Heltec.display->display(); //OLET anzeigen  
      for (size_t i = 20; i < 110; i++)
      {

         //Ergebnisse holen
         extAlarmState = digitalRead(extAlarmPin);

        ADC_VALUE1 = analogRead(Analog_channel_pin1);
        Serial.print("ADC VALUE1 = ");
        Serial.println(ADC_VALUE1);
        delay(5);
        voltage_value1 = (ADC_VALUE1 * 3.3 ) / (4095);
        voltage_value1 = voltage_value1 * 2.24;
        delay(5);

        ADC_VALUE2 = analogRead(Analog_channel_pin2);
        Serial.print("ADC VALUE2 = ");
        Serial.println(ADC_VALUE2);
        delay(5);
        voltage_value2 = (ADC_VALUE2 * 3.3 ) / (4095);
        voltage_value2 = voltage_value2 * 2.24;
        delay(5);

         

         Heltec.display->drawString(i, 38, ".");
         Heltec.display->display(); //OLET anzeigen 
         Serial.print("Ext Alarm:"); 
         Serial.println(extAlarmState);
         Serial.print("RS485 Messpunkt1:");
         Serial.println(voltage_value1);
         Serial.print("RS485 Messpunkt2:");
         Serial.println(voltage_value2);
         delay(5);
      }
      
      
     
      Heltec.display->sleep();
      delay(1000);
      Heltec.display->wakeup();
      Heltec.display->init();
      Heltec.display->setColor(WHITE);
      Heltec.display->setFont(ArialMT_Plain_24);
      Heltec.display->setTextAlignment(TEXT_ALIGN_CENTER);
      Heltec.display->clear(); //OLET löschen
      Heltec.display->flipScreenVertically();
      //Heltec.display->resetOrientation();
      Heltec.display->display(); //OLET anzeigen
      Heltec.display->drawString(62, 19, "Ergebnisse");
      Heltec.display->display(); //OLET anzeigen
      delay(500);
      Heltec.display->clear(); //OLET löschen

     
     if (extAlarmState == true) {
       if (ADC_VALUE1 == 0)
       {
          Heltec.display->drawString(69, 7, "keine");
          Heltec.display->drawString(67, 28, "Spannung");
          Heltec.display->display(); //OLET anzeigen   
          ExtAlarmOK = false;
          Heltec.display->display(); //OLET anzeigen
          delay(1500);
          Heltec.display->clear(); //OLET löschen
          Heltec.display->display(); //OLET anzeigen
          Serial.println("Weiter");
          InitBattery();
          t1.enable();
          t2.enable();
          t3.enable();
          Heltec.display->display(); //OLET anzeigen   
          ExtAlarmOK = false;
          return;
       }
       else
       {
       Heltec.display->drawString(69, 7, "Alarm");
       Heltec.display->drawString(71, 30, "OK");
       Heltec.display->display(); //OLET anzeigen   
       ExtAlarmOK = true;
       }
     } 
     else
     {
       Heltec.display->drawString(69, 7, "Alarm");
       Heltec.display->drawString(68, 30, "Fehler");
       Heltec.display->display(); //OLET anzeigen 
       ExtAlarmOK = false;      
     }
       
       delay(1000);
       Heltec.display->clear(); //OLET löschen
       Heltec.display->display(); //OLET anzeigen
       delay(500);
        
        /*
        Heltec.display->clear(); //OLET löschen
        Heltec.display->drawString(69, 7, String(voltage_value1));
        Heltec.display->drawString(68, 30, String(voltage_value2));
        Heltec.display->display(); //OLET anzeigen
        delay(1000);
        */

      if (voltage_value1 < 1.5 || voltage_value2 < 1.5) {   
        Heltec.display->clear(); //OLET löschen
        Heltec.display->drawString(69, 7, "RS485");
        Heltec.display->drawString(68, 30, "Fehler");
        Heltec.display->display(); //OLET anzeigen
        RS485OK = false;          
      }
      else
      {
        Heltec.display->clear(); //OLET löschen
        Heltec.display->drawString(69, 7, "RS485");
        Heltec.display->drawString(71, 30, "OK");
        Heltec.display->display(); //OLET anzeigen
        RS485OK = true;   
      }
      
      delay(1000);
      Heltec.display->clear(); //OLET löschen
      Heltec.display->display(); //OLET anzeigen
      Heltec.display->drawString(69, 19, "FERTIG!");
      Heltec.display->display(); //OLET anzeigen
      delay(1500);
      Heltec.display->clear(); //OLET löschen
      Heltec.display->display(); //OLET anzeigen
      Serial.println("Weiter");
      InitBattery();
      t1.enable();
      t2.enable();
      t3.enable();
      auswahlSelected = false;
}


void FunctionCheck3()
{
      t1.disable();
      t2.disable();
      t3.disable();
      if (BatteryVoltage < LOWBATT && BatteryVoltage > MINBATT)
      {
        i = 10;
      } 
      else
      {
        i = 100;
      }
      Serial.println("Messung Läuft");
      Heltec.display->sleep();
      delay(300);
      Heltec.display->wakeup();
      Heltec.display->init();
      Heltec.display->setColor(WHITE);
      Heltec.display->setFont(ArialMT_Plain_24);
      Heltec.display->setTextAlignment(TEXT_ALIGN_CENTER);
      Heltec.display->clear(); //OLET löschen
      Heltec.display->flipScreenVertically();
      //Heltec.display->resetOrientation();
      Heltec.display->display(); //OLET anzeigen
      Heltec.display->drawString(65, 5, "Messung");
      Heltec.display->drawString(68, 28, Geraet3);
      Heltec.display->drawString(65, 5, "Messung");
      Heltec.display->drawString(68, 28, Geraet3);
      Heltec.display->display(); //OLET anzeigen  
      for (size_t i = 20; i < 110; i++)
      {

         //Ergebnisse holen
         extAlarmState = digitalRead(extAlarmPin);

         Heltec.display->drawString(i, 38, ".");
         Heltec.display->display(); //OLET anzeigen 
         Serial.print("Ext Alarm:"); 
         Serial.println(extAlarmState);
         delay(7);
      }
      
      Heltec.display->sleep();
      delay(1000);
      Heltec.display->wakeup();
      Heltec.display->init();
      Heltec.display->setColor(WHITE);
      Heltec.display->setFont(ArialMT_Plain_24);
      Heltec.display->setTextAlignment(TEXT_ALIGN_CENTER);
      Heltec.display->clear(); //OLET löschen
      Heltec.display->flipScreenVertically();
      //Heltec.display->resetOrientation();
      Heltec.display->display(); //OLET anzeigen
      Heltec.display->drawString(62, 19, "Ergebnisse");
      Heltec.display->display(); //OLET anzeigen
      delay(500);
      Heltec.display->clear(); //OLET löschen

     if (extAlarmState == false) {
       Heltec.display->drawString(69, 7, "Alarm");
       Heltec.display->drawString(71, 30, "OK");
       Heltec.display->display(); //OLET anzeigen   
       ExtAlarmOK = true;
     } 
     else
     {
       Heltec.display->drawString(69, 7, "Alarm");
       Heltec.display->drawString(68, 30, "Fehler");
       Heltec.display->display(); //OLET anzeigen 
       ExtAlarmOK = false;      
     }
       
      delay(1000);
      Heltec.display->clear(); //OLET löschen
      Heltec.display->display(); //OLET anzeigen
      delay(500);
      
      delay(1000);
      Heltec.display->clear(); //OLET löschen
      Heltec.display->display(); //OLET anzeigen
      Heltec.display->drawString(69, 19, "FERTIG!");
      Heltec.display->display(); //OLET anzeigen
      delay(1500);
      Heltec.display->clear(); //OLET löschen
      Heltec.display->display(); //OLET anzeigen
      Serial.println("Weiter");
      InitBattery();
      t1.enable();
      t2.enable();
      t3.enable();
      auswahlSelected = false;
}


void BatteryCheck(){
//a = a + 1;
  //Serial.print("a");
  //Serial.println(a);
  BatteryVoltage = Sample();
  delay(ADC_READ_STABILIZE);

  if (BatteryVoltage < LOWBATT && BatteryVoltage > MINBATT)
  { 
    t3.disable();
    Heltec.display->clear(); //OLET löschen
    drawBattery(BatteryVoltage, BatteryVoltage < LIGHT_SLEEP_VOLTAGE);
    Heltec.display->display(); //OLET anzeigen
    Heltec.display->flipScreenVertically();
    //Heltec.display->resetOrientation();
    Heltec.display->setColor(WHITE);
    Heltec.display->setFont(ArialMT_Plain_16);
    Heltec.display->setTextAlignment(TEXT_ALIGN_CENTER);
    Heltec.display->drawString(60, 29, "Zu wenig Akku");
    Heltec.display->drawString(61, 46, "Bitte aufladen");
    Heltec.display->display();
    //Für die längere Anzeige (Zu wenig Akku!)
    sleep(3);
    return;
  }else
  {
    t3.enable();
  }
  
  if (BatteryVoltage < MINBATT)
  { // Under Low BatteryVoltage cut off shut down to protect battery as long as possible
    t3.disable();
    Heltec.display->clear(); //OLET löschen
    Heltec.display->display(); //OLET anzeigen
    Heltec.display->flipScreenVertically();
    //Heltec.display->resetOrientation();
    Heltec.display->setColor(WHITE);
    Heltec.display->setFont(ArialMT_Plain_16);
    Heltec.display->setTextAlignment(TEXT_ALIGN_CENTER);
    Heltec.display->drawString(60, 14, "Zu wenig Akku!!");
    Heltec.display->drawString(64, 30, "Shutdown!!");
    Heltec.display->display();
    delay(10000);
    esp_sleep_enable_timer_wakeup(LO_BATT_SLEEP_TIME);
    esp_deep_sleep_start();
  }
}