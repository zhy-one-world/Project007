#include "world_boss_ws.h"
#include "utility/cs_date.hpp"
#include "template/template_manager.h"

namespace faith
{

	world_boss_ws::world_boss_ws()
	{
		clear_data();
	}

	world_boss_ws::~world_boss_ws()
	{


	}
	void world_boss_ws::clear_data()
	{
		m_spawn_template_ptr = nullptr;
		m_npc_template_ptr = nullptr;
		m_is_creating = false;
		m_is_need_pre_create = false;
		m_is_recv_get_prize_msg = false;
		m_hp_per = 0;
		m_cur_guid.clear_data();
		m_boss_random_pos_id = -1;
	}

	void world_boss_ws::init_by_info(s_world_boss_info temp_info)
	{
		NpcSpawnPointTemplate* temp_respawn_template_ptr = GET_TEMPLATE(NpcSpawnPointTemplate, temp_info.npc_respawn_point_template_id);
		if (temp_respawn_template_ptr == nullptr)
		{
			return;
		}
		NpcTemplate* temp_npc_template_ptr = GET_TEMPLATE(NpcTemplate, temp_respawn_template_ptr->TemplateId);
		if (temp_npc_template_ptr == nullptr)
		{
			return;
		}
		m_spawn_template_ptr = temp_respawn_template_ptr;
		m_npc_template_ptr = temp_npc_template_ptr;
		m_world_boss_data = temp_info;
	}

	void world_boss_ws::init_by_spawn_id(int32 spawn_template_id)
	{
		NpcSpawnPointTemplate* temp_respawn_template_ptr = GET_TEMPLATE(NpcSpawnPointTemplate, spawn_template_id);
		if (temp_respawn_template_ptr == nullptr)
		{
			return;
		}
		NpcTemplate* temp_npc_template_ptr = GET_TEMPLATE(NpcTemplate, temp_respawn_template_ptr->TemplateId);
		if (temp_npc_template_ptr == nullptr)
		{
			return;
		}

		m_spawn_template_ptr = temp_respawn_template_ptr;
		m_npc_template_ptr = temp_npc_template_ptr;
		m_world_boss_data.line_id = -1;
		m_world_boss_data.world_boss_npc_array_index = -1;
		m_world_boss_data.npc_respawn_point_template_id = spawn_template_id;
	}

	bool world_boss_ws::is_alive()
	{
		if (m_world_boss_data.is_alive > 0)
		{
			return true;
		}
		return false;
	}

	void world_boss_ws::increase_respawn_phase()
	{
		if (m_spawn_template_ptr == nullptr
			|| m_spawn_template_ptr->RespawnByDayArray.size() != e_world_boss_respawn_unit_max * 7)
		{
			return;
		}
		time_info cur_time_info = time_helper::get_cur_time_new();
		int32 start_index = e_world_boss_respawn_unit_max * cur_time_info.day_in_week;

		int32 next_phase = m_world_boss_data.respawn_phase + 1;
		for (; next_phase < e_world_boss_respawn_unit_max; ++next_phase)
		{//next_phase = e_world_boss_respawn_unit_max时，其实就说明当日刷新次数已经用完了
			int32 this_index = start_index + next_phase;
			if (this_index < 0
				|| this_index >= m_spawn_template_ptr->RespawnByDayArray.size()
				|| m_spawn_template_ptr->RespawnByDayArray[this_index] <= 0)
			{//如果当次刷新的下一次刷新以及之后的所有刷新位都是0，那next_phase会累加，一直到等于e_world_boss_respawn_unit_max后跳出
				continue;
			}

			time_info respawn_time = time_helper::get_time_by_today_stamp_new(m_spawn_template_ptr->RespawnByDayArray[this_index]);

			//这里必须确保Inscrease之后的值为不能及时创建的值，防止在一个较低的respawn_phase时，发生连续创建的情况
			//若发生上述情况，则可能造成cs创建回包不及时（unit_arry_index设置不及时），导致不能正常移除之前创建的NPC的情况
			if (cur_time_info.hour_in_day < respawn_time.hour_in_day)
			{
				break;
			}
			else if (cur_time_info.hour_in_day == respawn_time.hour_in_day
					&& cur_time_info.minute_in_hour <= respawn_time.minute_in_hour)
			{
				break;
			}
		}
		m_world_boss_data.respawn_phase = next_phase;
		if (m_world_boss_data.respawn_phase >= e_world_boss_respawn_unit_max)
		{
			m_world_boss_data.respawn_phase = e_world_boss_respawn_unit_max;
		}
	}

	void world_boss_ws::reset_respawn_phase()
	{
		if (m_spawn_template_ptr == nullptr
			|| m_spawn_template_ptr->RespawnByDayArray.size() != e_world_boss_respawn_unit_max * 7)
		{
			return;
		}
		time_info cur_time_info = time_helper::get_cur_time_new();
		int32 start_index = e_world_boss_respawn_unit_max * cur_time_info.day_in_week;

		m_world_boss_data.respawn_phase = e_world_boss_respawn_unit_max;
		//重置这个刷新阶段时，先将其设置为刷新全部进行过的数值，即e_world_boss_respawn_unit_max

		for (int32 i = 0; i < e_world_boss_respawn_unit_max; ++i)
		{
			int32 this_index = start_index + i;
			if (this_index < 0
				|| this_index >= m_spawn_template_ptr->RespawnByDayArray.size())
			{
				continue;
			}
			if (m_spawn_template_ptr->RespawnByDayArray[this_index] > 0)
			{
				//当有有效刷新时间数据时，即该表格数组对应位置值大于0时
				//再设置为有效的id
				m_world_boss_data.respawn_phase = i;
				break;
			}
		}
	}


	bool world_boss_ws::is_can_respawn()
	{
		if (m_is_creating)
		{
			return false;
		}
		if (m_is_need_pre_create)
		{
			return true;
		}
		if (m_world_boss_data.npc_respawn_point_template_id <= 0)
		{
			return false;
		}
		if (m_spawn_template_ptr == nullptr
			|| m_spawn_template_ptr->RespawnByDayArray.size() != e_world_boss_respawn_unit_max * e_week_day_max)
		{
			return false;
		}
		if (m_world_boss_data.respawn_phase >= e_world_boss_respawn_unit_max)
		{
			//说明这个Boss今天刷新次数已经满了
			return false;
		}

		time_info cur_time_info = time_helper::get_cur_time_new();
		int32 start_index = e_world_boss_respawn_unit_max * cur_time_info.day_in_week;
		int32 real_index = start_index + m_world_boss_data.respawn_phase;
		if (real_index < 0
			|| real_index >= m_spawn_template_ptr->RespawnByDayArray.size()
			|| m_spawn_template_ptr->RespawnByDayArray[real_index] <= 0)
		{
			return false;
		}

		time_info respawn_time_info = time_helper::get_time_by_today_stamp_new(m_spawn_template_ptr->RespawnByDayArray[real_index]);
		if (cur_time_info.hour_in_day > respawn_time_info.hour_in_day)
		{
			return true;
		}
		else if (cur_time_info.hour_in_day == respawn_time_info.hour_in_day
				&& cur_time_info.minute_in_hour >= respawn_time_info.minute_in_hour)
		{
			return true;
		}
		return false;
	}

	void world_boss_ws::set_world_boss_dead(const xchar* killer_name)
	{
		m_world_boss_data.is_alive = 0;
		m_world_boss_data.boss_daily_refresh = 1;
		m_world_boss_data.world_boss_npc_array_index = -1;
		memcpy(m_world_boss_data.killer_name, killer_name, max_name_size);

		//自动回收就没有击杀者
		if (0 == strlen(killer_name))
		{
			return;
		}
		if (0 == strlen(m_world_boss_data.first_killer_name))
		{
			memcpy(m_world_boss_data.first_killer_name, killer_name, max_name_size);
			m_world_boss_data.first_killed_time = time_helper::get_cur_time_new().second;
			m_world_boss_data.is_get_prize = e_first_kill_prize_type_not_get;
		}
	}
}
