// https://www.hackerrank.com/challenges/best-divisor

#include "common/base.h"
#include "common/numeric/utils.h"

int main_best_divisor()
{
	auto isbetter = [](unsigned x, unsigned y)
	{
		uint64_t dsx = NumbericUtils::DigitsSum(x), dsy = NumbericUtils::DigitsSum(y);
		return (dsx > dsy) || ((dsx == dsy) && (x < y));
	};
	unsigned n;
	cin >> n;
	unsigned best = 1;
	for (unsigned d = 2; d <= n; ++d)
	{
		if ((n % d) != 0) continue;
		if (isbetter(d, best))
			best = d;
	}
	cout << best << endl;
	return 0;
}