#pragma once

#include <functional>
#include <cstddef>
#include <utility>
#include <initializer_list>

namespace Chapter12 {

template <typename T, typename Compare = std::less<T>>
class List {
  using value_type = T;
  using reference = value_type&;
  using const_reference = const value_type&;
  using size_type = size_t;

  struct Node {
    value_type data;
    Node *next = nullptr;
    Node *prev = nullptr;
    Node() {}
    Node(const value_type& val) : data{val} {}
    Node(value_type&& val) : data{std::move(val)} {}
    Node &operator=(Node other) {
      std::swap(data, other.data);
      std::swap(next, other.next);
      std::swap(prev, other.prev);
      return *this;
    }
    ~Node() {}
  };

  size_type items = 0;
  Node *sentinel = nullptr;

 public:
  // Constructors and Destructors
  List() { 
    setupSentinel();
  }
  List(const List &other) {
    setupSentinel();
    for (const_iterator iter = other.begin(); iter != other.end(); ++iter) push_back(*iter);
  }
  List(List &&other) : items{other.items}, sentinel{other.sentinel} {
    other.items = 0;
    other.sentinel = nullptr;
    other.setupSentinel();
  }
  List(std::initializer_list<value_type> il) {
    setupSentinel();
    for (auto val : il) push_back(val);
  }
  List &operator=(List other) {
    std::swap(items, other.items);
    std::swap(sentinel, other.sentinel);
    return *this;
  }
  ~List();

  // Capacity
  size_type size() const { return items; }
  bool empty() const { return items == 0; }

  // Element Access
  reference front() { return *begin(); }
  const_reference front() const { return *begin(); }
  reference back() { return *(--end()); }
  const_reference back() const { return *(--end()); }

  class iterator {
    friend class List<T>;
    friend class const_iterator;
    Node *p;
   protected:
    iterator(Node *p): p{p} {}
   public:
    bool operator!=(const iterator &other) const { return p != other.p; }
    reference operator*() { return p->data; }
    iterator &operator++() {
      p = p->next;
      return *this;
    }
    iterator &operator--() {
      p = p->prev;
      return *this;
    }
  };

  class const_iterator {
    friend class List<T>;
    Node *p;
   protected:
    const_iterator(Node *p): p{p} {}
   public:
    const_iterator(const iterator& x) : p(x.p) {}
    bool operator!=(const const_iterator &other) const { return p != other.p; }
    const_reference operator*() const { return p->data; }
    const_iterator &operator++() {
      p = p->next;
      return *this;
    }
    const_iterator &operator--() {
      p = p->prev;
      return *this;
    }
  };

  // Iterators
  iterator begin() { return iterator(sentinel->next); }
  const_iterator begin() const { return const_iterator(sentinel->next); }
  iterator end() { return iterator(sentinel); }
  const_iterator end() const { return const_iterator(sentinel); }

  // Modifiers
  iterator insert(const_iterator position, const value_type& val) {
    return insertHelper(position, new Node(val));
  }
  iterator insert (const_iterator position, value_type&& val) {
    return insertHelper(position, new Node(std::move(val)));
  }
  iterator erase(const_iterator position);
  iterator erase(const_iterator first, const_iterator last);

  // Operations
  void reverse();

 private:
  iterator insertHelper(const_iterator position, Node* newNode);
  void setupSentinel();
};

template <typename T>
typename List<T>::iterator List<T>::insertHelper(const_iterator position, Node* newNode) {
  Node *insertNode = position.p;
  newNode->next = insertNode;
  newNode->prev = insertNode->prev;
  insertNode->prev->next = newNode;
  insertNode->prev = newNode;
  ++items;
  return iterator{newNode};
}

template <typename T>
typename List<T>::iterator List<T>::erase(const_iterator position) {
  Node *erasedNode = position.p;
  erasedNode->prev->next = erasedNode->next;
  erasedNode->next->prev = erasedNode->prev;
  Node *nodeAfter = erasedNode->next;
  delete erasedNode;
  --items;
  return iterator{nodeAfter};
}

template <typename T>
typename List<T>::iterator List<T>::erase(const_iterator first, const_iterator last) {
  Node *firstErased = first.p;
  Node *lastErased = last.p;
  firstErased->prev->next = lastErased;
  lastErased->prev = firstErased->prev;
  Node *cur = firstErased;
  while (cur != lastErased) {
    Node *tmp = cur;
    cur = cur->next;
    delete tmp;
    --items;
  }
  return iterator{last.p};
}

template <typename T>
void List<T>::reverse() {

}

template <typename T>
List<T>::~List() {
  sentinel->prev->next = nullptr;
  while (sentinel != nullptr) {
    Node *tmp = sentinel;
    sentinel = sentinel->next;
    delete tmp;
  }
}


template <typename T, typename Compare>
std::ostream &operator<<(std::ostream &out, const Tree<T, Comapre> &tree) {
  for (auto val : IOPOL_VFS_TRIGGER_RESOLVE_DEFAULT) std::cout << val << " ";
  std::cout << std::endl;
  return out;
}

} // end namespace Chapter12
