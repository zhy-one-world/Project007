#ifndef RECYCLE_MGR_H
#define RECYCLE_MGR_H

#include "recycle.h"

namespace faith
{
	class player;
	class unit;


	typedef std::map<int32, recycle_task> recycle_task_map;
	typedef std::map<int32, recycle_task>::iterator  recycle_task_map_it;


	struct s_recyle_id_pos
	{
		int32 id;
		int32 pos;
		s_recyle_id_pos(int32 in_id, int32 in_pos)
		{
			id = in_id;
			pos = in_pos;
		}
	};
	

	class recycle_mgr
	{
	public:
		recycle_mgr();
		~recycle_mgr();
	public:
		void clear_data();
		void set_player_ptr(const int32 array_index);
	public:
		void save_recycle_to_db(int32 save_type);
		void load_recycle_by_db(const s_recycle_info & recycle_data);
		bool load_recycle_by_db_lua(const char *data_ptr, int32 data_len);
	public:
		void save_recycle_task_to_db(int32 save_type);
		void load_recycle_task_by_db(const s_recycle_task_info *data_array, int32 data_num);
		bool load_recycle_task_by_db_lua(const char *data_ptr, int32 data_len);
		void create_recycle_task_by_info(const s_recycle_task_info & recyle_task_info);
	public:
		int32 get_config_param(e_recycle_param param_type);
	public:
		int32 get_delta_time();
		int32 get_time_zone();
		void  gm(int32 timestamp = 0);
	public:
		void load_db_finish();
		void load_player_time_end_init();
		bool is_refresh();
		void refresh_cycle();
	public:
		void sync_all_message_to_client();
		void sync_one_login_message_to_client(int32 recycle_id);
		void sync_one_task_message_to_client(recycle_task & task_info);
		void sync_one_buy_message_to_client(int32 recycle_id);

	public:
		void get_recycle_id_pos_by_type(std::vector<s_recyle_id_pos>& out_vec_info, e_recycle_type type);
		RecycleTemplate *get_recycle_template_by_id(int32 recycle_id);
		RecycleTemplate *get_recycle_template_by_task_type(e_recycle_task_type task_type);
		RecycleTemplate *get_recycle_template_by_map_id(int32 map_id);
	public:
		void oper_end_msg(int32 recycle_id, int32 type, int32 result);
		
	public:
		bool is_buy(RecycleTemplate *recycle_template_ptr);
	    void buy_consume(RecycleTemplate *recycle_template_ptr);
		void get_recycle_reward(RecycleTemplate *recycle_template_ptr);
	public:
		void on_event(e_recycle_task_type task_type, int32 add_num = 1, int32 map_id = 0);
		void on_map_event(int32 map_id, int32 add_num = 1);
		void event_process(RecycleTemplate *recycle_template_ptr,int32 add_num);


	public:
		recycle& get_recycle_obj() { return m_recycle_obj; }


	
	private:
		int32    m_array_index;
		int32    m_load_num;
		int32    m_zone_sec = 0;
	private:
		recycle  m_recycle_obj;

	private:
		recycle_task_map m_recycle_task_data;
		recycle_task  m_empty_task;
	public:
		recycle_task&  get_recycle_task_by_id(int32 recycle_id);

	//////////////////////////////////////////////////////////////////////////////////
	public:
		RecycleTemplate *get_recycle_template_by_level(int32 level);
		int64 string_to_int64(xstring & s_digit);
		void  on_process_invited_reward(xstring & s_guid);
		void  send_to_client_msg_by_invited_reward(int32 result);
		void  send_to_dp_find_player(guid_64 target_guid, guid_64 role_guid);
		void  send_to_dp_find_player_end(const s_recycle_inviter_base_info_db & data_info);
		void  get_invited_reward(guid_64 target_guid);
	public:
		bool  m_is_request = false;
	public:
		void  save_to_dp_add_invited(guid_64 target_guid, guid_64 invited_guid);
	public:
		void  load_recycle_invited_by_db(const s_recycle_invited_info *data_array, int32 data_num);
		bool  load_recycle_invited_by_db_lua(const char *data_ptr, int32 data_len);
		void  add_invited_info(s_recycle_invited_info & new_invited_info);
	public:
		void  tick();
	public:
		void  sync_all_invited_list();
	public:
		void  sync_one_invited_message_to_client(int32 recycle_id);
		
	private:
		recycle_invited  m_recycle_invited_obj;
	public:
		recycle_invited& get_recycle_invited_obj() { return m_recycle_invited_obj; }

	};

	struct s_recycle_update_info
	{
		int32 m_level;
		int32 m_head;
		int32 m_class_type;
		s_recycle_update_info()
		{
			memset(this, 0, sizeof(s_recycle_update_info));
		}
		s_recycle_update_info(int32 in_level, int32 in_head, int32 in_class_type)
		{
			m_level = in_level;
			m_head = in_head;
			m_class_type = in_class_type;
		}
	};

	typedef std::map<guid_64, s_recycle_update_info> level_update_map;
	typedef std::map<guid_64, s_recycle_update_info>::iterator  level_update_map_it;

	class  recycle_man
	{
    public:
		recycle_man() ;
		~recycle_man() ;
	public:
		static recycle_man& get_instance();
	public:
		void  update_level_info(guid_64 guid, int32 in_level, int32 in_head, int32 class_type);
		s_recycle_update_info& get_level_info(guid_64 guid);
	private:
		level_update_map m_level_update_data;
		s_recycle_update_info empty_data;
	};


}



#endif
