#include <iostream>
#include <string>
#include "List.hpp"

struct Data {
    std::string name;
    int age;
};
std::ostream& operator<<(std::ostream& os, const Data obj){
    os << "{name: \"" << obj.name << "\", age: " << obj.age << "}";
    return os;
}

static void print_list(const List<Data>& list){
    int i = 0;
    list.foreach([&i](Data& os){
        std::cout<< "[" << i++ << "]" << os << std::endl;
    });
}

int main() {
    List<Data> list;
    list.push_back({"Ewa", 23});
    list.push_back({"Adam", 61});
    list.push_back({"Krzysztof", 35});
    list.push_back({"Grzegorz", 43});
    list.push_front({"Filip", 19});
    list.push_front({"Zdzisław", 0xbeef});

    // Show list
    print_list(list);

    // Get by index
    const Data& dat = list.at(5);
    std::cout << "\nget by ind 5: " << dat << std::endl;
    std::cout << "get by ind 0: " << list[0] << std::endl;

    // Find element
    Data *datp = list.find([](const Data& dat){return dat.name == "Krzysztof";});
    std::cout << "found: " << *datp << std::endl;

    // Replace
    list.replace(1, {"Przemek", 14});

    // Remove
    list.remove(0);

    print_list(list);


    return 0;
}
