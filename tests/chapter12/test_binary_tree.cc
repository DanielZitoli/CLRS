#include "gtest/gtest.h"

#include "chapter12/binary_tree.h"

// #include "tree_fixture.h"

// template <typename Derived>
// class Base {
// public:
//   void interface() {
//     // Call the implementation provided by the derived class
//     static_cast<Derived*>(this)->implementation();
//   }

//   void commonFunction() {
//     std::cout << "This is a common function in the base class." << std::endl;
//   }

//   // Pure virtual function
//   virtual void implementation() = 0;
// };

// // Derived class template
// template<typename T>
// class DerivedClass : public Base<DerivedClass<T>> {
// public:
//   void implementation() override {
//     std::cout << "Implementation provided by DerivedClass." << std::endl;
//   }

//   void specificFunction() {
//     std::cout << "This is a specific function in the derived class." << std::endl;
//   }
// };



TEST(Chapter12, Tree) {
  Chapter12::BinaryTree<int> tree;
  tree.insert(12);
  std::cout << tree;
}

// using MyTree = ::testing::Types<Chapter12::BinaryTree<int>>;

// INSTANTIATE_TYPED_TEST_SUITE_P(BinaryTreeTest, TreeTest, MyTree);
