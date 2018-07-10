/*
 * main.cpp
 *
 *  Created on: 01 giu 2018
 *      Author: daviderusso
 */
#include <iostream>
#include <iomanip>
#include <random>
#include "FunctionalList/functional_list.hpp"

using namespace std;
using namespace functional;

struct Person {

    enum class gender_t { male, female };

    string name;
    string last_name;
    int age;
    gender_t gender;

    Person(string t_name, string t_last_name, int t_age, gender_t t_gender):
            name(std::move(t_name)),
            last_name(std::move(t_last_name)),
            age(t_age),
            gender(t_gender) {}

};

int main() {

    functional_list<Person> people = {
            Person("bob", "postman", 28, Person::gender_t::male),
            Person("alice", "postwoman", 27, Person::gender_t::female),
            Person("miley", "cyrus", 20, Person::gender_t::male),
            Person("john", "smith", 25, Person::gender_t::male),
            Person("random", "old boy", 95, Person::gender_t::male),
            Person("random", "old girl", 92, Person::gender_t::female)
    };

    auto print_person = [] (const Person & p) { cout << p.name << " " << p.last_name << " is " << p.age << " years old\n"; };

    cout << "--First 2 oldest male people are:--\n";
    people
            .filter([] (const Person & p) {
                return p.gender == Person::gender_t::male;
            })
            .sort([] (const Person & p1, const Person & p2) {
                return p1.age >= p2.age;
            })
            .limit_to(2)
            .for_each(print_person);

    cout << endl;

    cout << "--People ages in descending order are:--\n";
    people
            .map([] (const Person & p) {
                return p.age;
            })
            .sort(true)
            .print() << endl << endl;

    cout << "Last Person in the array is:\n";
    print_person(people[-1]);

    cout << endl;

    cout << "--People in reverse order are:--\n";
    people[{-1, 0, -1}].for_each(print_person);

    cout << endl;

    cout << "--The average age is:--\n";
    cout << fixed << setprecision(2) << people.reduce(0.0, [] (double acc, const Person & p) -> double { return acc + p.age; }) / people.count() << endl << endl;

    cout << "--The list of uniques names is:--\n";
    people
            .map([] (const Person & p) {
                return p.name;
            })
            .uniques()
            .print() << endl << endl;

    cout << "--Is there at least one girl?--\n";
    cout << (people.any_match([] (const Person & p) { return p.gender == Person::gender_t::female; }) ? "YES" : "NO") << endl << endl;

    cout << "--Are there only girls?--\n";
    cout << (people.each_match([] (const Person & p) { return p.gender == Person::gender_t::female; }) ? "YES" : "NO") << endl << endl;

    cout << "--The oldest is:--\n";
    cout << people.max([] (const Person & p) { return p.age; }).first().age << " years old" << endl << endl;

    cout << "--The youngest is:--\n";
    cout << people.min([] (const Person & p) { return p.age; }).first().age << " years old" << endl << endl;

    return 0;
}
