#include "grid.hpp"

#include <iostream>
#include <iomanip>


template <typename T, size_t N>
std::ostream & operator<< (std::ostream & os, std::array<T,N> const & arr) {
    os << '{';
    for (T const& x : arr)
        os << std::setprecision(3) << x << ",\t";
    os << '}';
    return os;
}

template <size_t dim, major_order order, typename T>
std::ostream & operator<< (std::ostream & os, grid<dim, order, T> const & g) {
    for (auto x : g)
        os << x << '\n';
    os << "----------------";
    return os;
}

int main () {
    std::cout << grid<1> {11, {0, 10}} << std::endl;
    std::cout << grid<2> {{4, {0, 10}}, {5, {0, 4}}} << std::endl;
    std::cout << grid<2, major_order::col> {{4, {0, 10}}, {5, {0, 4}}} << std::endl;
}
