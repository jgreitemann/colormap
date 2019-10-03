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

#include <algorithm>
#include <array>
#include <functional>
#include <initializer_list>
#include <iterator>
#include <memory>
#include <stdexcept>
#include <type_traits>


namespace colormap {

enum class major_order {
    row,
    col
};

template <size_t dim, major_order order = major_order::row, typename T = double,
          typename = typename std::enable_if<std::is_floating_point<T>::value>::type>
struct grid {
    using base_grid = grid<1, major_order::row, T>;
    using base_iterator = typename base_grid::const_iterator;
    using range_t = typename base_grid::range_t;

    struct const_iterator {
        typedef std::array<T, dim> value_type;
        typedef long difference_type;
        typedef value_type reference;
        typedef std::unique_ptr<value_type> pointer;
        typedef std::bidirectional_iterator_tag iterator_category;
        const_iterator & operator++ () {
            if (order == major_order::row) {
                auto itit = its.rbegin();
                auto lastit = its.rend();
                --lastit;
                ++(*itit);
                while (itit->is_end() && itit != lastit) {
                    itit->reset();
                    ++itit;
                    ++(*itit);
                }
            } else {
                auto itit = its.begin();
                auto lastit = its.end();
                --lastit;
                ++(*itit);
                while (itit->is_end() && itit != lastit) {
                    itit->reset();
                    ++itit;
                    ++(*itit);
                }
            }
            return *this;
        }
        const_iterator & operator-- () {
            if (order == major_order::row) {
                auto itit = its.rbegin();
                auto lastit = its.rend();
                --lastit;
                --(*itit);
                while (itit->is_begin() && itit != lastit) {
                    itit->set_to_end();
                    ++itit;
                    --(*itit);
                }
            } else {
                auto itit = its.begin();
                auto lastit = its.end();
                --lastit;
                --(*itit);
                while (itit->is_begin() && itit != lastit) {
                    itit->set_to_end();
                    ++itit;
                    --(*itit);
                }
            }
            return *this;
        }
        const_iterator operator++ (int) {
            const_iterator old(*this);
            ++(*this);
            return old;
        }
        const_iterator operator-- (int) {
            const_iterator old(*this);
            --(*this);
            return old;
        }
        template <size_t d, typename = typename std::enable_if<(d < dim)>::type>
        const_iterator & move_forward () {
            if (order == major_order::row)
                its.rbegin()[d].move_forward();
            else
                its.begin()[d].move_forward();
            return *this;
        }
        template <size_t d, typename = typename std::enable_if<(d < dim)>::type>
        const_iterator & move_backward () {
            if (order == major_order::row)
                its.rbegin()[d].move_backward();
            else
                its.begin()[d].move_backward();
            return *this;
        }
        reference operator* () const {
            value_type res;
            std::transform(its.begin(), its.end(), res.begin(),
                           [] (auto it) { return *it; });
            return res;
        }
        pointer operator-> () const {
            return pointer(new value_type(**this));
        }
        friend bool operator!= (const_iterator const& lhs, const_iterator const& rhs) {
            for (size_t i = 0; i < dim; ++i)
                if (lhs.its[i] != rhs.its[i])
                    return true;
            return false;
        }
        friend bool operator== (const_iterator const& lhs, const_iterator const& rhs) {
            return !(lhs != rhs);
        }
        friend bool operator< (const_iterator const& lhs, const_iterator const& rhs) {
            if (order == major_order::row) {
                return std::lexicographical_compare(lhs.its.begin(), lhs.its.end(),
                                                    rhs.its.begin(), rhs.its.end());
            } else {
                return std::lexicographical_compare(lhs.its.rbegin(), lhs.its.rend(),
                                                    rhs.its.rbegin(), rhs.its.rend());
            }
        }
        friend bool operator> (const_iterator const& lhs, const_iterator const& rhs) {
            return !(lhs < rhs || lhs == rhs);
        }
        friend bool operator<= (const_iterator const& lhs, const_iterator const& rhs) {
            return !(lhs > rhs);
        }
        friend bool operator>= (const_iterator const& lhs, const_iterator const& rhs) {
            return !(lhs < rhs);
        }
        bool in_bulk () const {
            return std::all_of(its.begin(), its.end(),
                               std::mem_fn(&base_iterator::in_bulk));
        }
        friend grid;
    private:
        const_iterator () : its{} {};
        std::array<base_iterator, dim> its;
    };

    using grid_point_type = typename const_iterator::value_type;

    const_iterator begin () const {
        return begin_;
    }

    const_iterator end () const {
        const_iterator cp(begin_);
        if (order == major_order::row) {
            auto lastit = cp.its.rend();
            --lastit;
            lastit->set_to_end();
        } else {
            auto lastit = cp.its.end();
            --lastit;
            lastit->set_to_end();
        }
        return cp;
    }

    size_t size () const {
        size_t prod = 1;
        for (auto const& it : begin_.its)
            prod *= it.size();
        return prod;
    }

    std::array<size_t, dim> shape () const {
        std::array<size_t, dim> s;
        std::transform(begin_.its.begin(), begin_.its.end(), s.begin(),
                       [] (auto const& it) { return it.size(); });
        return s;
    }

    std::array<range_t, dim> ranges () const {
        std::array<range_t, dim> s;
        std::transform(begin_.its.begin(), begin_.its.end(), s.begin(),
                       [] (auto const& it) { return it.range(); });
        return s;
    }

    grid (std::initializer_list<base_grid> il) : begin_{} {
        if (il.size() != dim)
            throw std::runtime_error("number of grids does not match dimension");
        auto it = begin_.its.begin();
        for (base_grid const& g : il) {
            *it = g.begin();
            ++it;
        }
    }

private:
    const_iterator begin_;
};

template <major_order order, typename T>
struct grid<1, order, T> {
    using range_t = std::pair<T, T>;
    grid (size_t N, range_t range)
        : N(N), begin_(0, range, N) {
        if (N < 2)
            throw std::runtime_error("grid needs at least 2 points");
    }

    struct const_iterator {
        typedef T value_type;
        typedef long difference_type;
        typedef T const& reference;
        typedef T const* pointer;
        typedef std::random_access_iterator_tag iterator_category;
        const_iterator & operator++ () {
            ++i;
            x += dx;
            return *this;
        }
        const_iterator & operator-- () {
            --i;
            x -= dx;
            return *this;
        }
        const_iterator operator++ (int) {
            const_iterator old(*this);
            ++(*this);
            return old;
        }
        const_iterator operator-- (int) {
            const_iterator old(*this);
            --(*this);
            return old;
        }
        template <size_t d = 0, typename = typename std::enable_if<(d == 0)>::type>
        const_iterator & move_forward () {
            ++(*this);
            if (is_end())
                reset();
            return *this;
        }
        template <size_t d = 0, typename = typename std::enable_if<(d == 0)>::type>
        const_iterator & move_backward () {
            if (is_begin())
                return set_to_end();
            return --(*this);
        }
        const_iterator & operator+= (difference_type j) {
            i += j;
            x += j * dx;
            return *this;
        }
        const_iterator & operator-= (difference_type j) {
            i -= j;
            x -= j * dx;
            return *this;
        }
        bool is_begin () const {
            return i == 0;
        }
        bool is_end () const {
            return i == N;
        }
        bool in_bulk () const {
            return i > 0 && i < N-1;
        }
        const_iterator & reset () {
            return *this -= i;
        }
        const_iterator & set_to_end () {
            return *this += (N - i);
        }
        size_t size () const {
            return N;
        }
        range_t range () const {
            return {x - i * dx, x + (N - 1 - i) * dx};
        }
        reference operator* () const {
            return x;
        }
        pointer operator-> () const {
            return &x;
        }
        friend const_iterator operator+ (const_iterator const& it, difference_type j) {
            const_iterator cp(it);
            return cp += j;
        }
        friend const_iterator operator- (const_iterator const& it, difference_type j) {
            return it + (-j);
        }
        friend difference_type operator- (const_iterator const& lhs, const_iterator const& rhs) {
            return lhs.i - rhs.i;
        }
        friend bool operator== (const_iterator const& lhs, const_iterator const& rhs) {
            return lhs.i == rhs.i;
        }
        friend bool operator!= (const_iterator const& lhs, const_iterator const& rhs) {
            return !(lhs == rhs);
        }
        friend bool operator< (const_iterator const& lhs, const_iterator const& rhs) {
            return lhs.i < rhs.i;
        }
        friend bool operator> (const_iterator const& lhs, const_iterator const& rhs) {
            return lhs.i > rhs.i;
        }
        friend bool operator<= (const_iterator const& lhs, const_iterator const& rhs) {
            return !(lhs > rhs);
        }
        friend bool operator>= (const_iterator const& lhs, const_iterator const& rhs) {
            return !(lhs < rhs);
        }
        template <size_t, major_order, typename, typename>
        friend struct grid;
    private:
        const_iterator () = default;
        const_iterator (difference_type i, range_t range, size_t N)
            : dx((range.second - range.first) / (N-1)), i(i), N(N), x(range.first + i * dx) {}
        T dx;
        difference_type i;
        size_t N;
        T x;
    };

    const_iterator begin () const {
        return begin_;
    }

    const_iterator end () const {
        return begin_ + N;
    }

    T front () const {
        return *begin_;
    }

    T back () const {
        return *(begin_ + (N-1));
    }

    size_t size () const {
        return N;
    }

    range_t range () const {
        return { *front(), *back() };
    }

private:
    size_t N;
    const_iterator begin_;
};

}
