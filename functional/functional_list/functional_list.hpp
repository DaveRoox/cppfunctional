/*
 * functional.hpp
 *
 *  Created on: 30 mag 2018
 *      Author: daviderusso
 */

#ifndef FUNCTIONAL_LIST_HPP_
#define FUNCTIONAL_LIST_HPP_

#include <vector>
#include <iostream>

namespace functional{

    template<typename T>
    class functional_list {

    public:

        inline functional_list() noexcept;
        inline functional_list(const functional_list &) noexcept;
        inline explicit functional_list(const std::vector<T> &) noexcept;
        inline explicit functional_list(std::vector<T> &&) noexcept;
        inline functional_list(functional_list &&) noexcept;
        inline functional_list(const std::initializer_list<T> &) noexcept;
        inline functional_list(std::initializer_list<T> &&) noexcept;

        template<typename ... Elems>
        inline explicit functional_list(const Elems & ...) noexcept;

        template<typename ... Elems>
        inline explicit functional_list(Elems && ...) noexcept;

        inline functional_list & operator=(const functional_list &) noexcept;
        inline functional_list & operator=(functional_list &&) noexcept;

        inline void add(const T &) noexcept;
        inline void add(T &&) noexcept;

        functional_list operator[](const std::initializer_list<long> &) const;
        inline functional_list operator[](std::initializer_list<long> &&) const;
        inline T & operator[](long) const;
        inline std::size_t count() const noexcept;

        template<typename Func>
        inline functional_list filter(Func &&) const noexcept;

        template<typename Func>
        inline functional_list<typename std::result_of<Func(const T &)>::type> map(Func &&) const noexcept;

        template<typename Func, typename AccType = typename std::result_of<Func(const T &)>::type>
        inline AccType reduce(AccType &&, Func &&) const noexcept;

        template<typename Func>
        inline void for_each(Func &&) const noexcept;

        template<typename Func>
        inline functional_list max(Func &&) const;

        template<typename Func>
        inline functional_list min(Func &&) const;

        inline functional_list max() const;

        inline functional_list min() const;

        inline const T & first() const;

        inline const T & last() const;

        template<typename Func>
        inline bool each_match(Func &&) const noexcept;

        template<typename Func>
        inline bool any_match(Func &&) const noexcept;

        template<typename Func>
        inline bool no_match(Func &&) const noexcept;

        inline functional_list uniques() const noexcept; // todo: its complexity depends on contains() method. Currently O(n^2)

        inline bool contains(T &) const noexcept; // todo: bottleneck: O(n); O(1) through hashing; hashing for custom classes?
        inline bool contains(const T &) const noexcept; // todo: same
        inline bool contains(T &&) const noexcept; // todo: same

        inline functional_list limit_to(unsigned long) const noexcept;

        template<typename Func>
        inline functional_list sort(Func &&) const noexcept;

        inline functional_list sort(bool descending = false) const noexcept;

        inline std::ostream& print(const std::string & = " ", std::ostream & = std::cout) const noexcept;

        inline std::vector<T> to_vector() const noexcept;

        inline auto begin() const noexcept -> decltype(std::declval<std::vector<T>>().begin());

        inline auto cbegin() const noexcept -> decltype(std::declval<std::vector<T>>().cbegin());

        inline auto rbegin() const noexcept -> decltype(std::declval<std::vector<T>>().rbegin());

        inline auto crbegin() const noexcept -> decltype(std::declval<std::vector<T>>().crbegin());

        inline auto end() const noexcept -> decltype(std::declval<std::vector<T>>().end());

        inline auto cend() const noexcept -> decltype(std::declval<std::vector<T>>().cend());

        inline auto rend() const noexcept -> decltype(std::declval<std::vector<T>>().rend());

        inline auto crend() const noexcept -> decltype(std::declval<std::vector<T>>().crend());

        inline virtual ~functional_list() noexcept;

    protected:

        inline unsigned long m_normalize_index(long) const noexcept;

        template<typename Func>
        functional_list m_compare(Func &&, bool) const;

    private:

        std::unique_ptr<std::vector<T>> m_v;

        template<typename O> friend class functional_list;

    };

}


#include "functional_list.cpp"

#endif /* FUNCTIONAL_LIST_HPP_ */
