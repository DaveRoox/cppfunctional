/*
 * functional.cpp
 *
 *  Created on: 30 mag 2018
 *      Author: daviderusso
 */
#ifndef FUNCTIONAL_VECTOR_CPP_
#define FUNCTIONAL_VECTOR_CPP_

#include "functional_vector.hpp"
#include "../functional_exceptions/functional_exceptions.hpp"

#include <algorithm>

using namespace functional;

template<typename T>
const T &functional_vector<T>::operator[](long index) const {
    std::size_t size = this->size();
    if (size == 0)
        throw empty_list_exception();
    if (index >= (long) size)
        throw index_out_of_range_exception();
    index = m_normalize_index(index);
    return this->std::vector<T>::operator[](index);
}

template<typename T>
void functional_vector<T>::add(const T &elem) noexcept {
    this->push_back(elem);
}

template<typename T>
void functional_vector<T>::add(T &&elem) noexcept {
    this->push_back(std::move(elem));
}

template<typename T>
functional_vector<T> functional_vector<T>::operator[](const std::initializer_list<long> &range) const {

    std::vector<long> values{range};

    long input_size = values.size();

    if (input_size < 2 or input_size > 3)
        throw wrong_number_of_parameters_exception();

    long v_size = this->size();

    long start = values[0];
    long normalized_start = m_normalize_index(start);
    if (normalized_start >= v_size)
        throw exceeded_list_size_exception(normalized_start, v_size);

    long end = values[1];
    long normalized_end = m_normalize_index(end);
    if (normalized_end >= v_size)
        throw exceeded_list_size_exception(normalized_end, v_size);

    long step = 1;
    if (input_size == 3)
        step = values[2];
    if (step == 0) {
        std::cerr << values[2] << std::endl;
        throw non_zero_step_exception();
    }

    functional_vector<T> ranged_list;

    if (step > 0) {

        if (normalized_end >= normalized_start)                // [ ... start >>> ... >>> end ... ]
            for (long i = normalized_start; i <= normalized_end; i += step)
                ranged_list.add(this->std::vector<T>::operator[](i));
        else {                                                // [ ... >>> end ... start >>> ... ]
            long i, ssize = v_size;
            for (i = normalized_start; i < ssize; i += step)    // [ ... start >>> ... (size - 1) ]
                ranged_list.add(this->std::vector<T>::operator[](i));
            i = m_normalize_index(i) % ssize;
            for (; i <= normalized_end; i += step)            // [ ... c >>> ... >>> end ... ]
                ranged_list.add(this->std::vector<T>::operator[](i));
        }

    } else { // step < 0
        if (normalized_end <= normalized_start)                // [ ... end <<< ... <<< start ... ]
            for (long i = normalized_start; i >= normalized_end; i += step)
                ranged_list.add(this->std::vector<T>::operator[](i));
        else {                                                // [ ... <<< start ... end <<< ... ]
            long i;
            for (i = normalized_start; i >= 0; i += step)    // [ 0 ... <<< start ... ]
                ranged_list.add(this->std::vector<T>::operator[](i));
            i = m_normalize_index(i);
            for (; i >= normalized_end; i += step)            // [ ... end <<< ... <<< (size - c)... ]
                ranged_list.add(this->std::vector<T>::operator[](i));
        }

    }

    return ranged_list;
}

template<typename T>
functional_vector<T> functional_vector<T>::operator[](std::initializer_list<long> &&range) const {
    return operator[](range);
}

template<typename T>
std::size_t functional_vector<T>::m_normalize_index(long index) const noexcept {
    std::size_t size = this->size();
    while (index < 0)
        index += size;
    return static_cast<std::size_t>(index);
}

template<typename T>
std::ostream &
functional_vector<T>::print(const std::string &prefix, const std::string &separator, const std::string &postfix,
                            std::ostream &out) const noexcept {
    out << prefix;
    for (std::size_t i = 0, size = this->size() - 1; i < size; i++)
        out << this->std::vector<T>::operator[](i) << separator;
    if (this->size() > 0)
        out << this->std::vector<T>::operator[](this->size() - 1);
    out << postfix;
    return out;
}

template<typename T>
template<typename Func>
std::ostream &functional_vector<T>::printBy(Func &&printer, const std::string &prefix, const std::string &separator,
                                            const std::string &postfix, std::ostream &out) const noexcept {
    out << prefix;
    for (std::size_t i = 0, size = this->size() - 1; i < size; i++)
        out << printer(this->std::vector<T>::operator[](i)) << separator;
    if (this->size() > 0)
        out << printer(this->std::vector<T>::operator[](this->size() - 1));
    out << postfix;
    return out;
}

template<typename T>
template<typename Func>
functional_vector<T> functional_vector<T>::filter(Func &&test) const noexcept {
    functional_vector<T> fl;
    for (const T &x : *this)
        if (test(x))
            fl.push_back(x);
    return fl;
}

template<typename T>
template<typename Func, typename AccType>
AccType functional_vector<T>::reduce(AccType &&accumulator, Func &&reducer) const noexcept {
    for (const T &x : *this)
        accumulator = reducer(accumulator, x);
    return accumulator;
}

template<typename T>
template<typename Func>
functional_vector<typename std::result_of<Func(const T &)>::type>
functional_vector<T>::map(Func &&mapper) const noexcept {
    functional_vector<typename std::result_of<Func(const T &)>::type> fl;
    for (const T &x : *this)
        fl.push_back(mapper(x));
    return fl;
}

template<typename T>
template<typename Func>
std::map<typename std::result_of<Func(const T &)>::type, functional_vector<T>>
functional_vector<T>::groupBy(Func &&key) const noexcept {
    std::map<typename std::result_of<Func(const T &)>::type, functional_vector<T>> fmap;
    for (const T &x : *this)
        fmap[key(x)].add(x);
    return fmap;
}

template<typename T>
template<typename Func>
void functional_vector<T>::for_each(Func &&f) const noexcept {
    for (const T &x : *this)
        f(x);
}

template<typename T>
template<typename Func>
functional_vector<T> functional_vector<T>::maxBy(Func &&key) const {
    return m_compare(key, true);
}

template<typename T>
template<typename Func>
functional_vector<T> functional_vector<T>::minBy(Func &&key) const {
    return m_compare(key, false);
}

template<typename T>
functional_vector<T> functional_vector<T>::max() const {
    return m_compare([](const T &arg) -> const T & { return arg; }, true);
}

template<typename T>
functional_vector<T> functional_vector<T>::min() const {
    return m_compare([](const T &arg) -> const T & { return arg; }, false);
}

template<typename T>
template<typename Func>
functional_vector<T> functional_vector<T>::m_compare(Func &&key, bool greater) const {

    std::size_t size = this->size();

    if (size == 0)
        throw empty_list_exception();

    const T *_value = &(this->std::vector<T>::operator[](0));
    functional_vector<T> results{*_value};

    for (std::size_t i = 1; i < size; i++) {
        auto first = key(this->std::vector<T>::operator[](i));
        auto second = key(*_value);
        if ((greater and first > second) or (!greater and first < second)) {
            _value = &(this->std::vector<T>::operator[](i));
            results.clear();
            results.push_back(*_value);
        } else if (first == second)
            results.add(*_value);
    }

    return results;
}

template<typename T>
template<typename Func>
bool functional_vector<T>::each_match(Func &&test) const noexcept {
    for (const T &x : *this)
        if (!test(x))
            return false;
    return true;
}

template<typename T>
template<typename Func>
bool functional_vector<T>::any_match(Func &&test) const noexcept {
    for (const T &x : *this)
        if (test(x))
            return true;
    return false;
}

template<typename T>
template<typename Func>
bool functional_vector<T>::no_match(Func &&test) const noexcept {
    for (const T &x : *this)
        if (test(x))
            return false;
    return true;
}

template<typename T>
functional_vector<T> functional_vector<T>::uniques() const noexcept {
    functional_vector<T> fl;
    for (const T &x : *this)
        if (!fl.contains(x))
            fl.add(x);
    return fl;
}

template<typename T>
bool functional_vector<T>::contains(const T &t) const noexcept {
    for (const T &x : *this)
        if (x == t)
            return true;
    return false;
}

template<typename T>
bool functional_vector<T>::contains(T &t) const noexcept {
    for (T &x : *this)
        if (x == t)
            return true;
    return false;
}

template<typename T>
bool functional_vector<T>::contains(T &&t) const noexcept {
    return contains(t);
}

template<typename T>
inline std::ostream &operator<<(std::ostream &out, const functional_vector<T> &t_func_list) noexcept {
    return t_func_list.print(" ", out);
}

template<typename T>
functional_vector<T> functional_vector<T>::limit_to(unsigned long max_elements) const noexcept {
    long max_index = max_elements - 1, size = this->size() - 1;
    // todo: this->v->size() is not signed, but unsigned.
    // This means that the actual size of the vector could be bigger than the maximum number representable on a long (signed)
    return operator[]({0, max_index <= size ? max_index : size});
}

template<typename T>
template<typename Func>
functional_vector<T> functional_vector<T>::sort(Func &&func) const noexcept {
    functional_vector<T> fl{*this};
    std::sort(fl.begin(), fl.end(), func);
    return fl;
}

template<typename T>
functional_vector<T> functional_vector<T>::sort(bool descending) const noexcept {
    functional_vector<T> fl{*this};
    std::sort(fl.begin(), fl.end(), [descending](const T &t1, const T &t2) { return descending ? t1 > t2 : t2 > t1; });
    return fl;
}

template<typename T>
const T &functional_vector<T>::first() const {
    if (this->empty())
        throw empty_list_exception();
    return this->at(0);
}

template<typename T>
const T &functional_vector<T>::last() const {
    if (this->empty())
        throw empty_list_exception();
    return this->at(this->size() - 1);
}

template<typename T>
std::vector<T> functional_vector<T>::to_vector() const noexcept {
    return *this->m_v;
}

#endif
