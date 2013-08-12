// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#include <cstdlib>
#include <dirent.h>
#include <sys/stat.h>
#include "SSVUtils/FileSystem/Utils.h"
#include "SSVUtils/FileSystem/Path.h"
#include "SSVUtils/String/Utils.h"
#include "SSVUtils/Log/Log.h"

using namespace std;

namespace ssvu
{
	namespace FileSystem
	{
		namespace Internal
		{
			bool isFolderNoNormalize(const string& mPath)
			{
				struct stat fileStat;
				int err{stat(mPath.c_str(), &fileStat)};
				if(err != 0) return false;
				return (fileStat.st_mode & S_IFMT) == S_IFDIR;
			}
		}

		bool exists(const Path& mPath) { struct stat buf; return stat(mPath.getCStr(), &buf) != -1; }
		bool hasExtension(const Path& mFileName, const string& mExtension) { return endsWith(toLower(mFileName), toLower(mExtension)); }
		bool isFolder(const Path& mPath) { return Internal::isFolderNoNormalize(mPath); }
		bool isRootOrParent(const Path& mPath) { return endsWith(mPath, "./") || endsWith(mPath, "../"); }
		Path getParentPath(const Path& mPath)
		{
			string str(mPath);

			for(auto i(str.size() - 1); i > 0; --i)
				if(str[i] == '/') return {str.substr(0, i + 1)};

			return {""};
		}
		string getNameFromPath(const Path& mPath, const string& mPrefix, const string& mSuffix)
		{
			string str(mPath);
			return str.substr(mPrefix.size(), str.size() - mPrefix.size() - mSuffix.size());
		}
		string getFileContents(const Path& mPath)
		{
			std::ifstream ifs{mPath, std::ios_base::binary};
			return {std::istreambuf_iterator<char>(ifs), std::istreambuf_iterator<char>()};
		}
		void createFolder(const Path& mPath)
		{
			#ifdef _WIN32
				mkdir(mPath.c_str());
			#else
				mkdir(mPath.getCStr(), 0755);
			#endif
		}
		void removeFile(const Path& mPath)
		{
			if(remove(mPath.getCStr()) != 0) lo << lt("ssvu::FileSystem::removeFile") << "Error removing file: " << mPath;
		}

		void expandUserPath(Path& mPath)
		{
			// TODO: WIN32 NEEDS TESTING!

			string userHome;

			#ifdef _WIN32
				if(getenv("HOME") != NULL) userHome = getenv("HOME");
				else if(getenv("USERPROFILE") != NULL) userHome = getenv("USERPROFILE");
				else if(getenv("HOMEPATH") == NULL) return;
				else
				{
					string drive{getenv("HOMEDRIVE")};
					userHome = drive + getenv("HOMEPATH");
				}
			#else
				if(getenv("HOME") != NULL) userHome = getenv("HOME");
			#endif

			string str(mPath);
			replaceAll(str, "~", userHome);
			mPath = str;
		}

		Path getExpandedUserPath(Path mPath) { expandUserPath(mPath); return mPath; }
	}
}

