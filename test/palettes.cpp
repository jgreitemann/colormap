#include "colormap.hpp"
#include "grid.hpp"

#include <fstream>
#include <iostream>
#include <string>
#include <vector>


int main () {
    size_t M = 25;
    std::vector<std::string> names;
    std::vector<decltype(color::palettes)::mapped_type> pals;
    for (auto pair : color::palettes) {
        names.push_back(pair.first);
        pals.push_back(pair.second);
        std::cout << names.back() << std::endl;
    }

    grid<2, major_order::col> g {
        {400, {0., 1.}},
        {M * names.size(), {0., names.size() - 1e-6}}
    };

    auto lamb = [&] (auto coord) {
        size_t i = size_t(coord[1]);
        return pals[i](coord[0]);
    };
    auto pix = itadpt::map(g, lamb);

    color::pixmap<decltype(pix.begin())> pmap(pix.begin(), g.shape());
    std::ofstream os("palettes." + pmap.file_extension());
    pmap.write_binary(os);
}
