// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_JSON_NUM_NUMHELPER
#define SSVU_JSON_NUM_NUMHELPER

#include "SSVUtils/Union/Union.hpp"
#include "SSVUtils/Json/Common/Common.hpp"
#include "SSVUtils/Json/Num/Num.hpp"

namespace ssvu
{
    namespace Json
    {
        namespace Impl
        {
#define SSVJ_DEFINE_NUMHELPER(mType, mRepr)                       \
    template <>                                                   \
    struct NumHelper<mType>                                       \
    {                                                             \
        inline static void set(Num& mN, const mType& mX) noexcept \
        {                                                         \
            return VRM_PP_CAT(mN.set, mRepr)(mX);                 \
        }                                                         \
        inline static auto as(const Num& mN) noexcept             \
        {                                                         \
            return toNum<mType>(VRM_PP_CAT(mN.get, mRepr)());     \
        }                                                         \
    };

#define SSVJ_DEFINE_REPRHELPER(mType)                                       \
    template <>                                                             \
    struct ReprHelper<mType>                                                \
    {                                                                       \
        inline static auto get() { return VRM_PP_DEFER(Num::Repr::mType); } \
    };

            // Define helpers to set/get numeric types to/from specific
            // representations
            SSVJ_DEFINE_NUMHELPER(char, IntS)
            SSVJ_DEFINE_NUMHELPER(int, IntS)
            SSVJ_DEFINE_NUMHELPER(long int, IntS)
            SSVJ_DEFINE_NUMHELPER(unsigned char, IntU)
            SSVJ_DEFINE_NUMHELPER(unsigned int, IntU)
            SSVJ_DEFINE_NUMHELPER(unsigned long int, IntU)
            SSVJ_DEFINE_NUMHELPER(float, Real)
            SSVJ_DEFINE_NUMHELPER(double, Real)

            // Define helpers to get representation types from numeric types
            SSVJ_DEFINE_REPRHELPER(IntS)
            SSVJ_DEFINE_REPRHELPER(IntU)
            SSVJ_DEFINE_REPRHELPER(Real)

#undef SSVJ_DEFINE_NUMHELPER
#undef SSVJ_DEFINE_REPRHELPER
        }
    }
}

#endif
