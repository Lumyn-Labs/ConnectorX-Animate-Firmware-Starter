#pragma once

#include <Arduino.h>

#include <array>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include "ConfigurationParser/configs/AnimationGroup.h"
#include "ConfigurationParser/configs/AnimationSequence.h"
#include "ConfigurationParser/configs/Bitmap.h"
#include "ConfigurationParser/configs/Channel.h"
#include "ConfigurationParser/configs/Network.h"
#include "ConfigurationParser/configs/Sensor.h"
#include "generated/board_variant.h"

namespace Configuration {
static const std::array<std::string, CX_BOARD_LED_CHANNEL_COUNT>
    kBoardConfigLedChannelKeys = {
        CX_BOARD_LED_CHANNEL_1_KEY,
        CX_BOARD_LED_CHANNEL_2_KEY,
        CX_BOARD_LED_CHANNEL_3_KEY,
        CX_BOARD_LED_CHANNEL_4_KEY,
};

struct CXv3Configuration {
  uint8_t md5[16];
  std::optional<std::string> teamNumber;
  Network network;
#if CX_BOARD_FEATURES_ENABLE_LED
  std::optional<std::vector<Configuration::Channel>> channels;
  std::optional<std::vector<AnimationSequence>> sequences;
  std::optional<std::vector<Bitmap>> bitmaps;
  std::optional<std::vector<AnimationGroup>> animationGroups;
#endif
#if CX_BOARD_FEATURES_ENABLE_DEVICES
  std::optional<std::vector<Sensor>> sensors;
#endif
};
}  // namespace Configuration
