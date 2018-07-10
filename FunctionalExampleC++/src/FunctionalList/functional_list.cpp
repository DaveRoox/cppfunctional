/*
 * functional_list.cpp
 *
 *  Created on: 30 mag 2018
 *      Author: daviderusso
 */
#ifndef FUNCTIONAL_LIST_CPP_
#define FUNCTIONAL_LIST_CPP_

#include "functional_list.hpp"

#include <vector>
#include <algorithm>

using namespace functional;
using namespace std;

template<typename T>
functional_list<T>::functional_list() noexcept {
	this->m_v = std::unique_ptr<vector<T>>(new vector<T>());
}

template<typename T>
functional_list<T>::functional_list(const functional_list & t_other) noexcept {
	this->m_v = nullptr;
	*this = t_other;
}

template<typename T>
functional_list<T>::functional_list(functional_list && t_other) noexcept {
	this->m_v = nullptr;
	*this = move(t_other);
}

template<typename T>
functional_list<T>::functional_list(const vector<T> & t_vec) noexcept {
	this->m_v = std::unique_ptr<vector<T>>(new vector<T>(t_vec));
}

template<typename T>
functional_list<T>::functional_list(vector<T> && t_vec) noexcept {
	this->m_v = std::unique_ptr<vector<T>>(new vector<T>(move(t_vec)));
}

template<typename T>
functional_list<T>::functional_list(const std::initializer_list<T> & t_init_list) noexcept {
	this->m_v = std::unique_ptr<vector<T>>(new vector<T>(t_init_list));
}

template<typename T>
functional_list<T>::functional_list(std::initializer_list<T> && t_init_list) noexcept {
	this->m_v = std::unique_ptr<vector<T>>(new vector<T>(move(t_init_list)));
}

template<typename T>
template<typename ... Elems>
functional_list<T>::functional_list(const Elems & ... elems) noexcept {
	this->m_v = std::unique_ptr<vector<T>>(new vector<T>());
	for(const T & elem : {elems...})
		this->m_v->push_back(elem);
}

template<typename T>
template<typename ... Elems>
functional_list<T>::functional_list(Elems && ... elems) noexcept {
	this->m_v = std::unique_ptr<vector<T>>(new vector<T>());
	for(const T & elem : {elems...})
		this->m_v->push_back(move(elem));
}

template<typename T>
functional_list<T>::~functional_list() noexcept {
	this->m_v.reset();
}

template<typename T>
functional_list<T> & functional_list<T>::operator=(const functional_list<T> & t_other) noexcept {
	if(this != &t_other) {
		this->m_v.reset();
		this->m_v = std::unique_ptr<vector<T>>(new vector<T>(*t_other.m_v));
	}
	return *this;
}

template<typename T>
functional_list<T> & functional_list<T>::operator=(functional_list<T> && t_other) noexcept {
	if(this != &t_other) {
		this->m_v.reset();
		this->m_v = std::unique_ptr<vector<T>>(new vector<T>(move(*t_other.m_v)));
	}
	return *this;
}

template<typename T>
std::size_t functional_list<T>::count() const noexcept {
	return this->m_v->size();
}

template<typename T>
T & functional_list<T>::operator[](long index) {
	std::size_t size = this->m_v->size();
	if(size == 0)
		throw empty_list_exception();
	if(index >= (long)size)
		throw index_out_of_range_exception();
	index = m_normalize_index(index);
	return (*this->m_v)[index];
}

template<typename T>
void functional_list<T>::add(const T & elem) noexcept {
	this->m_v->push_back(elem);
}

template<typename T>
void functional_list<T>::add(T && elem) noexcept {
	this->m_v->push_back(move(elem));
}

template<typename T>
functional_list<T> functional_list<T>::operator[](const std::initializer_list<long> & range) const {

	vector<long> values{range};

	long input_size = values.size();

	if(input_size < 1 or input_size > 3)
		throw wrong_number_of_parameters_exception();

	long v_size = this->m_v->size();

	long start = values[0];
	long normalized_start = m_normalize_index(start);

	long end = v_size - 1;
	long normalized_end = end;
	if(input_size >= 2) {
		end = values[1];
		normalized_end = m_normalize_index(end);
	}
	if(end >= v_size)
		throw exceeded_list_size_exception();

	long step = 1;
	if(input_size == 3) {
		step = values[2];
		if(step < 0)
			step = -((-step) % v_size);
		else
			step %= v_size;
	}
	if(step == 0) {
		cerr << values[2] << endl;
		throw non_zero_step_exception();
	}

	functional_list<T> ranged_list;

	if(step > 0) {

		if(normalized_end >= normalized_start)				// [ ... start >>> ... >>> end ... ]
			for(long i = normalized_start; i <= normalized_end; i += step)
				ranged_list.add((*this->m_v)[i]);
		else {												// [ ... >>> end ... start >>> ... ]
			long i, ssize = v_size;
			for(i = normalized_start; i < ssize; i += step)	// [ ... start >>> ... (size - 1) ]
				ranged_list.add((*this->m_v)[i]);
			i = m_normalize_index(i);
			for(; i <= normalized_end; i+= step)			// [ ... c >>> ... >>> end ... ]
				ranged_list.add((*this->m_v)[i]);
		}

	}
	else { // step < 0
		if(normalized_end <= normalized_start)				// [ ... end <<< ... <<< start ... ]
			for(long i = normalized_start; i >= normalized_end; i += step)
                ranged_list.add((*this->m_v)[i]);
		else {												// [ ... <<< start ... end <<< ... ]
			long i;
			for(i = normalized_start; i >= 0; i += step)	// [ 0 ... <<< start ... ]
				ranged_list.add((*this->m_v)[i]);
			i = m_normalize_index(i);
			for(; i >= normalized_end; i+= step)			// [ ... end <<< ... <<< (size - c)... ]
				ranged_list.add((*this->m_v)[i]);
		}

	}

	return ranged_list;
}

template<typename T>
functional_list<T> functional_list<T>::operator[](std::initializer_list<long> && range) const {
	return operator[](range);
}

template<typename T>
std::size_t functional_list<T>::m_normalize_index(long index) const noexcept {
	std::size_t size = this->m_v->size();
	while(index < 0)
		index += size;
	return static_cast<std::size_t>(index);
}

template<typename T>
std::ostream& functional_list<T>::print(const std::string & separator, std::ostream & out) const noexcept {
	for(std::size_t i = 0, size = this->m_v->size() - 1; i < size; i++)
		out << (*this->m_v)[i] << separator;
	if(this->m_v->size() > 0)
		out << (*this->m_v)[this->m_v->size() - 1];
	return out;
}

template<typename T>
template<typename Func>
functional_list<T> functional_list<T>::filter(Func && test) const noexcept {
	functional_list<T> fl;
	for(const T & x : *this->m_v)
		if(test(x))
			fl.m_v->push_back(x);
	return fl;
}

template<typename T>
template<typename Func, typename AccType>
AccType functional_list<T>::reduce(AccType && accumulator, Func && reducer) const noexcept {
	for(const T & x : *this->m_v)
		accumulator = reducer(accumulator, x);
	return accumulator;
}

template<typename T>
template<typename Func>
functional_list<typename std::result_of<Func(T)>::type> functional_list<T>::map(Func && mapper) const noexcept {
	functional_list<typename std::result_of<Func(T)>::type> fl;
	for(const T & x : *this->m_v)
		fl.m_v->push_back(mapper(x));
	return fl;
}

template<typename T>
template<typename Func>
void functional_list<T>::for_each(Func && f) const noexcept {
	for(const T & x : *this->m_v)
		f(x);
}

template<typename T>
template<typename Func>
functional_list<T> functional_list<T>::max(Func && key) const {
	return m_compare(key, true);
}

template<typename T>
template<typename Func>
functional_list<T> functional_list<T>::min(Func && key) const {
	return m_compare(key, false);
}

template<typename T>
functional_list<T> functional_list<T>::max() const {
	return m_compare([] (const T & arg) -> const T & { return arg; }, true);
}

template<typename T>
functional_list<T> functional_list<T>::min() const {
	return m_compare([] (const T & arg) -> const T & { return arg; }, false);
}

template<typename T>
template<typename Func>
functional_list<T> functional_list<T>::m_compare(Func && key, bool greater) const {

	std::size_t size = this->m_v->size();

	if(size == 0)
		throw empty_list_exception();

	const T * _value = &(*this->m_v)[0];
	functional_list<T> results {*_value};

	if(greater) {
		for(std::size_t i = 1; i < size; i++) {
			auto first = key((*this->m_v)[i]);
			auto second = key(*_value);
			if(first > second) {
				_value = &(*this->m_v)[i];
				results.m_v->clear();
				results.add(*_value);
			}
			else if(first == second)
				results.add(*_value);
		}
	}
	else {
		for(std::size_t i = 1; i < size; i++) {
			auto first = key((*this->m_v)[i]);
			auto second = key(*_value);
			if(first < second) {
				_value = &(*this->m_v)[i];
				results.m_v->clear();
				results.add(*_value);
			}
			else if(first == second)
				results.add(*_value);
		}
	}

	return results;
}

template<typename T>
template<typename Func>
bool functional_list<T>::each_match(Func && test) const noexcept {
	for(const T & x : *this->m_v)
		if(!test(x))
			return false;
	return true;
}

template<typename T>
template<typename Func>
bool functional_list<T>::any_match(Func && test) const noexcept {
	for(const T & x : *this->m_v)
		if(test(x))
			return true;
	return false;
}

template<typename T>
template<typename Func>
bool functional_list<T>::no_match(Func && test) const noexcept {
	for(const T & x : *this->m_v)
		if(test(x))
			return false;
	return true;
}

template<typename T>
functional_list<T> functional_list<T>::uniques() const noexcept {
	functional_list<T> fl;
	for(T & x : *this->m_v)
		if(!fl.contains(x))
			fl.add(x);
	return fl;
}

template<typename T>
bool functional_list<T>::contains(const T & t) const noexcept {
	for(const T & x : *this->m_v)
		if(x == t)
			return true;
	return false;
}

template<typename T>
bool functional_list<T>::contains(T & t) const noexcept {
	for(T & x : *this->m_v)
		if(x == t)
			return true;
	return false;
}

template<typename T>
bool functional_list<T>::contains(T && t) const noexcept {
	return contains(t);
}

template<typename T>
ostream& operator<<(ostream & out, const functional_list<T> & t_func_list) noexcept {
	t_func_list.print(" ", out);
	return out;
}

template<typename T>
functional_list<T> functional_list<T>::limit_to(unsigned long max_elements) const noexcept {
	long max_index = max_elements - 1, size = this->m_v->size() - 1;
	// todo: this->v->size() is not signed, but unsigned.
	// This means that the actual size of the vector could be bigger than the maximum number representable on a long (signed)
	return operator[]({0, max_index <= size ? max_index : size});
}

template<typename T>
template<typename Func>
functional_list<T> functional_list<T>::sort(Func && func) const noexcept {
	functional_list<T> fl{*this};
	std::sort(fl.m_v->begin(), fl.m_v->end(), func);
	return fl;
}

template<typename T>
functional_list<T> functional_list<T>::sort(bool descending) const noexcept {
	functional_list<T> fl{*this};
	std::sort(fl.m_v->begin(), fl.m_v->end(), [descending](const T & t1, const T & t2) { return descending ? t1 > t2 : t2 > t1; });
	return fl;
}

template<typename T>
const T & functional_list<T>::first() const {
    if(this->m_v->empty())
        throw empty_list_exception();
    return this->m_v->at(0);
}

template<typename T>
const T & functional_list<T>::last() const {
    if(this->m_v->empty())
        throw empty_list_exception();
    return this->m_v->at(this->m_v->size() - 1);
}

#endif
