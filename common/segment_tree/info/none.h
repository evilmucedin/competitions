#pragma once

namespace st {
namespace info {
class None {
 public:
  using TSelf = None;

  static const bool is_none = true;
  static const bool use_data = false;

  template <class TData>
  void UpdateData(const TData& data) {}

  void UpdateLR(const TSelf& l, const TSelf& r) {}
};
}  // namespace info
}  // namespace st
