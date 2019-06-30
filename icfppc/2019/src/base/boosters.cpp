#include "base/boosters.h"

#include "base/booster_type.h"
#include "common/assert_exception.h"
#include <algorithm>
#include <cassert>

namespace base {
Boosters::Boosters() { Clear(); }

void Boosters::Clear() { std::fill(count.begin(), count.end(), 0); }

bool Boosters::Available(BoosterType type) const {
  assert(type < BoosterType::END);
  return count[unsigned(type)] > 0;
}

unsigned Boosters::Count(BoosterType type) const {
  assert(type < BoosterType::END);
  return count[unsigned(type)];
}

void Boosters::Add(BoosterType type) {
  assert(type < BoosterType::END);
  count[unsigned(type)] += 1;
}

void Boosters::Remove(BoosterType type) {
  assert(type < BoosterType::END);
  Assert(Available(type), "Using unavailable booster.");
  count[unsigned(type)] -= 1;
}
}  // namespace base