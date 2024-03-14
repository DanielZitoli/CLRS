#include "chapter6/heaps.h"

namespace Chapter6 {
  int parent(int index) { return (index - 1) / 2; }
  int left(int index) { return (2 * index) + 1; }
  int right(int index) { return (2 * index) + 2; }
}