#pragma once
#include <cstddef>
#include <stdexcept>
#include <functional>

template<typename T>
struct ListNode {
    ListNode *next = nullptr;
    ListNode *prev = nullptr;
    T data;

    ListNode(T data){
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
    void insert(size_t ind, T data);
    void replace(size_t ind, T data);
    void foreach(std::function<void(T&)> lambda);
    T& find(std::function<bool(T&)> comp_lambda) const;
    T& at(size_t ind) const;

    ~List();
private:
    ListNode<T> *node_at(size_t ind);
    inline void _push_back_or_front(T data, bool front);
    inline void _pop_back_or_front(bool front);
};

template<class T>
List<T>::~List() {
    ListNode<T> *node = head;
    ListNode<T> *next = nullptr;
    while(node){
        next = node->next;
        delete node;
        node = next;
    }
}

template<class T>
void List<T>::foreach(std::function<void(T&)> lambda) {
    ListNode<T> * node = head;
    while(node){
        lambda(node->data);
        node = node->next;
    }
}

template<class T>
ListNode<T> *List<T>::node_at(size_t ind) {
    if(ind >= size)
        throw std::runtime_error("Index out-of-bounds");

    ListNode<T> *node = head;
    for(size_t i = 0; i < ind; i++){
        node = node->next;
    }
    return node;
}

template<class T>
void List<T>::replace(size_t ind, T data) {
    if(head == nullptr) throw std::runtime_error("List is empty");

    try {
        ListNode<T> *node = this->node_at(ind);
        node->data = data;
    } catch (const std::exception& e){
        throw e;
    }
}

template<class T>
void List<T>::insert(size_t ind, T data) {
    if(head == nullptr) throw std::runtime_error("List is empty");
    try {
        ListNode<T> *next_node = this->node_at(ind);
        ListNode<T> *prev_node = next_node->prev;
        ListNode<T> *new_node = new ListNode<T>(data);

        new_node->prev = prev_node;
        new_node->next = next_node;

        if(!prev_node){
            head = new_node;
        } else {
            prev_node->next = new_node;
        }
        next_node->prev = new_node;

    } catch (const std::exception& e){
        throw e;
    }
    size++;
}

template<class T>
void List<T>::remove(size_t ind) {
    if(head == nullptr) throw std::runtime_error("List is empty");

    try {
        ListNode<T> *node = this->node_at(ind);
        ListNode<T> *prev = node->prev;
        ListNode<T> *next = node->next;

        if(head == node) head = next;
        if(tail == node) tail = prev;

        if(next) next->prev = prev;
        if(prev) prev->next = next;
        delete node;

        size--;
    } catch (const std::exception& e){
        throw e;
    }
}

template<class T>
inline void List<T>::_push_back_or_front(T data, bool front){
    auto *new_node = new ListNode<T>(data);

    if (head == nullptr) {
        head = tail = new_node;
    } else {
        if(!front) {
            new_node->prev = tail;
            tail->next = new_node;
            tail = new_node;
        } else {
            new_node->next = head;
            head->next = new_node;
            head = new_node;
        }
    }
    size++;
}

template<class T>
inline void List<T>::_pop_back_or_front(bool front){
    if (head == nullptr)
        return;

    if(head == tail){
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
void List<T>::push_back(T data){
    _push_back_or_front(data, false);
}

template<class T>
void List<T>::push_front(T data){
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
T& List<T>::at(size_t ind) const {
    if(ind >= size)
        throw std::runtime_error("Index out-of-bounds");

    ListNode<T> *node = head;
    for(size_t i = 0; i < ind; i++){
        node = node->next;
    }
    return node->data;
}

template<class T>
T& List<T>::find(std::function<bool(T&)> lambda) const {
    ListNode<T> *node = head;
    while(node){
        if(lambda(node->data))
            return node->data;
        node = node->next;
    }
    throw std::runtime_error("Not found");
}