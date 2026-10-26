// =============================================================================
// SSD1306 OLED screen module
// =============================================================================
//
// A 0.96" 128x64 SSD1306 OLED on the I2C (SCREEN) connector. Robot code sends
// it text as JSON, and it reports whether the screen answers.
//
// Add it to the device's config as a module of type "SSD1306" on I2C:
//
//   { "id": "screen", "type": "SSD1306", "connection": "I2C",
//     "pollingRateMs": 500 }
//
// Send it JSON to show text: { "text": "Hello", "size": 2 }. "size" is the
// text scale, 1 to 4 (1 is 6x8 pixels per character); it defaults to 2.
// =============================================================================

#pragma once

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <LumynLabs.h>
#include <Wire.h>

// What the module reports at each poll.
struct OledStatus
{
  uint8_t found;          // 1 when the screen answered on I2C at this poll
  uint8_t address;        // the screen's I2C address, 0x3C or 0x3D
  uint16_t messagesShown; // how many texts it has drawn since it started
};

class OledModule : public LumynLabs::Module<OledStatus>
{
public:
  static constexpr uint8_t kWidth = 128;
  static constexpr uint8_t kHeight = 64;

  explicit OledModule(const LumynLabs::ModuleConfig &config) : Module(config) {}
  ~OledModule() override { delete _display; }

  LumynLabs::ModuleError initModule() override
  {
    TwoWire &wire = peripherals().getI2C();
    for (uint8_t address : {0x3C, 0x3D})
    {
      if (answers(wire, address))
      {
        _address = address;
        break;
      }
    }
    if (!_address)
    {
      return LumynLabs::ModuleError::error(LumynLabs::ModuleErrorType::CommunicationFail);
    }

    _display = new Adafruit_SSD1306(kWidth, kHeight, &wire, -1);
    // The firmware has already started the I2C bus, so the driver mustn't.
    if (!_display->begin(SSD1306_SWITCHCAPVCC, _address, false, false))
    {
      return LumynLabs::ModuleError::error(LumynLabs::ModuleErrorType::InternalError);
    }
    show("Lumyn Labs", 2);
    return LumynLabs::ModuleError::ok();
  }

  LumynLabs::ModuleError readData(OledStatus *dataOut) override
  {
    dataOut->found = answers(peripherals().getI2C(), _address) ? 1 : 0;
    dataOut->address = _address;
    dataOut->messagesShown = _messagesShown;
    return LumynLabs::ModuleError::ok();
  }

  bool handleReceivedJson(ArduinoJson::JsonVariantConst json) override
  {
    const char *text = json["text"];
    if (!_display || !text)
    {
      return false;
    }
    int size = json["size"] | 2;
    show(text, constrain(size, 1, 4));
    _messagesShown++;
    return true;
  }

private:
  static bool answers(TwoWire &wire, uint8_t address)
  {
    wire.beginTransmission(address);
    return wire.endTransmission() == 0;
  }

  void show(const char *text, uint8_t size)
  {
    _display->clearDisplay();
    _display->setTextSize(size);
    _display->setTextColor(SSD1306_WHITE);
    _display->setCursor(0, 0);
    _display->print(text);
    _display->display();
  }

  Adafruit_SSD1306 *_display = nullptr;
  uint8_t _address = 0;
  uint16_t _messagesShown = 0;
};
