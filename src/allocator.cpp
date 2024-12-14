#include "allocator.hpp"

void* DynamicMemoryResource::do_allocate(size_t bytes, size_t alignment) {
    if (!_free_blocks.empty()) {
        void* ptr = _free_blocks.front();
        _free_blocks.pop_front();
        _allocated_blocks.push_back(ptr);
        return ptr;
    }

    void* ptr = std::aligned_alloc(alignment, bytes);
    if (!ptr) {
        throw std::bad_alloc();
    }
    _allocated_blocks.push_back(ptr);
    return ptr;
}

void DynamicMemoryResource::do_deallocate(void* p, size_t bytes, size_t alignment) {
    auto it = std::find_if(_allocated_blocks.begin(), _allocated_blocks.end(),
                           [p](void* ptr) { return ptr == p; });
    if (it != _allocated_blocks.end()) {
        _free_blocks.push_back(p);
        _allocated_blocks.erase(it);
    }
}

bool DynamicMemoryResource::do_is_equal(const std::pmr::memory_resource& other) const noexcept {
    return this == &other;
}

DynamicMemoryResource::~DynamicMemoryResource() {
    for (void* block : _allocated_blocks) {
        std::free(block);
    }
    for (void* block : _free_blocks) {
        std::free(block);
    }
}