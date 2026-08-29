
#pragma once
#include "type_def.hpp"

namespace faith
{
	std::string int_to_string(int64 num)
	{
		std::string temp_string;
		std::stringstream temp_string_stream;
		temp_string_stream << num;
		temp_string_stream >> temp_string;
		return temp_string;
	}

	std::string float_to_string(float num)
	{
		std::string temp_string;
		std::stringstream temp_string_stream;
		temp_string_stream << num;
		temp_string_stream >> temp_string;
		return temp_string;
	}
}
