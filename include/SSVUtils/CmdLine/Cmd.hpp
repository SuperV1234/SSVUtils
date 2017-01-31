// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_CMDLINE_CMD
#define SSVU_CMDLINE_CMD

#include "SSVUtils/Delegate/Delegate.hpp"
#include "SSVUtils/MemoryManager/MemoryManager.hpp"
#include "SSVUtils/CmdLine/Elements/Elements.hpp"

namespace ssvu
{
    namespace CmdLine
    {
        namespace Impl
        {
            // These functions exist only to avoid some code repetition below
            template <typename T>
            std::string buildCmdStr(const T& mC, std::string mStart = "",
                const std::string& mEnd = "", const std::string& mSep = " ")
            {
                for(auto i(0u); i < mC.size(); ++i)
                {
                    mStart += mC[i]->getUsageStr();
                    if(i < mC.size() - 1) mStart += mSep;
                }
                return mStart + mEnd;
            }
            template <template <typename...> class T, typename... Ts>
            std::string buildCmdStr(const T<std::string, Ts...>& mC,
                std::string mStart = "", const std::string& mEnd = "",
                const std::string& mSep = " ")
            {
                for(auto i(0u); i < mC.size(); ++i)
                {
                    mStart += mC[i];
                    if(i < mC.size() - 1) mStart += mSep;
                }
                return mStart + mEnd;
            }

            class ManagerElements
            {
            private:
                static constexpr SizeT maxTypes{50};

                PolyRecVector<BaseElement> elements;
                std::array<std::vector<BaseElement*>, maxTypes> groupedElements;

                template <EType TET>
                inline auto& getGroupVec() noexcept
                {
                    return groupedElements[castEnum(TET)];
                }
                template <EType TET>
                inline const auto& getGroupVec() const noexcept
                {
                    return groupedElements[castEnum(TET)];
                }

            public:
                template <typename T, typename... TArgs>
                inline T& create(TArgs&&... mArgs)
                {
                    SSVU_ASSERT_STATIC(isBaseOf<BaseElement, T>(),
                        "`T` must derive from `BaseElement`");

                    auto& result(elements.create<T>(FWD(mArgs)...));
                    getGroupVec<T::getEType()>().emplace_back(&result);
                    return result;
                }

                template <EType TET>
                inline auto getCount() const noexcept
                {
                    return getGroupVec<TET>().size();
                }
                template <EType TET>
                inline auto isEmpty() const noexcept
                {
                    return getCount<TET>() == 0;
                }

                template <EType TET>
                inline auto& getAll() noexcept
                {
                    return getGroupVec<TET>();
                }
                template <EType TET>
                inline const auto& getAll() const noexcept
                {
                    return getGroupVec<TET>();
                }

                template <EType TET>
                inline auto& getAt(SizeT mIdx) noexcept
                {
                    return castUp<ETypeBase<TET>>(*getAll<TET>()[mIdx]);
                }
                template <EType TET>
                inline const auto& getAt(SizeT mIdx) const noexcept
                {
                    return castUp<ETypeBase<TET>>(*getAll<TET>()[mIdx]);
                }
            };
        }

        class Cmd
        {
            friend class Ctx;

        private:
            std::vector<std::string> names;
            Impl::ManagerElements mgr;
            Delegate<void()> onAction;
            std::string desc;
            bool mainCmd{false}; // Is this the main cmd?

            inline Cmd() = default;
            inline static Cmd createCmdMain()
            {
                Cmd result;
                result.mainCmd = true;
                return result;
            }

            inline auto& findFlag(const std::string& mName)
            {
                for(auto& f : asRangeCastPtr<Flag&>(mgr.getAll<EType::Flag>()))
                    if(f.hasName(mName)) return f;
                throw Exception::createFlagNotFound(mName, getNamesStr());
            }

            template <EType TET>
            inline void setElementValue(SizeT mIdx, const std::string& mValue)
            {
                mgr.getAt<TET>(mIdx).set(mValue);
            }

            template <EType TET>
            auto getHelpStrForET(std::string& mStr,
                const Impl::ManagerElements& mMgr,
                const std::string& mTitle) const
            {
                if(mMgr.template isEmpty<TET>()) return;

                mStr += "\t" + mTitle + ":\n";

                for(auto p : getAll<TET>()) mStr += p->getHelpStr();

                mStr += "\n";
            }

        public:
            inline Cmd(const std::initializer_list<std::string>& mNames)
                : names{mNames}
            {
            }

            inline Cmd& operator+=(const Action& mFunc)
            {
                onAction += mFunc;
                return *this;
            }
            inline Cmd& operator()()
            {
                onAction();
                return *this;
            }

            template <typename T, typename... TArgs>
            inline T& create(TArgs&&... mArgs)
            {
                return mgr.create<T>(FWD(mArgs)...);
            }

            inline void activateFlag(const std::string& mName)
            {
                findFlag(mName) = true;
            }

            inline bool hasName(const std::string& mName) const
            {
                return contains(names, mName);
            }
            inline bool isMainCmd() const noexcept { return mainCmd; }

            inline const auto& getNames() const noexcept { return names; }

            template <EType TET>
            inline auto getCount() const noexcept
            {
                return mgr.getCount<TET>();
            }
            template <EType TET>
            inline const auto& getAll() const noexcept
            {
                return mgr.getAll<TET>();
            }

            template <EType TET>
            inline auto& getAt(SizeT mIdx) noexcept
            {
                return mgr.getAt<TET>(mIdx);
            }
            template <EType TET>
            inline const auto& getAt(SizeT mIdx) const noexcept
            {
                return mgr.getAt<TET>(mIdx);
            }

            std::string getNamesStr() const
            {
                return Impl::buildCmdStr(names, "<", ">", " || ");
            }
            template <EType TET>
            auto getStr() const
            {
                return Impl::buildCmdStr(getAll<TET>());
            }

            inline auto getHelpStr() const
            {
                std::string result;

                if(!desc.empty()) result += ">>" + desc + "\n\n";

                getHelpStrForET<EType::Arg>(result, mgr, "Required arguments");
                getHelpStrForET<EType::ArgOpt>(
                    result, mgr, "Optional arguments");
                getHelpStrForET<EType::ArgPack>(result, mgr, "Argument packs");
                getHelpStrForET<EType::Flag>(result, mgr, "Flags");
                getHelpStrForET<EType::FlagValue>(result, mgr, "Flag values");
                getHelpStrForET<EType::FlagValueOpt>(
                    result, mgr, "Optional flag values");

                return result;
            }

            SSVU_DEFINE_SINK_SETTER_SIMPLE(setDesc, desc)
            inline const auto& getDesc() const noexcept { return desc; }
        };
    }
}

#endif
