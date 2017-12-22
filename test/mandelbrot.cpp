#include "colormap.hpp"

#include <cmath>
#include <complex>
#include <fstream>
#include <iostream>
#include <utility>

#include <boost/multi_array.hpp>


int main () {
    size_t max_it = 1000;
    double bail_out = std::pow(2, 16);
    auto dim = std::make_pair(701, 401);
    auto canvas = std::make_pair(std::make_pair(-2.5,1.), std::make_pair(-1.,1.));
    auto delta = std::make_pair((canvas.first.second - canvas.first.first)/(dim.first-1),
                                (canvas.second.second - canvas.second.first)/(dim.second-1));
    auto func = [] (double x) { return pow(x, 0.1); };
    auto pal = color::palettes.at("inferno").rescale(1, func(max_it));
    auto black = pal(0.);

    using color_t = typename decltype(pal)::color_type;
    boost::multi_array<color_t,2> pixs(boost::extents[dim.second][dim.first]);
    for (size_t j = 0; j < dim.second; ++j) {
        for (size_t i = 0; i < dim.first; ++i) {
            std::complex<double> c = {canvas.first.first + i * delta.first,
                                      canvas.second.first + j * delta.second};
            std::complex<double> z = 0.;
            size_t it;
            for (it = 0; it < max_it && std::norm(z) < bail_out; ++it)
                z = z * z + c;
            if (it < max_it) {
                double log2_z = log(std::norm(z)) * 0.5 / log(2);
                double nu = log(log2_z) / log(2);
                pixs[j][i] = pal(func(it + 1 - nu));
            } else {
                pixs[j][i] = black;
            }
        }
    }

    boost::array<typename decltype(pixs)::index,2> dims = {{1, dim.first * dim.second}};
    pixs.reshape(dims);
    auto flat_it = pixs.begin()->begin();
    color::pixmap<decltype(flat_it)> pmap(flat_it, dim);

    {
        std::ofstream os("appleman_binary." + pmap.file_extension(), std::ios_base::binary);
        pmap.write_binary(os);
    }
    {
        std::ofstream os("appleman_ascii." + pmap.file_extension());
        pmap.write_ascii(os);
    }

}
