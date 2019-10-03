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

#include <iterator>
#include <memory>
#include <type_traits>
#include <utility>


namespace colormap {
namespace itadpt {

    template <typename BaseIterator, typename Functor,
              typename = typename std::enable_if<std::is_base_of<std::input_iterator_tag, typename std::iterator_traits<BaseIterator>::iterator_category>::value>::type>
    struct map_iterator_adapter {
    private:
        BaseIterator base;
        Functor * functor_ptr; // replace by
                               // std::optional<std::reference_wrapper<Functor>
                               // once C++17 can be required

    public:
        typedef typename std::iterator_traits<BaseIterator>::value_type domain_type;
        typedef std::result_of_t<Functor(domain_type)> value_type; // replace by
                                                                   // std::invoke_result
                                                                   // in C++17
        typedef typename std::iterator_traits<BaseIterator>::difference_type difference_type;
        typedef value_type reference;
        typedef std::unique_ptr<value_type> pointer;
        typedef typename std::iterator_traits<BaseIterator>::iterator_category iterator_category;

        template <typename Tag = iterator_category,
                  typename = typename std::enable_if<std::is_base_of<std::forward_iterator_tag, Tag>::value>::type>
        map_iterator_adapter () : base(), functor_ptr(nullptr) {}

        map_iterator_adapter (BaseIterator it, Functor & f)
            : base(it), functor_ptr(&f) {}

        reference operator* () const {
            return (*functor_ptr)(*base);
        }

        pointer operator-> () const {
            return pointer(new value_type((*functor_ptr)(*base)));
        }

        friend bool operator== (map_iterator_adapter const& lhs, map_iterator_adapter const& rhs) {
            return lhs.base == rhs.base;
        }

        friend bool operator!= (map_iterator_adapter const& lhs, map_iterator_adapter const& rhs) {
            return lhs.base != rhs.base;
        }

        map_iterator_adapter & operator++ () {
            base++;
            return *this;
        }

        template <typename Tag = iterator_category,
                  typename = typename std::enable_if<std::is_base_of<std::forward_iterator_tag, Tag>::value>::type>
        map_iterator_adapter operator++ (int) {
            map_iterator_adapter old(*this);
            ++(*this);
            return old;
        }

        template <typename Tag = iterator_category,
                  typename = typename std::enable_if<std::is_base_of<std::bidirectional_iterator_tag, Tag>::value>::type>
        map_iterator_adapter & operator-- () {
            base--;
            return *this;
        }

        template <typename Tag = iterator_category,
                  typename = typename std::enable_if<std::is_base_of<std::bidirectional_iterator_tag, Tag>::value>::type>
        map_iterator_adapter operator-- (int) {
            map_iterator_adapter old(*this);
            --(*this);
            return old;
        }

        template <typename Tag = iterator_category,
                  typename = typename std::enable_if<std::is_base_of<std::random_access_iterator_tag, Tag>::value>::type>
        map_iterator_adapter & operator+= (difference_type i) {
            base += i;
            return *this;
        }

        template <typename Tag = iterator_category,
                  typename = typename std::enable_if<std::is_base_of<std::random_access_iterator_tag, Tag>::value>::type>
        map_iterator_adapter & operator-= (difference_type i) {
            return *this += -i;
        }

        template <typename Tag = iterator_category,
                  typename = typename std::enable_if<std::is_base_of<std::random_access_iterator_tag, Tag>::value>::type>
        friend map_iterator_adapter operator+ (map_iterator_adapter it, difference_type i) {
            return it += i;
        }

        template <typename Tag = iterator_category,
                  typename = typename std::enable_if<std::is_base_of<std::random_access_iterator_tag, Tag>::value>::type>
        friend map_iterator_adapter operator+ (difference_type i, map_iterator_adapter const& it) {
            return it + i;
        }

        template <typename Tag = iterator_category,
                  typename = typename std::enable_if<std::is_base_of<std::random_access_iterator_tag, Tag>::value>::type>
        friend map_iterator_adapter operator- (map_iterator_adapter const& it, difference_type i) {
            return it + (-i);
        }

        template <typename Tag = iterator_category,
                  typename = typename std::enable_if<std::is_base_of<std::random_access_iterator_tag, Tag>::value>::type>
        friend difference_type operator- (map_iterator_adapter const& lhs, map_iterator_adapter const& rhs) {
            return lhs.base - rhs.base;
        }

        template <typename Tag = iterator_category,
                  typename = typename std::enable_if<std::is_base_of<std::random_access_iterator_tag, Tag>::value>::type>
        reference operator[] (difference_type i) {
            return *(*this + i);
        }

        template <typename Tag = iterator_category,
                  typename = typename std::enable_if<std::is_base_of<std::random_access_iterator_tag, Tag>::value>::type>
        friend bool operator< (map_iterator_adapter const& lhs, map_iterator_adapter const& rhs) {
            return lhs.base < rhs.base;
        }

        template <typename Tag = iterator_category,
                  typename = typename std::enable_if<std::is_base_of<std::random_access_iterator_tag, Tag>::value>::type>
        friend bool operator> (map_iterator_adapter const& lhs, map_iterator_adapter const& rhs) {
            return lhs.base > rhs.base;
        }

        template <typename Tag = iterator_category,
                  typename = typename std::enable_if<std::is_base_of<std::random_access_iterator_tag, Tag>::value>::type>
        friend bool operator<= (map_iterator_adapter const& lhs, map_iterator_adapter const& rhs) {
            return lhs.base <= rhs.base;
        }

        template <typename Tag = iterator_category,
                  typename = typename std::enable_if<std::is_base_of<std::random_access_iterator_tag, Tag>::value>::type>
        friend bool operator>= (map_iterator_adapter const& lhs, map_iterator_adapter const& rhs) {
            return lhs.base >= rhs.base;
        }
    };


    template <typename BaseIterator, typename Functor>
    map_iterator_adapter<BaseIterator, Functor>
    map_iterator (BaseIterator it, Functor & f) {
        return map_iterator_adapter<BaseIterator, Functor>(it, f);
    }

    template <typename Container, typename Functor>
    struct mapped {
        typedef Container container_type;
        typedef Functor map_type;
        typedef map_iterator_adapter<decltype(std::begin(std::declval<Container const&>())), Functor> const_iterator;
        typedef typename const_iterator::value_type value_type;

        mapped (Container const& c, Functor & f)
            : container(c), functor(f) {}

        const_iterator begin () const {
            return map_iterator(std::begin(container), functor);
        }

        const_iterator end () const {
            return map_iterator(std::end(container), functor);
        }

        size_t size () const {
            return container.size();
        }
    private:
        Container const& container;
        Functor & functor;
    };

    template <typename Container, typename Functor>
    mapped<Container,Functor> map (Container const& c, Functor & f) {
        return mapped<Container, Functor>(c, f);
    }

}
}
