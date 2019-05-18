#pragma once

#include "common/factorization/primes_list_extended/mertens.h"
#include "common/numeric/utils/usqrt.h"

namespace factorization {
class Mertens {
 protected:
  uint64_t u;
  ple::Mertens mertens;

  int S(uint64_t y) const {
    uint64_t v = USqrt(y), k = y / (v + 1);
    int r = 1 + int(k) * mertens(v);
    for (uint64_t n = y / u + 1; n <= k; ++n) r -= mertens(y / n);
    for (uint64_t n = 1; n <= v; ++n) r -= int(y / n) * mertens.GetMobius(n);
    return r;
  }

 public:
  Mertens(unsigned _u) : u(_u), mertens(u) { mertens.Precalc(); }

  int GetMobius(uint64_t x) const { return mertens.GetMobius(x); }

  int GetMertens(uint64_t x) const {
    if (x <= u) return mertens(x);
    int r = 0;
    for (uint64_t n = 1; n <= x / u; ++n) {
      int m = mertens.GetMobius(n);
      if (m) r += m * S(x / n);
    }
    return r;
  }

  int operator()(uint64_t x) const { return GetMertens(x); }
};
}  // namespace factorization
