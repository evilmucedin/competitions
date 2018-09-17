#pragma once

template<class TSumType, class TInfo>
class BSTInfoSum : public TInfo
{
public:
	using TBase = TInfo;
	using TSelf = BSTInfoSum<TSumType, TInfo>;
	static const bool is_none = false;
	static const bool use_data = true;

	TSumType sum;

	template<class TNode>
	void Update(TNode* node)
	{
		TBase::Update(node);
		sum = node->data + (node->l ? node->l->info.sum : TSumType(0)) + (node->r ? node->r->info.sum : TSumType(0));
	}
};
