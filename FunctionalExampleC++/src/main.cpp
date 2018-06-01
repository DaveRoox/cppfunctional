/*
 * main.cpp
 *
 *  Created on: 01 giu 2018
 *      Author: daviderusso
 */
#include <iostream>
#include "FunctionalList/functional_list.hpp"

using namespace std;
using namespace functional;

#define EXAMPLE_1
#define EXAMPLE_2
#define EXAMPLE_3

class Person {

public:

	enum class gender_t { male, female };

	string name;
	string last_name;
	int age;
	gender_t gender;

	Person(const string & t_name, const string & t_last_name, int t_age, gender_t t_gender):
		name(t_name),
		last_name(t_last_name),
		age(t_age),
		gender(t_gender) {}

};

int main() {

	functional_list<Person> people = {
			Person("davide", "russo", 28, Person::gender_t::male),
			Person("ele", "caiazza", 27, Person::gender_t::female),
			Person("arcangelo", "pisa", 20, Person::gender_t::male),
			Person("peppe", "primo", 25, Person::gender_t::male),
			Person("vecchio", "random", 95, Person::gender_t::male),
			Person("vecchia", "random", 92, Person::gender_t::female)
	};

	// Example 1
#ifdef EXAMPLE_1
	people
	.filter([] (const Person & p) {
		return p.gender == Person::gender_t::male;
	})
	.sort([] (const Person & p1, const Person & p2) {
		return p1.age >= p2.age;
	})
	.limit_to(300)
	.for_each([] (const Person & p) {
		cout << p.name << " " << p.last_name << " di età " << p.age << endl;
	});
#endif

	// Example 2
#ifdef EXAMPLE_2
	people
	.map([] (const Person & p) {
		return p.age;
	})
	.sort(true)
	.print() << endl;
#endif

	// Example 3
#ifdef EXAMPLE_3
	auto print_person = [] (const Person & p) {
		cout << p.name << " " << p.last_name << " di età " << p.age << endl;
	};
	cout << "L'ultima persona inserita è:\n";
	print_person(people[-1]);
	cout << endl;
	cout << "Le persone in ordine inverso di inserimento sono:\n";
	people[{-1, 0, -1}].for_each(print_person);
#endif


	functional_list<Person> n{move(people.filter([] (Person p) { return p.gender == Person::gender_t::female; }))};
	cout << "\nLe donne sono:\n";
	n.sort([](Person p1, Person p2) { return p1.age >= p2.age; }).for_each(print_person);

	return 0;
}
