#pragma once

#include <cstdint>

namespace hf
{

class Utility
{
public:
    static double HeightShortToDouble(int32_t s);
    static float HeightShortToFloat(int32_t s);

    static int32_t HeightDoubleToShort(double d);
    static int32_t HeightFloatToShort(float f);
    static short HeightCharToShort(char c) {
        return HeightToShort(c);
    }
    static short HeightUCharToShort(unsigned char uc) {
        return HeightToShort(uc);
    }

private:
    template <typename T>
    static short HeightToShort(T c)
    {
        auto smin = std::numeric_limits<T>::min();
        auto smax = std::numeric_limits<T>::max();
        auto dmin = std::numeric_limits<short>::min();
        auto dmax = std::numeric_limits<short>::max();
        return static_cast<float>(c - smin) / (smax - smin) * (dmax - dmin) + dmin;
    }

}; // Utility

}