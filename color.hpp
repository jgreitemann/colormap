#pragma once

#include <array>
#include <cstdint>
#include <iostream>
#include <limits>

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

}
