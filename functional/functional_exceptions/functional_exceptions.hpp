//
// Created by Davide Russo on 15/07/18.
//

#ifndef FUNCTIONAL_LIST_FUNCTIONAL_EXCEPTIONS_H
#define FUNCTIONAL_LIST_FUNCTIONAL_EXCEPTIONS_H

#include <stdexcept>
#include <string>

namespace functional {

    class empty_list_exception : public std::runtime_error {
    public:
        empty_list_exception() : std::runtime_error("Bad index exception: empty list") {}
    };

    class index_out_of_range_exception : public std::runtime_error {
    public:
        index_out_of_range_exception() : std::runtime_error("Bad index exception: index out of range") {}
    };

    class wrong_number_of_parameters_exception : public std::runtime_error {
    public:
        wrong_number_of_parameters_exception() : std::runtime_error(
                "Bad range exception: Range must include 2 '<start, end>' or 3 '<start, end, step>' values") {}
    };

    class non_zero_step_exception : public std::runtime_error {
    public:
        non_zero_step_exception() : std::runtime_error("Bad range exception: Step cannot be 0") {}
    };

    class exceeded_list_size_exception : public std::runtime_error {
    public:
        exceeded_list_size_exception(long index, long size) : std::runtime_error(
                "Bad range exception: the index cannot be equal to or exceed list size (" + std::to_string(index) +
                " >= " + std::to_string(size) + ")") {}
    };

}

#endif //FUNCTIONAL_LIST_FUNCTIONAL_EXCEPTIONS_H
