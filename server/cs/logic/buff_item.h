/********************************************************************
created:	2010/03/03
created:	3:3:2010   14:47
filename: 	d:\worksite\skill\LowServer\octopus\app\cs\Low\New_Buff\buff_item.h
file path:	d:\worksite\skill\LowServer\octopus\app\cs\Low\New_Buff
file base:	buff_item
file ext:	h
author:		zhy

purpose:
*********************************************************************/
#ifndef _buff_inst_h_
#define _buff_inst_h_

#include "Logic/buff_def.hpp"
#include "buff_action.h"
#include "event_processer.h"
#include "internet/buff.pb.h"

namespace faith
{
	struct event_trigger_buf;
	class buff_proto_buff_info;
	class buff_proto_buff_info_all;
	enum e_cyc_action_lanuch_type
	{
		ecalt_lanuch_interval,		//间隔触发
		ecalt_lanuch_timepoint,		//定时触发
	};

	struct effect
	{
		effect()
		{
			effected = false;
		}
		bool effected;
		buff_action action;
	};

	struct effect_cyc : public effect
	{
		effect_cyc()
		{
			m_lanuch_type = 0;
			m_interval = 0;
		}
		uint8	 m_lanuch_type;
		int64 m_interval;
	};

	struct effect_event : public effect
	{
		effect_event()
		{
			m_event_type = 0;
		}
		int8 m_event_type;
	};

	class buff_item :public game_event_handler
	{
		friend class buff_set;
	public:
		buff_item();
		~buff_item();
		//构造、销毁
	public:
		bool						on_event(s_trigger_param& trigger_param);
		void						buff_event_param(bool apply);
		void						trigger_event(int32 target_index, bool is_add);
		bool                        is_zlzz();
		void                        send_show();
	public:
		int32						heart_tick(const int64& new_time);
		bool&						is_valid() { return m_data_use; }
		void						set_valid_state(bool valid_value) { m_data_use = valid_value; }
		void						clear_data();
		void						init_data(const int32& owner_index, const int32& sender_index, const guid_64& buff_guid, const int32& buff_template_id, const int32& skill_template_id, const bool& is_ignore_att = false, const int32& effect_index = -1, int32 buff_level = 1);
		bool						load_from_template(const int32& buff_template_id, const int32& skill_template_id, int32 effect_index = -1);
	public:
		void						set_buff_info(EBuffInfo e_buff_info, int32 buff_value);
		int32						get_buff_info(EBuffInfo e_buff_info);
		buff_proto_buff_info&		get_buff_info_all() { return m_buff_data; }
		guid_64&					get_buff_guid() { return m_buff_guid; }
		void						set_buff_guid(const guid_64& buff_guid) { m_buff_guid = buff_guid; }
		BuffDataTemplate*			get_buff_data_template() { return m_buff_template_ptr; }
		s_buff_info					get_fake_buff_info_inst();
		void						check_need_update();
	public:
		void						change_buff_percent(const float& percent_value);
		float						get_buff_percent();
		void						change_life_time(const float& during_time_fix);
		void						add_life_time(const float& during_time_fix);
	public:
		void						set_sender_index(int32 sender_index) { m_sender_index = sender_index; }
		const int32&				get_sender_index() { return m_sender_index; }
		void						set_owner_index(int32 owner_index) { m_owner_index = owner_index; }
		const int32&				get_owner_index() { return m_owner_index; }
	public:
		int32						get_max_pile_count();
		int32						get_cur_pile_count();
		void						change_cur_pile_count(int32 pile_count);
		//属性
	public:
		bool						can_use();
		bool						can_cannel();
		int32						get_buff_template_id();
		int32						get_skill_template_id();
		int64&						get_buff_time();
		void						set_buff_time(int64 buff_time);
		void						add_buff_time(int64 buff_time);
		int32						get_type();
		int32						get_level();
		int32						get_race();
		int32						get_save_type();
		int32						is_pile_spec();
		int32						is_sync();
		int32						is_dead_del();
		int32						is_offline();
		int32						is_map_end();
		bool						is_keep();
		bool						is_empty();
		uint32						get_series();

		int32						is_same_level_cover();
		int32						is_diff_level_cover();

		int32						is_same_level_same_sender_wrap();
		int32						is_same_level_diff_sender_wrap();

		int32						is_same_level_time_cumulative();
		int32						is_diff_level_time_cumulative();

		int32						is_del_when_reset_skill();

		e_buff_add_judge			add_buff_type(const int32& sender_index, const int32& buff_template_id);

		void						set_is_ignore_att(bool is_ignore) { m_is_ignore_att = is_ignore; };
		//方法
	public:
		void							pile_one(buff_ex_env_param* pEnv, bool change_level = false);
		int32							dec_pile();
		void							dec_all_pile(bool time_over = false);
		//行为相关回调函数
	public:
		bool						begin_effect(buff_ex_env_param* pEnv);
		bool						end_effect(buff_ex_env_param* pEnv);
		bool						begin_buff(buff_ex_env_param* pEnv);
		bool						cyc_buff(const int64& new_time);
		void                        cyc_buff_show();
		bool						end_buff(buff_ex_env_param* pEnv, bool time_over = false);

		//数据库、同步相关工具函数
	private:
		void						send_buff_by_add(buff_ex_env_param* pEnv);
		void						send_buff_by_del();
		void						send_buff_by_inf(const int32& recv_id = 0);
		void						get_buff_info_msg(buff_proto_buff_info_all& buff_info_all_msg);
		void						save(buff_proto_buff_save_load& request_save);
		bool						load(const buff_proto_buff_info& buff_data);

	private:
		bool						m_is_end;
		bool						m_data_use;
		int32						m_sender_index;					//发送者
		int32						m_owner_index;					//拥有者
		guid_64						m_buff_guid;					//buff id (同一player上标识不同buff)
		int64						m_life_time;					//buff存在时间
		buff_proto_buff_info		m_buff_data;					//buff数据
		BuffDataTemplate*			m_buff_template_ptr;			//模板指针
		std::vector<effect>			m_begin_action;					//开始的action
		int32						m_begin_action_num;
		std::vector<effect_cyc>		m_cyc_effect;					//循环的action
		int32						m_cyc_action_num;
		std::vector<effect>			m_end_action;					//结束action
		int32						m_end_action_num;
		std::vector<effect>			m_effect_action;				//持续action
		int32						m_effect_action_num;
		std::vector<effect>			m_event_action;					//事件action
		int32						m_event_action_num;
		int64						m_last_action_tick;
		int64						m_jump_tick;
		buff_env_param				m_env;
		bool						m_trigger_event;
		bool						m_is_ignore_att;
		bool						m_is_send_client;
		bool                        m_is_send_one;
	};
}

#endif // _buff_item_h_
