// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_STRING_INTERNAL_SPLITHELPER
#define SSVU_STRING_INTERNAL_SPLITHELPER

#include <vector>
#include <string>
#include <limits>
#include "SSVUtils/String/Enums.h"

namespace ssvu
{
	namespace Internal
	{
		using StringSize = std::string::size_type;

		inline StringSize findFirstOf(const std::string& mStr, const std::vector<std::string>& mSeparator, StringSize& mLength)
		{
			StringSize result{std::numeric_limits<StringSize>::max()};
			for(const auto& s : mSeparator)
			{
				StringSize temp{mStr.find(s)};
				if(temp < result) { result = temp; mLength = s.size(); }
			}
			return result;
		}

		template<typename T> struct SplitFindHelper;
		template<> struct SplitFindHelper<char>
		{
			inline static StringSize getNextIdx(const std::string& mStr, const char& mSeparator, StringSize mStart)
			{
				return mStr.find(mSeparator, mStart);
			}
		};
		template<> struct SplitFindHelper<std::string>
		{
			inline static StringSize getNextIdx(const std::string& mStr, const std::string& mSeparator, StringSize mStart)
			{
				return mStr.find(mSeparator, mStart);
			}
		};
		template<> struct SplitFindHelper<std::vector<char>>
		{
			inline static StringSize getNextIdx(const std::string& mStr, const std::vector<char>& mSeparator, StringSize mStart)
			{
				return mStr.find_first_of(std::string{std::begin(mSeparator), std::end(mSeparator)}, mStart);
			}
		};

		template<typename T, Split TM> struct SplitHelper
		{
			inline static void split(std::vector<std::string>& mTarget, const std::string& mStr, const T& mSeparator)
			{
				StringSize p{0}, q;
				while((q = SplitFindHelper<T>::getNextIdx(mStr, mSeparator, p)) != std::string::npos)
				{
					auto tokenLength(q - p);

					// If we need to keep the separator in the splitted strings, add 1 to the token length (assuming a char has always length 1)
					if(TM == Split::KeepSeparator) tokenLength += 1;

					std::string token{mStr, p, tokenLength};
					if(!token.empty()) mTarget.push_back(token);
					p = q + 1;
				}
				std::string remaining{mStr, p};
				if(!remaining.empty()) mTarget.push_back(remaining);
			}
		};
		template<Split TM> struct SplitHelper<std::string, TM>
		{
			inline static void split(std::vector<std::string>& mTarget, std::string mStr, const std::string& mSeparator)
			{
				StringSize pos{0};
				std::string token;
				while((pos = mStr.find(mSeparator)) != std::string::npos)
				{
					auto tokenLength(pos);

					// If we need to keep the separator in the splitted strings, add the separator's length to the token length
					if(TM == Split::KeepSeparator) tokenLength += mSeparator.size();

					token = mStr.substr(0, tokenLength);
					if(!token.empty()) mTarget.push_back(token);
					mStr.erase(0, pos + mSeparator.size());
				}
				if(!mStr.empty()) mTarget.push_back(mStr);
			}
		};
		template<Split TM> struct SplitHelper<std::vector<std::string>, TM>
		{
			inline static void split(std::vector<std::string>& mTarget, std::string mStr, const std::vector<std::string>& mSeparator)
			{
				StringSize pos{0}, lastLength;
				std::string token;
				while((pos = findFirstOf(mStr, mSeparator, lastLength)) != std::string::npos)
				{
					auto tokenLength(pos);

					// If we need to keep the separator in the splitted strings, add the separator's length to the token length
					if(TM == Split::KeepSeparator) tokenLength += lastLength;

					token = mStr.substr(0, tokenLength);
					if(!token.empty()) mTarget.push_back(token);
					mStr.erase(0, pos + lastLength);
				}
				if(!mStr.empty()) mTarget.push_back(mStr);
			}
		};
	}
}

#endif
