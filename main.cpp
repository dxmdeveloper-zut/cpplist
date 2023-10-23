#include <iostream>
#include <string>
#include "linked_list.hpp"

struct Data {
    std::string name;
    int age;

    bool operator<(const Data &other) const {
        if (age != other.age)
            return age < other.age;
        return name < other.name;
    }

    bool operator>(const Data &other) const {
        if (age != other.age)
            return age > other.age;
        return name > other.name;
    }

    bool operator==(const Data &other) const {
        return age == other.age && name == other.name;
    }
};

std::ostream &operator<<(std::ostream &os, const Data obj) {
    os << "{name: \"" << obj.name << "\", age: " << obj.age << "}";
    return os;
}


static void print_list(const linked_list<Data> &list) {
    std::cout << list.to_string() << std::endl;
    int i = 0;
    list.foreach([&i](Data &os) {
        std::cout << "[" << i++ << "]" << os << std::endl;
    });
}

std::string test_result_text(bool result) {
    return std::string("\t\t test result: ") + (result ? "passed" : "failed");
}

void list_unit_tests(){
    //  === Unit tests ===
    Data testData;
    linked_list<Data> list;
    list.push_back({"Ewa", 35});
    list.push_back({"Adam", 61});
    list.push_back({"Krzysztof", 35});
    list.push_back({"Grzegorz", 43});
    list.push_front({"Filip", 19});
    list.push_front({"Zdzisław", 0xbeef});

    // Show list
    print_list(list);

    // Get by index
    const Data &dat = list.at(5);
    testData = {"Grzegorz", 43};
    std::cout << std::left << "\nget by ind 5: " << dat << test_result_text(dat == testData) << std::endl;

    testData = {"Zdzisław", 0xbeef};
    std::cout << "get by ind 0: " << list[0] << test_result_text(list[0] == testData) << std::endl;

    // Find element
    Data *datp = list.find([](const Data &dat) { return dat.name == "Krzysztof"; });
    std::cout << "found: " << *datp << test_result_text(datp->name == "Krzysztof") << std::endl << std::endl;

    // Replace
    list.replace(1, {"Przemek", 14});
    datp = list.find([](const Data &dat) { return dat.name == "Przemek"; });
    Data *datp2 = list.find([](const Data &dat) { return dat.name == "Filip"; });
    std::cout << "replace Filip to Przemek [index:1]" << test_result_text(datp && !datp2) << std::endl;

    // Remove
    list.remove(0);
    datp = list.find([](const Data &dat) { return dat.name == "Zdzisław"; });
    std::cout << "remove Zdzisław [index:0]" << test_result_text(!datp) << std::endl;

    // Remove if (all matching)
    size_t removed_n = list.remove_if([](const Data &dat) { return dat.age > 40; });
    datp = list.find([](const Data &dat) { return dat.age > 40; });
    std::cout << "remove all age > 40" << test_result_text(removed_n == 2 && !datp) << std::endl;

    // Remove one if (first matching)
    bool removed = list.remove_one_if([](const Data &dat) { return dat.age == 35; });
    datp = list.find([](const Data &dat) { return dat.age == 35; });
    std::cout << "remove one age == 35" << test_result_text(removed && datp) << std::endl;

    std::cout << "\nList after remove operations:" << std::endl;
    print_list(list);

    // Ordered Insert
    // sort with std::less (ascending)
    list.add_ordered({"Władysław", 24}, std::less<>());
    list.add_ordered({"Jerzy", 12}, std::less<>());
    list.add_ordered({"Jan", 60}, std::less<>());
    std::cout << "\nInsert Jerzy, 12 (ordered)" << test_result_text(list[0].age == 12) << std::endl;
    std::cout << "Insert Władysław, 24 (ordered)" << test_result_text(list[2].age == 24) << std::endl;
    std::cout << "Insert Jan, 60 (ordered)" << test_result_text(list[4].age == 60) << std::endl;

    std::cout << "\nList after ordered ascending insert:" << std::endl;
    print_list(list);

    // list clear
    list.clear();
    std::cout << "List after clear:" << list.to_string() << std::endl;

    // sort with std::greater (descending)
    int test_ages[] = {31, 11, 23, 111, 26};
    list.add_ordered({"Aneta", test_ages[0]}, std::greater<>());
    list.add_ordered({"Karol", test_ages[1]}, std::greater<>());
    list.add_ordered({"Patryk", test_ages[2]}, std::greater<>());
    list.add_ordered({"Ziemowit", test_ages[3]}, std::greater<>());
    list.add_ordered({"Przemysław", test_ages[4]}, std::greater<>());
    bool test_passed = true;
    int test_last_age = INT32_MAX;
    list.foreach([&](Data &dat) {
        if (dat.age > test_last_age) {
            test_passed = false;
        }
        test_last_age = dat.age;
    });
    std::cout << "\nNew list after ordered descending inserts:" << test_result_text(test_passed) << std::endl;
    print_list(list);

    // pop front and back test
    list.pop_front();
    list.pop_back();
    std::cout << "List removed from front and back"
              << test_result_text(list.get_size() == 3 && list[0].age != 111 && list[2].age != 11) << std::endl;

    // insert by index
    list.insert(1, {"Diabeł", 666});
    std::cout << "Insert Diabeł, 666 at index 1"
              << test_result_text(list[1].age == 666) << std::endl;


}

int main() {
    std::cout << " ==== Unit tests ==== " << std::endl;
    list_unit_tests();

    return 0;
}
