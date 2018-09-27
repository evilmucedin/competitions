#pragma once

#include "../base.h"
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

class LongUnsigned
{
protected:
    std::vector<unsigned> data;

public:
	using iterator = unsigned*;
	using const_iterator = const unsigned*;

    LongUnsigned() {}
    LongUnsigned(unsigned u) { if (u) data.push_back(u); }
    LongUnsigned(uint64_t u)
    {
        data.push_back(unsigned(u));
        data.push_back(unsigned(u >> 32));
        Normalize();
    }

    void Normalize() { for (; (data.size() > 0) && (data.back() == 0);) data.pop_back(); }
    bool Empty() const { return data.empty(); }
    size_t Size() const { return data.size(); }

	iterator begin() { return &data.front(); }
	const_iterator begin() const { return &data.front(); }
	iterator end() { return begin() + data.size(); }
	const_iterator end() const { return begin() + data.size(); }
    void swap(LongUnsigned& lu) { data.swap(lu.data); }

    bool operator==(const LongUnsigned& lu) const { return data == lu.data; }
    bool operator!=(const LongUnsigned& lu) const { return data != lu.data; }

    bool operator<(const LongUnsigned& lu) const
    {
        if (Size() < lu.Size()) return true;
        if (lu.Size() < Size()) return false;
        for (size_t i = Size(); i; )
        {
            --i;
            if (data[i] < lu.data[i]) return true;
            if (data[i] > lu.data[i]) return false;
        }
        return false;
    }

    bool operator<=(const LongUnsigned& lu) const
    {
        if (Size() < lu.Size()) return true;
        if (lu.Size() < Size()) return false;
        for (size_t i = Size(); i; )
        {
            --i;
            if (data[i] < lu.data[i]) return true;
            if (data[i] > lu.data[i]) return false;
        }
        return true;
    }

    bool operator>(const LongUnsigned& lu) const { return !(lu <= (*this)); }
    bool operator>=(const LongUnsigned& lu) const { return !(lu < (*this)); }

    LongUnsigned operator+(unsigned u) const
    {
        if (u == 0) return (*this);
        LongUnsigned lu;
        lu.data.reserve(data.size());
        uint64_t t64 = u;
        for (unsigned ud : data)
        {
            t64 += ud;
            lu.data.push_back(unsigned(t64));
            t64 >>= 32;
        }
        if (t64)
            lu.data.push_back(unsigned(t64));
        return lu;
    }

    LongUnsigned operator*(unsigned u) const
    {
        if (u == 0) return LongUnsigned();
        LongUnsigned lu;
        lu.data.reserve(data.size() + 1);
        uint64_t u64 = u, t64 = 0;
        for (unsigned ud : data)
        {
            t64 += u64 * ud;
            lu.data.push_back(unsigned(t64));
            t64 >>= 32;
        }
        if (t64)
            lu.data.push_back(unsigned(t64));
        return lu;
    }

    LongUnsigned operator/(unsigned u) const
    {
        assert(u);
        if (data.empty()) return LongUnsigned();
        LongUnsigned lu;
        lu.data.reserve(data.size());
        uint64_t u64 = u, t64 = data.back();
        if (t64 >= u64)
        {
            lu.data.push_back(t64 / u64);
            t64 %= u64;
        }
        for (auto p = end() - 2, pb = begin(); p >= pb;)
        {
            t64 = ((t64 << 32) + *p--);
            lu.data.push_back(t64 / u64);
            t64 %= u64;
        }
        std::reverse(lu.begin(), lu.end());
        return lu;
    }

    unsigned operator%(unsigned u) const
    {
        assert(u);
        if (data.empty()) return 0;
        if ((u & (u - 1)) == 0) return (data[0] % u);
        uint64_t u64 = u, t64 = data.back() % u64;
        for (auto p = end() - 2, pb = begin(); p >= pb;)
            t64 = ((t64 << 32) + *p--) % u64;
        return unsigned(t64);
    }

    LongUnsigned operator+(const LongUnsigned& r) const
    {
        if (Empty()) return r;
        if (r.Empty()) return *this;
        LongUnsigned lu;
        size_t l = std::max(data.size(), r.data.size());
        lu.data.reserve(l);
        uint64_t t64 = 0;
        for (unsigned i = 0; i < l; ++i)
        {
            t64 += (i < data.size()) ? data[i] : 0;
            t64 += (i < r.data.size()) ? r.data[i] : 0;
            lu.data.push_back(unsigned(t64));
            t64 >>= 32;
        }
        if (t64)
            lu.data.push_back(unsigned(t64));
        return lu;
    }

    LongUnsigned operator-(const LongUnsigned& r) const
    {
        if (r.Empty()) return *this;
        assert(r <= *this);
        LongUnsigned lu;
        size_t l = data.size();
        lu.data.reserve(l);
        int64_t i64 = 0;
        for (unsigned i = 0; i < l; ++i)
        {
            i64 += int64_t(data[i]);
            i64 -= (i < r.data.size()) ? int64_t(r.data[i]) : 0;
            lu.data.push_back(unsigned(i64));
            i64 >>= 32;
        }
        assert(!i64);
        lu.Normalize();
        return lu;
    }

    LongUnsigned& operator+=(unsigned u) { LongUnsigned t = (*this + u); swap(t); return *this; }
    LongUnsigned& operator*=(unsigned u) { LongUnsigned t = (*this * u); swap(t); return *this; }
    LongUnsigned& operator/=(unsigned u) { LongUnsigned t = (*this / u); swap(t); return *this; }
    LongUnsigned& operator+=(const LongUnsigned& r) { LongUnsigned t = (*this + r); swap(t); return *this; }
    LongUnsigned& operator-=(const LongUnsigned& r) { LongUnsigned t = (*this - r); swap(t); return *this; }

    static LongUnsigned Parse(const std::string& s, unsigned base = 10)
    {
        assert(base == 10);
        LongUnsigned lu;
        for (char c : s)
        {
            lu *= base;
            assert((c >= '0') && (c <= '9'));
            lu += unsigned(c - '0');
        }
        return lu;
    }

    static LongUnsigned Read(unsigned base = 10)
    {
        std::string s;
        std::cin >> s;
        return Parse(s, base);
    }

    std::vector<unsigned> ToVector(unsigned base) const
    {
        LongUnsigned t(*this);
        std::vector<unsigned> v;
        for (; !t.Empty(); t /= base)
            v.push_back(t % base);
        return v;
    }

    std::string ToString(unsigned base = 10) const
    {
        if (Empty()) return "0";
        assert(base == 10);
        std::vector<unsigned> v = ToVector(base);
        std::reverse(v.begin(), v.end());
        std::string s; s.reserve(v.size());
        for (unsigned u : v)
            s.push_back('0' + char(u));
        return s;
    }

    void Write(bool add_eod = true, unsigned base = 10) const
    {
        std::string s = ToString(base);
        std::cout << s;
        if (add_eod) std::cout << std::endl;
    }
};
