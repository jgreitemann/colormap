#pragma once

#include "color.hpp"
#include "map.hpp"

#include <map>
#include <string>


namespace color {

    namespace {
        using gray = color<space::grayscale>;
        using rgb = color<space::rgb>;
    }

    const map<gray> grayscale { gray {0}, gray {255} };

    const std::map<std::string, map<rgb>> palettes = {
        {
            "parula",
            map<rgb> {
                rgb {{0x35, 0x2a, 0x87}},
                rgb {{0x03, 0x63, 0xe1}},
                rgb {{0x14, 0x85, 0xd4}},
                rgb {{0x06, 0xa7, 0xc6}},
                rgb {{0x38, 0xb9, 0x9e}},
                rgb {{0x92, 0xbf, 0x73}},
                rgb {{0xd9, 0xba, 0x56}},
                rgb {{0xfc, 0xce, 0x2e}},
                rgb {{0xf9, 0xfb, 0x0e}}
            }
        },
        {
            "rdbu",
            map<rgb> {
                rgb {{0xB2, 0x18, 0x2B}},
                rgb {{0xD6, 0x60, 0x4D}},
                rgb {{0xF4, 0xA5, 0x82}},
                rgb {{0xFD, 0xDB, 0xC7}},
                rgb {{0xD1, 0xE5, 0xF0}},
                rgb {{0x92, 0xC5, 0xDE}},
                rgb {{0x43, 0x93, 0xC3}},
                rgb {{0x21, 0x66, 0xAC}},
            }
        }
    };

}
