#pragma once

#include "common/calculus/ext_polynomial/function.h"
#include "common/calculus/ext_polynomial/multivariable/term.h"
#include "common/calculus/multivariable/point.h"
#include <algorithm>
#include <vector>

namespace calculus {
namespace ext_polynomial {
namespace multivariable {
template <class TValue>
using SVFunction = calculus::ext_polynomial::Function<TValue>;

template <class TValue, unsigned _dim>
class Function {
 public:
  using TPoint = calculus::multivariable::Point<TValue, _dim>;
  using TTerm = Term<TValue, _dim>;
  using TFunctionSV = SVFunction<TValue>;
  static const unsigned dim = _dim;

  std::vector<TTerm> v;

  Function() {}
  Function(const TValue& x) { v.emplace_back(TTerm(x)); }
  Function(const TTerm& t) { v.emplace_back(t); }
  Function(const TFunctionSV& f) {
    static_assert(dim == 1);
    for (auto& t : f.v) v.emplace_back(TTerm(t));
  }

  TFunctionSV ToSVFunction() const {
    static_assert((dim == 0) || (dim == 1));
    TFunctionSV f;
    for (auto& t : v) f.AddTerm(t.ToSVTerm());
    return f;
  }

  TFunctionSV ToTValue() const {
    static_assert(dim == 0);
    TValue s(0);
    for (auto& t : v) s += v.a;
    return s;
  }

  bool Empty() const { return v.empty(); }

  void AddTerm(const TTerm& t) { v.emplace_back(t); }

  TValue Get(const TPoint& p) const {
    TValue s(0);
    for (const auto& t : v) s += t.Get(p);
    return s;
  }

  bool UnusedVariable(unsigned index) const {
    for (auto& t : v) {
      if (!UnusedVariable(index)) return false;
    }
    return true;
  }

  bool SortedTerms() const {
    for (size_t i = 1; i < v.size(); ++i) {
      if (v[i].tp < v[i - 1].tp) return false;
    }
    return true;
  }

  void SortTerms() {
    if (!SortedTerms()) {
      std::sort(v.begin(), v.end(),
                [](auto& x, auto& y) { return x.tp < y.tp; });
    }
  }

  void CompressSorted() {
    // ...
  }

  void Compress() {
    SortTerms();
    CompressSorted();
  }
};

template <unsigned dim>
using DFunction = Function<double, dim>;
}  // namespace multivariable
}  // namespace ext_polynomial
}  // namespace calculus
