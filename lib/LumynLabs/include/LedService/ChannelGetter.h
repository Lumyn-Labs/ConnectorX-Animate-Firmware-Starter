#pragma once

#include <string>

#include "constants/board_variant.h"

enum class LEDChannelPort {
  INVALID = -1,
  CHAN0 = CX_BOARD_LED_CHANNEL_1_PIN,
  CHAN1 = CX_BOARD_LED_CHANNEL_2_PIN,
  CHAN2 = CX_BOARD_LED_CHANNEL_3_PIN,
  CHAN3 = CX_BOARD_LED_CHANNEL_4_PIN,
};

static const CRGB kCorrection = CRGB(240, 90, 230);

CLEDController* AddFastLedController(LEDChannelPort, CRGB*, uint16_t);
LEDChannelPort GetLEDPortFromKey(const std::string&);
int GetLEDIndicatorFromKey(const std::string& key);
int GetLEDSelectorFromKey(const std::string& key);