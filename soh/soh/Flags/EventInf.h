#pragma once

#include "soh/Flags.h"

// clang-format off
namespace Flags {
    namespace EventInf {
        const Flag RENTED_HORSE_FROM_INGO_1 = Flag::FromEventInf(0x00);
        const Flag RACING_INGO = Flag::FromEventInf(0x01);
        const Flag UNKNOWN_02 = Flag::FromEventInf(0x02);
        const Flag UNKNOWN_05 = Flag::FromEventInf(0x05);
        const Flag RACING_INGO_SECOND_TIME = Flag::FromEventInf(0x06);
        const Flag SPECIAL_CUCCO_FINISHED_OR_BEGAN_GERUDO_ARCHERY = Flag::FromEventInf(0x08);
        const Flag UNKNOWN_0A = Flag::FromEventInf(0x0A);
        const Flag RENTED_HORSE_FROM_INGO_2 = Flag::FromEventInf(0x0F);
        const Flag MARATHON_ACTIVE = Flag::FromEventInf(0x10);
        const Flag MARKET_CROWD_TEXT_RANDOMIZER_0 = Flag::FromEventInf(0x20);
        const Flag MARKET_CROWD_TEXT_RANDOMIZER_1 = Flag::FromEventInf(0x21);
        const Flag MARKET_CROWD_TEXT_RANDOMIZER_2 = Flag::FromEventInf(0x22);
        const Flag MARKET_CROWD_TEXT_RANDOMIZER_3 = Flag::FromEventInf(0x23);
        const Flag MARKET_CROWD_TEXT_RANDOMIZER_4 = Flag::FromEventInf(0x24);
        const Flag ENTERED_MARKET = Flag::FromEventInf(0x30);
    }
}
// clang-format on