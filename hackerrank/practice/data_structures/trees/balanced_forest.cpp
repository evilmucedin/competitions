// https://www.hackerrank.com/challenges/balanced-forest

#include "common/base.h"
#include "common/graph/base_tree.h"
#include "common/graph/lca.h"
#include "common/vector/utils.h"

namespace {
	struct BalancedForest
	{
		unsigned n;
		vector<int64_t> vc;
		BaseTree t;
		vector<int64_t> vstc;

		int64_t RunR(unsigned node, unsigned parent)
		{
			int64_t total = vc[node];
			for (unsigned i : t.edges[node])
			{
				if (i == parent)
					continue;
				total += RunR(i, node);
			}
			vstc[node] = total;
			return total;
		}

		void Solve()
		{
			cin >> n;
			vc = ReadVector<int64_t>(n);
			t.Resize(n);
			t.ReadEdges();
			vstc.resize(n);
			RunR(0, -1);
			int64_t total = vstc[0];
			unordered_map<int64_t, unsigned> sc1;
			unordered_map<int64_t, vector<unsigned>> sc2;
			int64_t best = total;
			for (unsigned i = 0; i < n; ++i)
			{
				int64_t current = vstc[i];
				if (2 * current == total)
				{
					best = min(best, current);
					continue;
				}
				if (2 * current > total)
					current = total - current;
				if (3 * current < total)
				{
					sc2[total - current].push_back(i);
				}
				else
				{
					if (sc1.find(current) != sc1.end())
						best = min(best, current);
					else
						sc1[current] = i;
				}
			}
			LCA lca(t);
			for (auto p : sc1)
			{
				if (p.first >= best)
					continue;
				unsigned i = p.second;
				for (unsigned j : sc2[2 * p.first])
				{
					unsigned k = lca.GetLCA(i, j);
					if (k == i)
					{
						if (2 * vstc[i] > total)
						{
							best = p.first;
							break;
						}
					}
					else
					{
						if (2 * vstc[i] < total)
						{
							best = p.first;
							break;
						}
					}
				}
			}
			if (best == total)
				cout << -1 << endl;
			else
				cout << 3 * best - total << endl;
		}
	};
}

int main_balanced_forest()
{
	unsigned T;
	cin >> T;
	for (unsigned it = 0; it < T; ++it)
	{
		BalancedForest s;
		s.Solve();
	}
	return 0;
}
