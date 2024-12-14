#pragma once

#include <memory_resource>
#include <iterator>
#include <initializer_list>
#include "allocator.hpp"
#include <concepts>
#include <memory>

template<typename T, typename Allocator = std::pmr::polymorphic_allocator<T>>
    requires std::is_default_constructible_v<T> && std::is_same_v<Allocator, std::pmr::polymorphic_allocator<T>>
class List {
private:
    struct Node {
        T value_;
        Node* prev_;
        Node* next_;

        explicit Node(const T& value) : value_(value), prev_(nullptr), next_(nullptr) {}
    };

    using NodeAllocator = typename std::allocator_traits<Allocator>::template rebind_alloc<Node>;
    using NodeAllocatorTraits = std::allocator_traits<NodeAllocator>;

public:
    class ListIterator {
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using pointer = T*;
        using reference = T&;

        ListIterator& operator++();
        ListIterator operator++(int);
        reference operator*() const;
        pointer operator->() const;
        bool operator==(const ListIterator& other) const;
        bool operator!=(const ListIterator& other) const;

    private:
        Node* node_;
        explicit ListIterator(Node* node) : node_(node) {}

        friend class List;
    };

    List(Allocator alloc = Allocator{});
    List(size_t sz, Allocator alloc = Allocator{});
    List(const std::initializer_list<T>& items, Allocator alloc = Allocator{});
    List(const List& other);
    List(List&& other) noexcept;
    ~List();

    ListIterator begin() const noexcept;
    ListIterator end() const noexcept;

    T& Front() const;
    T& Back() const;

    bool IsEmpty() const noexcept;
    size_t Size() const noexcept;

    ListIterator Find(const T& value) const;
    void Erase(ListIterator pos);
    void Insert(ListIterator pos, const T& value);
    void PushBack(const T& value);
    void PushFront(const T& value);
    void PopBack();
    void PopFront();
    void clear();

private:
    NodeAllocator alloc_;
    Node* head_;
    Node* tail_;
    size_t sz_;

    Node* createNode(const T& value);
    void destroyNode(Node* node);
};