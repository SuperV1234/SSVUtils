// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_IMPL_BENCHMARK_TYPEDEFS
#define SSVU_IMPL_BENCHMARK_TYPEDEFS

#include <chrono>

namespace ssvu
{
namespace Benchmark
{
namespace Impl
{
/// @typedef Shortcut typedef for
/// `std::chrono::timepoint<std::chrono::high_resolution_clock>`.
using TP = std::chrono::time_point<std::chrono::high_resolution_clock>;

/// @typedef Shortcut typedef for `std::chrono::nanoseconds`.
using Duration = std::chrono::nanoseconds;
} // namespace Impl
} // namespace Benchmark
} // namespace ssvu

#endif
