#include "Seeed_Arduino_GroveAI.h"
#include <M5StickCPlus.h>
#include <Wire.h>

GroveAI ai(Wire);
uint8_t state = 0;
void setup()
{
  M5.begin();
  Wire.begin(32,33);
  Serial.begin(115200);
  pinMode(10, OUTPUT);
   Serial.println("begin");
  if (ai.begin(ALGO_OBJECT_DETECTION, MODEL_EXT_INDEX_1)) // Object detection and pre-trained model 1
  {
    Serial.print("Version: ");
    Serial.println(ai.version());
    Serial.print("ID: ");
    Serial.println( ai.id());
    Serial.print("Algo: ");
    Serial.println( ai.algo());
    Serial.print("Model: ");
    Serial.println(ai.model());
    Serial.print("Confidence: ");
    Serial.println(ai.confidence());
    state = 1;
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.setCursor(0, 10);
    M5.Lcd.setTextColor(WHITE);
    M5.Lcd.setTextSize(1);
    M5.Lcd.printf("Sensor detected");
    delay(1500);
  }
  else
  {
    Serial.println("Algo begin failed.");
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.setCursor(0, 10);
    M5.Lcd.setTextColor(WHITE);
    M5.Lcd.setTextSize(1);
    M5.Lcd.printf("No Sensor");
  }
}

void loop()
{
  if (state == 1)
  {
    uint32_t tick = millis();
    if (ai.invoke()) // begin invoke
    {
      while (1) // wait for invoking finished
      {
        CMD_STATE_T ret = ai.state(); 
        if (ret == CMD_STATE_IDLE)
        {
          break;
        }
        delay(20);
      }

     uint8_t len = ai.get_result_len(); // receive how many hole detected
     if(len)
     {
       int time1 = millis() - tick; 
       Serial.print("Time consuming: ");
       Serial.println(time1);
       Serial.print("Number of holes: ");
       Serial.println(len);
       object_detection_t data;       //get data
        M5.Lcd.fillScreen(BLACK);
       for (int i = 0; i < len; i++)
       {
          Serial.println("result:detected");
          Serial.print("Detecting and calculating: ");
          Serial.println(i+1);
          ai.get_result(i, (uint8_t*)&data, sizeof(object_detection_t)); //get result
  
          Serial.print("confidence:");
          Serial.println(data.confidence);
          Serial.print("pos x:");
          Serial.println(data.x);
          Serial.print("pos y:");
          Serial.print(data.y);
          Serial.println();
          M5.Lcd.drawCircle(data.x, data.y, 3, RED);
        }
        digitalWrite(10, LOW);
     }
     else
     {
       Serial.println("No identification");
       M5.Lcd.fillScreen(BLACK);
       digitalWrite(10, HIGH);
     }
    }
    else
    {
      delay(1000);
      Serial.println("Invoke Failed.");
    }
  }
  else
  {
    state == 0;
  }
}
