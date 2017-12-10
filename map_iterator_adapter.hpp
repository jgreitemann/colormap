#pragma once

#include "map.hpp"

#include <memory>


namespace color {

    template <typename BaseIterator, typename Color>
    struct map_iterator_adapter : public BaseIterator {
        typedef Color value_type;
        typedef map<Color> map_type;

        map_iterator_adapter (BaseIterator it, map_type const& pal)
            : BaseIterator(it), palette(pal) {}

        Color operator* () const {
            return palette(**((BaseIterator *)this));
        }

        std::unique_ptr<Color> const operator-> () const {
            return std::unique_ptr<Color>(new Color(palette(**((BaseIterator *)this))));
        }

    private:
        map_type const& palette;
    };

    template <typename BaseIterator, typename Color>
    map_iterator_adapter<BaseIterator,Color>
    adapt_iterator (BaseIterator it, map<Color> const& pal) {
        return map_iterator_adapter<BaseIterator,Color>(it, pal);
    }

    template <typename Container, typename Palette>
    struct color_mapped {
        typedef Palette map_type;
        typedef typename Palette::color_type value_type;
        typedef Container container_type;
        typedef map_iterator_adapter<typename Container::const_iterator, typename Palette::color_type> const_iterator;

        color_mapped (Container const& c, Palette const& pal)
            : container(c), palette(pal) {}

        const_iterator begin () const {
            return adapt_iterator(container.begin(), palette);
        }

        const_iterator end () const {
            return adapt_iterator(container.end(), palette);
        }

    private:
        Container const& container;
        map_type const& palette;
    };

    template <typename Container, typename Palette>
    color_mapped<Container,Palette> map_color (Container const& c, Palette const& pal) {
        return color_mapped<Container,Palette>(c, pal);
    }

}
