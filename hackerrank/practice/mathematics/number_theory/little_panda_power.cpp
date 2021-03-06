// https://www.hackerrank.com/challenges/littlepandapower

#include "common/modular/arithmetic.h"
#include "common/stl/base.h"

int main_little_panda_power() {
  unsigned T;
  cin >> T;
  for (unsigned it = 0; it < T; ++it) {
    uint64_t A, X;
    int64_t B;
    cin >> A >> B >> X;
    cout << modular::TArithmetic_C32U::PowSSafe(A, B, X) << endl;
  }
  return 0;
}
