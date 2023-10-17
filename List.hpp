#pragma once

#include <cstddef>
#include <stdexcept>
#include <functional>

template<typename T>
struct ListNode {
    ListNode *next = nullptr;
    ListNode *prev = nullptr;
    T data;

    ListNode(T data) {
        this->data = data;
    }
};

template<class T>
class List {
private:
    ListNode<T> *head = nullptr;
    ListNode<T> *tail = nullptr;
    size_t size = 0;
public:
    void push_back(T data);

    void push_front(T data);

    void pop_back();

    void pop_front();

    void remove(size_t ind);

    /// @return count of removed items
    size_t remove_if(std::function<bool(const T &)> compr);

    /// @return 1 if element was found and removed. 0 otherwise.
    bool remove_one_if(std::function<bool(const T &)> compr);

    void insert(size_t ind, T data);

    void replace(size_t ind, T data);

    void clear();

    void foreach(std::function<void(T &)> lambda) const;

    T *find(std::function<bool(const T &)> compr) const;

    T &at(size_t ind) const;

    T &operator[](size_t ind) const;

    ~List();

private:
    void remove_node(ListNode<T> *node);

    ListNode<T> *node_at(size_t ind) const;

    inline void _push_back_or_front(T data, bool front);

    inline void _pop_back_or_front(bool front);
};

template<class T>
void List<T>::clear() {
    ListNode<T> *node = head;
    ListNode<T> *next = nullptr;
    while (node) {
        next = node->next;
        delete node;
        node = next;
    }
    head = tail = nullptr;
    size = 0;
}

template<class T>
List<T>::~List() {
    this->clear();
}

template<class T>
void List<T>::foreach(std::function<void(T &)> lambda) const {
    ListNode<T> *node = head;
    while (node) {
        lambda(node->data);
        node = node->next;
    }
}

template<class T>
ListNode<T> *List<T>::node_at(size_t ind) const {
    if (ind >= size) return nullptr;

    ListNode<T> *node = nullptr;

    if (size - ind > ind) {
        node = head;
        for (size_t i = 0; i < ind; i++) {
            node = node->next;
        }
        return node;
    } else {
        node = tail;
        for (size_t i = size - 1; i > ind; i--) {
            node = node->prev;
        }
        return node;
    }
    throw std::runtime_error("Corrupted list");
}

template<class T>
void List<T>::replace(size_t ind, T data) {
    if (head == nullptr) throw std::runtime_error("List is empty");

    ListNode<T> *node = this->node_at(ind);
    node->data = data;
}

template<class T>
void List<T>::insert(size_t ind, T data) {
    if (head == nullptr) throw std::runtime_error("List is empty");
        ListNode<T> *next_node = this->node_at(ind);
        ListNode<T> *prev_node = next_node->prev;
        ListNode<T> *new_node = new ListNode<T>(data);

        new_node->prev = prev_node;
        new_node->next = next_node;

        if (!prev_node) {
            head = new_node;
        } else {
            prev_node->next = new_node;
        }
        next_node->prev = new_node;

    size++;
}

template<class T>
void List<T>::remove(size_t ind) {
    if (head == nullptr) return;

    ListNode<T> *node = this->node_at(ind);
    ListNode<T> *prev = node->prev;
    ListNode<T> *next = node->next;

    if (next) next->prev = prev;
    else tail = prev;

    if (prev) prev->next = next;
    else head = next;

    delete node;

    size--;
}

template<class T>
size_t List<T>::remove_if(std::function<bool(const T &)> compr) {
    ListNode<T> *node = head;
    size_t rn = 0;
    while (node) {
        if (compr(node->data)) {
            ListNode<T> *next = node->next;
            this->remove_node(node);
            node = next;
            rn++;
        } else node = node->next;
    }
    return rn;
}

template<class T>
bool List<T>::remove_one_if(std::function<bool(const T &)> compr) {
    ListNode<T> *node = head;
    while (node) {
        if (compr(node->data)) {
            this->remove_node(node);
            return true;
        }
        node = node->head;
    }
    return false;
}

template<class T>
inline void List<T>::_push_back_or_front(T data, bool front) {
    auto *new_node = new ListNode<T>(data);

    if (head == nullptr) {
        head = tail = new_node;
    } else {
        if (!front) {
            new_node->prev = tail;
            tail->next = new_node;
            tail = new_node;
        } else {
            new_node->next = head;
            head->prev = new_node;
            head = new_node;
        }
    }
    size++;
}

template<class T>
inline void List<T>::_pop_back_or_front(bool front) {
    if (head == nullptr)
        return;

    if (head == tail) {
        delete head;
        tail = head = nullptr;
    } else if (front) {
        ListNode<T> *new_head = head->next;
        delete head;
        head = new_head;
    } else {
        ListNode<T> *new_tail = tail->prev;
        delete tail;
        tail = new_tail;
    }
    size--;
}

template<class T>
void List<T>::push_back(T data) {
    _push_back_or_front(data, false);
}

template<class T>
void List<T>::push_front(T data) {
    _push_back_or_front(data, true);
}

template<class T>
void List<T>::pop_back() {
    _pop_back_or_front(false);
}

template<class T>
void List<T>::pop_front() {
    _pop_back_or_front(true);
}

template<class T>
T &List<T>::at(size_t ind) const {
    auto node = node_at(ind);
    if (!node) throw std::runtime_error("index out-of-bounds");
    return node->data;
}

template<class T>
T &List<T>::operator[](size_t ind) const {
    return this->at(ind);
}

template<class T>
T *List<T>::find(std::function<bool(const T &)> compr) const {
    ListNode<T> *node = head;
    while (node) {
        if (compr(node->data))
            return &node->data;
        node = node->next;
    }
    return nullptr;
}

template<class T>
void List<T>::remove_node(ListNode<T> *node) {
    if (size == 0) return;

    ListNode<T> next_node = node->next;
    ListNode<T> prev_node = node->prev;

    if (next_node) next_node->prev = prev_node;
    else tail = prev_node;

    if (prev_node) prev_node->next = next_node;
    else head = next_node;

    size--;
}