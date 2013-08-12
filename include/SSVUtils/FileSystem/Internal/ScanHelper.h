// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_FILESYSTEM_INTERNAL_SCANHELPER
#define SSVU_FILESYSTEM_INTERNAL_SCANHELPER

#include <dirent.h>
#include <sys/stat.h>
#include "SSVUtils/FileSystem/Enums.h"
#include "SSVUtils/FileSystem/Utils.h"
#include "SSVUtils/FileSystem/Path.h"
#include "SSVUtils/Utils/UtilsContainers.h"

namespace ssvu
{
	namespace FileSystem
	{
		namespace Internal
		{
			template<Mode TM, Type TT, Pick TP, Sort TS> static void scan(std::vector<Path>& mTarget, const Path& mPath, const std::string& mDesired)
			{
				if(!isFolder(mPath)) { lo << lt("ssvu::FileSystem::ScanHelper") << "Directory \"" << mPath << "\" not found" << std::endl; return; }

				DIR* dir{opendir(mPath.getCStr())};
				dirent* entry{readdir(dir)};

				while(entry != NULL)
				{
					std::string name{entry->d_name};
					Path path{mPath + name};
					if(!isRootOrParent(path))
					{
						if(isFolder(path))
						{
							if(TT == Type::All || TT == Type::Folder) { mTarget.push_back(path); }
							if(TM == Mode::Recurse) { Internal::scan<Mode::Recurse, TT, TP, TS>(mTarget, path, mDesired); }
						}
						else if(TT == Type::All || TT == Type::File)
						{
							if(TP == Pick::Any)			{ mTarget.push_back(path); }
							else if(TP == Pick::ByExt)	{ if(endsWith(name, mDesired)) mTarget.push_back(path); }
							else if(TP == Pick::ByName)	{ if(name == mDesired) mTarget.push_back(path); }
						}
					}

					entry = readdir(dir);
				}

				closedir(dir);
				if(TS == Sort::Alphabetic) sort(mTarget);
			}
		}
	}
}

#endif
