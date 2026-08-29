#pragma once

#include "logic/type_def.hpp"

namespace faith
{
	class raid_info
	{
	private:
		int32	m_cur_wave;
		int32	m_max_wave;
		int32	m_cur_monster;
		int32	m_max_monster;
	};
}