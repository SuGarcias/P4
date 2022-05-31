#include <Arduino.h>
#include <WiFi.h>
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

#define version 'Oriol_1.0'

const char* ssid = "Xiaomi_11T_Pro";
const char* password = "f5cbd8a82232";

void funcioLED( void * parameter );
void funcioOTA( void * parameter );
void anotherTask( void * parameter )
{
/* loop forever */
for(;;)
{
Serial.println("this is another Task");
delay(1000);
}
/* delete a task when finish,
this will never happen because this is infinity loop */
vTaskDelete( NULL );
}




void setup()
{
    Serial.begin(112500);
    /* we create a new task here */
    xTaskCreate(
        funcioLED, /* Task function. */
        "Funcion LED", /* name of task. */
        1000, /* Stack size of task */
        NULL, /* parameter of the task */
        1, /* priority of the task */
        NULL); /* Task handle to keep track of created task */

    xTaskCreate(
        anotherTask, /* Task function. */
        "another Task", /* name of task. */
        1000, /* Stack size of task */
        NULL, /* parameter of the task */
        1, /* priority of the task */
        NULL);

    xTaskCreate(
        funcioOTA, /* Task function. */
        "OTA ", /* name of task. */
        10000, /* Stack size of task */
        NULL, /* parameter of the task */
        1, /* priority of the task */
        NULL);
    Serial.println("Oriol_1.0");
}


/* the forever loop() function is invoked by Arduino ESP32 loopTask */
void loop()
    {
        Serial.println("this is ESP32 Task");
        delay(1000);
    }
/* this function will be invoked when additionalTask was created */

void funcioLED( void * parameter )
    {

    #define LED 16
  // put your setup code here, to run once:

    pinMode(LED,OUTPUT);


        for(;;)
        {

        // put your main code here, to run repeatedly:
        digitalWrite(LED,HIGH);
        Serial.println("PUT ON");
        delay(200);
        Serial.println("PUT OFF");
        digitalWrite(LED,LOW);
        delay(200);

        }

    vTaskDelete( NULL );
    }

void funcioOTA( void * parameter )
    {
      Serial.println("Booting");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }

  // Port defaults to 3232
  // ArduinoOTA.setPort(3232);

  // Hostname defaults to esp3232-[MAC]
   ArduinoOTA.setHostname("POL_ORIOL");

  // No authentication by default
  // ArduinoOTA.setPassword("admin");

  // Password can be set with it's md5 value as well
  // MD5(admin) = 21232f297a57a5a743894a0e4a801fc3
  // ArduinoOTA.setPasswordHash("21232f297a57a5a743894a0e4a801fc3");

  ArduinoOTA
    .onStart([]() {
      String type;
      if (ArduinoOTA.getCommand() == U_FLASH)
        type = "sketch";
      else // U_SPIFFS
        type = "filesystem";

      // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
      Serial.println("Start updating " + type);
    })
    .onEnd([]() {
      Serial.println("\nEnd");
    })
    .onProgress([](unsigned int progress, unsigned int total) {
      Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
    })
    .onError([](ota_error_t error) {
      Serial.printf("Error[%u]: ", error);
      if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
      else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
      else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
      else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
      else if (error == OTA_END_ERROR) Serial.println("End Failed");
    });

  ArduinoOTA.begin();

  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

    for(;;)
    {
      ArduinoOTA.handle();
      delay(50);
    }
}