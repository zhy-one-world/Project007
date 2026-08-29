/********************************************************************
  created: 2019年6月11日
  file base: cross_server_city_war_ws_mgr
  file ext: hpp
  author: zhangshuo
  purpose: 跨服城战管理类
*********************************************************************/
#ifndef _CROSS_SERVER_CITY_WAR_WS_MGR_WS_H
#define _CROSS_SERVER_CITY_WAR_WS_MGR_WS_H
#include "server/client_session_mgr.hpp"
#include "Logic/legion_def.hpp"
#include "cross_server_pk_msg.hpp"


namespace faith
{
	class legion_proto_cross_city_info_one;

	class cross_server_city_war_ws_mgr
	{
	public:
		explicit	cross_server_city_war_ws_mgr();
		static cross_server_city_war_ws_mgr& get_instance()
		{
			static cross_server_city_war_ws_mgr instance;
			return instance;
		}

		void						init_manager();
		void						tick(int64 time_new);
		void						clear_data();
		void						tick_10_sec(int64 time_new);

		//gate func
		e_error_code				is_can_join(guid_64 legion_guid);
		void						create_map_with_game_begin();
		void						clear_msg_with_game_end();
		bool						is_need_refresh_rank_city();//是否需要第一次初始化军团排行
		void						refresh_rank_city();//初始化霸主军团数组
		void						send_load_db_msg();
		void						req_load_db_msg(const s_cross_city_info* city_info, int32 data_num, bool is_empty_msg);
		void						refresh_rank_with_del_legion(guid_64 legion_guid);//更新排行当有军团被删除时
		void						refresh_rank_with_server_list();//更新排行当server_list确认时
		void						refresh_chief_with_legion(guid_64 legion_guid, guid_64 chief_guid, xstring chief_name);
		void						refresh_legion_name(guid_64 legion_guid,xstring legion_name);
		int32						is_need_add_legion_with_rank_list();
		void						add_legion_with_rank_list();//补充军团到小组中
		void						end_cross_city_war(guid_64 map_guid, int32 cross_city_idex, guid_64 first_legion_guid, guid_64 second_legion_guid, guid_64 third_legion_guid, const guid_64* first_member_guid, const guid_64* second_member_guid, const guid_64* third_member_guid, int32 first_member_num, int32 second_member_num, int32 third_member_num);//一场战斗结束后执行的逻辑
		void						sort_legion_with_city_war_end();//所有战斗结束以后，进行升降级操作，因为需要发奖励，删最后一组操作要放到发奖励的后面
		bool						is_all_city_war_end();
		void						send_legion_reward_with_rank();//根据排行发奖励
		void						del_last_group_legion();//删掉最后一组军团
		void						send_city_info_all_to_other_ws(int32 server_id = 0);//同步gate信息到其他服务器
		void						set_city_war_master();
		void						save_city_info_to_db();//存库
		void						recv_other_ws_transfer_gate_map(guid_64 role_guid, int32 server_id, guid_64 legion_guid);
		void						clear_city_master();
		void						set_refresh_cross_city_war(int32 is_refresh) { m_is_refresh_cross_city_war = is_refresh; };
		void						set_legion_all_member_list(guid_64 legion_guid, const guid_64* legion_all_member, int32 member_num);
		int32						get_legion_member_list_idex(guid_64 legion_guid);

		void						send_legion_notice_with_begin(e_overload_city_war_type cur_state);
		
		void						func_clear_all_city_war_msg();

		void						fil_cross_city_info(server2dp_proto_s_cross_city_info& city_info, s_cross_city_info& info);
	private:
		guid_64						m_city_war_map_guid_arr[max_cross_server_city_group_num];//城战地图guid数组
		bool						m_city_war_map_end[max_cross_server_city_group_num];//地图是否已结束
		bool						m_is_create_map;//已经创建地图
		bool						m_is_load_end;
		bool						m_is_have_change;
		int32						m_is_refresh_cross_city_war;

		//normal func
	public:
		bool						is_in_game_time();
		bool						is_can_join_with_cur_server(guid_64 role_guid, bool is_cross);
		int32						get_cur_legion_in_rank(guid_64 legion_guid);

		guid_64						get_map_guid_with_legion_guid(guid_64 legion_guid);
		bool						get_map_is_end_with_legion_guid(guid_64 legion_guid);

		void						recv_gate_msg_send_legion_reward(guid_64 legion_guid, int32 rank_idex, guid_64* play_guid_list, int32 play_num);
		void						recv_gate_msg_city_info_all(s_cross_city_info* city_info, int32 data_num);

		void						send_city_info_all_to_client(client_session* session_ptr);
		void						send_city_info_all_to_all_client();
		void						fill_one_city_msg(const s_cross_city_info& city_info,legion_proto_cross_city_info_one& one_msg, int32 legion_rank);
		void						send_to_gate_change_legion_msg(guid_64 legion_guid, e_change_cross_city_type chang_type, xstring role_name = "", guid_64 role_guid = guid_64());
		bool						is_need_send_last_group_msg();//是否需要发最后一组的信息
		int64						get_cross_city_war_refresh_time();
		bool						is_can_open_overload_city_war_with_cur_server();

		s_cross_city_info*			get_cur_all_city_war_legion() { return m_city_war_all_legion_info; };

	private:
		s_cross_city_info			m_city_war_all_legion_info[max_cross_server_all_city_num];

		int64						m_last_tick_time;
		int64						m_timer;
		int64						m_last_min_time;

		s_city_war_all_member_info	m_all_legion_member_info[max_cross_server_all_city_num];
	};
}

#endif
