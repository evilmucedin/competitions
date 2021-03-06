// https://www.hackerrank.com/challenges/russian-peasant-exponentiation

#include "common/modular/proxy/complex.h"
#include "common/stl/base.h"

using TComplexProxy = modular::proxy::Complex<false>;
using TComplex = TComplexProxy::TComplex;

int main_russian_peasant_exponentiation() {
  TComplexProxy p;
  unsigned T;
  cin >> T;
  for (unsigned it = 0; it < T; ++it) {
    int64_t a, b, k, m;
    cin >> a >> b >> k >> m;
    p.SetMod(m);
    TComplex f = p.PowU(TComplex(a, b), k);
    cout << f.re << " " << f.im << endl;
  }
  return 0;
}
