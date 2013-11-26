// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_BIMAP
#define SSVU_BIMAP

#include <map>

namespace ssvu
{
	/// @brief Bi-directional key-value container.
	/// @details Implemented as a pair of std::map
	/// @tparam T Key/Value type 1
	/// @tparam U Key/Value type 2
	/// @tparam TContainer Underlying container type. Can be std::map, std::unordered_map, ...
	template<typename T, typename U, template<typename...> class TContainer = std::map> class Bimap
	{
		private:
			TContainer<T, U> map1; /*!< Map container 1. */
			TContainer<U, T> map2; /*!< Map container 2. */

		public:
			/// @brief Default constructor.
			/// @details Initializes an empty Bimap.
			Bimap() = default;

			/// @brief Initializer list constructor.
			/// @details Initializes a Bimap from a std::initalizer_list of pairs.
			/// @param mPairs Initializer list of std::pair<T, U>.
			Bimap(const std::initializer_list<std::pair<T, U>>& mPairs) { for(const auto& p : mPairs) insert(p); }

			/// @brief Insert a pair in the Bimap.
			/// @param mPair std::pair<T, U> to insert.
			inline void insert(const std::pair<T, U>& mPair) { map1[mPair.first] = mPair.second; map2[mPair.second] = mPair.first; }

			/// @brief Erase a pair from the Bimap.
			/// @param mKey Key of the pair to erase.
			inline void erase(const T& mKey) { map2.erase(map1.at(mKey)); map1.erase(mKey); }

			/// @brief Erase a pair from the Bimap.
			/// @param mKey Key of the pair to erase.
			inline void erase(const U& mKey) { map1.erase(map2.at(mKey)); map2.erase(mKey); }

			/// @brief Returns a value from the Bimap.
			/// @param mKey Key of the value to return.
			inline const U& at(const T& mKey) const { return map1.at(mKey); }

			/// @brief Returns a value from the Bimap.
			/// @param mKey Key of the value to return.
			inline const T& at(const U& mKey) const { return map2.at(mKey); }

			/// @brief Returns a value from the Bimap, or creates it if unexistant.
			/// @param mKey Key of the value to return or create.
			inline U& operator[](const T& mKey) { return map1[mKey]; }

			/// @brief Returns a value from the Bimap, or creates it if unexistant.
			/// @param mKey Key of the value to return or create.
			inline T& operator[](const U& mKey) { return map2[mKey]; }

			/// @brief Looks for a value in the Bimap.
			/// @param mValue Key of the value to find.
			inline auto find(const T& mValue) const -> decltype(std::declval<const decltype(map1)>().find(mValue)) { return map1.find(mValue); }

			/// @brief Looks for a value in the Bimap.
			/// @param mValue Key of the value to find.
			inline auto find(const U& mValue) const -> decltype(std::declval<const decltype(map2)>().find(mValue)) { return map2.find(mValue); }

			/// @brief Checks if a value is in the Bimap.
			/// @param mValue Key of the value to find.
			inline bool has(const T& mValue) const { return find(mValue) != end(map1); }

			/// @brief Checks if a value is in the Bimap.
			/// @param mValue Key of the value to find.
			inline bool has(const U& mValue) const { return find(mValue) != end(map2); }

			/// @brief Returns the first map container.
			inline decltype(map1)& getMap1() noexcept { return map1; }

			/// @brief Returns the second map container.
			inline decltype(map2)& getMap2() noexcept { return map2; }

			/// @brief Returns the first map container. (const version)
			inline const decltype(map1)& getMap1() const noexcept { return map1; }

			/// @brief Returns the second map container. (const version)
			inline const decltype(map2)& getMap2() const noexcept { return map2; }
	};
}

#endif