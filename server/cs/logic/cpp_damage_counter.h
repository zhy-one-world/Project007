#pragma once
#include "logic/type_def.hpp"

namespace hld
{
	class unit;
	struct use_buff_param;
	class cpp_damage_counter
	{
	public:
		static void	buff_damage_by_server(use_buff_param& param, bool be_server);//服务器伤害计算
	public:
		static void	buff_damage_by_client(use_buff_param& param);//客户端做命中和暴击处理
	private:
		static void damage_reflect(unit& attacker, unit& targeter, double end_damage, use_buff_param& param);//伤害反射
		static void suck_blood(unit& attacker, unit& targeter, double end_damage, use_buff_param& param);//吸血
	};
}
