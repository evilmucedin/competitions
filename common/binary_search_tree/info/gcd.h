#pragma once

#include "../info.h"
#include "../../numeric/utils/gcd.h"

template<class TGCDType>
class BSTInfoGCD : public BSTInfoNone
{
public:
	using TBase = BSTInfoNone;
	using TSelf = BSTInfoGCD<TGCDType>;
	static const bool is_none = false;
	static const bool use_data = true;

	TGCDType gcd;

	template<class TNode>
	void Update(TNode* node)
	{
		TBase::Update(node);
		gcd = GCD<TGCDType>(node->data, GCD<TGCDType>((node->l ? node->l->info.gcd : 0), (node->r ? node->r->info.gcd : 0)));
	}
};