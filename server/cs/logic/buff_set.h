/********************************************************************
created:	2010/03/01
created:	1:3:2010   14:21
filename: 	d:\worksite\skill\LowServer\octopus\app\cs\Low\New_Buff\buff_set.h
file path:	d:\worksite\skill\LowServer\octopus\app\cs\Low\New_Buff
file base:	buff_set
file ext:	h
author:		zhy

purpose:
*********************************************************************/
#ifndef _BUFF_SET_H_
#define _BUFF_SET_H_

#include "Logic/buff_def.hpp"
#include "buff_item.h"

namespace hld
{
	class buff_proto_buff_save_load;

	//	struct game_event;
	typedef std::vector<buff_item*> buff_item_map;
	typedef buff_item_map::iterator buff_item_map_it;
	class buff_set
	{
		typedef std::vector<buff_item> buff_inst_vector;
		typedef buff_inst_vector::iterator buff_inst_vector_it;
	public:
		buff_set();
		~buff_set();
	private://内存已经创建好了 禁止拷贝
		buff_set(const buff_set& unit_ref);
		buff_set& operator=(const buff_set&);
	public:
		void set_owner_array_index(int32 unit_array_index);
		void heart_tick(const int64& new_time);
		void clear_data();
	public:
		bool save_buff(int32 save_type_ex);
		void load_buff(const buff_proto_buff_save_load& load_data);
		void sync_to_client(const int32& recv_id);
	public:
		buff_item* add_buff_item(const int32& sender_index, const int32& buff_template_id, int32 skill_template_id, bool is_ignore_att = false, int32 effect_index = -1, int32 buff_level = 1);
		void remove_buff_item(const guid_64& buff_guid);
		buff_item* get_buff_item_by_guid(const guid_64& buff_guid, int32 sender_index = 0, bool same_sender_del = false);
		buff_item* get_buff_item_by_series(const int32& sender_index, const int32& series_type, bool same_sender_del = false);
		buff_item* get_buff_item_by_template_id(const int32& sender_index, const int32& buff_template_id, bool same_sender_del = false);
	public:

		buff_item*  add_buff(const int32& sender_index, const int32& buff_template_id, buff_ex_env_param* pEnv, bool is_ignore_att = false, int32 effect_index = -1);
		buff_item* add_buff_inst(const int32& sender_index, const int32& buff_template_id, buff_ex_env_param* pEnv, bool is_ignore_att = false, int32 effect_index = -1);


		buff_item*  add_buff_pile(const guid_64& buff_guid, buff_ex_env_param* pEnv);

		bool dec_buff_pile(const int32& sender_index, const guid_64& buff_guid, bool same_sender_del);

		bool del_buff_pile_by_template_id(const int32& sender_index, const int32& buff_template_id, bool same_sender_del);

		bool del_buff_by_inst_id(const int32& sender_index, const guid_64& buff_guid, bool server, bool same_sender_dell);

		bool del_buff_by_template_id(const int32& sender_index, const int32& buff_template_id, bool same_sender_del);

		bool del_buff_by_template_id_one(const int32& sender_index, const int32& buff_template_id);

		bool del_buff_by_series_id(const int32& sender_index, const int32& series_id, bool same_sender_del);

		bool del_buff_when_map_end();

		bool del_all_buff(const int32& sender_index, bool same_sender_del);
		bool del_buff_by_type(const int32& buff_type);


		bool is_have_buff_by_inst_id(const int32& sender_index, const guid_64& buff_guid, bool same_sender_del);

		bool is_have_buff_by_template_id(const int32& sender_index, const int32& buff_template_id, bool same_sender_del);

		bool is_have_buff_by_serise_id(const int32& sender_index, int32 serise_id, bool same_sender_del);

		int32 get_buff_pile_by_inst_id(const int32& sender_index, const guid_64& buff_guid, bool same_sender_del);

		int32 get_buff_pile_by_template_id(const int32& sender_index, const int32& buff_template_id, bool same_sender_del);

		int32 get_buff_pile_by_series_id(const int32& sender_index, const int32& series_id, bool same_sender_del);

		void offline_buffs();

		guid_64 find_buff_by_template_id(const int32& sender_index, const int32& buff_template_id, bool same_sender_del);
		void mod_buff_during_time_by_series_id(const int32& series_id, const int32& during_time_fix);

		int32 init_fake_buff_info(s_buff_info * fake_buff_info, int32 max_num);

		void update_all_buff_need_refresh();
	private:
		buff_item* tile_buff(const int32& sender_index, const int32& buff_template_id, buff_ex_env_param* pEnv, bool is_ignore_att = false, int32 effect_index = -1);
		buff_item*  cover_buff(const int32& sender_index, const guid_64& buff_guid, const int32& buff_template_id, buff_ex_env_param* pEnv, int32 effect_index = -1);
		buff_item*  wrap_buff(const int32& sender_index, const guid_64& buff_guid, const int32& buff_template_id, buff_ex_env_param* pEnv);
		buff_item*  cumulative_buff(const int32& sender_index, const guid_64& buff_guid, const int32& buff_template_id, buff_ex_env_param* pEnv);
		buff_item*  percent_buff(const int32& sender_index, const guid_64& buff_guid, const int32& buff_template_id, buff_ex_env_param* pEnv);

		bool kill_buff(const guid_64& buff_guid, bool server = true);
	private:
		int32					m_owner_array_index;
		buff_item_map			m_buff_inst_vector;
		uint32					m_buff_inst_vector_len;

		int64					m_buff_update_time;		//每秒刷新一次是否需要向客户端刷新buff
	};
}

#endif // _buff_set_h_