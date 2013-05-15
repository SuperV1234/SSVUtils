// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_ENCRYPTION_BASE64
#define SSVU_ENCRYPTION_BASE64

#include <string>

namespace ssvu
{
	std::string Base64Encode(const std::string& mString);
	std::string Base64Decode(const std::string& mString);
}

#endif

// TODO: put all encryption stuff in an Encryption namespace and create consistent public interfaces
