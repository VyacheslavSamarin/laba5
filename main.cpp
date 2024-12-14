#include "include/list.hpp"
#include "include/allocator.hpp"
#include <iostream>
#include <memory_resource>  // для работы с polymorphic_allocator

struct my_struct {
    int age;
    double weight;
};

int main() {
    DynamicMemoryResource mem_resource;
    std::pmr::polymorphic_allocator<int> int_alloc(&mem_resource);

    // Создаем список `int_list`, используя `polymorphic_allocator`
    List<int> int_list(int_alloc);
    int_list.PushBack(100);
    int_list.PushBack(111);

    for (auto value : int_list) {
        std::cout << value << std::endl;
    }
    std::cout << std::endl;

    // Создаем аллокатор для `my_struct` и список `struct_list`
    std::pmr::polymorphic_allocator<my_struct> struct_alloc(&mem_resource);
    List<my_struct> struct_list(struct_alloc);
    struct_list.PushBack(my_struct{10, 35});
    struct_list.PushBack(my_struct{30, 88});

    for (auto& item : struct_list) {
        std::cout << "age: " << item.age << ", weight: " << item.weight << std::endl;
    }

    return 0;
}