/*
 * main.cpp
 *
 *  Created on: 01 giu 2018
 *      Author: daviderusso
 */
#include <iostream>
#include <math.h>
#include "FunctionalList/functional_list.hpp"

using namespace std;
using namespace functional;

/*#define EXAMPLE_1
#define EXAMPLE_2
#define EXAMPLE_3*/

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

	srand(time(NULL));

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


	functional_list<double> list_of_double;
	constexpr const double upper_bound = 50.;
	constexpr const int N = 100;

	for(int i = 0; i < N; i++)
		list_of_double.add(upper_bound * (double)(rand() % 10000) / 10000.);

	cout << "[";
	list_of_double.print(", ") << "]\n";

	auto upper_half_list = list_of_double.filter([](double d) { return d >= upper_bound * .5; });
	auto lower_half_list = list_of_double.filter([](double d) { return d < upper_bound * .5; });
	cout << "Ci sono " << upper_half_list.count() << " elementi maggiori della metà dell'intervallo\n";

	cout << "La media della metà superiore è: " << upper_half_list.reduce(0.0, [] (double acc, double x) { return acc + x; }) / upper_half_list.count() << endl;
	cout << "La media della metà inferiore è: " << lower_half_list.reduce(0.0, [] (double acc, double x) { return acc + x; }) / lower_half_list.count() << endl;

	upper_half_list.max().print() << endl;
	lower_half_list.min().print() << endl;

	auto sorted_lower_half = lower_half_list.sort(true);
	auto sorted_upper_half = upper_half_list.sort();
	cout << sorted_lower_half << " " << sorted_upper_half << endl;

	upper_half_list = upper_half_list.map([] (double x) { return x * x; });
	lower_half_list = lower_half_list.map([] (double x) { return sqrt(x); });

	cout << lower_half_list.sort() << " " << upper_half_list.sort() << endl;

	return 0;
}
