#pragma once

#include <cstdlib>
#include <stdexcept>
#include <memory_resource>
#include <list>
#include <algorithm> 

class DynamicMemoryResource: public std::pmr::memory_resource {
    public:
        DynamicMemoryResource() = default;

        void* do_allocate(size_t bytes, size_t aligment) {
            if (!_free_blocks.empty()) {
                void* ptr = _free_blocks.front();
                _free_blocks.pop_front();
                _allocated_blocks.push_back(ptr);
                return ptr;
            }

            void* ptr = std::aligned_alloc(aligment, bytes);
            if (!ptr) {
                throw std::bad_alloc();
            }
            _allocated_blocks.push_back(ptr);
            return ptr;
        }

        void do_deallocate(void* p, size_t bytes, size_t aligment) {
            auto it = std::find_if(_allocated_blocks.begin(), _allocated_blocks.end(),
                       [p](void* ptr) { return ptr == p; });
            if (it != _allocated_blocks.end()) {
                _free_blocks.push_back(p);
                _allocated_blocks.erase(it);
            }
        }

        bool do_is_equal(const std::pmr::memory_resource& other) const noexcept {
            return this == &other;
        }

        ~DynamicMemoryResource() override {
            for (void* block: _allocated_blocks) {
                std::free(block);
            }
            for (void* block: _free_blocks) {
                std::free(block);
            }
        }

    private:
        std::list<void*> _allocated_blocks;
        std::list<void*> _free_blocks;
};
