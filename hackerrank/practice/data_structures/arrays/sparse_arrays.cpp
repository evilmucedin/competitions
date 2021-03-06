// https://www.hackerrank.com/challenges/sparse-arrays

#include "common/stl/base.h"

#include <string>
#include <unordered_map>

int main_sparse_arrays() {
  unsigned n, q;
  string s;
  unordered_map<string, unsigned> m;
  cin >> n;
  for (unsigned i = 0; i < n; ++i) {
    cin >> s;
    m[s] += 1;
  }
  cin >> q;
  for (unsigned i = 0; i < q; ++i) {
    cin >> s;
    cout << m[s] << endl;
  }
  return 0;
}
