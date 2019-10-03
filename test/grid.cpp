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

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include <doctest/doctest.h>

#include <colormap/grid.hpp>


using namespace colormap;

TEST_CASE("1d-grid") {
    double dx = 10./41;
    double x = 0.;
    for (double xp : grid<1> {42, {0, 10}}) {
        CHECK(xp == doctest::Approx(x));
        x += dx;
    }
}

TEST_CASE("2d-grid-row-major") {
    double dx = 10./3;
    double dy = 10./4;
    double x = 0.;
    double y = 0.;
    for (auto p : grid<2> {{4, {0, 10}}, {5, {0, 10}}}) {
        CHECK(p[0] == doctest::Approx(x));
        CHECK(p[1] == doctest::Approx(y));
        y += dy;
        if (y > doctest::Approx(10.)) {
            y = 0.;
            x += dx;
        }
    }
}

TEST_CASE("2d-grid-col-major") {
    double dx = 10./3;
    double dy = 10./4;
    double x = 0.;
    double y = 0.;
    for (auto p : grid<2, major_order::col> {{4, {0, 10}}, {5, {0, 10}}}) {
        CHECK(p[0] == doctest::Approx(x));
        CHECK(p[1] == doctest::Approx(y));
        x += dx;
        if (x > doctest::Approx(10.)) {
            x = 0.;
            y += dy;
        }
    }
}

TEST_CASE("ordering-row-major") {
    grid<2> g {{4, {0, 10}}, {5, {0, 10}}};
    for (auto bigger = g.begin(); bigger != g.end(); ++bigger) {
        for (auto smaller = g.begin(); smaller != bigger; ++smaller) {
            CHECK(smaller < bigger);
            CHECK(smaller <= bigger);
            CHECK(bigger > smaller);
            CHECK(bigger >= smaller);
        }
    }
}

TEST_CASE("ordering-col-major") {
    grid<2, major_order::col> g {{4, {0, 10}}, {5, {0, 10}}};
    for (auto bigger = g.begin(); bigger != g.end(); ++bigger) {
        for (auto smaller = g.begin(); smaller != bigger; ++smaller) {
            CHECK(smaller < bigger);
            CHECK(smaller <= bigger);
            CHECK(bigger > smaller);
            CHECK(bigger >= smaller);
        }
    }
}
