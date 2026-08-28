/********************************************************************
  created: 2019年4月4日
  file base: cross_server_harry_ws_mgr
  file ext: hpp
  author: zhangshuo
  purpose: 
*********************************************************************/
#ifndef _CROSS_SERVER_HARRY_WS_MGR_WS_H_
#define _CROSS_SERVER_HARRY_WS_MGR_WS_H_

#include "server/client_session_mgr.hpp"
#include "Logic/cross_server_harry_def.hpp"
#include "server_def.hpp"
#include "cross_server_harry_msg.hpp"

namespace hld
{
	class game_proto_cross_server_harry_one_msg;

	class cross_server_harry_ws_mgr
	{
	public:
		static cross_server_harry_ws_mgr& get_instance()
		{
			static cross_server_harry_ws_mgr instance;
			return instance;
		}
		void						init_manager();
		void						tick(int64 time_new);
		void						tick_10sec(int64 time_new);
		void						tick_sec(int64 time_new);
		void						clear_data();
		
		void						init_server_money();//初始化服务器繁荣值
		void						init_server_money_with_refresh();//初始化每周更新的繁荣值

		bool						is_in_harry_game_time();
		bool						is_in_harry_special_game_time();

		void						save_to_db_server_harry_all_msg();
		void						send_load_db_server_harry_all_msg();
		void						recv_load_all_server_harry_msg(bool is_empty, int32 data_num, const s_server_harry_msg* server_harry_msg_arr);
		void						send_del_one_server_msg_to_dp(int32 server_id);

		void						func_when_add_other_server(int32 server_id);
		void						func_when_remove_other_server(int32 server_id);
		void						init_one_msg_by_server_id(int32 server_id);
		void						refresh_server_harry_with_server_list();
		bool						is_have_this_server_id(int32 server_id);

		void						send_all_harry_msg_to_other_server(int32 server_id = 0, bool is_need_set_name = false);//gate数据发送到其他服
		void						recv_gate_harry_msg(const s_server_harry_msg* server_harry_msg_arr, int32 data_num, bool is_need_set_name);

		void						clear_change_value();//清理缓存的变化量
		void						send_cur_server_change_harry_msg_to_gate_server();//将本服的数据同步到gate上
		void						recv_server_harry_msg(const s_harry_money_change* server_harry_msg, int32 data_num);//其他服

		void						change_server_money(bool is_special, int32 change_num, int32 server_id);//同步

		s_server_harry_msg&			get_cur_server_money();
		s_server_harry_msg&			get_server_money_by_server_id(int32 server_id);

		void						send_harry_msg_to_one_client(client_session* session_ptr);
		void						fill_harry_one_msg(game_proto_cross_server_harry_one_msg& one_msg, const s_server_harry_msg& harry_msg);
		void						send_harry_msg_to_all_client();

		void						send_server_harry_msg_to_cs(int32 conn_index = -1);

		void						make_special_money_with_special_time();//特殊时间内，给其他WS添加特殊采集次数
		void						clear_all_server_special_money();//清理掉所有服务器的特殊采集次数
		void						make_special_money_for_other_server();//为剩下的服务器添加采集次数

		void						set_server_harry_best_player(int32 server_id, int64 first_player_guid, int64 second_player_guid, int64 third_player_guid, const xchar* player_name);
		void						send_server_harry_best_player_msg_to_gate();

		bool						is_can_join_harry_map(client_session* session_ptr);

		void						send_award_with_player_defend_value();//根据守护值排行发奖励
		void						give_award_with_player_defend_value(guid_64 player_guid, int32 player_rank, const xchar* player_name);

		void						send_award_with_server_money_rank();//根据服务器繁荣度排行发奖励
		void						recv_gate_with_server_money_rank(int32 server_rank);//服务器发放服务器排名奖励
		void						get_mail_data(s_mail_info& mail_info, s_item_info* item_array, int32& item_num, const std::vector<s_item_template_info>& item_list, const xstring& title, const xstring& content);

		void						load_fake_data(int32 active_type);//读取前三名玩家的数据
		void						load_fake_data_with_fake_type(int32 fake_type);

		void						handle_base_detail_info(int32 big_type, const s_unit_info role_info);
		void						handle_item_info(int32 big_type, const s_item_info* item_info);
		void						handle_att_info(int32 big_type, const s_base_att_info& att_info);
		void						handle_skill_info(int32 big_type, const s_skill_info* skill_info);
		void						handle_buff_info(int32 big_type, const s_buff_info* buff_info);
		void						handle_belief_info(int32 big_type, const s_belief_info* belief_info);
		void						handle_fight_att_info(int32 big_type, const s_arena_char_fight_att& att_info);

		void						create_harry_player(int32 active_type);
		void						set_create_need_harry_player(bool is_need) { m_need_create_player = is_need; };
		void						set_is_need_create_harry_player(bool need_create);//每天零点刷新一次，防止特殊情况导致雕像不出现

		void						init_harry_map();
		void						get_valid_harry_map(guid_64& map_guid, int32& war_idex);
		void						recv_other_ws_transfer_gate_map(guid_64 role_guid, int32 server_id);

		void						load_end_fake_player(int32 idex);
	private:
		explicit cross_server_harry_ws_mgr();

		s_server_harry_msg			m_server_harry_msg_arr[max_server_num];
		s_server_harry_msg			m_empty_server_harry_msg;

		s_server_harry_msg			m_dp_harry_msg_arr[dp_save_max_server_num];

		std::map<int32, int32>		m_server_change_harry_msg;//普通采集变化量
		int32						m_server_change_special_money_num;//特殊采集变化量

		int64						m_last_tick_time;
		int64						m_timer;
		int64						m_last_10sec_tick_time;
		int64						m_last_sec_tick_time;

		int64						m_need_set_next_special_money_time;//GATE服设置下一批特殊采集的时间
		std::vector<int32>			m_need_set_special_server_idex_arr;//需要设置的服务器有哪些
		bool						m_is_set_special_server_money;

		bool						m_is_have_change;//是否有变化，决定是否需要发送到各个WS同步

		s_fake_player_info			m_player_info_array[e_fake_type_max];//前三名的玩家的数据

		bool						m_need_create_player = false;//需要创建雕像

		bool						m_is_load_harry_player = false;//是否已经加载雕像数据
		bool						m_is_load_end = false;

		bool						m_is_need_init_server_money = false;//是否需要重置所有服务器繁荣度

		bool						m_is_load_all_harry_msg = false;//是否已经读取掠夺全部数据

		bool						m_can_load_fake_player = false;//可以开始读取雕像数据
		bool						m_is_recv_gate_server_msg = false;

		guid_64						m_harry_map_guid;
	};
}

#endif
