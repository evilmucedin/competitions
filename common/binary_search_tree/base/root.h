#pragma once

namespace bst {
template <class TNode>
inline TNode* Root(TNode* node) {
  static_assert(TNode::use_parent, "use_parent should be true");
  if (!node) return node;
  for (; node->p;) node = node->p;
  return node;
}
}  // namespace bst
