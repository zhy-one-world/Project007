#ifndef _STARARK_MGR_H
#define _STARARK_MGR_H


#include "Logic/starark_def.h"
#include "template/ServerTemplateHead.h"


namespace faith
{


	struct my_iterator
	{

		my_iterator(int32 step, s_starark_memory_info *p_array);

		void get_area(int32 step, int32& start, int32& end);

		void  operator++()
		{
			++m_pos;
		}
		void  operator++(int32)
		{
			m_pos++;
		}
	
		void  begin()
		{
			m_pos = m_start;
		}
		s_starark_memory_info *get()
		{
			return m_array + m_pos;
		}
		int32 get_idx()
		{
			return m_pos;
		}

		bool  end()
		{
			return m_pos < m_end;
		}
		int32 m_start;
		int32 m_end;
		int32 m_pos;
		s_starark_memory_info *m_array;
	};


	class starark_mgr 
	{
	public:
		starark_mgr();
		~starark_mgr();
	public:
		void clear_data();
		void set_player_ptr(const int32 array_index);
	public:
		void save_starark_to_db(int32 save_type);
		void load_starark_by_db(const s_starark_info & data_info);
		bool load_starark_by_proto_db(const char *data, int32 len);
		void info_to_string();
		void string_to_info();
	public:
		void sync_all_message_to_client();
	public:
		void  on_operate(int32 oper_type, int32 param1, int32 param2,  int32 param3);
		void  oper_msg(int32 oper_type, int32 result, int32 pos = 0, int32 num = 0);
		void  on_load(int32 oper_type, int32 step, int32 item_id , int32 step_pos); 
		void  on_unload(int32 oper_type, int32 step, int32 item_id, int32 step_pos);
		void  on_change(int32 oper_type);
		void  on_buy(int32 oper_type, int32 num);
		void  on_round(int32 oper_type);
		void  on_five_round(int32 oper_type);
	public:
		void  get_act_item_by_step(int32 step, int32 item_id, int32 &item_num); // para1-3
		const std::vector<int32> * get_act_all_weight();                        // param5
		const std::vector<int32> * get_act_cost_array();                        // condtion 
		bool  get_act_need_item_num(int32 times, int32& need_item_id, int32& need_item_num);     // reward

		bool  get_act_need_five_item_num(int32 times, int32& need_item_id, int32& need_item_num);     // reward
	public:
	public:
		void  get_area(int32 step, int32& start, int32& end);
		void  get_per_area_num(int32 cur_num_array[starark_max_step_num]);
		int32 get_step(const std::vector<int32> *p_config, int32 cur_num_array[starark_max_step_num]);
		int32 get_step_offset(int32 step, int32 num);
		int32 get_step_times(const std::vector<int32> *p_config,int32 step);

	public:
		int32 get_data(int32 idx) const;
		void  set_data(int32 idx, int32 value);
		s_starark_memory_info * get_round() { return m_round_ary; }
		void  set_round_data(int32 idx, bool is_add, int32 item_id = 0, int32 item_num = 0);
	public:
		void  restart();
		void  refresh_recycle();


	private:
		int32 m_array_index;
		s_starark_info   m_data;
		s_starark_memory_info  m_round_ary[starark_round_max];
			
	};

}


#endif