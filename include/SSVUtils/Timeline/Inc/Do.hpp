// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_IMPL_TIMELINE_DO
#define SSVU_IMPL_TIMELINE_DO

#include "SSVUtils/Core/Common/Frametime.hpp"
#include "SSVUtils/Timeline/Inc/Command.hpp"

#include <functional>


namespace ssvu
{
class Do final : public Command
{
protected:
    std::function<void()> action;
    void update(FT) override;

public:
    Do(Timeline& mTimeline, const std::function<void()>& mAction) noexcept;
};
} // namespace ssvu

#endif
