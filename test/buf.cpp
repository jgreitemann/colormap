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

#include <array>
#include <fstream>

#include <colormap/map.hpp>
#include <colormap/itadpt/map_iterator_adapter.hpp>
#include <colormap/palettes.hpp>
#include <colormap/pixmap.hpp>


using namespace colormap;

int main () {
    // float buf[100];
    float buf[100];
    for (size_t i = 0; i < 100; ++i) {
        buf[i] = i;
    }

    // get a colormap and rescale it
    auto pal = palettes.at("inferno").rescale(0., 100.);
    // and use it to map the values to colors
    auto pix = itadpt::map(buf, pal);

    // Construct a PPM object from the `mapped` object `pix`. Color space is
    // inferred from color type of pix. "inferno" is an RGB palette, so `pmap`
    // will represent a PPM image. For a grayscale colormap, it would result in
    // a PGM image.
    pixmap<decltype(pix.begin())> pmap(pix.begin(), std::make_pair(10, 10));

    std::ofstream os("buf." + pmap.file_extension(),
                     std::ios_base::binary);
    pmap.write_binary(os);

}
