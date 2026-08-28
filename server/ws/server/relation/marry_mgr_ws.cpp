/********************************************************************
file name:	marry_mgr_ws
created:	2017/12/06 13:49
author:		Allen_lei
purpose:
*********************************************************************/

#include "marry_mgr_ws.hpp"
#include <Logic/time_def.hpp>
#include <marry_msg.hpp> 
#include "../mail/event_ws_mgr.h"
#include "../client_session.hpp"
#include "../client_session_mgr.hpp" 
#include <Utility/init_unit.h>
#include "../ranking/ranking_mgr_ws.h"
#include "world_server.hpp"
#include "globle_data.h"
#include "system/scene/cs_map_system.h"
#include "server/team/team_ws_mgr.h"
#include "server/mail/mail_event_ws.h"
#include "relation.pb.h"
#include "net.pb.h"
#include "world_boss_def.hpp"

namespace hld
{ 
	marry_mgr_ws::marry_mgr_ws()
	{
		clear_data();
	}

	marry_mgr_ws::~marry_mgr_ws()
	{

	}

	void	marry_mgr_ws::clear_data()
	{ 
		m_divorce_waiting_array.clear();
		m_marry_waiting_array.clear();
		m_marry_info_map.clear();
		m_new_tick_minute = 0;
	}

	void	marry_mgr_ws::init_manager()
	{
		clear_data();
 	}

	void	marry_mgr_ws::load_data_end(const marry_record_data(&load_data)[max_load_num], int32 load_num)
	{
		if (load_num <= 0)
		{
			return;
		}

		for (int32 index = 0; index < load_num; index++)
		{
			std::pair<ui64, marry_record_data> main_pair;
			main_pair.first =  load_data[index].main_couple_guid.server_64;
			main_pair.second = load_data[index];
			m_marry_info_map.insert(main_pair);
			std::pair<ui64, marry_record_data> sub_pair;
			sub_pair.first = load_data[index].sub_couple_guid.server_64;
			sub_pair.second = load_data[index];
			m_marry_info_map.insert(sub_pair); 
		}  
	}

	void	marry_mgr_ws::save_marry_data(const marry_record_data& marry_data, bool is_divorce)
	{ 
		ws2dp_save_marry_couple_info req; 
		req.data_one = marry_data; 
		req.is_divorce = is_divorce;
		ws_client::getInstance().send_to_dp(&req, sizeof(req));
	} 

	int32	marry_mgr_ws::make_marry_data_piece(marry_record_data& new_data, const guid_64& main_guid, const guid_64& sub_guid, marry_state data_state)
	{
		client_session* main_session = client_session_mgr::getInstance().get_session(main_guid);
		if (nullptr == main_session)
		{
			return e_marry_operate_type_one_offline;
		}
		client_session* sub_session = client_session_mgr::getInstance().get_session(sub_guid);
		if (nullptr == sub_session)
		{
			return e_marry_operate_type_one_offline;
		}
		new_data.main_couple_guid = main_guid;
		xchar* main_name = main_session->get_role_name();
		if (nullptr != main_name)
		{
			new_data.set_main_name(main_name);
		}
		new_data.main_class_template_id = main_session->get_role_info_data(e_role_info_template_id);
		new_data.main_level = main_session->get_role_info_data(e_role_info_exp_level);
		new_data.sub_couple_guid = sub_guid;
		xchar* sub_name = sub_session->get_role_name();
		if (nullptr != sub_name)
		{
			new_data.set_sub_name(sub_name);
		} 
		new_data.sub_class_template_id = sub_session->get_role_info_data(e_role_info_template_id);
		new_data.sub_level = sub_session->get_role_info_data(e_role_info_exp_level);
		new_data.marry_state = data_state;
		new_data.save_event_time = 0;
		if (marry_state_with_sb == data_state)
		{
			new_data.save_event_time = time_helper::get_cur_time_new().second;
		}
		return e_marry_operate_type_success;
	}


	bool	marry_mgr_ws::insert_into_data_map(marry_record_data& couple_data)
	{
		std::pair<ui64, marry_record_data> tmp;
		tmp.first = couple_data.main_couple_guid.server_64;
		tmp.second = couple_data;
		m_marry_info_map.insert(tmp);

		std::pair<ui64, marry_record_data> temp;
		temp.first = couple_data.sub_couple_guid.server_64;
		temp.second = couple_data;
		m_marry_info_map.insert(temp);
		//存档
		save_marry_data(couple_data);
		send_mail_to_couple(couple_data.main_couple_guid, couple_data.sub_couple_guid, false);
		return true;

	} 
	bool	marry_mgr_ws::remove_from_data_map(const guid_64& main_guid, const guid_64& couple_guid)
	{
		marry_record_data*	couple_data_ptr = find_marry_record_info(main_guid);
		if (nullptr == couple_data_ptr)
		{
			return false;
		}
		save_marry_data(*couple_data_ptr,true);
		send_mail_to_couple(main_guid, couple_guid, true);

		//从夫妻排行榜中删除
		ranking_mgr_ws::delete_ranking_by_role_guid(e_RankingIndex_couple, main_guid);

		auto ite = m_marry_info_map.find(main_guid.server_64);
		if (ite != m_marry_info_map.end())
		{
			m_marry_info_map.erase(ite);
		}
		else
		{
			return false;
		}

		auto ite_sub = m_marry_info_map.find(couple_guid.server_64);
		if (ite_sub != m_marry_info_map.end())
		{
			m_marry_info_map.erase(ite_sub);
		}
		else
		{
			return false;
		}
		return true;
	}  

	void	marry_mgr_ws::add_into_waiting_array(marry_record_data& new_waiting)
	{
		del_from_waiting_array(new_waiting.main_couple_guid);
		del_from_waiting_array(new_waiting.sub_couple_guid);
		m_marry_waiting_array.push_back(new_waiting); 
	}

	void	marry_mgr_ws::del_from_waiting_array(const guid_64& role_guid)
	{ 
		for (auto ite = m_marry_waiting_array.begin(); ite != m_marry_waiting_array.end(); )
		{
			if (role_guid == ite->main_couple_guid
				|| role_guid == ite->sub_couple_guid )
			{ 
				ite = m_marry_waiting_array.erase(ite); 
			} 
			else
			{
				ite++;
			}
		} 
	}

	void	marry_mgr_ws::add_into_waiting_divorce_array(marry_record_data& divorce_waiting)
	{
		del_from_waiting_divorce_array(divorce_waiting.main_couple_guid);
		del_from_waiting_divorce_array(divorce_waiting.sub_couple_guid);
		m_divorce_waiting_array.push_back(divorce_waiting);
	}
	void	marry_mgr_ws::del_from_waiting_divorce_array(const guid_64& role_guid)
	{
		for (auto ite = m_divorce_waiting_array.begin(); ite != m_divorce_waiting_array.end(); )
		{
			if (role_guid == ite->main_couple_guid
				|| role_guid == ite->sub_couple_guid)
			{
				ite = m_divorce_waiting_array.erase(ite);
			}
			else
			{
				ite++;
			}
		}

	}


	marry_record_data*	marry_mgr_ws::find_marry_record_info(const guid_64& role_guid)
	{
		auto ite = m_marry_info_map.find(role_guid.server_64);
		if (ite != m_marry_info_map.end())
		{
			return &(ite->second);
		}
		return  nullptr;
	}

	marry_couple_data	marry_mgr_ws::get_couple_data(const guid_64& role_guid)
	{
		marry_couple_data tmp_couple_data;
		marry_record_data* temp = find_marry_record_info(role_guid);
		if (nullptr != temp)
		{
			if (temp->main_couple_guid == role_guid)
			{ 
				tmp_couple_data.role_guid = temp->sub_couple_guid;
				tmp_couple_data.set_role_name(temp->sub_couple_name) ;
				tmp_couple_data.role_class_template_id = temp->sub_class_template_id;
				tmp_couple_data.role_level = temp->sub_level; 
				tmp_couple_data.marry_time = temp->save_event_time;
				return tmp_couple_data;
			}
			else
			{
				tmp_couple_data.role_guid = temp->main_couple_guid;
				tmp_couple_data.set_role_name(temp->main_couple_name);
				tmp_couple_data.role_class_template_id = temp->main_class_template_id;
				tmp_couple_data.role_level = temp->main_level;
				tmp_couple_data.marry_time = temp->save_event_time;
				return tmp_couple_data;
			}
		} 
		return tmp_couple_data;
	}

	bool	marry_mgr_ws::is_couple(const guid_64& role_guid, const guid_64& couple_guid)
	{
		marry_record_data* marry_pair_data = find_marry_record_info(role_guid);
		if (nullptr == marry_pair_data)
		{
			return false;
		}
		if (role_guid == marry_pair_data->main_couple_guid)
		{
			if (marry_pair_data->sub_couple_guid == couple_guid)
			{
				return true;
			} 
		}
		else if(role_guid == marry_pair_data->sub_couple_guid)
		{
			if (marry_pair_data->main_couple_guid == couple_guid)
			{
				return true;
			} 
		}
		return false;
	}

	void	marry_mgr_ws::session_offline_marry_handle(const guid_64& role_guid)
	{
		marry_record_data*  waiting_divorce_ptr = get_waiting_divorce_data(role_guid);
		if (waiting_divorce_ptr != nullptr)
		{
			send_operate_result_notice(e_marry_operate_type_one_refuse_divorce, waiting_divorce_ptr->main_couple_guid, waiting_divorce_ptr->sub_couple_guid);
			del_from_waiting_divorce_array(role_guid);
		}
		marry_record_data*  waiting_marry_ptr = get_waiting_marry_data(role_guid);
		if (waiting_marry_ptr != nullptr)
		{
			send_operate_result_notice(e_marry_operate_type_one_refuse, waiting_marry_ptr->main_couple_guid, waiting_marry_ptr->sub_couple_guid);
			del_from_waiting_array(role_guid);
		}
	}

	void	marry_mgr_ws::session_online_marry_handle(const guid_64& role_guid)
	{  
		marry_couple_data	couple_data = get_couple_data(role_guid); 
		marry_couple_data	role_data = get_couple_data(couple_data.role_guid);
		//给对方发送上线公告等   
		send_couple_online_notice(couple_data.role_guid, role_data.role_name);
		//给自己推送情侣信息
		c2ws_get_couple_info(couple_data, role_guid);

		//刷新排行榜
		marry_record_data* data_ptr = find_marry_record_info(role_guid);
		if (data_ptr != nullptr)
		{
			s_ranking_player_info role_info;
			role_info.role_guid = data_ptr->main_couple_guid;
			role_info.set_role_name(data_ptr->main_couple_name);
			role_info.supporting_guid = data_ptr->sub_couple_guid;
			role_info.set_supporting_name(data_ptr->sub_couple_name);
			role_info.ranking_type = e_RankingIndex_couple;
			role_info.ranking_value = data_ptr->heart_value;
			role_info.server_id = world_server::getInstance().get_server_id();
			role_info.save_time = time_helper::get_cur_time_new().second;
			ranking_mgr_ws::sync_player_info(role_info);
		}
	}


	void	marry_mgr_ws::update_marry_data(const guid_64& role_guid)
	{ 
		client_session* role_session_ptr = client_session_mgr::getInstance().get_session(role_guid);
		if (nullptr == role_session_ptr)
		{
			return;
		}
		marry_record_data*	data_ptr = find_marry_record_info(role_guid);
		if (nullptr == data_ptr)
		{
			return;
		}
		if (role_guid == data_ptr->main_couple_guid)
		{
			data_ptr->main_level = role_session_ptr->get_role_info_data(e_role_info_exp_level);
		}
		else
		{
			data_ptr->sub_level = role_session_ptr->get_role_info_data(e_role_info_exp_level);
		} 

	}

	void	marry_mgr_ws::sync_couple_data_info(const guid_64& role_guid, const marry_record_data& marry_record_ref)
	{ 
		client_session* role_session_ptr = client_session_mgr::getInstance().get_session(role_guid);
		if (nullptr == role_session_ptr)
		{
			return;
		}
		relation_proto_s2c_ret_couple_info pack;
		pack.set_main_guid(marry_record_ref.main_couple_guid.server_64);
		pack.set_main_name(marry_record_ref.main_couple_name);
		pack.set_main_level(marry_record_ref.main_level);
		pack.set_main_class_id(marry_record_ref.main_class_template_id);
		pack.set_couple_guid(marry_record_ref.sub_couple_guid.server_64);
		pack.set_couple_name(marry_record_ref.sub_couple_name);
		pack.set_couple_level(marry_record_ref.sub_level);
		pack.set_couple_class_id(marry_record_ref.sub_class_template_id);
		pack.set_save_time(marry_record_ref.save_event_time);
		pack.set_marry_state(marry_record_ref.marry_state);
		role_session_ptr->send_to_client(&pack, e_msgindex_s2c_ret_couple_info);
	}


	void	marry_mgr_ws::sync_couple_data_info( const guid_64& role_guid)
	{ 
		marry_record_data* record_ptr = find_marry_record_info(role_guid);
		if (record_ptr == nullptr)
		{
			return;
		}
		client_session* role_session_ptr = client_session_mgr::getInstance().get_session(role_guid);
		if (nullptr == role_session_ptr)
		{
			return;
		}
		relation_proto_s2c_ret_couple_info pack; 
		pack.set_main_guid(record_ptr->main_couple_guid.server_64);
		pack.set_main_name(record_ptr->main_couple_name);
		pack.set_main_level(record_ptr->main_level);
		pack.set_main_class_id(record_ptr->main_class_template_id);
		pack.set_couple_guid(record_ptr->sub_couple_guid.server_64);
		pack.set_couple_name(record_ptr->sub_couple_name);
		pack.set_couple_level(record_ptr->sub_level);
		pack.set_couple_class_id(record_ptr->sub_class_template_id);
		pack.set_save_time(record_ptr->save_event_time);
		pack.set_marry_state(record_ptr->marry_state);
		role_session_ptr->send_to_client(&pack, e_msgindex_s2c_ret_couple_info);
	}

	marry_record_data*	marry_mgr_ws::get_waiting_marry_data(const guid_64& role_guid)
	{  
		for (auto ite = m_marry_waiting_array.begin(); ite != m_marry_waiting_array.end();++ite )
		{
			if (ite->main_couple_guid == role_guid
				|| ite->sub_couple_guid == role_guid)
			{
				return &(*ite);
			} 
		} 
		return nullptr;
	}

	marry_record_data*	marry_mgr_ws::get_waiting_divorce_data(const guid_64& role_guid)
	{
		for (auto ite = m_divorce_waiting_array.begin(); ite != m_divorce_waiting_array.end(); ++ite)
		{
			if (ite->main_couple_guid == role_guid
				|| ite->sub_couple_guid == role_guid)
			{
				return &(*ite);
			}
		}
		return nullptr;
	}

	void	marry_mgr_ws::send_operate_result_notice(int32 operate_result_type,const guid_64& main_guid, const guid_64& sub_guid, int32 support_id)
	{
		if (operate_result_type < 0)
		{
			operate_result_type = e_marry_operate_type_unknow_error;
		}

		relation_proto_s2c_marry_operate_result result;
		result.set_operate_result_type(operate_result_type);
		result.set_support_id(support_id);
		client_session* main_session_ptr = client_session_mgr::getInstance().get_session(main_guid);
		if (nullptr != main_session_ptr)
		{  
			main_session_ptr->send_to_client(&result, e_msgindex_s2c_marry_operate_result); 
		}
		client_session* sub_session_ptr = client_session_mgr::getInstance().get_session(sub_guid);
		if (nullptr != sub_session_ptr)
		{    
			sub_session_ptr->send_to_client(&result, e_msgindex_s2c_marry_operate_result); 
		}
	}
	 
	void	marry_mgr_ws::send_forced_divorce_mail(const guid_64& addresss,const  xstring&  couple_role_name)
	{ 
		s_mail_info temp_mail_info;
		std::vector<guid_64> temp_item_guid_array;
		const xchar* sender_name = globle_data::get_instance().get_mail_common_text_id(e_mail_common_text_sender_system);
		if (sender_name == nullptr)
		{
			return;
		}
		int32 sender_name_len = std::strlen(sender_name);
		if (sender_name_len > max_name_size)
		{
			sender_name_len = max_name_size;
		}
		init_unit::init_mail_data_array(temp_mail_info, temp_item_guid_array, 0, 0, 0, 0, 0);
		memcpy(temp_mail_info.sender_name, sender_name, sender_name_len); 
		xstring title = globle_data::get_instance().get_mail_common_text_id(e_mail_common_text_force_divorce_title);
		int32 mail_title_len = title.size() > max_mail_title_length ? max_mail_title_length : title.size();
		memcpy(temp_mail_info.mail_title, title.c_str(), mail_title_len); 
		
		std::vector<std::string> content_params;
		content_params.push_back(globle_data::get_instance().get_mail_common_text_id(e_mail_common_text_force_divorce_content));
		content_params.push_back(couple_role_name);
		std::string context = init_unit::implode(content_params);
		int32 mail_content_len = context.size() > max_text_contents_length ? max_text_contents_length : context.size();
		memcpy(temp_mail_info.text_contents, context.c_str(), mail_content_len); 
		mail_ws_mgr::get_instance().send_mail(addresss, 0, temp_mail_info, nullptr, 0);
	}

	void    marry_mgr_ws::ws2cs_get_reward(const guid_64& role_guid, int32 get_end_state)
	{
		if (get_end_state < 0 || get_end_state >= marry_state_max)
		{
			return;
		}
		client_session* role_session_ptr = client_session_mgr::getInstance().get_session(role_guid);
		if (nullptr == role_session_ptr )
		{ 
			return;
		}
		ws2cs_marry_get_reward pak;
		pak.sender_guid = role_guid;
		pak.old_marry_state = get_end_state;
		bool is_use = proto_by_lua(e_msgindex_ws2cs_marry_get_reward);
		if (is_use == false)
		{
			role_session_ptr->send_to_cs(&pak, sizeof(pak));
		}
		else
		{
			hld::ws2cs_proto::marry_get_reward pro_msg;
			pak.to_proto(pro_msg);
			role_session_ptr->send_to_cs_lua(&pro_msg, e_msgindex_ws2cs_marry_get_reward);
		}
	}

	void     marry_mgr_ws::cs2ws_get_reward_end(const guid_64& role_guid, const int32& old_marry_state, const int32& get_result)
	{
		marry_record_data* data_ptr = find_marry_record_info(role_guid);
		if (data_ptr == nullptr)
		{
			return;
		}

		if ( e_marry_operate_type_success != get_result)
		{
			data_ptr->marry_state = old_marry_state;
			send_operate_result_notice(get_result, role_guid, guid_64());
			return;
		}
		bool is_get = false;
		switch (old_marry_state)
		{
		case marry_state_with_sb:
		{
			is_get = true;
			if (data_ptr->marry_state == marry_state_main_is_getting_reward)
			{
				data_ptr->marry_state = marry_state_main_had_get_reward;
			}
			else if (marry_state_sub_is_getting_reward == data_ptr->marry_state)
			{
				data_ptr->marry_state = marry_state_sub_had_get_reward;
			}
		}
			break;
		case marry_state_main_had_get_reward:
		{
			is_get = true;
			if (data_ptr->marry_state == marry_state_sub_is_getting_reward)
			{
				data_ptr->marry_state = marry_state_both_had_get_reward;
			}
		}
		break;
		case marry_state_sub_had_get_reward:
		{
			is_get = true;
			if (data_ptr->marry_state == marry_state_main_is_getting_reward)
			{
				data_ptr->marry_state = marry_state_both_had_get_reward;
			}
		}
		break;
		default:
			break;
		}
		if (is_get)
		{
			save_marry_data(*data_ptr);
			send_operate_result_notice(e_marry_operate_type_get_reward_success, role_guid, guid_64());
			sync_couple_data_info(role_guid, *data_ptr );
		}

		return;
	}
	void    marry_mgr_ws::c2ws_get_marry_mission(const guid_64& role_guid,int32 is_near)
	{
		client_session* main_session_ptr = client_session_mgr::getInstance().get_session(role_guid);
		if (nullptr == main_session_ptr)
		{
			return;
		}
		//判断组队
		team_ws* team_ref = team_ws_mgr::get_instance().get_team(main_session_ptr->get_team_guid());
		if (nullptr == team_ref)
		{
			main_session_ptr->send_notice("90096775");
			return;
		}
		if (team_ref->get_captain_guid() != role_guid)
		{
			main_session_ptr->send_notice("90096775");
			return;
		}
		if (team_ref->get_member_num() != 2)
		{
			main_session_ptr->send_notice("90096775");
			return;
		}
		marry_couple_data couple_data = marry_mgr_ws::get_instance().get_couple_data(role_guid);
		client_session* couple_session_ptr = client_session_mgr::getInstance().get_session(couple_data.role_guid);
		if (nullptr == couple_session_ptr)
		{
			main_session_ptr->send_notice("90096777");
			//对方不在线
			return;
		}

		auto map_template = cs_map_system::get_map_template(couple_session_ptr->get_map_guid());
		if (nullptr == map_template)
		{
			main_session_ptr->send_notice("90096777");
			return;
		}
		if (map_template->Type > e_map_type_main && map_template->Type != e_map_type_big_map)
		{
			main_session_ptr->send_notice("90096777");
			return;
		}
		if (couple_session_ptr->m_online_state != client_session::e_os_online || couple_session_ptr->m_status != client_session::e_ss_ingame)
		{
			main_session_ptr->send_notice("90096777");
			return;
		}
		if (team_ref->get_member(couple_data.role_guid) == nullptr)
		{
			main_session_ptr->send_notice("90096775");
			return;
		}
		if (is_near == 0)
		{
			main_session_ptr->send_notice("90096776");
			couple_session_ptr->send_notice("90096776");
			return;
		}
		main_session_ptr->send_notice("90096912");
		relation_proto_s2c_confirm_receive_marry_mission req;
		couple_session_ptr->send_to_client(&req, e_msgindex_s2c_confirm_receive_marry_mission);
	}
	void    marry_mgr_ws::c2ws_get_married_reward(const guid_64& role_guid)
	{
		if (role_guid.is_valid() == false)
		{
			return;
		}
		marry_record_data* data_ptr = find_marry_record_info(role_guid);
		if (data_ptr == nullptr)
		{
			send_operate_result_notice(e_marry_operate_type_divorce_isnt_marry, role_guid, guid_64());
			return;
		}
		bool is_main = false;
		if (role_guid == data_ptr->main_couple_guid)
		{
			is_main = true;
		} 

		if (data_ptr->marry_state == marry_state_main_is_getting_reward)
		{
			if (is_main)
			{
				return;
			}
		}else if (data_ptr->marry_state == marry_state_sub_is_getting_reward)
		{
			if (is_main)
			{
				return;
			}
		}
		 
		switch (data_ptr->marry_state)
		{
		case marry_state_both_had_get_reward:
		{
			send_operate_result_notice(e_marry_operate_type_had_getted_reward, role_guid, guid_64());
			return;
		}
			break;
		case marry_state_main_had_get_reward:
		{
			if (is_main)
			{
				send_operate_result_notice(e_marry_operate_type_had_getted_reward, role_guid, guid_64());
				return;
			}
			data_ptr->marry_state = marry_state_main_is_getting_reward;
			ws2cs_get_reward(role_guid, marry_state_main_had_get_reward);
		}
			break;
		case marry_state_sub_had_get_reward:
		{
			if (!is_main)
			{
				send_operate_result_notice(e_marry_operate_type_had_getted_reward, role_guid, guid_64());
				return;
			}
			data_ptr->marry_state = marry_state_sub_is_getting_reward;
			ws2cs_get_reward(role_guid, marry_state_sub_had_get_reward);
		}
		break;
		case  marry_state_with_sb:		//还没有一个人领过奖励
		{ 
			if (is_main)
			{
				data_ptr->marry_state = marry_state_main_is_getting_reward;
				ws2cs_get_reward(role_guid, marry_state_with_sb);
			}
			else
			{
				data_ptr->marry_state = marry_state_sub_is_getting_reward;
				ws2cs_get_reward(role_guid, marry_state_with_sb);
			}
		}
			break;
		default:
			break;
		}
  
	}



	int32	marry_mgr_ws::check_is_can_marry(const guid_64& main_guid,const guid_64& sub_guid )
	{
		if ( main_guid.is_valid() == false || sub_guid.is_valid() == false)
		{ 
			return e_marry_operate_type_unknow_error;
		} 
		client_session* main_session_ptr = client_session_mgr::getInstance().get_session(main_guid);
		if (nullptr == main_session_ptr)
		{
			return e_marry_operate_type_one_offline;
		}
		client_session* sub_session_ptr = client_session_mgr::getInstance().get_session(sub_guid);
		if (nullptr == sub_session_ptr)
		{
			return e_marry_operate_type_one_offline;
		} 
		//判断队伍
		team_ws* team_ref = team_ws_mgr::get_instance().get_team(main_session_ptr->get_team_guid());
		if (nullptr == team_ref)
		{
			return e_marry_operate_type_team_error;
		}
		if (team_ref->get_captain_guid() != main_guid)
		{
			return e_marry_operate_type_team_error;
		}
		if (team_ref->get_member_num() != 2)
		{
			return e_marry_operate_type_team_error;
		}

		if (team_ref->get_member(sub_guid) == nullptr)
		{
			return e_marry_operate_type_team_error;
		}
		//检查是否结婚过
		marry_record_data* main_marry_record_ptr = find_marry_record_info(main_guid);
		marry_record_data* sub_marry_record_ptr = find_marry_record_info(sub_guid);
		if (main_marry_record_ptr || sub_marry_record_ptr)
		{
			return e_marry_operate_type_one_is_cant_marry_state;
		}
		relation* sub_friend = main_session_ptr->get_relation_list_mgr().get_relation_list(e_relationlist_type_friend)->get_relation(sub_guid);
		relation* main_friend = sub_session_ptr->get_relation_list_mgr().get_relation_list(e_relationlist_type_friend)->get_relation(main_guid);
		if (nullptr == sub_friend || nullptr == main_friend)
		{
			return e_marry_operate_type_not_friend;
		}
		if (sub_friend->get_data_value(e_relation_data_friendliness) < GAMECONFIG->CanMarryValue)
		{
			return e_marry_operate_type_low_friendliness;
		}
		if (main_friend->get_data_value(e_relation_data_friendliness) < GAMECONFIG->CanMarryValue)
		{
			return e_marry_operate_type_low_friendliness;
		}

		return e_marry_operate_type_success;
	}
	 

	void	marry_mgr_ws::req_check_marry_with_sb(const guid_64& main_guid, const guid_64& sub_guid,int32 marry_item)
	{  
		int32 result_type = check_is_can_marry(main_guid, sub_guid);
		if (e_marry_operate_type_success != result_type)
		{
			send_operate_result_notice(result_type, main_guid, sub_guid);
			return;
		} 		

		client_session* main_session_ptr = client_session_mgr::getInstance().get_session(main_guid);
		if (nullptr == main_session_ptr)
		{
			send_operate_result_notice(e_marry_operate_type_one_offline, main_guid, guid_64());
			return;
		}
		//发送cs 检查是否可以结婚， 道具 和功能 是否解锁
		hld::ws2cs_check_item_enough pack;
		pack.item_id = marry_item; 
		pack.main_guid = main_guid;
		pack.sub_guid = sub_guid;
		pack.is_cost_item = false;
		pack.is_divorce = false;
		bool is_use = proto_by_lua(e_msgindex_ws2cs_check_item_enough);
		if (is_use == false)
		{
			main_session_ptr->send_to_cs(&pack, sizeof(pack));
		}
		else
		{
			hld::ws2cs_proto::check_item_enough pro_msg;
			pack.to_proto(pro_msg);
			main_session_ptr->send_to_cs_lua(&pro_msg, e_msgindex_ws2cs_check_item_enough);
		}
	}

	void	marry_mgr_ws::cs2ws_check_marry_end(const guid_64& main_guid, const guid_64& sub_guid, int32 result_type, int32 item_id)
	{ 
		int32 result = result_type;
		if (e_marry_operate_type_success == result)
		{
			marry_record_data newly_married;
			result = make_marry_data_piece(newly_married, main_guid, sub_guid, marry_state_waiting_choice);
			if (e_marry_operate_type_success == result)
			{
				newly_married.save_event_time = item_id;
				add_into_waiting_array(newly_married);
				result = e_marry_operate_type_check_marry_success;
				send_operate_result_notice(result, main_guid, sub_guid, item_id);
				return;
			} 
		} 
		send_operate_result_notice(result, main_guid, sub_guid);
	}

	bool	marry_mgr_ws::check_is_in_marry_ready(const guid_64& main_guid, const guid_64& sub_guid)
	{
		if (main_guid.is_valid() == false
			|| sub_guid.is_valid() == false)
		{
			return false;
		}
		 
		int32 waiting_num = m_marry_waiting_array.size();
		for (int32 count = 0; count < waiting_num; count ++)
		{
			if (main_guid == m_marry_waiting_array[count].main_couple_guid )
			{
				if (sub_guid == m_marry_waiting_array[count].sub_couple_guid)
				{
					if (marry_state_both_agree == m_marry_waiting_array[count].marry_state)
					{
						return true;
					}
				}
			} 
		}
		return false; 
	}
	 

	void	marry_mgr_ws::agree_marry_with_sb(const guid_64& main_guid, const guid_64& sub_guid, int32 marry_item)
	{ 			
		//再检查一遍
		if (!check_is_in_marry_ready(main_guid, sub_guid))
		{
			send_operate_result_notice(e_marry_operate_type_unknow_error, main_guid, sub_guid);
			return;
		}  

		int32 result_type = check_is_can_marry(main_guid, sub_guid);
		if (e_marry_operate_type_success != result_type)
		{
			send_operate_result_notice(result_type, main_guid, sub_guid);
			return;
		} 
		client_session* main_session_ptr = client_session_mgr::getInstance().get_session(main_guid);
		if (nullptr == main_session_ptr)
		{
			send_operate_result_notice(e_marry_operate_type_one_offline, main_guid, sub_guid);
			return;
		} 
		ws2cs_check_item_enough pack;
		pack.main_guid = main_guid;
		pack.sub_guid = sub_guid;
		pack.item_id = marry_item;
		pack.is_cost_item = true;
		pack.is_divorce = false;
		bool is_use = proto_by_lua(e_msgindex_ws2cs_check_item_enough);
		if (is_use == false)
		{
			main_session_ptr->send_to_cs(&pack, sizeof(pack));
		}
		else
		{
			hld::ws2cs_proto::check_item_enough pro_msg;
			pack.to_proto(pro_msg);
			main_session_ptr->send_to_cs_lua(&pro_msg, e_msgindex_ws2cs_check_item_enough);
		}
	}
	

	void	marry_mgr_ws::cs2ws_cost_marry_item_end(const guid_64& main_guid, const guid_64& sub_guid, int32 result_type)
	{
		//扣除道具成功，结婚成功 
		int32 result = result_type;
		if (e_marry_operate_type_success == result_type)
		{
			marry_record_data newly_married;
			result = make_marry_data_piece(newly_married, main_guid, sub_guid, marry_state_with_sb);
			if (e_marry_operate_type_success == result)
			{
				marry_record_data* waiting_data_ptr= get_waiting_marry_data(main_guid);
				if (waiting_data_ptr == nullptr)
				{
					return;
				}
				int32 marry_item_id = waiting_data_ptr->save_event_time;
				send_marry_notice_to_all(marry_item_id, newly_married.main_couple_name, newly_married.sub_couple_name);
				del_from_waiting_array(main_guid);
				del_from_waiting_array(sub_guid);
				newly_married.marry_item_id = marry_item_id;
				insert_into_data_map(newly_married);
				result = e_marry_operate_type_marry_success;
				//同步couple 数据 
				sync_couple_data_info(main_guid, newly_married);
				sync_couple_data_info(sub_guid, newly_married);
				//告诉cs结婚成功了 
				send_marry_notice_to_cs(newly_married, true, marry_item_id);
			} 
		}
		send_operate_result_notice(result, main_guid, sub_guid); 
	}

	void	marry_mgr_ws::c2ws_reply_marry_choice(const guid_64& role_guid,   bool is_agree)
	{ 
		if (role_guid.is_valid() == false )
		{
			send_operate_result_notice(e_marry_operate_type_unknow_error, role_guid, guid_64());
			return;
		}

		marry_record_data* waiting_ptr = get_waiting_marry_data(role_guid);
		if (nullptr == waiting_ptr)
		{
			send_operate_result_notice(e_marry_operate_type_one_refuse, role_guid, guid_64());
			return;
		}
		if (!is_agree)
		{
			guid_64 address_guid;
			if (waiting_ptr->main_couple_guid == role_guid)
			{
				address_guid = waiting_ptr->sub_couple_guid;
			}
			else
			{
				address_guid = waiting_ptr->main_couple_guid;
			}

			send_operate_result_notice(e_marry_operate_type_one_refuse, address_guid, guid_64());
			del_from_waiting_array(role_guid); 
			return;
		}
		agree_marry_proc(role_guid, waiting_ptr);
	} 

	void	marry_mgr_ws::agree_marry_proc(const guid_64& role_guid, marry_record_data* waiting_data_ptr)
	{
		if (nullptr == waiting_data_ptr)
		{
			return;
		}

		bool is_main = false;
		if (role_guid == waiting_data_ptr->main_couple_guid)
		{
			is_main = true;
		}
		else
		{
			is_main = false;
		}
		switch (waiting_data_ptr->marry_state)
		{
		case marry_state_waiting_choice:
		{
			guid_64 receive_guid;
			if (role_guid == waiting_data_ptr->main_couple_guid)
			{
				receive_guid = waiting_data_ptr->sub_couple_guid;
			}
			else
			{
				receive_guid = waiting_data_ptr->main_couple_guid;
			}
			if (is_main)
			{
				waiting_data_ptr->marry_state = marry_state_one_main_agree;
			}
			else
			{
				waiting_data_ptr->marry_state = marry_state_one_sub_agree;
			}

			send_operate_result_notice(e_marry_operate_type_one_agree, receive_guid, guid_64());
		}
		break;
		case marry_state_one_main_agree:
		{
			if (is_main)
			{
				return;
			}
			waiting_data_ptr->marry_state = marry_state_both_agree;
			agree_marry_with_sb(waiting_data_ptr->main_couple_guid, waiting_data_ptr->sub_couple_guid, waiting_data_ptr->save_event_time);		//此时save_event_time传入的是信物id

		}
		break;
		case marry_state_one_sub_agree:
		{
			if (!is_main)
			{
				return;
			}
			waiting_data_ptr->marry_state = marry_state_both_agree;
			agree_marry_with_sb(waiting_data_ptr->main_couple_guid, waiting_data_ptr->sub_couple_guid, waiting_data_ptr->save_event_time);		//此时save_event_time传入的是信物id
		}
		break;
		default:
			break;
		}
	}


	void	marry_mgr_ws::c2ws_get_couple_info(const marry_couple_data& couple_info_ref, const guid_64& role_guid)
	{
		if (role_guid.is_valid() == false)
		{ 
			return;
		}
		if (couple_info_ref.role_guid.is_valid() == false)
		{
			return;
		} 
		sync_couple_data_info( role_guid);
	}
	 
	void	marry_mgr_ws::c2ws_req_check_divorce(const guid_64& main_guid, const guid_64& sub_guid, bool is_force)
	{
		marry_record_data* data_ptr = find_marry_record_info(main_guid);
		if (nullptr == data_ptr)
		{
			send_operate_result_notice(e_marry_operate_type_divorce_isnt_marry, main_guid, sub_guid);
			return;
		}
		int32 result = check_is_can_divorce(main_guid, sub_guid, is_force);
		//协议离婚或者不成功 检查结束了
		if (e_marry_operate_type_success  != result)
		{
			if (e_marry_operate_type_check_divorce_success == result)
			{
				marry_record_data data = *data_ptr;
				data.marry_state = marry_state_waiting_choice;
				add_into_waiting_divorce_array(data);
			}
			send_operate_result_notice(result, main_guid, sub_guid);
			return;
		} 
		guid_64 target_guid;
		if (data_ptr->main_couple_guid == main_guid)
		{
			target_guid = data_ptr->sub_couple_guid;
		}
		else if (data_ptr->sub_couple_guid == main_guid)
		{
			target_guid = data_ptr->main_couple_guid;
		}
		//检测对方的离线时间，从而判断是否需要花钱	
		ws2dp_marry_get_last_login_time req;
		req.sender_guid = main_guid;
		req.target_guid = target_guid;
		ws_client::getInstance().send_to_dp(&req,sizeof(req));
	}


	int32	marry_mgr_ws::check_is_can_divorce(const guid_64& main_guid, const guid_64& sub_guid, bool is_force)
	{ 
		if ( main_guid.is_valid() == false || sub_guid.is_valid() == false)
		{ 
			return e_marry_operate_type_unknow_error;
		} 
		client_session* main_session_ptr = client_session_mgr::getInstance().get_session(main_guid);
		if (nullptr == main_session_ptr)
		{
			return e_marry_operate_type_one_refuse_divorce;
		} 
		//检查是否结婚过
		marry_record_data* data_ptr = find_marry_record_info(main_guid);
		if (nullptr == data_ptr)
		{
			return e_marry_operate_type_divorce_isnt_marry;
		}
		//判断队伍 
		if (!is_force)
		{
			client_session* sub_session_ptr = client_session_mgr::getInstance().get_session(sub_guid);
			if (nullptr == sub_session_ptr)
			{
				return e_marry_operate_type_one_refuse_divorce;
			}
			team_ws* team_ref = team_ws_mgr::get_instance().get_team(main_session_ptr->get_team_guid());
			if (nullptr == team_ref)
			{
				return e_marry_operate_type_divorce_team_error;
			}
			if (team_ref->get_captain_guid() != main_guid)
			{
				return e_marry_operate_type_divorce_team_error;
			}
			if (team_ref->get_member_num() != 2)
			{
				return e_marry_operate_type_divorce_team_error;
			}
			if (team_ref->get_member(sub_guid) == nullptr)
			{
				return e_marry_operate_type_divorce_team_error;
			} 
		}
		else
		{
			return e_marry_operate_type_success;
		}
		 
		return e_marry_operate_type_check_divorce_success;
	}

	void	marry_mgr_ws::cs2ws_check_divorce_end(const guid_64& main_guid,  int32 result_type)
	{
		if (result_type != e_marry_operate_type_success)
		{
			send_operate_result_notice(result_type, main_guid, guid_64());
			return;
		}
		marry_record_data*	data_ptr = find_marry_record_info(main_guid);
		if (nullptr == data_ptr)
		{
			send_operate_result_notice(e_marry_operate_type_divorce_isnt_marry, main_guid, guid_64());
			return;
		}  
		add_into_waiting_divorce_array(*data_ptr);
		send_operate_result_notice(e_marry_operate_type_check_force_divorce_success, main_guid, guid_64());
		return; 
	}

	void	marry_mgr_ws::cs2ws_force_divorce_end(const guid_64& main_guid, int32 result_type)
	{
		if (result_type != e_marry_operate_type_success)
		{
			send_operate_result_notice(result_type, main_guid, guid_64());
			return;
		}
		marry_record_data*	data_ptr = find_marry_record_info(main_guid);
		if (nullptr == data_ptr)
		{
			send_operate_result_notice(e_marry_operate_type_one_refuse_divorce, main_guid, guid_64());
			return;
		}


		guid_64 addreess_guid;
		xstring role_name = "";
		if (main_guid == data_ptr->main_couple_guid)
		{
			addreess_guid = data_ptr->sub_couple_guid;
			role_name = data_ptr->main_couple_name;
		}
		else
		{
			addreess_guid = data_ptr->main_couple_guid;
			role_name = data_ptr->sub_couple_name;
		} 
		send_forced_divorce_mail(addreess_guid, role_name);
		send_operate_result_notice(e_marry_operate_type_divorce_success, data_ptr->main_couple_guid, data_ptr->sub_couple_guid);
		send_marry_notice_to_cs(*data_ptr, false); 
		del_from_waiting_divorce_array(main_guid); 
		remove_from_data_map(data_ptr->main_couple_guid, data_ptr->sub_couple_guid);
		return;
	}

	void	marry_mgr_ws::c2ws_reply_divorce_choice(const guid_64& role_guid, bool is_agree, bool is_force)
	{
		marry_record_data*	data_ptr = get_waiting_divorce_data(role_guid);
		if (nullptr == data_ptr)
		{
			send_operate_result_notice(e_marry_operate_type_one_refuse_divorce, role_guid, guid_64());
			return;
		}
		guid_64 addreess_guid;
		xstring role_name = "";
		if (role_guid == data_ptr->main_couple_guid)
		{
			addreess_guid = data_ptr->sub_couple_guid;
			role_name = data_ptr->main_couple_name;
		}
		else
		{
			addreess_guid = data_ptr->main_couple_guid;
			role_name = data_ptr->sub_couple_name;
		}
		//拒绝
		if (!is_agree)
		{ 
			del_from_waiting_divorce_array(role_guid);
			if (is_force)
			{
				return;
			}
			send_operate_result_notice(e_marry_operate_type_one_refuse_divorce, addreess_guid, guid_64());
			return;
		}
		//处理协议离婚 同意流程
		if (is_force)
		{  
			if (data_ptr->marry_state == marry_state_divorce_no_need_cost_money)
			{
				send_operate_result_notice(e_marry_operate_type_divorce_success, data_ptr->main_couple_guid, data_ptr->sub_couple_guid);
				send_marry_notice_to_cs(*data_ptr, false);
				remove_from_data_map(data_ptr->main_couple_guid, data_ptr->sub_couple_guid);
				del_from_waiting_divorce_array(role_guid);
				send_forced_divorce_mail(addreess_guid, role_name);
				return;
			}
			else
			{
				//扣钱 
				hld::ws2cs_check_item_enough pack;
				pack.item_id = 1;		//从配表里获取
				pack.main_guid = role_guid; 
				pack.is_cost_item = true;
				pack.is_divorce = true;
				client_session* main_session_ptr = client_session_mgr::getInstance().get_session(role_guid);
				if (nullptr == main_session_ptr)
				{
					send_operate_result_notice(e_marry_operate_type_one_offline, role_guid, guid_64());
					return;
				}
				bool is_use = proto_by_lua(e_msgindex_ws2cs_check_item_enough);
				if (is_use == false)
				{
					main_session_ptr->send_to_cs(&pack, sizeof(pack));
				}
				else
				{
					hld::ws2cs_proto::check_item_enough pro_msg;
					pack.to_proto(pro_msg);
					main_session_ptr->send_to_cs_lua(&pro_msg, e_msgindex_ws2cs_check_item_enough);
				}
			}
		} 
		//处理协议离婚双方同意流程
		agree_divorce_proc(role_guid, data_ptr);
	}
	 
	void	marry_mgr_ws::agree_divorce_proc(const guid_64& role_guid, marry_record_data* waithing_data_ptr)
	{
		if (nullptr == waithing_data_ptr)
		{
			return;
		}
		bool is_main = false;
		if (waithing_data_ptr->main_couple_guid == role_guid)
		{
			is_main = true;
		}
		else
		{
			is_main = false;
		}
		switch (waithing_data_ptr->marry_state)
		{
		case marry_state_waiting_choice:
		{
			if (is_main)
			{
				waithing_data_ptr->marry_state = marry_state_one_main_agree;
			}
			else
			{
				waithing_data_ptr->marry_state = marry_state_one_sub_agree;
			}
			guid_64 address_guid;
			if (waithing_data_ptr->main_couple_guid == role_guid)
			{
				address_guid = waithing_data_ptr->sub_couple_guid;
			}
			else
			{
				address_guid = waithing_data_ptr->main_couple_guid;
			} 
			send_operate_result_notice(e_marry_operate_type_one_agree_divorce, address_guid, guid_64()); 
		}
		break;
		case marry_state_one_main_agree:
		{
			if (is_main)
			{
				return;
			}
			waithing_data_ptr->marry_state = marry_state_both_agree;
			send_operate_result_notice(e_marry_operate_type_divorce_success, waithing_data_ptr->main_couple_guid, waithing_data_ptr->sub_couple_guid);
			send_marry_notice_to_cs(*waithing_data_ptr, false);
			remove_from_data_map(waithing_data_ptr->main_couple_guid, waithing_data_ptr->sub_couple_guid); 
			del_from_waiting_divorce_array(waithing_data_ptr->main_couple_guid);
		}
		break;
		case marry_state_one_sub_agree:
		{
			if (!is_main)
			{
				return;
			}
			waithing_data_ptr->marry_state = marry_state_both_agree;
			send_operate_result_notice(e_marry_operate_type_divorce_success, waithing_data_ptr->main_couple_guid, waithing_data_ptr->sub_couple_guid);
			send_marry_notice_to_cs( *waithing_data_ptr, false);
			remove_from_data_map(waithing_data_ptr->main_couple_guid, waithing_data_ptr->sub_couple_guid);
			del_from_waiting_divorce_array(waithing_data_ptr->main_couple_guid);		}
		break;

		default:
			break;
		} 
	}

	void	marry_mgr_ws::dp2ws_ret_last_login_time_end(const guid_64& role_guid, const int64& last_login_time)
	{
		marry_record_data* data_ptr = find_marry_record_info(role_guid);
		if (nullptr == data_ptr)
		{
			send_operate_result_notice(e_marry_operate_type_divorce_isnt_marry, role_guid, guid_64());
			return;
		} 
		int64 now_time = time_helper::get_cur_time_new().second;
		int32 inter_day = (now_time - last_login_time) / hld::day_time_second;
		if (inter_day >=  7)
		{
			marry_record_data divorce_data = *data_ptr;
			divorce_data.marry_state = marry_state_divorce_no_need_cost_money;
			add_into_waiting_divorce_array(divorce_data);
			send_operate_result_notice(e_marry_operate_type_offline_over_seven, data_ptr->main_couple_guid, data_ptr->sub_couple_guid);
			return;
		}
		else
		{
			client_session* main_session_ptr = client_session_mgr::getInstance().get_session(role_guid);
			if (nullptr == main_session_ptr)
			{
				send_operate_result_notice(e_marry_operate_type_one_refuse_divorce, data_ptr->main_couple_guid, data_ptr->sub_couple_guid);
				return;
			}

			//需要花钱 发送给cs 检测金币够不够
			hld::ws2cs_check_item_enough pack;
			pack.item_id = 1;		//随便给一个 cs 以gameconfig 为准
			pack.main_guid = role_guid; 
			if (role_guid == data_ptr->main_couple_guid)
			{
				pack.sub_guid = data_ptr->sub_couple_guid;
			}
			else
			{
				pack.sub_guid = data_ptr->main_couple_guid;
			}
			pack.is_cost_item = false;
			pack.is_divorce = true;
			bool is_use = proto_by_lua(e_msgindex_ws2cs_check_item_enough);
			if (is_use == false)
			{
				main_session_ptr->send_to_cs(&pack, sizeof(pack));
			}
			else
			{
				hld::ws2cs_proto::check_item_enough pro_msg;
				pack.to_proto(pro_msg);
				main_session_ptr->send_to_cs_lua(&pro_msg, e_msgindex_ws2cs_check_item_enough);
			}
		} 
	}

	void	marry_mgr_ws::send_couple_online_notice(const guid_64& couple_guid, const xstring& couple_name)
	{
		if (couple_guid.is_valid() == false)
		{
			return;
		}
		int32 notice_id = 93000191; 
		int32 create_time = time_helper::get_cur_time_new().second;
		std::vector<std::string> vec_notice_str;
		vec_notice_str.push_back(template_manager::get_instance().get_str_id_by_notice_id(notice_id));
		vec_notice_str.push_back(couple_name);
		std::string notice_str = init_unit::implode(vec_notice_str);
		event_ws_mgr::get_instance().send_notice_to_all(notice_id, create_time, couple_guid, notice_str);
	}

	void	marry_mgr_ws::send_marry_notice_to_cs(const  marry_record_data& record_info, bool is_marry, int32 marry_item_id)
	{
		client_session* main_session_ptr = client_session_mgr::getInstance().get_session(record_info.main_couple_guid);
		if (nullptr == main_session_ptr)
		{
			send_operate_result_notice(e_marry_operate_type_one_offline, record_info.main_couple_guid, record_info.sub_couple_guid);
			return;
		}
		ws2cs_marry_result_end pack;
		pack.result_type = e_marry_operate_type_marry_success;
		pack.main_guid = record_info.main_couple_guid;
		memcpy(pack.main_name, record_info.main_couple_name,max_name_size);
		pack.sub_guid = record_info.sub_couple_guid;
		memcpy(pack.sub_name, record_info.sub_couple_name, max_name_size);
		pack.is_marry = is_marry;
		pack.marry_item_id = marry_item_id;
		bool is_use = proto_by_lua(e_msgindex_ws2cs_marry_result);
		if (is_use == false)
		{
			main_session_ptr->send_to_cs(&pack, sizeof(pack));
		}
		else
		{
			hld::ws2cs_proto::marry_result_end pro_msg;
			pack.to_proto(pro_msg);
			main_session_ptr->send_to_cs_lua(&pro_msg, e_msgindex_ws2cs_marry_result);
		}
	}

	void	marry_mgr_ws::send_marry_notice_to_all(int32 marry_item_id, const xstring& main_name, const xstring& sub_name )
	{
		if (marry_item_id <= 0)
		{
			return;
		}
		ItemTemplate* item_ptr = GET_TEMPLATE(ItemTemplate, marry_item_id);
		if (nullptr == item_ptr )
		{
			return;
		}
		NpcSpawnPointTemplate* npc_spawn_point_template_ptr = GET_TEMPLATE(NpcSpawnPointTemplate, 82012619);
		if (npc_spawn_point_template_ptr == nullptr)
		{
			return;
		}
		//发送全服公告

		std::vector<int32> param_arr;
		param_arr.reserve(5);
		param_arr.push_back(hld::world_boss_line);
		param_arr.push_back(npc_spawn_point_template_ptr->MapId);
		param_arr.push_back(npc_spawn_point_template_ptr->PosX);
		param_arr.push_back(npc_spawn_point_template_ptr->PosY);
		param_arr.push_back(npc_spawn_point_template_ptr->PosZ);

		int32 notice_id = 93000190;
		std::vector<std::string> vec_notice_str;
		vec_notice_str.push_back(template_manager::get_instance().get_str_id_by_notice_id(notice_id));
		vec_notice_str.push_back(main_name);
		vec_notice_str.push_back(sub_name);
		std::string notice_str = init_unit::implode(vec_notice_str);
		int32 create_time = time_helper::get_cur_time_new().second;
		//event_ws_mgr::get_instance().send_notice_to_all(notice_id, create_time, guid_64(), notice_str);
		event_ws_mgr::get_instance().send_notice_with_param(guid_64(), "", notice_str.c_str(), param_arr, e_notice_param_go_to_location, e_chat_type_world);

		switch (item_ptr->item_color)
		{
		case e_item_color_blue:
			break;
		case e_item_color_purple:
		case e_item_color_orange:
		{
			//全服特效
			relation_proto_s2c_send_marry_effect_notice net_pro;
			net_pro.set_effect_level(item_ptr->item_color); 
			client_session_mgr::getInstance().send_message_to_all_client(&net_pro, e_msgindex_s2c_send_marry_effect_notice); 
		} 
			break; 
		default:
			break;
		}


	}

	void marry_mgr_ws::change_player_name_func(const guid_64& role_guid, const xstring& role_name)
	{
		auto iter_marry_info = m_marry_info_map.begin();
		for (; iter_marry_info != m_marry_info_map.end(); ++iter_marry_info)
		{
			bool is_change = false;
			if (iter_marry_info->second.main_couple_guid == role_guid)
			{
				xchar char_main_role_name[max_name_size + 1] = { 0 };
				memcpy(char_main_role_name, role_name.c_str(), role_name.size() > max_name_size ? max_name_size : role_name.size());
				iter_marry_info->second.set_main_name(char_main_role_name);
				is_change = true;
			}

			if (iter_marry_info->second.sub_couple_guid == role_guid)
			{
				xchar char_sub_role_name[max_name_size + 1] = { 0 };
				memcpy(char_sub_role_name, role_name.c_str(), role_name.size() > max_name_size ? max_name_size : role_name.size());
				iter_marry_info->second.set_sub_name(char_sub_role_name);
				is_change = true;
			}

			if (is_change)
			{
				sync_couple_data_info(iter_marry_info->second.main_couple_guid, iter_marry_info->second);
				sync_couple_data_info(iter_marry_info->second.sub_couple_guid, iter_marry_info->second);
			}
			
		}

		auto iter_marry_wait = m_marry_waiting_array.begin();
		for (; iter_marry_wait != m_marry_waiting_array.end(); ++iter_marry_wait)
		{
			if (iter_marry_wait->main_couple_guid == role_guid)
			{
				xchar char_main_role_name[max_name_size + 1] = { 0 };
				memcpy(char_main_role_name, role_name.c_str(), role_name.size() > max_name_size ? max_name_size : role_name.size());
				iter_marry_wait->set_main_name(char_main_role_name);
			}

			if (iter_marry_wait->sub_couple_guid == role_guid)
			{
				xchar char_sub_role_name[max_name_size + 1] = { 0 };
				memcpy(char_sub_role_name, role_name.c_str(), role_name.size() > max_name_size ? max_name_size : role_name.size());
				iter_marry_wait->set_sub_name(char_sub_role_name);

			}
		}

		auto iter_divorce_wait = m_divorce_waiting_array.begin();
		for (; iter_divorce_wait != m_divorce_waiting_array.end(); ++iter_divorce_wait)
		{
			if (iter_divorce_wait->main_couple_guid == role_guid)
			{
				xchar char_main_role_name[max_name_size + 1] = { 0 };
				memcpy(char_main_role_name, role_name.c_str(), role_name.size() > max_name_size ? max_name_size : role_name.size());
				iter_divorce_wait->set_main_name(char_main_role_name);
			}

			if (iter_divorce_wait->sub_couple_guid == role_guid)
			{
				xchar char_sub_role_name[max_name_size + 1] = { 0 };
				memcpy(char_sub_role_name, role_name.c_str(), role_name.size() > max_name_size ? max_name_size : role_name.size());
				iter_divorce_wait->set_sub_name(char_sub_role_name);
			}
		}
	}
	void marry_mgr_ws::sync_wedding_ring_info(const guid_64& role_guid)
	{
		client_session* main_session = client_session_mgr::getInstance().get_session(role_guid);
		if (nullptr != main_session)
		{
			if (main_session->is_self_server())
			{
				ws2cs_update_wedding_ring_state msg;
				marry_record_data* data_ptr = find_marry_record_info(role_guid);
				msg.role_guid = role_guid;
				if (nullptr == data_ptr)
					msg.is_marry = false;
				else
				{
					msg.is_marry = true;
					msg.marry_item_id = data_ptr->marry_item_id;
					if (role_guid == data_ptr->main_couple_guid)
						msg.mate_ring_level = data_ptr->sub_ring_level;
					else
						msg.mate_ring_level = data_ptr->main_ring_level;
					msg.heart_value = data_ptr->heart_value;
				}
				bool is_use = proto_by_lua(e_msgindex_ws2cs_update_wedding_ring_state);
				if (is_use == false)
				{
					main_session->send_to_cs(&msg, sizeof(msg));
				}
				else
				{
					hld::ws2cs_proto::update_wedding_ring_state pro_msg;
					msg.to_proto(pro_msg);
					main_session->send_to_cs_lua(&pro_msg, e_msgindex_ws2cs_update_wedding_ring_state);
				}
			}
			else //跨服的话 去本服数据库取出结婚信息发给玩家
			{
				ws2dp_load_marry_info_cross_server msg;
				msg.role_guid = role_guid;
				//msg.server_id = world_server::getInstance().get_cross_group_id();
				ws_client::getInstance().send_to_dp(&msg, sizeof(msg), main_session->get_role_info_data(e_role_info_server_id));
			}
		}
	}

	void marry_mgr_ws::sync_marry_mission_info(const guid_64& role_guid)
	{
		//add_heart_value(role_guid, 10);
		client_session* main_session = client_session_mgr::getInstance().get_session(role_guid);
		if (nullptr != main_session)
		{
			if (main_session->is_self_server())
			{
				ws2cs_sync_marry_task msg;
				marry_record_data* data_ptr = find_marry_record_info(role_guid);
				msg.role_guid = role_guid;
				if (nullptr == data_ptr)
					msg.is_marry = false;
				else
				{
					msg.is_marry = true;
				}
				bool is_use = proto_by_lua(e_msgindex_ws2cs_sync_marry_task);
				if (is_use == false)
				{
					main_session->send_to_cs(&msg, sizeof(msg));
				}
				else
				{
					hld::ws2cs_proto::sync_marry_task pro_msg;
					msg.to_proto(pro_msg);
					main_session->send_to_cs_lua(&pro_msg, e_msgindex_ws2cs_sync_marry_task);
				}
			}
			else //跨服的话 去本服数据库取出结婚信息发给玩家
			{
				ws2dp_load_marry_info_cross_server msg;
				msg.role_guid = role_guid;
				//msg.server_id = world_server::getInstance().get_cross_group_id();
				ws_client::getInstance().send_to_dp(&msg, sizeof(msg), main_session->get_role_info_data(e_role_info_server_id));
			}
		}
	}

	void marry_mgr_ws::cs_update_wedding_ring_level(const guid_64& role_guid, int32 ring_level)
	{
		marry_record_data* data_ptr = find_marry_record_info(role_guid);
		if (data_ptr == nullptr)
			return;
		//找到配偶信息
		bool is_main = false;
		guid_64 mate_guid;
		if (data_ptr->main_couple_guid == role_guid)
		{
			mate_guid = data_ptr->sub_couple_guid;
			is_main = true;
		}
		else
		{
			mate_guid = data_ptr->main_couple_guid;
		}

		marry_record_data* mate_date_ptr = find_marry_record_info(mate_guid);
		if (mate_date_ptr == nullptr)
			return;

		//1.同时更改自己和对方的
		if (is_main)
		{
			data_ptr->main_ring_level = ring_level;
			mate_date_ptr->main_ring_level = ring_level;
		}
		else
		{
			data_ptr->sub_ring_level = ring_level;
			mate_date_ptr->sub_ring_level = ring_level;
		}

		//2. 保存
		save_marry_data(*data_ptr);

		//3. 同步给对方
		client_session* mate_session = client_session_mgr::getInstance().get_session(mate_guid);
		if (nullptr != mate_session)
		{
			ws2cs_update_wedding_ring_level msg;
			msg.role_guid = mate_guid;
			msg.mate_ring_level = ring_level;
			bool is_use = proto_by_lua(e_msgindex_ws2cs_update_wedding_ring_level);
			if (is_use == false)
			{
				mate_session->send_to_cs(&msg, sizeof(msg));
			}
			else
			{
				hld::ws2cs_proto::update_wedding_ring_level pro_msg;
				msg.to_proto(pro_msg);
				mate_session->send_to_cs_lua(&pro_msg, e_msgindex_ws2cs_update_wedding_ring_level);
			}
		}
	}

	bool marry_mgr_ws::add_heart_value(const guid_64& role_guid, int32 add_value)
	{
		marry_record_data* data_ptr = find_marry_record_info(role_guid);
		if (data_ptr == nullptr)
			return false;

		//找到配偶信息
		guid_64 mate_guid;
		if (data_ptr->main_couple_guid == role_guid)
		{
			mate_guid = data_ptr->sub_couple_guid;
		}
		else
		{
			mate_guid = data_ptr->main_couple_guid;
		}

		marry_record_data* mate_date_ptr = find_marry_record_info(mate_guid);
		if (mate_date_ptr == nullptr)
			return false;

		//1.同时更改自己和对方的
		data_ptr->heart_value += add_value;
		mate_date_ptr->heart_value = data_ptr->heart_value;

		//2. 保存
		save_marry_data(*data_ptr);

		//3. 同步给双方
		client_session* main_session = client_session_mgr::getInstance().get_session(role_guid);
		if (nullptr != main_session)
		{
			ws2cs_refresh_heart_value msg;
			msg.role_guid = role_guid;
			msg.heart_value = data_ptr->heart_value;
			bool is_use = proto_by_lua(e_msgindex_ws2cs_refresh_heart_value);
			if (is_use == false)
			{
				main_session->send_to_cs(&msg, sizeof(msg));
			}
			else
			{
				hld::ws2cs_proto::refresh_heart_value pro_msg;
				msg.to_proto(pro_msg);
				main_session->send_to_cs_lua(&pro_msg, e_msgindex_ws2cs_refresh_heart_value);
			}
		}
		client_session* mate_session = client_session_mgr::getInstance().get_session(mate_guid);
		if (nullptr != mate_session)
		{
			ws2cs_refresh_heart_value msg;
			msg.role_guid = mate_guid;
			msg.heart_value = data_ptr->heart_value;
			bool is_use = proto_by_lua(e_msgindex_ws2cs_refresh_heart_value);
			if (is_use == false)
			{
				mate_session->send_to_cs(&msg, sizeof(msg));
			}
			else
			{
				hld::ws2cs_proto::refresh_heart_value pro_msg;
				msg.to_proto(pro_msg);
				mate_session->send_to_cs_lua(&pro_msg, e_msgindex_ws2cs_refresh_heart_value);
			}
		}

		//4.添加到排行榜中
		s_ranking_player_info role_info;
		role_info.role_guid = data_ptr->main_couple_guid;
		role_info.set_role_name(data_ptr->main_couple_name);
		role_info.supporting_guid = data_ptr->sub_couple_guid;
		role_info.set_supporting_name(data_ptr->sub_couple_name);
		role_info.ranking_type = e_RankingIndex_couple;
		role_info.ranking_value = data_ptr->heart_value;
		role_info.server_id = world_server::getInstance().get_server_id();
		role_info.save_time = time_helper::get_cur_time_new().second;
		ranking_mgr_ws::sync_player_info(role_info);
		return true;
	}

	void marry_mgr_ws::req_bless_one_couple(client_session* client_ptr, const guid_64& target_guid)
	{
		if (nullptr == client_ptr)
			return;
		marry_record_data* data_ptr = find_marry_record_info(target_guid);
		if (nullptr == data_ptr)
			return;
		if (client_ptr->get_role_guid() == data_ptr->main_couple_guid || client_ptr->get_role_guid() == data_ptr->sub_couple_guid)
			return;
		int32 ranking_level = -1;
		int64 ranking_value = 0;
		ranking_mgr_ws::get_player_rank_level(e_RankingIndex_couple, data_ptr->main_couple_guid, ranking_level, ranking_value);
		if (-1 == ranking_level)
			return;
		ws2cs_req_cost_bless_money msg;
		msg.role_guid = client_ptr->get_role_guid();
		msg.target_guid = target_guid;
		client_ptr->send_to_cs(&msg, sizeof(msg));
	}

	void marry_mgr_ws::req_bless_one_couple_end(const char* role_name, const guid_64& target_guid)
	{
		marry_record_data* data_ptr = find_marry_record_info(target_guid);
		if (nullptr == data_ptr)
			return;
		int32 ranking_level = -1;
		int64 ranking_value = 0; 
		ranking_mgr_ws::get_player_rank_level(e_RankingIndex_couple, data_ptr->main_couple_guid, ranking_level, ranking_value);
		if (-1 == ranking_level)
			return;

		int32 add_value = GAMECONFIG->CoupleBlessAddValue;
		add_heart_value(target_guid, add_value);

		const xchar* title = globle_data::get_instance().get_mail_common_text_id(e_mail_common_text_couple_bless_title);
		const xchar* content = globle_data::get_instance().get_mail_common_text_id(e_mail_common_text_couple_bless_context);
		if (title && content)
		{
			std::vector<s_item_template_info> drop_item_list;
			drop_item_list.clear();
			std::vector<std::string> content_params;
			content_params.clear();
			content_params.push_back(std::string(content));
			content_params.push_back(std::string(role_name));
			content_params.push_back(init_unit::change_i32_to_string(add_value));
			std::string contenttext = init_unit::implode(content_params);
			mail_ws_mgr::get_instance().send_mail_system(data_ptr->main_couple_guid, 0, drop_item_list, title, contenttext);
			mail_ws_mgr::get_instance().send_mail_system(data_ptr->sub_couple_guid, 0, drop_item_list, title, contenttext);
		}

		client_session* role_ptr = client_session_mgr::getInstance().get_session_by_role_name(role_name);
		if (role_ptr)
		{
			relation_proto_s2c_bless_one_couple_result msg;
			msg.set_result(1);
			role_ptr->send_to_client(&msg, e_msgindex_s2c_bless_one_couple_result);
		}
	}

	guid_64 marry_mgr_ws::get_main_role_guid(const guid_64& role_guid)
	{
		marry_record_data* data_ptr = find_marry_record_info(role_guid);
		if (nullptr == data_ptr)
			return 0;
		return  data_ptr->main_couple_guid;
	}

	void marry_mgr_ws::send_mail_to_couple(const guid_64& main_guid, const guid_64& sub_guid, bool is_divorce)
	{
		if(false == is_divorce)
		{
			const xchar* title = globle_data::get_instance().get_mail_common_text_id(e_mail_common_text_couple_marry_title);
			const xchar* content = globle_data::get_instance().get_mail_common_text_id(e_mail_common_text_couple_marry_context);
			if (title && content)
			{
				std::vector<s_item_template_info> drop_item_list;
				drop_item_list.clear();
				mail_ws_mgr::get_instance().send_mail_system(main_guid, 0, drop_item_list, title, content);
				mail_ws_mgr::get_instance().send_mail_system(sub_guid, 0, drop_item_list, title, content);
			}
		}
		else
		{
			const xchar* title = globle_data::get_instance().get_mail_common_text_id(e_mail_common_text_couple_divorce_title);
			const xchar* content = globle_data::get_instance().get_mail_common_text_id(e_mail_common_text_couple_divorce_context);
			if (title && content)
			{
				std::vector<s_item_template_info> drop_item_list;
				drop_item_list.clear();
				mail_ws_mgr::get_instance().send_mail_system(main_guid, 0, drop_item_list, title, content);
				mail_ws_mgr::get_instance().send_mail_system(sub_guid, 0, drop_item_list, title, content);
			}
		}
	}
}
 
