#include "solvers/worker/simple.h"

#include "base/action.h"
#include "base/action_type.h"
#include "base/ext/path.h"
#include "base/ext/world.h"
#include <cassert>
#include <string>

namespace solvers {
namespace worker {
std::string Simple::Name() const { return "simple"; }

base::Action Simple::NextMove(base::ext::World& world) {
  const auto& map = world.GetMap();
  const auto& worker = world.WGet();
  return base::ext::Path::PathToTarget(map, map.Index(worker.Location()),
                                       map.Unwrapped());
}

base::ActionsList Simple::Solve(base::ext::World& world) {
  base::ActionsList al;
  for (; !world.Solved();) {
    world.WGet().PickupItem();
    auto a = NextMove(world);
    assert(a.type != base::ActionType::DO_NOTHING);
    assert(a.type != base::ActionType::END);
    world.WApply(a);
    al.push_back(a);
  }
  return al;
}
}  // namespace worker
}  // namespace solvers
