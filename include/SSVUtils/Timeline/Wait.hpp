// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_TIMELINE_WAIT
#define SSVU_TIMELINE_WAIT

#include <cassert>
#include "SSVUtils/Timeline/Command.hpp"
#include "SSVUtils/Timeline/Timeline.hpp"

namespace ssvu
{
	class Wait final : public Command
	{
		private:
			FT time, currentTime;

		protected:
			inline void update(FT mFT) override
			{
				timeline.ready = false;

				currentTime -= mFT;
				if(currentTime > 0) return;

				timeline.remainder = currentTime;
				timeline.next(); reset();
			}
			inline void reset() override { currentTime = time; }

		public:
			Wait(Timeline& mTimeline, FT mTime) : Command{mTimeline}, time{mTime}, currentTime{mTime} { assert(time > 0); }
	};

	namespace Internal
	{
		template<bool TWhile> class WaitLoop final : public Command
		{
			private:	Predicate predicate;
			protected:	inline void update(FT) override { timeline.ready = false; if(predicate() != TWhile) timeline.next(); }
			public:		WaitLoop(Timeline& mTimeline, const Predicate& mPredicate) : Command{mTimeline}, predicate{mPredicate} { }
		};
	}

	using WaitWhile = Internal::WaitLoop<true>;
	using WaitUntil = Internal::WaitLoop<false>;
}

#endif