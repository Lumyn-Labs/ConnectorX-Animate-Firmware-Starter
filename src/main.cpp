#include <SystemService/SystemService.h>

#include "Modules/builtin_modules/VL53L1X.h"

void setup()
{
  bool success = SystemManagerService.init();
  if (!success)
  {
    Serial.println("FAILURE");
    while (1)
    {
      delay(1000);
    }
  }

  success = SystemManagerService.initServices();
  if (!success)
  {
    Serial.println("FAILURE");
    while (1)
    {
      delay(1000);
    }
  }
}

void loop() { vTaskDelete(NULL); }

void setup1() {}

void loop1() { vTaskDelete(NULL); }