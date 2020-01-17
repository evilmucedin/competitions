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

  std::vector<TTerm> terms;

  Function() {}
  Function(const TValue& x) { terms.emplace_back(TTerm(x)); }
  Function(const TTerm& t) { terms.emplace_back(t); }
  Function(const TFunctionSV& f) {
    static_assert(dim == 1);
    for (auto& t : f.terms) terms.emplace_back(TTerm(t));
  }

  TFunctionSV ToSVFunction() const {
    static_assert((dim == 0) || (dim == 1));
    TFunctionSV f;
    for (auto& t : terms) f.AddTerm(t.ToSVTerm());
    return f;
  }

  TFunctionSV ToTValue() const {
    static_assert(dim == 0);
    TValue s(0);
    for (auto& t : terms) s += terms.a;
    return s;
  }

  bool Empty() const { return terms.empty(); }

  void AddTerm(const TTerm& t) { terms.emplace_back(t); }

  TValue Get(const TPoint& p) const {
    TValue s(0);
    for (const auto& t : terms) s += t.Get(p);
    return s;
  }

  size_t Size() const { return terms.size(); }
  TTerm& operator()(size_t index) { return terms[index]; }
  const TTerm& operator()(size_t index) const { return terms[index]; }

  bool UnusedVariable(unsigned index) const {
    for (auto& t : terms) {
      if (!UnusedVariable(index)) return false;
    }
    return true;
  }

  bool SortedTerms() const {
    for (size_t i = 1; i < terms.size(); ++i) {
      if (terms[i].tp < terms[i - 1].tp) return false;
    }
    return true;
  }

  void SortTerms() {
    if (!SortedTerms()) {
      std::sort(terms.begin(), terms.end(),
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