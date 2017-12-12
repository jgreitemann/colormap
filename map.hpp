#pragma once

#include "color.hpp"

#include <initializer_list>
#include <limits>
#include <map>
#include <utility>


namespace color {

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
