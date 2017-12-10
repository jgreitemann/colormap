#pragma once

#include <cstdint>
#include <initializer_list>
#include <iostream>
#include <limits>
#include <map>
#include <string>
#include <utility>


namespace color {

    enum class space {
        grayscale,
        rgb
    };

    template <space cs, typename T = std::uint8_t>
    struct color;

    template <typename T>
    struct color<space::grayscale, T> {
        static T depth () { return std::numeric_limits<T>::max(); }

        color mix (color const& other, double mix) const {
            return { T(other.val * mix + val * (1. - mix)) };
        }

        std::ostream & write (std::ostream & os) const {
            return os.write(reinterpret_cast<char const *>(&val), sizeof(T));
        }

        friend std::ostream & operator<< (std::ostream & os, color const& c) {
            os << long(c.val) << ' ';
            return os;
        }
        T val;
    };

    template <typename T>
    struct color<space::rgb, T> {
        static T depth () { return std::numeric_limits<T>::max(); }

        color mix (color const& other, double mix) const {
            decltype(channels) mixed_channels;
            for (size_t i = 0; i < channels.size(); ++i)
                mixed_channels[i] = channels[i].mix(other.channels[i], mix);
            return { mixed_channels };
        }

        std::ostream & write (std::ostream & os) const {
            for (auto const& ch : channels)
                ch.write(os);
            return os;
        }

        friend std::ostream & operator<< (std::ostream & os, color const& c) {
            for (auto const& ch : c.channels)
                os << ch;
            return os;
        }
        std::array<color<space::grayscale,T>,3> channels;
    };

    template <typename Color>
    struct map {
        using color_type = Color;

        map (std::initializer_list<Color> il) : range {0, 1} {
            double step = 1. / (il.size() - 1);
            size_t i = 0;
            for (Color const& c : il)
                supports[(i++) * step] = c;
        }

        map (std::initializer_list<std::pair<double,Color>> il) {
            range = { std::numeric_limits<double>::max(),
                      std::numeric_limits<double>::min() };
            for (auto [val, c] : il) {
                if (range.first > val) range.first = val;
                if (range.second < val) range.second = val;
                supports[val] = c;
            }
        }

        map rescale (double x_min, double x_max) const {
            map rescaled(*this);
            rescaled.range = {x_min, x_max};
            return rescaled;
        }

        Color operator() (double x) const {
            x = (x - range.first) / (range.second - range.first);
            auto a = supports.lower_bound(x);
            auto b = supports.upper_bound(x);
            if (a == supports.end())
                return (--a)->second;
            if (b == supports.end() || a == supports.begin())
                return a->second;
            --a;
            double mix = (x - a->first) / (b->first - a->first);
            return a->second.mix(b->second, mix);
        }

    private:
        std::map<double, Color> supports;
        std::pair<double,double> range;
    };

    namespace {
        using gray = color<space::grayscale>;
        using rgb = color<space::rgb>;
    }
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
    const map<gray> grayscale { gray {0}, gray {255} };
}
