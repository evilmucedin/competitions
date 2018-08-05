#include "message.h"
#include "lemming.h"

#include "common/base.h"
#include "common/disjoint_set.h"

int main_lemming()
{
	int nodes = NumberOfNodes();
	int node_id = MyNodeId();
	int64_t rows = GetRows();
	int64_t columns = GetColumns();
	if (rows <= 2 * nodes)
	{
		if (node_id == 0)
			nodes = 1;
        else
		    return 0;
	}

	int64_t ifirst = node_id * rows / nodes;
	int64_t ilast = (node_id + 1) * rows / nodes;
	assert(ilast - ifirst >= 1);

    DisjointSet ds((ilast - ifirst + 1) * columns);
    int index = 0;
    for (int64_t i = ifirst; i < ilast; ++i)
    {
        for (int64_t j = 0; j < columns; ++j, ++index)
        {
            char c = GetDirection(i, j);
            if (c == '^')
            {
                if (i != ifirst)
                    ds.Union(index, index - columns);
            }
            else if (c == '>')
            {
                if (j + 1 != columns)
                    ds.Union(index, index + 1);
            }
            else if (c == 'v')
            {
                if (i + 1 != ilast)
                    ds.Union(index, index + columns);
            }
            else if (c == '<')
            {
                if (j != 0)
                    ds.Union(index, index - 1);
            }
            else
                assert(false);
        }
    }
    
    int32_t total_unions = 0;
    if (node_id != 0)
    {
        assert(ifirst > 0);
        Receive(node_id - 1);
        total_unions = GetInt(node_id - 1);
        for (int64_t j = 0; j < columns; ++j)
        {
            index = GetInt(node_id - 1) + (ilast - ifirst) * columns;
            if ((GetDirection(ifirst - 1, j) == 'v') || (GetDirection(ifirst, j) == '^'))
                ds.Union(index, j);
        }
    }
    total_unions += int32_t(ds.GetUnions());

    if (node_id != nodes - 1)
    {
        PutInt(node_id + 1, total_unions);
        unordered_map<int, int> m; int l = 0;
        for (int64_t j = 0; j < columns; ++j)
        {
            index = ds.Find((ilast - ifirst - 1) * columns + j);
            auto it = m.find(index);
            if (it == m.end())
            {
                m[index] = l++;
            }
            PutInt(node_id + 1, m[index]);
        }
        Send(node_id + 1);
    }
    else
    {
        cout << rows * columns - total_unions << endl;
    }
	return 0;
}