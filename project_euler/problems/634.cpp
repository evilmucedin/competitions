#include "common/factorization/primes_list.h"
#include "common/factorization/utils/cube_free_count.h"
#include "common/factorization/utils/square_free.h"
#include "common/numeric/utils/pow.h"
#include "common/numeric/utils/usqrt.h"
#include "common/stl/base.h"

#include <functional>

// Time: O(N^(1/3) * log(log(N)))
// Memory: O(N^(1/3))
int main_634() {
  uint64_t M = 2 * PowU(10u, 6), N = 9 * PowU<uint64_t>(10, 18);
  factorization::PrimesList primes_list(M);
  uint64_t SqrtN = USqrt(N), s = 0;
  for (uint64_t b = 2; b * b * b <= N / 4; ++b) {
    if (SquareFree(primes_list.FactorizeTable(b))) {
      s += USqrt(N / (b * b * b)) - 1;
    }
  }
  for (uint64_t b = 2; b * b * b <= SqrtN; ++b) {
    if (primes_list.IsPrime(b)) s -= 1;
    s += CubeFreeCount(SqrtN / (b * b * b), primes_list.GetPrimes());
  }
  cout << s << endl;
  return 0;
}
