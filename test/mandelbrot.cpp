#include "colormap.hpp"
#include "grid.hpp"

#include <algorithm>
#include <cmath>
#include <complex>
#include <fstream>
#include <iostream>
#include <utility>
#include <vector>


int main () {
    size_t max_it = 1000;
    double bail_out = std::pow(2, 16);
    auto func = [] (double x) { return pow(x, 0.1); };

    grid<2, major_order::col> g { {701, {-2.5, 1.}}, {401, {-1., 1.}} };
    std::vector<double> vals;
    vals.reserve(g.size());
    std::transform(g.begin(), g.end(), std::back_inserter(vals), [&] (std::array<double,2> c_arr) {
            std::complex<double> c { c_arr[0], c_arr[1] };
            std::complex<double> z = 0.;
            size_t it;
            for (it = 0; it < max_it && std::norm(z) < bail_out; ++it)
                z = z * z + c;
            if (it < max_it) {
                double log2_z = log(std::norm(z)) * 0.5 / log(2);
                double nu = log(log2_z) / log(2);
                return func(it + 1 - nu);
            } else {
                return 0.;
            }
        });
    auto pal = color::palettes.at("inferno").rescale(1, func(max_it));
    color::pixmap pmap(color::adapt_iterator(vals.begin(), pal), g.shape());

    {
        std::ofstream os("appleman_binary." + pmap.file_extension(), std::ios_base::binary);
        pmap.write_binary(os);
    }
    {
        std::ofstream os("appleman_ascii." + pmap.file_extension());
        pmap.write_ascii(os);
    }

}
