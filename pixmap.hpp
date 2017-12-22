#pragma once

#include "color.hpp"

#include <iostream>
#include <iterator>
#include <sstream>


namespace color {

    template <typename ForwardIterator>
    struct pixmap {
        using color_type = typename std::iterator_traits<ForwardIterator>::value_type;
        using shape_type = std::pair<size_t, size_t>;

        pixmap (ForwardIterator begin, shape_type shape)
            : begin(begin), shape(shape) {};

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
            }
        }

    private:
        ForwardIterator begin;
        shape_type shape;

        short magic_number (bool binary) const {
            switch (color_type::color_space()) {
            case space::grayscale: return binary ? 5 : 2;
            case space::rgb:       return binary ? 6 : 3;
            }
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
