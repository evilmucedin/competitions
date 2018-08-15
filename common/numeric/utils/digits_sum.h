#pragma once

#include "../../base.h"

namespace {

uint64_t DigitsSum(uint64_t x, uint64_t base = 10)
{
	uint64_t s = 0;
	for (; x; x /= base)
		s += x % base;
	return s;
}

}  // namespace