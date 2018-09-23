#pragma once

#include "none.h"

// Support only VectorStaticSize from LinearAlgebra.
class STActionRotateVector : public STActionNone
{
protected:
	template<class TVector>
	static void Rotate(TVector& v, unsigned r)
	{
		if (r == 0) return;
		static thread_local TVector vt;
		unsigned size = v.Size();
		for (unsigned i = 0; i < size; ++i)
			vt((i + r) % size) = v(i);
		v.swap(vt);
	}

public:
	using TBase = STActionNone;
	using TSelf = STActionRotateVector;
	static const bool is_none = false;

	unsigned r = 0;

	bool IsEmpty() const { return r == 0; }
	void Clear() { r = 0; };

	template<class TNode>
	void Add(TNode* node, unsigned _r)
	{
		Rotate(node->info.segment_sum, _r);
		r = (r + _r) % node->info.segment_sum.Size();
	}

	template<class TNode>
	void Apply(TNode* node)
	{
		if (IsEmpty()) return;
        if (node->IsLeaf())
		    Rotate(node->GetData(), r);
        else
        {
		    node->l->AddAction(r);
		    node->r->AddAction(r);
        }
		r = 0;
	}
};