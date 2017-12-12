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

        color (T v = 0) : val(v) {}

        template <typename Float,
                  typename = typename std::enable_if_t<std::is_floating_point<Float>::value>>
        color (Float v) : val(v * depth()) {}

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
    private:
        T val;
    };

    template <typename T>
    struct color<space::rgb, T> {
        static T depth () { return std::numeric_limits<T>::max(); }

        color () : channels {{ {}, {}, {} }} {}

        color (T r, T g, T b) : channels {{ {r}, {g}, {b} }} {}

        template <typename Float,
                  typename = typename std::enable_if_t<std::is_floating_point<Float>::value>>
        color (Float r, Float g, Float b) : channels {{ {r}, {g}, {b}}} {}

        color mix (color const& other, double mix) const {
            color mixed;
            for (size_t i = 0; i < channels.size(); ++i)
                mixed.channels[i] = channels[i].mix(other.channels[i], mix);
            return mixed;
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
    private:
        std::array<color<space::grayscale,T>,3> channels;
    };

}
