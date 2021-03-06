#include "common/factorization/table/mobius.h"
#include "common/factorization/table/utils/square_free_count.h"
#include "common/modular_io.h"
#include "common/numeric/utils/pow.h"
#include "common/numeric/utils/usqrt.h"
#include "common/stl/base.h"

// Time: O(N^(1/2) * log(N))
// Memory: O(N^(1/2))
int main_632() {
  uint64_t N = PowU<uint64_t>(10, 16), NSqrt = USqrt(N);
  factorization::table::Mobius mobius(NSqrt);
  vector<unsigned> vk(NSqrt + 1, 0);
  for (uint64_t p : mobius.GetPrimes()) {
    for (uint64_t l = p; l <= NSqrt; l += p) ++vk[l];
  }
  vector<uint64_t> vsk(*max_element(vk.begin(), vk.end()) + 1, 0);
  for (uint64_t a = 1; a <= NSqrt; ++a) {
    vsk[vk[a]] += SquareFreeCount(mobius, N / (a * a));
  }
  TModularD m = 1;
  for (uint64_t sk : vsk) m *= TModularD(sk);
  cout << m << endl;
  return 0;
}
