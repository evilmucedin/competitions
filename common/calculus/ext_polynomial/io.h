#pragma once

#include "common/calculus/ext_polynomial/function.h"
#include "common/calculus/ext_polynomial/term.h"
#include <iostream>

namespace calculus {
namespace ext_polynomial {
template <class TValue, class TTerm>
inline std::ostream& operator<<(std::ostream& s,
                                const Function<TValue, TTerm>& f) {
  if (f.Empty()) {
    s << 0;
  } else {
    for (auto& t : f.terms) s << t.ToString("x");
  }
  return s;
}
}  // namespace ext_polynomial
}  // namespace calculus
