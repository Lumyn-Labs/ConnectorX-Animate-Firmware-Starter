#pragma once

#include "generated/board_variant.h"

enum class LEDChannelPort {
  INVALID = -1,
  CHAN0 = CX_BOARD_LED_CHANNEL_1_PIN,
  CHAN1 = CX_BOARD_LED_CHANNEL_2_PIN,
  CHAN2 = CX_BOARD_LED_CHANNEL_3_PIN,
  CHAN3 = CX_BOARD_LED_CHANNEL_4_PIN,
};

static const CRGB kCorrection = CRGB(240, 90, 230);

static CLEDController* AddFastLedController(LEDChannelPort port, CRGB* buf,
                                            uint16_t count) {
  if (!buf) {
    return nullptr;
  }
  if (port == LEDChannelPort::CHAN0)
    return &FastLED.addLeds<WS2812, static_cast<uint8_t>(LEDChannelPort::CHAN0),
                            GRB>(buf, count).setCorrection(kCorrection);
  if (port == LEDChannelPort::CHAN1)
    return &FastLED.addLeds<WS2812, static_cast<uint8_t>(LEDChannelPort::CHAN1),
                            GRB>(buf, count).setCorrection(kCorrection);
  if (port == LEDChannelPort::CHAN2)
    return &FastLED.addLeds<WS2812, static_cast<uint8_t>(LEDChannelPort::CHAN2),
                            GRB>(buf, count).setCorrection(kCorrection);
  if (port == LEDChannelPort::CHAN3)
    return &FastLED.addLeds<WS2812, static_cast<uint8_t>(LEDChannelPort::CHAN3),
                            GRB>(buf, count).setCorrection(kCorrection);
  return nullptr;
}
static LEDChannelPort GetLEDPortFromKey(const std::string& key) {
  if (key == CX_BOARD_LED_CHANNEL_1_KEY)
    return LEDChannelPort::CHAN0;
  if (key == CX_BOARD_LED_CHANNEL_2_KEY)
    return LEDChannelPort::CHAN1;
  if (key == CX_BOARD_LED_CHANNEL_3_KEY)
    return LEDChannelPort::CHAN2;
  if (key == CX_BOARD_LED_CHANNEL_4_KEY)
    return LEDChannelPort::CHAN3;

  return LEDChannelPort::INVALID;
}

static int GetLEDIndicatorFromKey(const std::string& key) {
  if (key == CX_BOARD_LED_CHANNEL_1_KEY)
    return 29;
  if (key == CX_BOARD_LED_CHANNEL_2_KEY)
    return 21;
  if (key == CX_BOARD_LED_CHANNEL_3_KEY)
    return 7;
  if (key == CX_BOARD_LED_CHANNEL_4_KEY)
    return 3;

  return -1;
}
