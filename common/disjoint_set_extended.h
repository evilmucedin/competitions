#pragma once

#include "base.h"

template <class TTData>
class DisjointSetExtended
{
public:
    using TData = TTData;

protected:
    unordered_map<TData, unsigned> m;
    vector<unsigned> p;
    vector<unsigned> rank;
    vector<unsigned> vsize;
    stack<unsigned> ts;
	unsigned unions = 0;

public:
    void Reserve(unsigned size)
    {
        m.reserve(size);
        p.reserve(size);
        rank.reserve(size);
        vsize.reserve(size);
    }
    
	unsigned Size() const { return unsigned(p.size()); }
	unsigned GetUnions() const { return unions; }

	unsigned GetIndex(const TData& v)
    {
        auto it = m.find(v);
        if (it != m.end())
            return it->second;
		unsigned index = unsigned(Size());
        m[v] = index;
        p.push_back(index);
        rank.push_back(0);
        vsize.push_back(1);
        return index;
    }
    
	unsigned Find(const TData& v)
    {
		unsigned x = GetIndex(v);
		unsigned px = p[x];
        if (px == x)
            return px;
		unsigned ppx = p[px];
        if (ppx == px)
            return px;
        do
        {
            ts.push(x);
            x = px;
            px = ppx;
            ppx = p[px];
        } while (px != ppx);
        while (!ts.empty())
        {
            x = ts.top();
            p[x] = ppx;
            ts.pop();
        }
        return ppx;
    }

	unsigned GetSize(const TData& v)
    {
        return vsize[Find(v)];
    }

    void Union(const TData& v1, const TData& v2)
    {
        UnionI(Find(v1), Find(v2));
    }

protected:
    void UnionI(unsigned i1, unsigned i2)
    {
        if (i1 == i2)
            return;
        ++unions;
        if (rank[i1] > rank[i2])
        {
            p[i2] = i1;
            vsize[i1] += vsize[i2];
        }
        else
        {
            p[i1] = i2;
            if (rank[i1] == rank[i2])
                ++rank[i1];
            vsize[i2] += vsize[i1];
        }
    }
};
