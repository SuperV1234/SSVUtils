// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_ENCRYPTION_OBFUSCATEDVALUE
#define SSVU_ENCRYPTION_OBFUSCATEDVALUE

#include "SSVUtils/Core/String/Utils.hpp"
#include "SSVUtils/Core/Stringifier/Stringifier.hpp"
#include "SSVUtils/Encoding/Encoding.hpp"

#include <string>
#include <type_traits>
#include <cassert>

namespace ssvu
{
template <typename T, Encoding::Type TCrypto = Encoding::Type::Base64,
    typename TEnable = void>
class ObfuscatedValue;

/// @brief Base64 "obfuscated" value
/// @details Quick (but not really effective) way to protect a value against
/// memory scanners (such as Cheat Engine).
/// Obviously introduces a runtime cost to get/set the internal value.
/// @tparam T Type of the underlying arithmetic value.
template <typename T, Encoding::Type TCrypto>
class ObfuscatedValue<T, TCrypto, std::enable_if_t<std::is_arithmetic_v<T>>>
{
private:
    /// @brief Dummy value used to "fool" memory scanners.
    T dummy;

    /// @brief The "real" value, under the form of a Base64 string.
    std::string encodedValue;

    /// @brief Converts the encodedValue to the arithmetic type.
    /// @details Used internally.
    /// @param mStr String to convert to arithmetic type.
    T fromStr(const std::string& mStr) const
    {
        std::istringstream stream(mStr);
        T t;
        stream >> t;
        return t;
    }

public:
    /// @brief Constructs an ObfuscatedValue from an arithmetic value.
    /// @param mValue The arithmetic value that will be stored internally.
    inline ObfuscatedValue(const T& mValue)
    {
        set(mValue);
    }

    /// @brief Sets the internal Base64 string.
    /// @param mValue Value to use.
    inline void set(const T& mValue)
    {
        dummy = mValue;
        encodedValue = Encoding::encode<TCrypto>(toStr(mValue));
    }

    /// @brief Converts the internal Base64 string and returns the
    /// unobfuscated value.
    inline T get() const
    {
        return fromStr(Encoding::decode<TCrypto>(encodedValue));
    }

    /// @brief Implicit conversion to the obfuscated type.
    inline operator T() const
    {
        return get();
    }

    /// @brief Adds a value to the internal one.
    /// @param mValue Value to add.
    inline T operator+=(const T& mValue)
    {
        set(get() + mValue);
        return get();
    }

    /// @brief Subtracts a value from the internal one.
    /// @param mValue Value to subtract.
    inline T operator-=(const T& mValue)
    {
        set(get() - mValue);
        return get();
    }

    /// @brief Multiplies the internal value by another value.
    /// @param mValue Value to multiply with.
    inline T operator*=(const T& mValue)
    {
        set(get() * mValue);
        return get();
    }

    /// @brief Divides the internal value by another value.
    /// @param mValue Value to divide with.
    inline T operator/=(const T& mValue)
    {
        assert(mValue != 0);
        set(get() / mValue);
        return get();
    }

    /// @brief Shortcut operator for set.
    /// @param mValue Value to use.
    inline void operator=(const T& mValue)
    {
        set(mValue);
    }
};
} // namespace ssvu

#endif
