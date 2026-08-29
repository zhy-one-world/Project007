#ifndef _CLOUD_SHOP_MGR_WS_H_
#define _CLOUD_SHOP_MGR_WS_H_

#include "logic/type_def.hpp"
#include "ws_client.hpp"
#include "Logic/cloud_shop_def.hpp"

namespace faith
{
	class cloud_shop_proto_cloud_shop_resp;
	class game_proto_gain_treasure_boss_info_one;
	class game_proto_gain_treasure_boss_info_all;

	typedef std::vector<cloud_shop_big_reward_content> cloud_shop_big_rewards;
	typedef std::map<guid_64, cloud_shop_role_record_data> cloud_shop_one_player_info_map;
	typedef std::map<int32, cloud_shop_one_player_info_map> cloud_shop_info_map;
	class cloud_shop_mgr
	{
	public:
		static cloud_shop_mgr& get_instance()
		{
			static cloud_shop_mgr instance;
			return instance;
		}

		explicit cloud_shop_mgr();
		void										clear_data();
		void										tick(const int64& new_time);
		void										init_manager();
		xstring										make_today_reward_string(const cloud_shop_big_rewards& big_reward_player_vec);
		void										check_state_need_change_by_time(const int64& new_time);
		void										load_role_record_data(const cloud_shop_role_record_data* load_data, int32 load_num, bool is_end);
		void										change_player_name_func(const guid_64& role_guid, const xstring& role_name);	//角色改名同步
		void										load_cloud_shop_players_info();
		cloud_shop_big_rewards						calc_big_reward(e_time_limit_activity_type activity_type, int32 big_reward_num, int32 basic_reward_num, const std::vector<int32>& rewards_array);
		void										send_cloud_shop_awards(e_time_limit_activity_type activity_type);
		void										send_rewards_and_clear_info(e_time_limit_activity_type activity_type);
		int32										get_left_count(e_time_limit_activity_type activity_type);
		void										handler_client_request(client_session* const pSession, e_client_req_type req_type);
		void										player_buy(e_time_limit_activity_type activity_type, const guid_64& player_uid, const xstring& player_name, int32 class_type, int32 req_type, int32 server_id = 0, int32 sender_server_id = 0);
		void										send_buy_result_to_player(const guid_64& player_uid, const int32 error_id, const int32 buy_type, int32 server_id = 0, int32 sender_server_id = 0);
		int32										calc_player_basic_award(e_time_limit_activity_type activity_type);
		void										add_basic_award_to_player(e_time_limit_activity_type activity_type, const guid_64& player_uid, const int32 award_index);
		void										save_activity_role_content(e_time_limit_activity_type activity_type, const guid_64 role_id);
		void										get_activity_info(e_time_limit_activity_type activity_type, cloud_shop_proto_cloud_shop_resp &msg, const guid_64 &player_uid);
		xstring										get_big_reward_string();
		void										clear_cloud_shop_player_info(e_time_limit_activity_type activity_type);
		bool										is_in_sell_time(e_client_req_type req_type, e_time_limit_activity_type activity_type);
		bool										get_is_liberty_buy(e_time_limit_activity_type activity_type);

		//cross
		void										recv_other_ws_buy_msg(guid_64 role_guid, int32 server_id, int32 req_type, const xstring& role_name, int32 class_type, int32 sender_server_id);
		void										send_open_msg_to_all_client();
		void										send_open_msg_to_client(client_session* session_ptr);
		void										send_open_msg_to_other_server(guid_64 role_guid, int32 server_id);

	private:
		cloud_shop_info_map							m_cloud_shop_info_map;//所有玩家购买map
	};
}

#endif
