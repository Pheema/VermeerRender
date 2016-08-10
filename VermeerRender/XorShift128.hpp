#pragma once

#include <climits>
#include <random>

namespace VermeerRender
{
    // From: https://en.wikipedia.org/wiki/Xorshift
    class XorShift128
    {
    public:
        static constexpr unsigned min() { return 0u; }
        static constexpr unsigned max() { return UINT_MAX; }
        unsigned operator()() { return random(); }
        XorShift128() {};
        explicit XorShift128(unsigned seed) : w(seed) {};

        unsigned
        random()
        {
            unsigned t = w;
            t ^= t << 11;
            t ^= t >> 8;
            x = y; y = z; z = w;
            w ^= w >> 19;
            w ^= t;
            return w;
        }

    private:
        unsigned x = 123456789u;
        unsigned y = 362436069u;
        unsigned z = 521288629u;
        unsigned w = 486213789u;
    };
}