// colormap -- color palettes, map iterators, grids, and PPM export
// Copyright (C) 2018-2019  Jonas Greitemann
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along
// with this program; if not, write to the Free Software Foundation, Inc.,
// 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

#pragma once

#include <array>
#include <cstdint>
#include <iostream>
#include <limits>

namespace colormap {

    enum class space {
        grayscale,
        rgb,
        rgba
    };

    template <space, typename = std::uint8_t>
    struct color;

    template <typename T>
    struct color<space::grayscale, T> {
        static T depth () { return std::numeric_limits<T>::max(); }
        static space color_space () { return space::grayscale; }

        color (T v = 0) : val(v) {}

        template <typename Float,
                  typename = typename std::enable_if<std::is_floating_point<Float>::value>::type>
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

    template <typename T, size_t N>
    struct basic_color {
        static T depth () { return std::numeric_limits<T>::max(); }
        static space color_space () { return space::rgb; }

        basic_color () : channels {{}} {}

        basic_color (std::array<color<space::grayscale,T>, N> const& chns)
            : channels(chns) {}

        template <typename Color,
                  typename = typename std::enable_if<std::is_base_of<basic_color, Color>::value>::type>
        Color mix (Color const& other, double mix) const {
            Color mixed;
            for (size_t i = 0; i < channels.size(); ++i)
                mixed.channels[i] = channels[i].mix(other.channels[i], mix);
            return mixed;
        }

        std::ostream & write (std::ostream & os) const {
            for (auto const& ch : channels)
                ch.write(os);
            return os;
        }

        friend std::ostream & operator<< (std::ostream & os, basic_color const& c) {
            for (auto const& ch : c.channels)
                os << ch;
            return os;
        }
    protected:
        std::array<color<space::grayscale,T>,N> channels;
    };

    template <typename T>
    struct color<space::rgb, T> : public basic_color<T, 3> {
        using Base = basic_color<T, 3>;

        using Base::Base;
        color (T r, T g, T b) : Base {{{ {r}, {g}, {b} }}} {}

        template <typename Float,
                  typename = typename std::enable_if<std::is_floating_point<Float>::value>::type>
        color (Float r, Float g, Float b) : Base {{{ {r}, {g}, {b} }}} {}

        friend color<space::rgba, T>;
    };

    template <typename T>
    struct color<space::rgba, T> : public basic_color<T, 4> {
        using Base = basic_color<T, 4>;

        using Base::Base;
        color (T r, T g, T b, T a) : Base {{{ {r}, {g}, {b}, {a} }}} {}
        color (color<space::rgb, T> const& rgb, T a = std::numeric_limits<T>::max())
            : Base {{{ rgb.channels[0], rgb.channels[1], rgb.channels[2], {a} }}} {}

        template <typename Float,
                  typename = typename std::enable_if<std::is_floating_point<Float>::value>::type>
        color (Float r, Float g, Float b, Float a) : Base {{{ {r}, {g}, {b}, {a} }}} {}

    };

}
