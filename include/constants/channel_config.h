#pragma once

#include <array>
#include <string>

#include "constants/board_variant.h"

namespace Configuration {
static const std::array<std::string, CX_BOARD_LED_CHANNEL_COUNT>
    kBoardConfigLedChannelKeys = {
        CX_BOARD_LED_CHANNEL_1_KEY,
        CX_BOARD_LED_CHANNEL_2_KEY,
        CX_BOARD_LED_CHANNEL_3_KEY,
        CX_BOARD_LED_CHANNEL_4_KEY,
};
}