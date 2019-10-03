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

#include <iostream>
#include <iterator>
#include <sstream>

#include <colormap/color.hpp>


namespace colormap {

    template <typename ForwardIterator>
    struct pixmap {
        using color_type = typename std::iterator_traits<ForwardIterator>::value_type;
        using shape_type = std::pair<size_t, size_t>;

        pixmap (ForwardIterator begin, shape_type shape)
            : begin(begin), shape(shape) {};

        pixmap (ForwardIterator begin, std::array<size_t,2> const& alt_shape)
            : begin(begin), shape{alt_shape[0], alt_shape[1]} {};

        std::ostream & write_ascii (std::ostream & os) const {
            ForwardIterator it(begin);
            os << header(false);
            for (size_t i = 0; i < shape.second; ++i) {
                for (size_t j = 0; j < shape.first; ++j, ++it) {
                    os << *it;
                }
                os << '\n';
            }
            return os;
        }

        std::ostream & write_binary (std::ostream & os) const {
            ForwardIterator it(begin);
            std::string hdr = header(true);
            os.write(hdr.c_str(), hdr.size());
            for (size_t i = 0; i < shape.second; ++i) {
                for (size_t j = 0; j < shape.first; ++j, ++it) {
                    color_type pix = *it;
                    pix.write(os);
                }
            }
            return os;
        }

        std::string file_extension () const {
            switch (color_type::color_space()) {
            case space::grayscale: return "pgm";
            case space::rgb:       return "ppm";
            default:
                throw std::runtime_error("no extension for color space");
            }
            return "";
        }

    private:
        ForwardIterator begin;
        shape_type shape;

        short magic_number (bool binary) const {
            switch (color_type::color_space()) {
            case space::grayscale: return binary ? 5 : 2;
            case space::rgb:       return binary ? 6 : 3;
            default:
                throw std::runtime_error("no magic number for color space");
            }
            return -1;
        }

        std::string header (bool binary) const {
            std::stringstream ss;
            ss << 'P' << magic_number(binary) << '\n'
               << shape.first << ' ' << shape.second << '\n'
               << size_t(color_type::depth()) << '\n';
            return ss.str();
        }
    };

}
