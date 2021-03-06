// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_IMPL_CONTAINER_VECMAPBASE
#define SSVU_IMPL_CONTAINER_VECMAPBASE

#include "SSVUtils/Core/Common/Casts.hpp"

namespace ssvu
{
namespace Impl
{
/// @brief Base CRTP class for vector-based sorted containers.
template <typename TDerived>
class VecMapBase
{
private:
    inline auto& getTD() noexcept
    {
        return castUp<TDerived>(*this);
    }
    inline const auto& getTD() const noexcept
    {
        return castUp<TDerived>(*this);
    }

public:
    /// @brief Returns whether or not `mValue` is present in the
    /// container.
    template <typename T>
    inline bool SSVU_ATTRIBUTE(pure) has(const T& mValue) const noexcept
    {
        return getTD().is(getTD().lookup(mValue), mValue);
    }

    // Getters for the internal vector storage
    inline auto& getData() noexcept
    {
        return getTD().data;
    }
    inline const auto& getData() const noexcept
    {
        return getTD().data;
    }

    // Standard (partial) vector interface support
    inline void reserve(std::size_t mV)
    {
        getData().reserve(mV);
    }
    inline void clear() noexcept
    {
        getData().clear();
    }
    inline auto size() const noexcept
    {
        return getData().size();
    }
    inline auto empty() const noexcept
    {
        return getData().empty();
    }
    inline auto capacity() const noexcept
    {
        return getData().capacity();
    }

    // Standard iterator support
    inline auto begin() noexcept
    {
        return std::begin(getData());
    }
    inline auto end() noexcept
    {
        return std::end(getData());
    }
    inline auto begin() const noexcept
    {
        return std::begin(getData());
    }
    inline auto end() const noexcept
    {
        return std::end(getData());
    }
    inline auto cbegin() const noexcept
    {
        return std::cbegin(getData());
    }
    inline auto cend() const noexcept
    {
        return std::cend(getData());
    }
    inline auto rbegin() noexcept
    {
        return std::rbegin(getData());
    }
    inline auto rend() noexcept
    {
        return std::rend(getData());
    }
    inline auto crbegin() const noexcept
    {
        return std::crbegin(getData());
    }
    inline auto crend() const noexcept
    {
        return std::crend(getData());
    }
};

// Equality/inequality
template <typename TDerived>
inline bool SSVU_ATTRIBUTE(pure) operator==(const VecMapBase<TDerived>& lhs,
    const VecMapBase<TDerived>& rhs) noexcept
{
    return lhs.getData() == rhs.getData();
}

template <typename TDerived>
inline bool SSVU_ATTRIBUTE(pure) operator!=(const VecMapBase<TDerived>& lhs,
    const VecMapBase<TDerived>& rhs) noexcept
{
    return lhs.getData() != rhs.getData();
}

} // namespace Impl
} // namespace ssvu

#endif
