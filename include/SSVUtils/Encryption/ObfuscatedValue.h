// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_ENCRYPTION_OBFUSCATEDVALUE
#define SSVU_ENCRYPTION_OBFUSCATEDVALUE

#include <string>
#include <sstream>
#include <type_traits>
#include "SSVUtils/Encryption/Base64.h"

namespace ssvu
{
	template<typename T, typename TEnable = void> class ObfuscatedValue;

	template<typename T> class ObfuscatedValue<T, typename std::enable_if<std::is_arithmetic<T>::value>::type>
	{
		private:
			T dummy;
			std::string encodedValue;

			T fromString(const std::string& s) const { std::istringstream stream{s}; T t; stream >> t; return t; }

		public:
			ObfuscatedValue(T mValue) { set(mValue); }

			void set(T mValue) { dummy = mValue; encodedValue = base64_encode(toStr(mValue)); }
			T get() const { return fromString(base64_decode(encodedValue)); }
			operator T() const { return get(); }
			T operator +=(const T& mValue) { set(get() + mValue); return get(); }
			T operator -=(const T& mValue) { set(set() - mValue); return get(); }
			T operator *=(const T& mValue) { set(set() * mValue); return get(); }
			T operator /=(const T& mValue) { set(set() / mValue); return get(); }
			void operator =(const T& mValue) { set(mValue); }
	};
}

#endif
