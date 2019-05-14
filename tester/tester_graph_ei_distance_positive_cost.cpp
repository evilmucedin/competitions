#include "tester/tester_graph_ei_distance_positive_cost.h"

#include "common/graph/graph_ei/edge_cost_proxy.h"
#include "common/hash.h"
#include "common/heap/binary_heap.h"
#include "common/heap/binomial_ukey_value_map.h"
#include "common/heap/dheap_ukey_pos_map.h"
#include "common/heap/dheap_ukey_value_map.h"
#include "common/timer.h"

#include "tester/graph_ei_distance_positive_cost.h"

#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>

template <class TData>
using TBinaryHeap = heap::BinaryHeap<TData>;
template <class TData>
using TDHeap2 = heap::DHeap<2u, TData>;
template <class TData>
using TDHeap4 = heap::DHeap<4u, TData>;
template <class TData>
using TDHeap8 = heap::DHeap<8u, TData>;
template <class TData>
using TDHeap16 = heap::DHeap<16u, TData>;

TesterGraphEIDistancePositiveCost::TesterGraphEIDistancePositiveCost(
    unsigned graph_size, unsigned edges_per_node)
    : g(graph_size) {
  size_t h = 0, mask = (1u << 30) - 1u;
  for (unsigned i = 0; i < graph_size; ++i) {
    for (unsigned j = 0; j < edges_per_node; ++j) {
      h = hash_combine(h, i);
      g.AddEdge(i, h % graph_size, h & mask);
    }
  }
}

template <template <class TData> class THeap>
size_t TesterGraphEIDistancePositiveCost::TestCBH(
    const std::string& name) const {
  Timer t;
  size_t h = 0, max_cost = -1ull;
  std::vector<uint64_t> v;
  for (unsigned i = 0; i < g.Size(); ++i) {
    v = DistanceFromSourcePositiveCost_CBH<THeap>(g, edge_proxy, i, max_cost);
    for (uint64_t d : v) h = hash_combine(h, d);
  }
  std::cout << "Test results  [" << name << "]: " << h << "\t"
            << t.GetMilliseconds() << std::endl;
  return h;
}

template <class THeap>
size_t TesterGraphEIDistancePositiveCost::TestKPM(
    const std::string& name) const {
  Timer t;
  size_t h = 0, max_cost = -1ull;
  std::vector<uint64_t> v;
  for (unsigned i = 0; i < g.Size(); ++i) {
    v = DistanceFromSourcePositiveCost_KPM<THeap>(g, edge_proxy, i, max_cost);
    for (uint64_t d : v) h = hash_combine(h, d);
  }
  std::cout << "Test results  [" << name << "]: " << h << "\t"
            << t.GetMilliseconds() << std::endl;
  return h;
}

template <class THeap>
size_t TesterGraphEIDistancePositiveCost::TestKVM(
    const std::string& name) const {
  Timer t;
  size_t h = 0, max_cost = -1ull;
  std::vector<uint64_t> v;
  for (unsigned i = 0; i < g.Size(); ++i) {
    v = DistanceFromSourcePositiveCost_KVM<THeap>(g, edge_proxy, i, max_cost);
    for (uint64_t d : v) h = hash_combine(h, d);
  }
  std::cout << "Test results  [" << name << "]: " << h << "\t"
            << t.GetMilliseconds() << std::endl;
  return h;
}

bool TesterGraphEIDistancePositiveCost::TestAll() {
  std::unordered_set<size_t> hs;
  hs.insert(TestCBH<TBinaryHeap>("  BH"));
  hs.insert(TestCBH<TDHeap2>("DH 2"));
  hs.insert(TestCBH<TDHeap4>("DH 4"));
  hs.insert(TestCBH<TDHeap8>("DH 8"));
  hs.insert(TestCBH<TDHeap16>("DH16"));
  hs.insert(TestKPM<heap::DHeapUKeyPosMap<2, uint64_t>>("KPM2"));
  hs.insert(TestKPM<heap::DHeapUKeyPosMap<4, uint64_t>>("KPM4"));
  hs.insert(TestKPM<heap::DHeapUKeyPosMap<8, uint64_t>>("KPM8"));
  hs.insert(TestKVM<heap::DHeapUKeyValueMap<2, uint64_t>>("DM 2"));
  hs.insert(TestKVM<heap::DHeapUKeyValueMap<4, uint64_t>>("DM 4"));
  hs.insert(TestKVM<heap::DHeapUKeyValueMap<8, uint64_t>>("DM 8"));
  hs.insert(TestKVM<heap::DHeapUKeyValueMap<16, uint64_t>>("DM16"));
  hs.insert(TestKVM<heap::BinomialUKeyValueMap<uint64_t>>("BNML"));
  return hs.size() == 1;
}

bool TestGraphEIDistancePositiveCost(bool time_test) {
  if (time_test) {
    TesterGraphEIDistancePositiveCost t1(5000, 4), t2(2000, 500);
    return t1.TestAll() && t2.TestAll();
  } else {
    TesterGraphEIDistancePositiveCost t(100, 4);
    return t.TestAll();
  }
}
