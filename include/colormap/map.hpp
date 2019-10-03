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

#include <initializer_list>
#include <limits>
#include <map>
#include <utility>

#include <colormap/color.hpp>


namespace colormap {

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
            for (auto p : il) {
                double & val = p.first;
                if (range.first > val) range.first = val;
                if (range.second < val) range.second = val;
            }
            for (auto p : il) {
                double & val = p.first;
                Color & c = p.second;
                supports[(val - range.first) / (range.second - range.first)] = c;
            }
            range = {0., 1.};
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
            if (b == supports.end() || a == supports.begin() || a->first == x)
                return a->second;
            --a;
            double mix = (x - a->first) / (b->first - a->first);
            return a->second.mix(b->second, mix);
        }

    private:
        std::map<double, Color> supports;
        std::pair<double,double> range;
    };

}
