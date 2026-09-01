/********************************************************************
	created:	2014/07/26
	created:	26:7:2014   16:22
	file base:	guid_gen
	file ext:	cpp
	author:		zhy
	
	purpose:	
*********************************************************************/

#include <time.hpp>
#include <random.hpp>
#include "random.h"

namespace faith
{
	int32 random_gen::randseed = 0;

	void random_gen::set_seed(int64 seed_value)
	{
		utility::set_seed(seed_value);
	}

	int32 random_gen::get_random(int32 nmin, int32 nmax)
	{

		if (nmax < 0)
		{
			return utility::rand() % (nmax - 1 - nmin) + nmin;
		}
		else
		{
			return utility::rand() % (nmax + 1 - nmin) + nmin;
		}

	}
	uint32 random_gen::skill_random()    //生成第n+1个的随机数过程
	{
		uint32 r;
		/* 返回一个无符号32位整型的伪随机数. */
		r = randseed = randseed * 1103515245 + 12345;
		return (r << 16) | ((r >> 16) & 0xFFFF);
	}
	void random_gen::skill_seed(int32 seed_value)
	{
		randseed = seed_value;
	}
}
