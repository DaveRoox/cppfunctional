/*
 * main.cpp
 *
 *  Created on: 01 giu 2018
 *      Author: daviderusso
 */
#include <iostream>
#include <random>
#include "FunctionalList/functional_list.hpp"

using namespace std;
using namespace functional;

struct person {

    enum class gender_t { male, female };

    string name;
    string last_name;
    int age;
    gender_t gender;

    person(string t_name, string t_last_name, int t_age, gender_t t_gender):
            name(std::move(t_name)),
            last_name(std::move(t_last_name)),
            age(t_age),
            gender(t_gender) {}

};

int main() {

    functional_list<person> people = {
            person("bob", "postman", 28, person::gender_t::male),
            person("alice", "postwoman", 27, person::gender_t::female),
            person("miley", "cyrus", 20, person::gender_t::male),
            person("john", "smith", 25, person::gender_t::male),
            person("random", "old boy", 95, person::gender_t::male),
            person("random", "old girl", 92, person::gender_t::female)
    };

    auto print_person = [] (const person & p) { cout << p.name << " " << p.last_name << " is " << p.age << " years old\n"; };

    cout << "--First 2 oldest male people are:--\n";
    people
            .filter([] (const person & p) {
                return p.gender == person::gender_t::male;
            })
            .sort([] (const person & p1, const person & p2) {
                return p1.age >= p2.age;
            })
            .limit_to(2)
            .for_each(print_person);

    cout << endl;

    cout << "--People ages in descending order are:--\n";
    people
            .map([] (const person & p) {
                return p.age;
            })
            .sort(true)
            .print() << endl << endl;

    cout << "Last person in the array is:\n";
    print_person(people[-1]);

    cout << endl;

    cout << "--People in reverse order are:--\n";
    people[{-1, 0, -1}].for_each(print_person);

    return 0;
}
