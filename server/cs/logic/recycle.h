#ifndef RECYCLE_H
#define RECYCLE_H

#include "Logic/recycle_def.h"


namespace faith
{
	class player;
	class unit;

	class recycle
	{
	public:
		recycle();
		~recycle();
	public:
		void clear_data();
		void cycle_reset();
	public:
		s_recycle_info& get_db_data() { return m_data_info;}
		void  set_db_data(const s_recycle_info& in_data_info);
	public:
		void set_start_time(int32 in_start_time);
		int32 get_start_time();
		//bool is_range_time();
		//int32 get_delta_time();
	public:
		bool  get_login_flag_bit(int32 idx);
		void  set_login_flag_bit(int32 idx);
	public:
		bool  get_prop_flag_bit(int32 idx);
		void  set_prop_flag_bit(int32 idx);
	public:
		bool  is_key_reward();
		void  set_key_reward(guid_64 in_inviter_guid);
	public:
		bool  get_invited_level_flag_bit(int32 idx);
		void  set_invited_level_flag_bit(int32 idx);


	private:
		s_recycle_info m_data_info;

	};

	/////////////////////////////////////  recycle  task////////////////////////////////////////////////////////

	class recycle_task
	{
	public:
		recycle_task();
		~recycle_task();
	public:
		void clear_data();
		bool init_recycle_task_by_template(int32 recycle_task_id);
		bool init_recycle_task_by_info(const s_recycle_task_info &task_info);
		RecycleTemplate* get_config_ptr() { return m_config_ptr; }
	public:
		bool set_inst_data(int32 idx, int32 value);
		int32 get_inst_data(int32 idx) const;
		s_recycle_task_info& get_recycle_task_info_all() { return m_recycle_task_info; }
	public:
		bool is_finish_num();
		bool add_finish_num(int32 add_num);



	
	private:
		RecycleTemplate  *m_config_ptr;
		s_recycle_task_info  m_recycle_task_info;
	};

	typedef std::map<guid_64, s_recycle_invited_info> recycle_invited_map;
	typedef std::map<guid_64, s_recycle_invited_info>::iterator  recycle_invited_map_it;


	class recycle_invited
	{
	public:
		recycle_invited();
		~recycle_invited();
	public:
		void clear_data();
		void set_player_ptr(const int32 array_index);

	public:
		void add(const s_recycle_invited_info & s_invited_info, bool is_send = false);
	
	public:
		void sync_all_invited_list();
		void sync_one_invited_one(const s_recycle_invited_info & temp_info);
		int32 get_grade_level(int32 head, int32 class_type);

		bool is_get_reward(int32 level, int32  num);
	public:
		void tick();




	private:
		int32  m_array_index;
		recycle_invited_map m_data;


	};




}



#endif
