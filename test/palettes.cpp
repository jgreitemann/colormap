#include "colormap.hpp"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>


int main () {
    size_t N = 400;
    size_t M = 25;

    std::vector<double> vals(N);
    for (size_t i = 0; i < N; ++i)
        vals[i] = 1. * i / (N-1);

    std::string header = [&] {
        std::stringstream ss;
        ss << "P6\n"
           << N << ' ' << M * color::palettes.size() << '\n'
           << size_t(decltype(color::palettes)::mapped_type::color_type::depth())
           << '\n';
        return ss.str();
    } ();
    std::ofstream os("palettes.ppm");
    os.write(header.c_str(), header.size());

    for (auto & [name, pal] : color::palettes) {
        std::cout << name << std::endl;
        for (size_t j = 0; j < M; ++j) {
            for (auto pix : color::map_color(vals, pal)) {
                pix.write(os);
            }
        }
    }
}
