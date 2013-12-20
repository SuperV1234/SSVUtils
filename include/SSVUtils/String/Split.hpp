// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_STRING_SPLIT
#define SSVU_STRING_SPLIT

#include <string>
#include <vector>
#include "SSVUtils/String/Enums.hpp"
#include "SSVUtils/String/Internal/SplitHelper.hpp"

namespace ssvu
{
	/// @brief Splits a string in smaller strings, filling a target vector<string>.
	/// @tparam T Type of the separator. Can be a char, a string, a vector<char> or a vector<string>.
	/// @tparam TM Separation mode. Can be Split::Normal or Split::KeepSeparator (keeps the separator in the splitted strings).
	/// @param mTarget Vector to fill with the splitted strings.
	/// @param mStr String to split.
	/// @param mSeparator Separator to split at. Every occurrence of the separator will cause a split.
	template<Split TM = Split::Normal, typename T> inline void split(std::vector<std::string>& mTarget, const std::string& mStr, const T& mSeparator)
	{
		Internal::SplitHelper<T, TM>::split(mTarget, mStr, mSeparator);
	}

	/// @brief Splits a string in smaller strings, returning a vector<string>.
	/// @tparam T Type of the separator. Can be a char, a string, a vector<char> or a vector<string>.
	/// @tparam TM Separation mode. Can be Split::Normal or Split::KeepSeparator (keeps the separator in the splitted strings).
	/// @param mStr String to split.
	/// @param mSeparator Separator to split at. Every occurrence of the separator will cause a split.
	/// @return Returns a std::vector containing all splitted strings.
	template<Split TM = Split::Normal, typename T> inline std::vector<std::string> getSplit(const std::string& mStr, const T& mSeparator)
	{
		std::vector<std::string> result;
		split<TM, T>(result, mStr, mSeparator);
		return result;
	}
}

#endif
