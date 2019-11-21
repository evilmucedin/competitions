#pragma once

#include "common/modular.h"

#include <vector>

class TesterInterpolation {
 public:
  static const unsigned power1 = 10, power2 = 100, n = 1000, k = 100;

 protected:
  std::vector<TModularD> vp1, vp2;

 public:
  void Init();
  TesterInterpolation();

  static bool TestSumOfPowers(unsigned power, const std::vector<TModularD>& vp);
  static bool TestBaseNewtonPolynomial(unsigned power,
                                       const std::vector<TModularD>& vp);
  static bool TestAll(unsigned power, const std::vector<TModularD>& vp);

  bool TestAll() const;
};
