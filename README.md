# CppFunctional

This is a personal project born to make some practice with generic programming, lambda and some of the features in modern C++.

Currently the project provides an extension of std::vector to support some of the most famous methods used in the functional programming.
In the future the project will support other data structures extensively used, such as map, set, unordered_map, unordered_set and so on. 

Any help in terms of contribution or suggestion is appreciated.

Let's start with an example: (<a href="https://github.com/DaveRoox/functional/blob/master/main.cpp">main.cpp</a>)

First of all, let's define a very simple data structure:
```c++17
struct Person {

    enum class gender_t {
        male, female
    };

    string name;
    string last_name;
    int age;
    gender_t gender;

    Person(string t_name, string t_last_name, int t_age, gender_t t_gender) :
            name(std::move(t_name)),
            last_name(std::move(t_last_name)),
            age(t_age),
            gender(t_gender) {}

};
```

Then, we can initialize our functional_vector and define an utility lambda:

```c++
functional_vector<Person> people = {
            Person("bob", "postman", 28, Person::gender_t::male),
            Person("alice", "postwoman", 27, Person::gender_t::female),
            Person("miley", "cyrus", 20, Person::gender_t::female),
            Person("selena", "gomez", 20, Person::gender_t::female),
            Person("john", "smith", 25, Person::gender_t::male),
            Person("random", "old boy", 95, Person::gender_t::male),
            Person("random", "old girl", 92, Person::gender_t::female)
    };
auto print_person =
            [] (const Person &p) { cout << p.name << " " << p.last_name << " is " << p.age << " years old\n"; };
```

<b>---------------------------------------------------------------------------</b>

Let's try some method: **filter**, **sort**, **limit_to** and **for_each**

```c++
cout << "--First 2 oldest male people are:--\n";
people
        .filter([](const Person &p) {
            return p.gender == Person::gender_t::male;
        })
        .sort([](const Person &p1, const Person &p2) {
            return p1.age >= p2.age;
        })
        .limit_to(2)
        .for_each(print_person);
```

output:

<img align='middle' src='https://user-images.githubusercontent.com/23279650/42753940-dc421b42-88f2-11e8-8ced-18bd8c023e87.png' /><br/>

<b>---------------------------------------------------------------------------</b>

Descending order by age:

```c++
cout << "--People ages in descending order are:--\n";
people
        .map([](const Person &p) {
            return p.age;
        })
        .sort(true)
        .print() << endl << endl;
```

output:

<img align='middle' src='https://user-images.githubusercontent.com/23279650/42754084-6f2431d4-88f3-11e8-82b7-9601ad46bddb.png' /><br/>

<b>---------------------------------------------------------------------------</b>

Playing with **ranges** and **negative indices**:

```c++
cout << "Last Person in the array is:\n";
print_person(people[-1]);

cout << endl;

cout << "--People in reverse order are:--\n";
people[{-1, 0, -1}].for_each(print_person);
```

output:

<img align='middle' src='https://user-images.githubusercontent.com/23279650/42754190-dedd4c68-88f3-11e8-9ced-5fd83268804e.png' /><br/>

<b>---------------------------------------------------------------------------</b>

Average age:

```c++
cout << "--The average age is:--\n";
cout << fixed << setprecision(2)
     << people.reduce(0.0, [](double acc, const Person &p) -> double { return acc + p.age; }) / people.size()
     << endl << endl;
```

output:

<img align='middle' src='https://user-images.githubusercontent.com/23279650/42754286-367bf4ec-88f4-11e8-9366-4d9ce0202873.png' /><br/>

<b>---------------------------------------------------------------------------</b>

Some other methods: **any_match**, **each_match**, **max_by**, **min_by**, **first**

```c++
cout << "--Is there at least one girl?--\n";
cout << (people.any_match([](const Person &p) { return p.gender == Person::gender_t::female; }) ? "YES" : "NO")
     << endl << endl;

cout << "--Are they all girls?--\n";
cout << (people.each_match([](const Person &p) { return p.gender == Person::gender_t::female; }) ? "YES" : "NO")
     << endl << endl;

cout << "--The oldest is:--\n";
cout << people.max_by([](const Person &p) { return p.age; }).first().age << " years old" << endl << endl;

cout << "--The youngest is:--\n";
cout << people.min_by([](const Person &p) { return p.age; }).first().age << " years old" << endl << endl;
```

output:

<img align='middle' src='https://user-images.githubusercontent.com/23279650/42754414-b55d6408-88f4-11e8-9043-8239b91e714e.png' /><br/>

<b>---------------------------------------------------------------------------</b>

As last example the **group_by** method:

```c++
cout << "--The people grouped by age are divided in the following way:--\n";
for (const auto &x : people.group_by([](const auto &p) { return p.age; })) {
    auto &age = x.first;
    auto &people_with_that_age = x.second;
    cout << age << " Y/O: ";
    people_with_that_age.print_by(
            [](const auto &p) { return "'" + p.name + " " + p.last_name + "'"; },
            "[", // prefix
            ", ", // separator
            "]" // postfix
    ) << endl;
}
```

output:

<img align='middle' src='https://user-images.githubusercontent.com/23279650/42754491-f3e38fea-88f4-11e8-98e8-a109030b5c1b.png' /><br/>
