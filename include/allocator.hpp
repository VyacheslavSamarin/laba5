#pragma once

#include <cstdlib>
#include <stdexcept>
#include <memory_resource>
#include <list>
#include <algorithm>

class DynamicMemoryResource : public std::pmr::memory_resource {
public:
    DynamicMemoryResource() = default;

    void* do_allocate(size_t bytes, size_t alignment) override;
    void do_deallocate(void* p, size_t bytes, size_t alignment) override;
    bool do_is_equal(const std::pmr::memory_resource& other) const noexcept override;

    ~DynamicMemoryResource() override;

private:
    std::list<void*> _allocated_blocks;
    std::list<void*> _free_blocks;
};