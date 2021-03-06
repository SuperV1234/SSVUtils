// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_CMDLINE_ELEMENTS_BASES_BASEARG
#define SSVU_CMDLINE_ELEMENTS_BASES_BASEARG

#include "SSVUtils/CmdLine/Elements/Bases/BaseElement.hpp"

#include <string>

namespace ssvu
{
namespace CmdLine
{
namespace Impl
{
struct BaseArg : public BaseElement
{
    virtual void set(const std::string&) = 0;
    inline std::string getUsageStr() const override
    {
        return "(ARG " + getName() + ")";
    }
};
} // namespace Impl
} // namespace CmdLine
} // namespace ssvu

#endif
