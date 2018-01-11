#pragma once

#include <memory>
#include <utility>


namespace itadpt {

    template <typename BaseIterator, typename Functor>
    struct map_iterator_adapter : public BaseIterator {
    private:
        Functor & functor;
    public:
        typedef typename BaseIterator::value_type domain_type;
        typedef decltype(functor(domain_type())) value_type;

        map_iterator_adapter (BaseIterator it, Functor & f)
            : BaseIterator(it), functor(f) {}

        value_type operator* () const {
            return functor(**((BaseIterator *)this));
        }

        std::unique_ptr<value_type> const operator-> () const {
            return std::unique_ptr<value_type>(new value_type(functor(**((BaseIterator *)this))));
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
        typedef map_iterator_adapter<typename Container::const_iterator, Functor> const_iterator;
        typedef typename const_iterator::value_type value_type;

        mapped (Container const& c, Functor & f)
            : container(c), functor(f) {}

        const_iterator begin () const {
            return map_iterator(container.begin(), functor);
        }

        const_iterator end () const {
            return map_iterator(container.end(), functor);
        }

        size_t size () const {
            container.size();
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
