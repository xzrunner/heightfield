#include "heightfield/Utility.h"

#include <limits>

namespace hf
{

double Utility::HeightShortToDouble(int32_t s)
{
    auto smin = std::numeric_limits<short>::min();
    auto smax = std::numeric_limits<short>::max();
    auto dmin = -1.0;
    auto dmax = 1.0;
    return (s - smin) / (smax - smin) * (dmax - dmin) + dmin;
}

float Utility::HeightShortToFloat(int32_t s)
{
    return static_cast<float>(HeightShortToDouble(s));
}

int32_t Utility::HeightDoubleToShort(double d)
{
    auto smin = -1.0;
    auto smax = 1.0;
    auto dmin = std::numeric_limits<short>::min();
    auto dmax = std::numeric_limits<short>::max();
    return static_cast<int32_t>((d - smin) / (smax - smin) * (dmax - dmin) + dmin);
}

int32_t Utility::HeightFloatToShort(float f)
{
    auto smin = -1.0f;
    auto smax = 1.0f;
    auto dmin = std::numeric_limits<short>::min();
    auto dmax = std::numeric_limits<short>::max();
    return static_cast<int32_t>((f - smin) / (smax - smin) * (dmax - dmin) + dmin);
}

}