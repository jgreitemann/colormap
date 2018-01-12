#pragma once

#include <iterator>
#include <memory>
#include <type_traits>
#include <utility>


namespace itadpt {

    template <typename BaseIterator, typename Functor,
              typename = typename std::enable_if<std::is_base_of<std::input_iterator_tag, typename std::iterator_traits<BaseIterator>::iterator_category>::value>::type>
    struct map_iterator_adapter {
    private:
        BaseIterator base;
        Functor & functor;
    public:
        typedef typename std::iterator_traits<BaseIterator>::value_type domain_type;
        typedef decltype(functor(domain_type())) value_type;
        typedef typename std::iterator_traits<BaseIterator>::difference_type difference_type;
        typedef value_type reference;
        typedef std::unique_ptr<value_type> pointer;
        typedef typename std::iterator_traits<BaseIterator>::iterator_category iterator_category;

        map_iterator_adapter (BaseIterator it, Functor & f)
            : base(it), functor(f) {}

        reference operator* () const {
            return functor(**((BaseIterator *)this));
        }

        pointer operator-> () const {
            return pointer(new value_type(functor(**((BaseIterator *)this))));
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
