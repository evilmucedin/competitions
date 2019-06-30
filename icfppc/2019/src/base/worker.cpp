#include "base/worker.h"

#include "base/action.h"
#include "base/action_type.h"
#include "base/booster_type.h"
#include "base/boosters.h"
#include "base/direction.h"
#include "base/manipulator.h"
#include "base/point.h"
#include "base/rotation.h"
#include "base/world.h"
#include "common/assert_exception.h"
#include <algorithm>
#include <cassert>

namespace base {
Worker::Worker(const Point& _p, World& world) : p(_p), pworld(&world) {
  time_fast_wheels = -1;
  time_drill = -1;
}

const Point& Worker::Location() const { return p; }

void Worker::Wrap() {
  assert(pworld);
  manipulators.Wrap(p, pworld->GetMap());
}

void Worker::PickupItem() {
  assert(pworld);
  auto item = pworld->GetMap().PickupItem(p);
  if (item != BoosterType::NONE) {
    pworld->GetBoosters().Add(item);
  }
}

void Worker::operator()(const Action& action) {
  assert(pworld);
  PickupItem();
  switch (action.type) {
    case ActionType::MOVE_UP:
    case ActionType::MOVE_DOWN:
    case ActionType::MOVE_LEFT:
    case ActionType::MOVE_RIGHT: {
      auto& map = pworld->GetMap();
      bool fast_wheels_enabled = time_fast_wheels > pworld->GetTime();
      bool drill_enabled = time_drill > pworld->GetTime();
      Direction d(action.type);
      p = d(p);
      Assert(map.Inside(p), "Moving worker outside of map.");
      if (drill_enabled) {
        if (map.Obstacle(p)) {
          map.Drill(p);
        }
      } else {
        Assert(!map.Obstacle(p), "Moving worker to obstacle.");
      }
      Wrap();
      if (fast_wheels_enabled) {
        Point pt = d(p);
        if (map.Inside(pt)) {
          if (drill_enabled || !map.Obstacle(pt)) {
            PickupItem();
            p = pt;
            if (drill_enabled && map.Obstacle(p)) {
              map.Drill(p);
            }
            Wrap();
          }
        }
      }
    } break;
    case ActionType::DO_NOTHING:
      break;
    case ActionType::ROTATE_CLOCKWISE:
    case ActionType::ROTATE_COUNTERCLOCKWISE: {
      Rotation r(action.type);
      manipulators.Rotate(r.GetType());
      Wrap();
    } break;
    case ActionType::ATTACH_MANIPULATOR:
      Assert(pworld->GetBoosters().Available(BoosterType::EXTENSION),
             "Booster EXTENSION not aviable for use.");
      pworld->GetBoosters().Remove(BoosterType::EXTENSION);
      manipulators.Add(Point{action.x, action.y});
      Wrap();
      break;
    case ActionType::ATTACH_FAST_WHEELS:
      Assert(pworld->GetBoosters().Available(BoosterType::FAST_WHEELS),
             "Booster FAST_WHEELS not aviable for use.");
      pworld->GetBoosters().Remove(BoosterType::FAST_WHEELS);
      time_fast_wheels =
          std::max(time_fast_wheels, pworld->GetTime()) + TIME_FAST_WHEELS;
      break;
    case ActionType::USING_DRILL:
      Assert(pworld->GetBoosters().Available(BoosterType::DRILL),
             "Booster DRILL not aviable for use.");
      pworld->GetBoosters().Remove(BoosterType::DRILL);
      time_drill = std::max(time_drill, pworld->GetTime()) + TIME_DRILL;
      break;
    case ActionType::SET_BEACON:
      Assert(pworld->GetBoosters().Available(BoosterType::TELEPORT),
             "Booster TELEPORT not aviable for use.");
      pworld->GetBoosters().Remove(BoosterType::TELEPORT);
      pworld->GetMap().AddBeacon(p);
      break;
    case ActionType::SHIFT: {
      Point pt{action.x, action.y};
      Assert(pworld->GetMap().CheckBeacon(pt),
             "Beacon is not exist on required location.");
      p = pt;
      Wrap();
    } break;
    default:
      Assert(false, "Unknown action.");
  }
}
}  // namespace base