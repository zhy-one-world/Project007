/********************************************************************
	created:	2014/07/26
	created:	26:7:2014   16:22
	file base:	guid_gen
	file ext:	h
	author:		zhy
	
	purpose:	
*********************************************************************/
#pragma once

#include "logic/type_def.hpp"

namespace faith
{
	class random_gen
	{
	public:
		static void set_seed(int64 seed);
		static int32 get_random(int32 nmin, int32 nmax);
		static uint32 skill_random();
		static void skill_seed(int32 seed_value);
	private:
		static int32 randseed;
	};
}
