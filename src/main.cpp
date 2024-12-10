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

#if CX_BOARD_FEATURES_ENABLE_DEVICES
  // TODO: auto-register a list of built-in devices when initializing
  SystemManagerService.registerModuleType(
      "VL53L1X", [](const Configuration::Sensor &cfg)
      { return std::make_shared<Modules::Builtin::VL53L1XModule>(cfg); });
#endif

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