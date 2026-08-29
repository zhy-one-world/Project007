#ifndef _WS_CITY_WAR_TERRITORY_MGR_H_
#define _WS_CITY_WAR_TERRITORY_MGR_H_


#include "Logic/legion_def.hpp"

namespace faith
{
	class city_war_territory_mgr
	{
	public:
		explicit city_war_territory_mgr();
		static city_war_territory_mgr& get_instance()
		{
			static city_war_territory_mgr instance;
			return instance;
		}
		
		void						save_bid_info_into_db(int32 territory_id, int32 bid_index);
		void						req_load_bid_info_from_db();
		void						clear_bid_info_in_db();
		void						load_bid_info(const s_city_war_bid_info& bid_info);
		void						on_load_bid_info_finish();

		s_city_war_territory_info*	get_territory_info(int32 territory_id);
		territory_lists_type&		get_territory_lists() { return m_territory_lists; }
		s_city_war_territory_info*	get_territory_info_by_map(guid_64 map_guid);
		const city_war_rank_type&	get_legion_city_war_rank() { return m_legion_city_war_rank; }

		void						clear_territory_legion_bid_info(int32 territory_id, guid_64 legion_guid);
		void						set_occupy_legion_guid(int32 territory_id, guid_64 legion_guid);
		guid_64						get_occupy_legion_guid(int32 territory_id);
		int32						bid_one_territory(guid_64 legion_guid, int32 territory_id, int32 bid_index, guid_64 role_guid);
		bool						set_is_prior_maintain(guid_64 legion_guid, int32 territory_id);
		void						on_begin_war(int32 territory_id, guid_64 map_guid);
		void						on_end_war(int32 territory_id, guid_64 winner_legion_guid, int32 server_id, bool need_send_notice = true);//增加一个bool值判断需要是否发公告
		void						cacul_occupy_territory_rank();
		void						territory_notice(int32 territory_id, guid_64 winner_legion_guid);//判断是否是王城争霸并且发相应公告

		void						get_all_attended_legions(unit_guid_map& legion_guid_map);
		void						get_overload_city_war_bid_legion();//该函数用来获取霸主战的参战军团同时存库

		void						add_abstention_city_war_legion(int32 territory_id, xstring legion_name);

		void						del_all_overload_city_bid_info();//定时清理争霸战所有竞标城池信息
		bool						is_have_overload_city_bid_info();//是否拥有争霸战竞标城池
		std::unordered_map<int32, xstring>&	get_absabstention_city_war_legion() { return m_abstention_city_war_legion; };

		void						set_need_del_overload_city_bid_info(bool need_del) {m_need_del_overload_city_bid_info = need_del; };
		bool						get_need_del_overload_city_bid_info() { return m_need_del_overload_city_bid_info; };
		void						territory_war_clear();//用于跨服城战开启清除城池数据
		
		void						req_cross_server_apply_city_war_bid(guid_64 legion_guid, int32 territory_id,int32 bid_index, guid_64 role_guid);
		void						check_cross_server_apply_city_war_bid(int32 serverid, guid_64 legion_guid, int32 territory_id, int32 bid_index, guid_64 role_guid);
		void						cross_server_apply_city_war_bid_response(int32 territory_id, int32 bid_idnex, int32 result, int32 need_price, guid_64 role_guid, int32 next_bid_price, guid_64 old_legion_guid);
		void						cross_server_return_city_war_bid_response(guid_64 old_legion, int32 cur_bid_price, int32 terr_id);
		void						cross_server_apply_city_war_bid_end(int32 territory_id, int32 bid_index, guid_64 legion_guid, guid_64 role_guid, int32 next_bid_price,int32 server_id, xchar legion_name[], int32 need_price);
		void						cross_server_apply_city_war_bid_end(guid_64 role_guid, s_city_war_territory_info info, int32 need_price, guid_64 legion_guid);
		void						push_city_info_to_legion(guid_64 legion_guid, s_city_war_territory_info city_info);

		void						req_cross_server_enter_city_war_map(guid_64 role_guid,int32 terr_id, guid_64 legion_guid);
		void						recv_other_ws_transfer_gate_map(guid_64 role_guid, int32 terr_id, guid_64 legin_guid, int32 server_id);
		int32						get_war_idx(guid_64 legion_guid, int terr_id);

		void						req_cross_server_territory_prior_maintain(guid_64 legin_guid, int32 TerritoryIds[], guid_64 role_guid,int32 territoryids_num);
		void						req_cross_server_territory_prior_maintain_gate(guid_64 legion_guid, int32 TerritoryIds[], int32 server_id, guid_64 role_guid, int32 territoryids_num);
		void						req_cross_server_territory_prior_maintain_end(guid_64 role_guid, int32 TerritoryIds[], bool prior_rets[], int32 territoryids_num);

		void						req_cross_server_legion_city_war_info(guid_64 role_guid);
		void						req_cross_server_legion_city_war_info_gate(guid_64 role_guid, int32 server_id);
		void						req_cross_server_legion_city_war_info_end(guid_64 role_guid, s_city_war_territory_info war_info[]);

		void						req_get_occupation_daily_award(guid_64 role_guid, guid_64 legion_guid, int32 terri_id);
		void						req_get_occupation_daily_award_gate(guid_64 role_guid, guid_64 legion_guid, int32 terri_id, int32 server_id);
		void						req_get_occupation_daily_award_end(guid_64 role_guid, guid_64 legion_guid, int32 terri_id, int32 notice);
		void						send_cross_territory_war_begin_notice_to_other_server(int32 server_id);

		void						send_cross_territory_war_end_one_territory_to_other_server(int32 server_id, guid_64 legion_guid, int32 terri_id,bool need_send_winner_msg);
		void						send_cross_territory_war_end_Info_to_other_server(int32 server_id, guid_64 legion_guid, int32 terri_id, bool is_winner, bool isdefense_win);
		void						on_cross_server_territory_war_end_info(guid_64 legion_guid, int32 terri_id, bool is_winner,bool is_defense_win);
		int32						get_city_num_cross_server();
		void						ws2ws_refresh_award_state(int32 server_id, guid_64 legion_guid);
		void						ws2ws_refresh_award_state_end(guid_64 legion_guid);
		void						send_city_info_all_to_other_ws(int32 server_id);
		void						send_city_info_all_to_other_ws_end(guid_64 legion_guid, s_city_war_bid_info bid_infos[],int32 bid_infos_num);
		void						pre_load_bid_info();
		void						send_cross_territory_war_maintain_terrories_to_other_server(int32 server_id);
		void						send_city_info_all_to_all_ws();
		void						remove_bid_info_when_server_remove(int32 server_id);
		void						refresh_bid_info_when_server_list_end();
		void						clear_all_bid_info();
		void						clear_bid_info_for_time();

		void						change_legion_name(guid_64 legion_guid, xstring legion_name);
	private:
		territory_lists_type		m_territory_lists; //map内部本身就是按序存储的 比如红黑树 插入<key, value>键值对时 就会按照key的大小顺序进行存储
		std::unordered_map<int32, xstring>	m_abstention_city_war_legion;//没钱维护的城池列表
		s_city_war_territory_info	m_overlord_territory_info; //领主争霸战信息 处理最终战
		city_war_rank_type			m_legion_city_war_rank; //生成排行榜的时候还要生成争霸战需要的信息
		int32						m_finish_war_territory_num;
		bool						m_need_del_overload_city_bid_info;
		territory_war_bid_info	    m_bid_infos;
	};
}

#endif