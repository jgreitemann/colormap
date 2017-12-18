#include "colormap.hpp"

#include <cmath>
#include <complex>
#include <fstream>
#include <iostream>
#include <utility>


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

    std::string header = [&] {
        std::stringstream ss;
        ss << "P6\n"
        << dim.first << ' ' << dim.second << '\n'
        << size_t(decltype(pal)::color_type::depth())
        << '\n';
        return ss.str();
    } ();
    std::ofstream os("appleman.ppm");
    os.write(header.c_str(), header.size());

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
                pal(func(it + 1 - nu)).write(os);
            } else {
                black.write(os);
            }
        }
    }

}
