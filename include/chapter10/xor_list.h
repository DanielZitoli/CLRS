#pragma once

#include <cstddef>
#include <utility>
#include <initializer_list>
#include <stdexcept>
#include <iostream>

namespace Chapter10 {

template <typename T>
class XORList {
  using value_type = T;
  using reference = value_type&;
  using const_reference = const value_type&;
  using size_type = size_t;

  struct Node {
    value_type data;

    Node() {}
    Node(const value_type& val) : data{val} {}
    Node(value_type&& val) : data{std::move(val)} {}
    virtual ~Node() = default;

    virtual Node *getOther(Node *x) = 0;
    virtual void setDirection(Node *oldPointer, Node *newPointer) = 0;
    virtual void setNext(Node *oldPointer, Node *newPointer) = 0;
    virtual void setPrev(Node *oldPointer, Node *newPointer) = 0;
  };

  struct SentinelNode : public Node {
    Node *prev = nullptr;
    Node *next = nullptr;

    SentinelNode() {}
    SentinelNode(const value_type& val) : Node{val} {}
    SentinelNode(value_type&& val) : Node{std::move(val)} {}
    ~SentinelNode() = default;

    virtual Node *getOther(Node *x) override {
      if (x == prev) {
        return next;
      } else if (x == next) {
        return prev;
      } else {
        throw std::logic_error("Node *x doesn't match existing node value");
      }
    }
    
    void setNext(Node *oldPointer, Node *newPointer) override {
      if (oldPointer != next) throw std::logic_error("oldPointer does not match next");
      next = newPointer;
    }

    void setPrev(Node *oldPointer, Node *newPointer) override {
      if (oldPointer != prev) throw std::logic_error("oldPointer does not match prev");
      prev = newPointer;
    }

    void setDirection(Node *oldPointer, Node *newPointer) override {
      if (oldPointer == prev) {
        prev = newPointer;
      } else if (oldPointer == next) {
        next = newPointer;
      } else {
        throw std::logic_error("Node *oldPointer doesn't match existing node value");
      }
    }
  };

  struct XORNode : public Node {
    size_type ptrs = 0;

    XORNode() {}
    XORNode(const value_type& val) : Node{val} {}
    XORNode(value_type&& val) : Node{std::move(val)} {}
    ~XORNode() = default;

    Node *getOther(Node *x) override {
      size_type initialDirection = reinterpret_cast<size_type>(x);
      return reinterpret_cast<Node *>(ptrs ^ initialDirection);
    }
    void setDirection(Node *oldPointer, Node *newPointer) override {
      size_type oldDir = reinterpret_cast<size_type>(oldPointer);
      size_type newDir = reinterpret_cast<size_type>(newPointer);

      ptrs ^= oldDir ^ newDir; // ptrs = (ptrs ^ oldDir) ^ newDir = (reverseDir) ^ newDir
    }

    void setNext(Node *oldPointer, Node *newPointer) override {
      setDirection(oldPointer, newPointer);
    }

    void setPrev(Node *oldPointer, Node *newPointer) override {
      setDirection(oldPointer, newPointer);
    }
  };

  size_type items = 0;
  SentinelNode *sentinel = nullptr;

 public:
  // Constructors and Destructors
  XORList() {
    setupSentinel();
  }
  XORList(const XORList &other) {
    setupSentinel();
    for (const_iterator iter = other.begin(); iter != other.end(); ++iter) push_back(*iter);
  }
  XORList(XORList &&other) : items{other.items}, sentinel{other.sentinel} {
    other.items = 0;
    other.sentinel = nullptr;
    other.setupSentinel();
  }
  XORList(std::initializer_list<value_type> il) {
    setupSentinel();
    for (auto val : il) push_back(val);
  }
  XORList &operator=(XORList other) {
    std::swap(items, other.items);
    std::swap(sentinel, other.sentinel);
    return *this;
  }
  ~XORList();

  // Capacity
  size_type size() const { return items; }
  bool empty() const { return items == 0; }

  // Element Access
  reference front() { return *begin(); }
  const_reference front() const { return *begin(); }
  reference back() { return *(--end()); }
  const_reference back() const { return *(--end()); }

  class iterator {
    friend class XORList<T>;
    friend class const_iterator;
    Node *p;
    Node *next;
   protected:
    iterator(Node *p, Node *next): p{p}, next{next} {}
   public:
    bool operator!=(const iterator &other) const { return p != other.p; }
    reference operator*() { return p->data; }
    iterator &operator++() {
      Node *prev = p;
      p = next;
      next = p->getOther(prev);
      return *this;
    }
    iterator &operator--() {
      Node *prev = p->getOther(next);
      next = p;
      p = prev;
      return *this;
    }
  };

  class const_iterator {
    friend class XORList<T>;
    Node *p;
    Node *next;
   protected:
    const_iterator(Node *p, Node *next): p{p}, next{next} {}
   public:
    const_iterator(const iterator& x) : p{x.p}, next{x.next} {}
    bool operator!=(const const_iterator &other) const { return p != other.p; }
    const_reference operator*() const { return p->data; }
    const_iterator &operator++() {
      Node *prev = p;
      p = next;
      next = p->getOther(prev);
      return *this;
    }
    const_iterator &operator--() {
      Node *prev = p->getOther(next);
      next = p;
      p = prev;
      return *this;
    }
  };

  // Iterators
  iterator begin() { 
    return iterator(sentinel->next, sentinel->next->getOther(sentinel)); 
  }
  const_iterator begin() const { return const_iterator(sentinel->next, sentinel->next->getOther(sentinel)); }
  iterator end() { return iterator(sentinel, sentinel->next); }
  const_iterator end() const { return const_iterator(sentinel, sentinel->next); }

  // Modifiers
  void push_front (const value_type& val) { insert(begin(), val); }
  void push_front (value_type&& val) { insert(begin(), std::move(val)); }
  void push_back (const value_type& val) { insert(end(), val); }
  void push_back (value_type&& val) { insert(end(), std::move(val)); }
  void pop_front() { erase(begin()); }
  void pop_back() { erase(--end()); }


  iterator insert(const_iterator position, const value_type& val) {
    return insertHelper(position, new XORNode(val));
  }
  iterator insert (const_iterator position, value_type&& val) {
    return insertHelper(position, new XORNode(std::move(val)));
  }
  iterator erase(const_iterator position);
  iterator erase(const_iterator first, const_iterator last);

  // Operations
  void reverse() {
    std::swap(sentinel->next, sentinel->prev);
  }

 private:
  iterator insertHelper(const_iterator position, Node* newNode);
  void setupSentinel();
};

template <typename T>
typename XORList<T>::iterator XORList<T>::insertHelper(const_iterator position, Node* newNode) {
  Node *insertNode = position.p;
  Node *insertNodeNext = position.next;
  Node *insertNodePrev = insertNode->getOther(insertNodeNext);
  // std::cout << "node: " << insertNode << " - next node: " << insertNodeNext << " - prev node: " << insertNodePrev << std::endl;
  newNode->setNext(nullptr, insertNode); // set next
  newNode->setPrev(nullptr, insertNodePrev); // set prev

  insertNodePrev->setNext(insertNode, newNode); //// TODO
  insertNode->setPrev(insertNodePrev, newNode);
  ++items;
  return iterator{newNode, insertNode};
}

template <typename T>
typename XORList<T>::iterator XORList<T>::erase(const_iterator position) {
  Node *erasedNode = position.p;
  Node *erasedNodeNext = position.next;
  Node *erasedNodePrev = erasedNode->getOther(erasedNodeNext);
  erasedNodePrev->setNext(erasedNode, erasedNodeNext);
  erasedNodeNext->setPrev(erasedNode, erasedNodePrev); 
  delete erasedNode;
  --items;
  return iterator{erasedNodeNext, erasedNodeNext->getOther(erasedNodePrev)};
}

template <typename T>
typename XORList<T>::iterator XORList<T>::erase(const_iterator first, const_iterator last) {
  Node *firstErased = first.p;
  Node *firstErasedNext = first.next;
  Node *firstErasedPrev = firstErased->getOther(firstErasedNext);
  Node *lastErased = last.p;
  Node *lastErasedNext = last.next;
  Node *lastErasedPrev = lastErased->getOther(lastErasedNext);

  // destroy nodes in range
  Node *cur = firstErased;
  Node *next = firstErasedNext;
  while (cur != lastErased) {
    Node *tmp = cur;
    cur = next;
    next = cur->getOther(tmp);
    delete tmp;
    --items;
  }
 
  // connect nodes before and after range
  firstErasedPrev->setNext(firstErased, lastErased);
  lastErased->setPrev(lastErasedPrev, firstErasedPrev);

  return iterator{lastErased, lastErasedNext};
}

template <typename T>
XORList<T>::~XORList() {
  if (!empty()){
    sentinel->prev->setNext(sentinel, nullptr);

    Node *cur = sentinel;
    Node *next = sentinel->next;
    while (next != nullptr) {
      Node *tmp = cur;
      cur = next;
      next = cur->getOther(tmp);
      delete tmp;
    }
    delete cur;
  }
}

template <typename T>
void XORList<T>::setupSentinel() {
  sentinel = new SentinelNode();
  sentinel->next = sentinel; 
  sentinel->prev = sentinel;
}

template <typename T>
std::ostream &operator<<(std::ostream &out, const XORList<T> &list) {
  for (auto val : list) std::cout << val << " ";
  std::cout << std::endl;
  return out;
}

} // end namespace Chapter10
