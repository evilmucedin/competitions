#pragma once

class STActionNone {
 public:
  using TSelf = STActionNone;
  static const bool is_none = true;

  bool IsEmpty() const { return true; }
  void Clear(){};
  template <class TNode, class TActionValue>
  void Add(TNode* node, const TActionValue& value) {}
  template <class TNode>
  void Apply(const TNode* node) {}
};
