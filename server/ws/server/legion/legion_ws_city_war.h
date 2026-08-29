#ifndef _WS_LEGION_WS_CITY_WAR_H_
#define _WS_LEGION_WS_CITY_WAR_H_

#include "Logic/legion_def.hpp"
#include "../client_session.hpp"

namespace faith
{
	class legion_proto_get_city_war_info_end;
	class legion_proto_city_war_territory_info;
	class legion_proto_city_war_bid_info;
	class legion_proto_req_city_war_rank_info_end;
	class legion_proto_get_banquet_info_end;
	class legion_proto_get_abstention_city_war_end;

	class legion_ws_city_war
	{
	public:
		explicit legion_ws_city_war();

		void						tick(const uint64& tick_time);
		void						tick_1_sec(const uint64& tick_time);
		void						tick_1_min(const uint64& tick_time);
		
		/********************************************/
		/*                王城争霸                  */
		/********************************************/
		void						save_city_info_into_db(guid_64 overlord_legion, xchar* city_master_name);
		void						load_city_info_from_db();
		const guid_64&				get_overlord_legion() { return m_overlord_legion; }
		xchar*						get_overlord_legion_name();
		void						set_overlord_legion(guid_64 legion_guid, bool is_from_db, bool is_from_cross_city = false);
		void						set_city_master_name(const xchar* city_master_name, bool is_from_db);
		guid_64						get_city_master_guid(); // 获得城主的GUID
		xchar*						get_city_master_name();
		xchar*						get_city_master_name_old();//只在城战结束时获取一次，其他时候不要调用该函数
		void						check_and_send_city_master_online_acconcement(const guid_64& online_guid);
		void						send_get_city_master_stuff(client_session* session);
		void						send_del_city_master_stuff(const guid_64& city_master_guid);
		void						ws2dp_del_overlord_legion_stuff_proc(e_del_overlord_stuff_event del_type, const guid_64& target_guid);
		void						send_one_add_city_war_winner_title(client_session* member_session);
		void						send_one_del_city_war_winner_title(const guid_64& member_guid);
		void						send_add_city_war_winner_title(const guid_64& new_legion_winner_guid);
		void						send_del_city_war_winner_title(const guid_64& old_legion_winner_guid);
		void						del_overlord_legion(bool is_legion_del = false, bool is_cross_city = false);
		void						get_maintain_time(tm& begin_tm, tm& end_tm);

		/********************************************/
		/*               普通城战相关               */
		/********************************************/
		void						send_city_war_end_notice(guid_64 legion_guid);
		guid_64						get_occupy_legion_guid(int32 terr_id);
		char*						get_occupy_legion_name(int32 terr_id);

		bool						is_city_war_hold_day(int32 terr_id = overlord_war_territory_flag + 1);
		bool						is_city_war_begin(int32 terr_id);
		bool						is_city_war_end_today(int32 terr_id);
		int32						get_legion_bid_index(int32 terr_id, guid_64 legion_guid);
		bool						exist_bid_info(int32 terr_id, guid_64 legion_guid);
		bool						exist_bid_info(guid_64 map_guid, guid_64 legion_guid);
		int32						get_bid_info_count(int32 terr_id);
		void						begin_war(int32 terr_id);
		void						end_war(int32 terr_id, guid_64 winner_legion_guid,bool need_send_notice = true);//增加bool值判断是否需要发公告
		void						end_war(guid_64 map_guid, guid_64 winner_legion_guid);
		guid_64						get_city_war_map_guid(int32 terr_id);
		bool						is_during_city_war(guid_64 legion_guid);
		void						begin_all_city_war(const uint64& tick_time);
		bool						is_during_bid_time(int32 terr_id);
		bool						is_during_city_war_prepare_time(int32 terr_id);
		bool						is_during_city_war_time(int32 terr_id);
		void						get_begin_and_end_tm(int32 terr_id, tm& begin_tm, tm& end_tm);

		bool						fill_get_city_war_info_msg(legion_proto_get_city_war_info_end& get_city_war_info_msg, int32 begin_idx = -1, int32 end_idx = -1);
		void						fill_territory_info_msg(legion_proto_city_war_territory_info* territory_info_msg, s_city_war_territory_info& terr_info_ref);
		void						fill_territory_info_msg_cross(legion_proto_city_war_territory_info* territory_info_msg, s_city_war_territory_info& terr_info_ref);
		void						fill_bid_info_msg(legion_proto_city_war_bid_info* bid_info_msg, s_city_war_bid_info& bid_info_ref);
		void						fill_bid_info_msg_cross(legion_proto_city_war_bid_info* bid_info_msg, s_city_war_bid_info& bid_info_ref);
		void						send_territory_info_one(client_session* session, int32 terr_id);
		void						fill_rank_info_msg(legion_proto_req_city_war_rank_info_end& req_rank_infos_msg);
		void						send_city_war_info_all_msg(client_session* session);
		void						send_city_war_info_to_all_attended();
		void						set_cross_server_territory_war_state(bool war_state) { m_cross_server_territory_war_state = war_state; };

		/********************************************/
		/*               王城舞会相关               */
		/********************************************/
		bool						is_banquet_holding();
		e_banquet_level				get_banquet_level() { return m_banquet_level; }
		e_legion_error				request_hold_banquet(e_banquet_level banquet_level);
		void						start_hold_banquet(e_banquet_level banquet_level);
		void						stop_hold_banquet();
		void						take_part_in_banquet(client_session* session);
		bool						is_in_banquet_time();

		void						fill_get_banquet_info_msg(legion_proto_get_banquet_info_end& get_banquet_info_msg, guid_64 role_guid);
		
		void						send_overlord_notice(std::string overlord_name, guid_64 overlord_guid, std::string city_name);		//成为城主的公告，参数为城主名和城市名
		void						check_send_city_war_begin_notice(int32 territory_id);
		void						send_overlord_war_begin_notice();
		void						send_common_war_begin_notice();
		void						send_cross_server_war_begin_notice();
		void						tick_notice(const uint64& tick_time);
		void						check_cross_server_territory_war_begin();

		void						add_legion_bouns_count_city_war();
		bool						fill_get_abstention_city_war_legion(legion_proto_get_abstention_city_war_end& get_abstention_msg);
	private:
		ActivityCommonConfigTemplate* m_city_activity_cfg_ptr;

		/********************************************/
		/*                 城战相关                 */
		/********************************************/
		guid_64						m_overlord_legion;
		xchar						m_city_master_name[max_name_size + 1];
		bool						m_is_start_city_war;		// 王城舞会当前是否正在举行中
		/********************************************/
		/*               王城舞会相关               */
		/********************************************/
		bool						m_is_banquet_holding;		// 王城舞会当前是否正在举行中
		bool						m_request_hold_banquet;		// 是否请求举办舞会
		e_banquet_level				m_banquet_level;			// 舞会的级别(普通?丰盛?豪华?)
		float						m_banquet_holding_time;		// 王城舞会从开始举办到目前经过的时间
		int32						m_participant_count;		// 参与者的数目
		int32						m_participant_max;			// 最大参与者数目
		unit_guid_map				m_participant_record;		// 参与者记录
		bool						m_need_maintain_city_war;	// 需要扣除维护费用

		bool						m_cross_server_territory_war_state;//跨服城战状态
	};
}

#endif