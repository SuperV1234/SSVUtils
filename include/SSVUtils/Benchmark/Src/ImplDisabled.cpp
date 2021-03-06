// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#include "SSVUtils/Internal/API.hpp"
#include "SSVUtils/Benchmark/Inc/Data.hpp"
#include "SSVUtils/Benchmark/Inc/ImplDisabled.hpp"

#include <string>

namespace ssvu
{
namespace Benchmark
{
namespace Impl
{
SSVU_INLINE void start(std::string)
{
}
SSVU_INLINE Data getEndData()
{
    return Data{};
}
SSVU_INLINE void endLo()
{
}
SSVU_INLINE void groupReset(const std::string&)
{
}
SSVU_INLINE void groupResume(const std::string&)
{
}
SSVU_INLINE void groupPause(const std::string&)
{
}
SSVU_INLINE void groupEndLo(const std::string&)
{
}
} // namespace Impl
} // namespace Benchmark
} // namespace ssvu
