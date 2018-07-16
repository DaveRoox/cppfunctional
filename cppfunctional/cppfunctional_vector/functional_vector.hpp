/*
 * cppfunctional.hpp
 *
 *  Created on: 30 mag 2018
 *      Author: daviderusso
 */

#ifndef FUNCTIONAL_VECTOR_HPP_
#define FUNCTIONAL_VECTOR_HPP_

#include <vector>
#include <map>
#include <iostream>

namespace functional {

    template<typename T>
    class functional_vector final : public std::vector<T> {

        using std::vector<T>::vector;

    public:

        inline void add(const T &) noexcept;

        inline void add(T &&) noexcept;

        functional_vector operator[](const std::initializer_list<long> &) const;

        inline functional_vector operator[](std::initializer_list<long> &&) const;

        inline const T &operator[](long) const;

        template<typename Func>
        inline functional_vector filter(Func &&) const noexcept;

        template<typename Func>
        inline functional_vector<typename std::result_of<Func(const T &)>::type> map(Func &&) const noexcept;

        template<typename Func, typename AccType = typename std::result_of<Func(const T &)>::type>
        inline AccType reduce(AccType &&, Func &&) const noexcept;

        template<typename Func>
        inline void for_each(Func &&) const noexcept;

        template<typename Func>
        inline functional_vector maxBy(Func &&) const;

        template<typename Func>
        inline functional_vector minBy(Func &&) const;

        inline functional_vector max() const;

        inline functional_vector min() const;

        template<typename Func>
        inline std::map<typename std::result_of<Func(const T &)>::type, functional_vector<T>>
        groupBy(Func &&) const noexcept;

        inline const T &first() const;

        inline const T &last() const;

        template<typename Func>
        inline bool each_match(Func &&) const noexcept;

        template<typename Func>
        inline bool any_match(Func &&) const noexcept;

        template<typename Func>
        inline bool no_match(Func &&) const noexcept;

        inline functional_vector
        uniques() const noexcept; // todo: its complexity depends on contains() method. Currently O(n^2)

        inline bool
        contains(T &) const noexcept; // todo: bottleneck: O(n); O(1) through hashing; hashing for custom classes?

        inline bool contains(const T &) const noexcept; // todo: same

        inline bool contains(T &&) const noexcept; // todo: same

        inline functional_vector limit_to(unsigned long) const noexcept;

        template<typename Func>
        inline functional_vector sort(Func &&) const noexcept;

        inline functional_vector sort(bool descending = false) const noexcept;

        inline std::ostream &
        print(const std::string &prefix = "", const std::string &separator = " ", const std::string &postfix = "",
              std::ostream & = std::cout) const noexcept;

        template<typename Func>
        inline std::ostream &printBy(Func &&, const std::string &prefix = "", const std::string &separator = " ",
                                     const std::string &postfix = "", std::ostream & = std::cout) const noexcept;

    protected:

        inline unsigned long m_normalize_index(long) const noexcept;

        template<typename Func>
        functional_vector m_compare(Func &&, bool) const;

    private:

        template<typename O> friend
        class functional_vector;

    };

}


#include "functional_vector.cpp"

#endif /* FUNCTIONAL_VECTOR_HPP_ */
