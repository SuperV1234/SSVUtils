
// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_TEMPLATESYSTEM_DICTIONARY
#define SSVU_TEMPLATESYSTEM_DICTIONARY

#include "SSVUtils/Core/Core.hpp"
#include "SSVUtils/Container/Container.hpp"
#include "SSVUtils/TemplateSystem/Internal/Expander.hpp"

namespace ssvu
{
    namespace TemplateSystem
    {
        class Dictionary
        {
            friend class Expander;

        public:
            using DictVec = std::vector<Dictionary>;

        private:
            VecMap<std::string, std::string> replacements;
            VecMap<std::string, DictVec> sections;
            Dictionary* parentDict{nullptr};

            template <typename TKey>
            class Proxy
            {
            private:
                TKey key;
                Dictionary& dict;

            public:
                inline Proxy(TKey mKey, Dictionary& mDict)
                    : key{mKey}, dict{mDict}
                {
                }

                inline auto& asStr() noexcept
                {
                    return dict.replacements[key];
                }

                inline const auto& asStr() const noexcept
                {
                    return dict.replacements.at(key);
                }

                template <typename T>
                inline auto& operator=(T&& mReplacement)
                {
                    dict.replacements[key] = FWD(mReplacement);
                    return *this;
                }
                template <typename T>
                inline auto& operator+=(T&& mDict)
                {
                    dict.sections[key].emplace_back(FWD(mDict));
                    return *this;
                }
            };

            template <typename TKey>
            class ConstProxy
            {
            private:
                TKey key;
                const Dictionary& dict;

            public:
                inline ConstProxy(TKey mKey, const Dictionary& mDict)
                    : key{mKey}, dict{mDict}
                {
                }


                inline const auto& asStr() const noexcept
                {
                    return dict.replacements.at(key);
                }
            };

            // Init single replacement
            template <typename T>
            inline void initImpl(T&& mKey, const std::string& mReplacement)
            {
                replacements[FWD(mKey)] = mReplacement;
            }
            template <typename T>
            inline void initImpl(T&& mKey, std::string&& mReplacement)
            {
                replacements[FWD(mKey)] = mv(mReplacement);
            }

            // Init section replacement
            template <typename T>
            inline void initImpl(T&& mKey, const DictVec& mDicts)
            {
                sections[FWD(mKey)] = mDicts;
            }
            template <typename T>
            inline void initImpl(T&& mKey, DictVec&& mDicts)
            {
                sections[FWD(mKey)] = mv(mDicts);
            }

            // Copy/move init
            inline void initImpl(const Dictionary& mDict)
            {
                parentDict = mDict.parentDict;
                replacements = mDict.replacements;
                sections = mDict.sections;
            }
            inline void initImpl(Dictionary&& mDict) noexcept
            {
                parentDict = mDict.parentDict;
                mDict.parentDict = nullptr;
                replacements = mv(mDict.replacements);
                sections = mv(mDict.sections);
            }

            inline void init() noexcept
            {
            }
            template <typename T1, typename... TArgs>
            inline void init(T1&& mA1, TArgs&&... mArgs)
            {
                initImpl(FWD(mA1));
                init(FWD(mArgs)...);
            }
            template <typename T1, typename T2, typename... TArgs>
            inline void init(T1&& mA1, T2&& mA2, TArgs&&... mArgs)
            {
                initImpl(FWD(mA1), FWD(mA2));
                init(FWD(mArgs)...);
            }

            inline void refreshParents()
            {
                for(auto& v : sections)
                    for(auto& d : v.second)
                    {
                        d.parentDict = this;
                        d.refreshParents();
                    }
            }

            template <typename... TArgs>
            bool expandImpl(TArgs&&... mArgs) const
            {
                Expander e(*this, FWD(mArgs)...);
                return e.expand();
            }

        public:
            template <typename... TArgs>
            inline Dictionary(TArgs&&... mArgs)
            {
                init(FWD(mArgs)...);
            }

            inline bool has(const std::string& mKey) const noexcept
            {
                return replacements.count(mKey) > 0;
            }

            inline std::string getExpanded(std::string mSrc,
                Settings mSettings = Settings::EraseUnexisting)
            {
                refreshParents();

                std::string buf, bufKey;
                buf.reserve(toSizeT(mSrc.size() * 1.5));
                bufKey.reserve(25);

                bool found{true};
                while(found)
                {
                    found = expandImpl(
                        mSrc, buf, bufKey, 0, mSrc.size(), 0, mSettings);

                    mSrc = buf;
                    buf.clear();
                }

                // De-escape '{'
                if(mSrc.size() > 0)
                {
                    for(auto i(0u); i < mSrc.size() - 1; ++i)
                    {
                        if(mSrc[i] == '\\' && mSrc[i + 1] == '{') ++i;
                        buf += mSrc[i];
                    }

                    buf += mSrc[mSrc.size() - 1];
                }

                return buf;
            }

            template <typename T>
            inline auto operator[](T&& mKey) noexcept
            {
                return Proxy<T>{FWD(mKey), *this};
            }

            template <typename T>
            inline auto operator[](T&& mKey) const noexcept
            {
                return ConstProxy<T>{FWD(mKey), *this};
            }
        };
    }
}

#endif
