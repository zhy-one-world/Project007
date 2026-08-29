#ifndef PHANTOM_MGR_H
#define PHANTOM_MGR_H

#include "phantom.h"

namespace faith
{
	typedef std::map<int32, cphantom> phantom_map;
	typedef std::map<int32, cphantom>::iterator  phantom_map_it;

	struct s_id_pos
	{
		int32 id;
		int32 pos;
		s_id_pos(int32 in_id, int32 in_pos)
		{
			id = in_id;
			pos = in_pos;
		}
	};
	typedef std::map<int32, std::vector<s_id_pos>> config_map;
	typedef std::map<int32, std::vector<s_id_pos>>::iterator config_map_it;

	class player;
	class unit;
	class citem;
	class cphantom_config;


	class cphantom_mgr
	{
	public:
		cphantom_mgr();
		~cphantom_mgr();
	public:
		void clear_data();
		void set_player_ptr(const int32 array_index);
	public:
		void save_phantom_to_db(int32 save_type);
		void load_phantom_by_db(const s_phantom_info * phantom_data, int32 load_num);
		bool load_phantom_by_db_lua(const char *data_ptr, int32 data_len);
	public:
		void sync_all_message_to_client();
		void sync_one_message_to_client(cphantom &temp_phantom);
	public:
		void create_phantom_by_info(const s_phantom_info & phantom_info);
	public:
		int32 get_player_class();
		void oper_active(int32 phantom_id);
		void active_skill(int32 skillid);
		void oper_msg(int32 phantom_id, int32 result);
	public:
		void on_item_oper(citem* changed_item_ptr, int32 item_template_id);
		bool phantom_event(int32 phantom_id, int32 pos);
	public:
		void on_item_load_end();
		bool is_item_finish(std::vector<s_id_pos> & in_vec);
		bool is_phantom_finish(int32 phantom_id, int32 pos);
		bool is_item_con(citem *item_ptr);

		void init_all_skill();
	public:
		cphantom& lua_find_object(int32 phantom_id);
		
		
	private:
		int32 m_array_index;
		phantom_map m_data_map;
		cphantom  m_empty_obj;
	public:
		static cphantom_config m_phantom_config;
	};

	class cphantom_config
	{
	public:
		cphantom_config();
		~cphantom_config(){}
	public:
		std::vector<s_id_pos>& get_id_pos_list(int32 item_template_id);
		config_map& get_all_config();
		void build();
		void add(int32 item_template_id, int32 id, int32 pos);
		bool is_exist_id_pos(std::vector<s_id_pos> & vec_list, int32 id, int32 pos);
	public:
		config_map m_config_map;
		bool m_is_read = false;
		std::vector<s_id_pos> m_empty_list;
	};






}


#endif