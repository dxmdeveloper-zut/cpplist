#include <iostream>
#include <string>
#include "List.hpp"

struct Data {
    std::string name;
    int age;
};

int main() {
    List<Data> list;
    list.push_back({"Ewa", 23});
    list.push_back({"Adam", 61});
    list.push_back({"Krzysztof", 35});
    list.push_back({"Grzegorz", 43});

    // Show list
    int i = 0;
    list.foreach([&i](Data& os){
        std::cout<< "[" << i << "]{name: \"" << os.name << "\", age: " << os.age << "}\n";
        i++;
    });

    // Get by index
    Data& dat = list.at(1);
    std::cout << "\nget by ind 1: {" << dat.name << ", age:" << dat.age << "}\n";

    // Find
    dat = list.find([](Data& dat){return dat.name == "Krzysztof";});
    std::cout << "found: {" << dat.name << ", age:" << dat.age << "}\n";

    // Replace
    list.replace(1, {"Przemek", 14});
    std::cout << "replaced: {" << dat.name << ", age:" << dat.age << "}\n";

    // Remove

    return 0;
}
