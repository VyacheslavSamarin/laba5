#include "list.hpp"

template<typename T, typename Allocator>
List<T, Allocator>::List(Allocator alloc) 
    : alloc_(alloc), head_(nullptr), tail_(nullptr), sz_(0) {}

template<typename T, typename Allocator>
List<T, Allocator>::List(size_t sz, Allocator alloc) 
    : List(alloc) {
    for (size_t i = 0; i < sz; ++i) {
        PushBack(T());
    }
}

template<typename T, typename Allocator>
List<T, Allocator>::List(const std::initializer_list<T>& items, Allocator alloc) 
    : List(alloc) {
    for (const auto& item : items) {
        PushBack(item);
    }
}

template<typename T, typename Allocator>
List<T, Allocator>::List(const List& other) 
    : List(other.alloc_) {
    for (const auto& item : other) {
        PushBack(item);
    }
}

template<typename T, typename Allocator>
List<T, Allocator>::List(List&& other) noexcept 
    : alloc_(std::move(other.alloc_)), head_(other.head_), tail_(other.tail_), sz_(other.sz_) {
    other.head_ = nullptr;
    other.tail_ = nullptr;
    other.sz_ = 0;
}

template<typename T, typename Allocator>
List<T, Allocator>::~List() {
    clear();
}

template<typename T, typename Allocator>
typename List<T, Allocator>::ListIterator List<T, Allocator>::begin() const noexcept {
    return ListIterator(head_);
}

template<typename T, typename Allocator>
typename List<T, Allocator>::ListIterator List<T, Allocator>::end() const noexcept {
    return ListIterator(nullptr);
}

template<typename T, typename Allocator>
T& List<T, Allocator>::Front() const {
    return head_->value_;
}

template<typename T, typename Allocator>
T& List<T, Allocator>::Back() const {
    return tail_->value_;
}

template<typename T, typename Allocator>
bool List<T, Allocator>::IsEmpty() const noexcept {
    return head_ == nullptr;
}

template<typename T, typename Allocator>
size_t List<T, Allocator>::Size() const noexcept {
    return sz_;
}

template<typename T, typename Allocator>
typename List<T, Allocator>::ListIterator List<T, Allocator>::Find(const T& value) const {
    for (auto it = begin(); it != end(); ++it) {
        if (*it == value) {
            return it;
        }
    }
    return end();
}

template<typename T, typename Allocator>
void List<T, Allocator>::Erase(ListIterator pos) {
    Node* cur_node = pos.node_;
    if (cur_node == head_) {
        PopFront();
    } else if (cur_node == tail_) {
        PopBack();
    } else {
        Node* prev = cur_node->prev_;
        Node* next = cur_node->next_;
        prev->next_ = next;
        next->prev_ = prev;
        destroyNode(cur_node);
        --sz_;
    }
}

template<typename T, typename Allocator>
void List<T, Allocator>::Insert(ListIterator pos, const T& value) {
    Node* cur_node = pos.node_;
    if (cur_node == head_) {
        PushFront(value);
    } else {
        Node* new_node = createNode(value);
        Node* prev = cur_node->prev_;
        new_node->prev_ = prev;
        new_node->next_ = cur_node;
        prev->next_ = new_node;
        cur_node->prev_ = new_node;
        ++sz_;
    }
}

template<typename T, typename Allocator>
void List<T, Allocator>::PushBack(const T& value) {
    Node* new_node = createNode(value);
    if (tail_) {
        tail_->next_ = new_node;
        new_node->prev_ = tail_;
        tail_ = new_node;
    } else {
        head_ = tail_ = new_node;
    }
    ++sz_;
}

template<typename T, typename Allocator>
void List<T, Allocator>::PushFront(const T& value) {
    Node* new_node = createNode(value);
    if (head_) {
        head_->prev_ = new_node;
        new_node->next_ = head_;
        head_ = new_node;
    } else {
        head_ = tail_ = new_node;
    }
    ++sz_;
}

template<typename T, typename Allocator>
void List<T, Allocator>::PopBack() {
    if (!tail_) return;
    if (head_ == tail_) {
        destroyNode(tail_);
        head_ = tail_ = nullptr;
    } else {
        Node* tmp = tail_->prev_;
        tmp->next_ = nullptr;
        destroyNode(tail_);
        tail_ = tmp;
    }
    --sz_;
}

template<typename T, typename Allocator>
void List<T, Allocator>::PopFront() {
    if (!head_) return;
    if (head_ == tail_) {
        destroyNode(head_);
        head_ = tail_ = nullptr;
    } else {
        Node* tmp = head_->next_;
        tmp->prev_ = nullptr;
        destroyNode(head_);
        head_ = tmp;
    }
    --sz_;
}

template<typename T, typename Allocator>
void List<T, Allocator>::clear() {
    while (head_) {
        PopFront();
    }
}

template<typename T, typename Allocator>
typename List<T, Allocator>::Node* List<T, Allocator>::createNode(const T& value) {
    Node* node = NodeAllocatorTraits::allocate(alloc_, 1);
    NodeAllocatorTraits::construct(alloc_, node, value);
    return node;
}

template<typename T, typename Allocator>
void List<T, Allocator>::destroyNode(Node* node) {
    NodeAllocatorTraits::destroy(alloc_, node);
    NodeAllocatorTraits::deallocate(alloc_, node, 1);
}

template<typename T, typename Allocator>
typename List<T, Allocator>::ListIterator& List<T, Allocator>::ListIterator::operator++() {
    node_ = node_->next_;
    return *this;
}

template<typename T, typename Allocator>
typename List<T, Allocator>::ListIterator List<T, Allocator>::ListIterator::operator++(int) {
    ListIterator tmp = *this;
    ++(*this);
    return tmp;
}

template<typename T, typename Allocator>
typename List<T, Allocator>::ListIterator::reference List<T, Allocator>::ListIterator::operator*() const {
    return node_->value_;
}

template<typename T, typename Allocator>
typename List<T, Allocator>::ListIterator::pointer List<T, Allocator>::ListIterator::operator->() const {
    return &node_->value_;
}

template<typename T, typename Allocator>
bool List<T, Allocator>::ListIterator::operator==(const ListIterator& other) const {
    return node_ == other.node_;
}

template<typename T, typename Allocator>
bool List<T, Allocator>::ListIterator::operator!=(const ListIterator& other) const {
    return node_ != other.node_;
}