#pragma once

#include <stdexcept>
#include <string>
#include <sstream>


class Exception: public std::runtime_error
{
public:
	Exception(const std::string& msg, int line):
		std::runtime_error("")
	{
		std::stringstream ss;
		ss << msg << " on line " << line;
		static_cast<std::runtime_error&>(*this) = std::runtime_error(ss.str());
	}
};

