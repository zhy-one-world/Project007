/********************************************************************
created: 2016年8月16日
file base: chat_mgr
file ext: cpp
author: luoxingyu

purpose: about role's chat_mgr
*********************************************************************/

#include "internet/chat.pb.h"
#include "chat_def.hpp"
#include "chat_mgr.h"
#include "connection_mgr.hpp"
#include "internal/core.hpp"
#include "invalid_word/invalid_ansi_word.h"
#include "invalid_word/trie_filter.h"
#include "logic/item_set.h"
#include "logic/player.hpp"
#include "server_log.hpp"
#include "system/item/item_system.h"
#include "template/StringConst_S.h"
#include "template/template_manager.h"
#include "internet/net.pb.h"
#include "utility/init_unit.h"
#include "internal/chat_msg.hpp"
#include "internet/cs2dp.pb.h"
#include "utility/parse_msg.h"

namespace hld
{
	cchat_mgr::cchat_mgr()
	{
		m_player_ptr = nullptr;
		clear_data();
	}
	cchat_mgr::~cchat_mgr(void)
	{

	}
	void cchat_mgr::clear_data()
	{
		m_is_disable_private_chat = false;
		m_last_send_chat_time = 0;
		m_last_chat_str = "";
		m_last_chat_time = 0;
		m_last_send_server_chat_time = 0;

		for (int32 i = 0; i < chat_record_max_num; i++)
		{
			memset((void *)(m_chat_record_array + i), 0, sizeof(s_chat_record));
		}
	}

	void cchat_mgr::send_globel_message(std::string chat_text)
	{
		cs2ws_send_globel_message msg;

		int32 text_len = chat_text.length() + 1;
		if (text_len > globel_message_max_size)
		{
			text_len = globel_message_max_size;
		}

		guid_64 sender_guid = m_player_ptr->get_unit_guid();
		msg.role_guid = sender_guid;
		memcpy(&msg.message_text, chat_text.c_str(), text_len);

		connection_mgr::getInstance().send_to_ws( &msg, sizeof(msg));
	}

	void cchat_mgr::send_notice(int32 notice_template_id, std::string show_string)
	{
		cs2ws_send_notice msg;
		if (nullptr == m_player_ptr)
		{
			return;
		}
		msg.role_guid = m_player_ptr->get_unit_guid();
		msg.notice_id = notice_template_id;

		int32 text_len = show_string.length();
		if (text_len > globel_message_max_size)
		{
			return;
		}
		memcpy(msg.notice_string, show_string.c_str(), show_string.length());
		connection_mgr::getInstance().send_to_ws( &msg, sizeof(msg));
	}

	void cchat_mgr::send_notice(int32 notice_template_id, std::string show_string, guid_64 role_guid)
	{

		cs2ws_send_notice msg;
		msg.role_guid = role_guid;
		msg.notice_id = notice_template_id;

		int32 text_len = show_string.length();
		if (text_len > globel_message_max_size)
		{
			return;
		}
		memcpy(msg.notice_string, show_string.c_str(), show_string.length());
		connection_mgr::getInstance().send_to_ws( &msg, sizeof(msg));
	}

	void cchat_mgr::send_notice(int32 notice_template_id, std::string show_string, s_item_info* item_data, int32 item_num)
	{
		cs2ws_send_notice msg;
		if (nullptr == m_player_ptr)
		{
			return;
		}

		msg.role_guid = m_player_ptr->get_unit_guid();
		msg.notice_id = notice_template_id;
		msg.item_num = item_num;
		for (int32 i = 0; i <= item_num; ++i)
		{
			for (int32 j = 0; j < e_item_info_max; ++j)
			{
				msg.data_ary[i].data_ary[j] = item_data[i].data_ary[j];
			}	
		}

		int32 text_len = show_string.length();
		if (text_len > globel_message_max_size)
		{
			return;
		}
		memcpy(msg.notice_string, show_string.c_str(), show_string.length());
		connection_mgr::getInstance().send_to_ws( &msg, sizeof(msg));
	}

	void cchat_mgr::receive_notice(int32 notice_template_id, guid_64 sender_guid, int32 create_time)
	{
		if (m_player_ptr == nullptr)
		{
			return;
		}

		chat_proto_notice_info msg;
		msg.add_role_guid(sender_guid.A);
		msg.add_role_guid(sender_guid.B);
		msg.set_notice_id(notice_template_id);
		msg.set_create_time(create_time);
		m_player_ptr->send_message_to_self(&msg, e_msgindex_s2c_receive_notice);
	}

	void cchat_mgr::receive_top_tip(int32 string_id, int32 top_tip_type)
	{
		if (m_player_ptr == nullptr
			|| top_tip_type < e_top_tip_type_normal
			|| top_tip_type >= e_top_tip_type_max)
		{
			return;
		}

		chat_proto_receive_top_tip msg;
		msg.set_string_id(string_id);
		msg.set_top_tip_type(top_tip_type);
		m_player_ptr->send_message_to_self(&msg, e_msgindex_s2c_receive_top_tip);
	}

	bool cchat_mgr::parse_notice_by_symbol_type(const int32& notice_id, const xstring& unit_name, const std::vector<xstring>& other_name, const std::vector<xstring>& describe_str, const std::vector<xstring>& item_name, const  std::vector<int32>& num_type_data, xstring& _out_notice_str)
	{
		if (notice_id <= 0)
		{
			return false;
		}

		NoticeTemplate* notice_template_ptr = GET_TEMPLATE(NoticeTemplate, notice_id);
		if (nullptr == notice_template_ptr)
		{
			return false;
		}
		if (notice_template_ptr->ContentText.size() <= 0)
		{
			return false;
		}
		int32  param_type_size = notice_template_ptr->ParseTypeArrray.size();
		if (param_type_size <= 0)
		{
			return false;
		}
		_out_notice_str = "";
		xstring notice_str_id = template_manager::get_instance().int_to_string(notice_template_ptr->ContentText[0]);
		int32 other_name_index = 0;
		int32 item_name_index = 0;
		int32 describe_str_index = 0;
		int32 num_type_data_index = 0;
		int32 item_data_array_index = 0; 
		int32 item_data_index = 0;
		bool is_had_item_link = false;
		std::vector<std::string> notice_str_params_vec;
		notice_str_params_vec.push_back(notice_str_id);
		for (int32 param_type_index = 0; param_type_index < param_type_size; param_type_index++)
		{
			switch (e_notice_param_type(notice_template_ptr->ParseTypeArrray[param_type_index]))
			{
			case hld::e_notice_param_type_unit_name:
			{
				notice_str_params_vec.push_back(unit_name);
			}
			break;
			case hld::e_notice_param_type_item_name:
			{
				if (item_name_index >= item_name.size())
				{
					return false;
				}
				notice_str_params_vec.push_back(item_name[item_name_index++]);
			}
			break;
			case hld::e_notice_param_type_num_data:
			{
				if (num_type_data_index >= num_type_data.size())
				{
					return false;
				}
				notice_str_params_vec.push_back(template_manager::get_instance().int_to_string(num_type_data[num_type_data_index++]));
			}
			break;
			case hld::e_notice_param_type_item_data:
			{
				if (!is_had_item_link)
				{
					is_had_item_link = true;
					item_data_index = notice_str_params_vec.size();
					notice_str_params_vec.push_back(" %d");
				}
				else
				{
					notice_str_params_vec[item_data_index] += " %d";
				}
			}
				break;
			case hld::e_notice_param_type_describe_str:
			{
				if (describe_str_index >= describe_str.size())
				{
					return false;
				}
				notice_str_params_vec.push_back(describe_str[describe_str_index++]);
			}
			break;
			case hld::e_notice_param_type_other_name:
			{
				if (other_name_index >= unit_name.size())
				{
					return false;
				}
				notice_str_params_vec.push_back(other_name[other_name_index++]);
			}
			break;

			case hld::e_notice_param_type_max:
				break;
			default:
				break;
			}

		}
		_out_notice_str = init_unit::implode(notice_str_params_vec);
		return true;
	}

	bool cchat_mgr::make_and_send_notic_msg_data(const guid_64&  unit_guid,const int32& notice_template_id, const std::string& show_string, const std::vector<s_item_info>& item_data)
	{
		cs2ws_send_notice msg;
		msg.role_guid = unit_guid;
		msg.notice_id = notice_template_id;  
		int32 item_num = item_data.size(); 
		for (int32 index = 0; index < item_num; ++index)
		{
			if (msg.item_num >= chat_max_item)
			{
				int32 text_len = show_string.length();
				if (text_len > globel_message_max_size)
				{
					return false;
				}
				msg.item_num -= 1;			//消息里的定义不是item数量而是索引
				memcpy(msg.notice_string, show_string.c_str(), show_string.length());
				connection_mgr::getInstance().send_to_ws( &msg, sizeof(msg));
				msg.clear_item_info(); 
			}
			for (int32 info_type = 0; info_type < e_item_info_max; ++info_type)
			{
				msg.data_ary[msg.item_num].data_ary[info_type] = item_data[index].data_ary[info_type];
			}
			msg.item_num++;
		}

		msg.item_num -= 1;
		int32 text_len = show_string.length();
		if (text_len > globel_message_max_size)
		{
			return false;
		}
		memcpy(msg.notice_string, show_string.c_str(), show_string.length());
		connection_mgr::getInstance().send_to_ws( &msg, sizeof(msg));
		return  true;
	}

	void cchat_mgr::send_notice_new(const int32&  notice_template_id, const std::vector<xstring>& other_name, const std::vector<xstring>& describe_str, const std::vector<xstring>& item_name, const  std::vector<int32>& num_type_data, const  std::vector<s_item_info>& item_data)
	{
		if (nullptr == m_player_ptr)
		{
			return;
		}
		xstring notice_str = "";
		if (parse_notice_by_symbol_type(notice_template_id, m_player_ptr->get_name(), other_name, describe_str, item_name, num_type_data, notice_str))
		{
			make_and_send_notic_msg_data(m_player_ptr->get_unit_guid(), notice_template_id, notice_str, item_data);
		}
	}

	void cchat_mgr::send_notice_new(const int32&  notice_template_id, const s_chat_notice_info& notice_data)
	{
		if (notice_template_id <= 0 )
		{
			return;
		}
		send_notice_new(notice_template_id, notice_data.other_name, notice_data.describe_str, notice_data.item_name, notice_data.num_type_data, notice_data.item_data);
	}

	void cchat_mgr::send_change_first_ranking_notice(const int32& ranking_type, bool is_first)
	{
		if (nullptr == m_player_ptr)
		{
			return;
		}
		// 排行榜首位变更公告
		if (is_first)
		{ 
			int32 string_id = 0;
			switch (ranking_type) 
			{			// 没有配表，手动分吧 
			case e_RankingIndex_gs:
				string_id = 90090955;
				break;
			case e_RankingIndex_gs_barserker:
				string_id = 90090956;
				break;
			case e_RankingIndex_gs_wizard:
				string_id = 90090957;
				break;
			case e_RankingIndex_gs_guardian:
				string_id = 90090958;
				break;
			case e_RankingIndex_gs_assassinator:
				string_id = 90090959;
				break;
			case e_RankingIndex_level:
				string_id = 90090960;
				break;
			case e_RankingIndex_level_barserker:
				string_id = 90090961;
				break;
			case e_RankingIndex_level_wizard:
				string_id = 90090962;
				break;
			case e_RankingIndex_level_guardian:
				string_id = 90090963;
				break;
			case e_RankingIndex_level_assassinator:
				string_id = 90090964;
				break;
			case e_RankingIndex_equipment_main:
				string_id = 90090965;
				break;
			case e_RankingIndex_equipment_weapon:
				string_id = 90090966;
				break;
			case e_RankingIndex_equipment_necklace:
				string_id = 90090967;
				break;
			case e_RankingIndex_equipment_armor:
				string_id = 90090968;
				break;
			case e_RankingIndex_equipment_pants:
				string_id = 90090969;
				break;
			case e_RankingIndex_equipment_hat:
				string_id = 90090970;
				break; 
			case e_RankingIndex_equipment_shoes:
				string_id = 90090971;
				break;
			case e_RankingIndex_equipment_glove:
				string_id = 90090972;
				break;
			case e_RankingIndex_equipment_ring:
				string_id = 90090973;
				break;
			case e_RankingIndex_equipment_earring:
				string_id = 90090974;
				break;
			case e_RankingIndex_mount:
				string_id = 90090975;
				break;
			case e_RankingIndex_mount_train:
				string_id = 90090976;
				return;
			case e_RankingIndex_wing:
				string_id = 90090977;
				break;
			case e_RankingIndex_wing_train:
				string_id = 90090978;
				return;
			case e_RankingIndex_box_map_level:
				string_id = 90090635;
				break;
			case e_RankingIndex_worship:
				string_id = 90090632;
				break;
			case e_RankingIndex_belief:
				string_id = 90090979;
				break;
			case e_RankingIndex_element:
				string_id = 90090980;
				break;
			case e_RankingIndex_inspiration:
				string_id = 90090981;
				break;
			case e_RankingIndex_military_rank:
				string_id = 90090982;
				break;
			case e_RankingIndex_achievement:
				string_id = 90090983;
				break;
			case e_RankingIndex_demonstower:
				string_id = 90090638;
				break;
			case e_RankingIndex_arena:
				string_id = 90090639;
				break;
			case e_RankingIndex_elves_gs:
				string_id = 90069643;
				break;
			case e_RankingIndex_feather_heart:
				string_id = 90201880;
				break;
			default:
				return;
			}

			int32 notice_id = 93000027;// 因为没有排行榜的表，所以这个noticeid的值写死了
			std::vector<std::string> vec_notice_str;
			vec_notice_str.push_back(template_manager::get_instance().get_str_id_by_notice_id(notice_id));
			vec_notice_str.push_back(std::string(m_player_ptr->get_name()));
			vec_notice_str.push_back(template_manager::get_instance().int_to_string(string_id));
			xstring notice_str = init_unit::implode(vec_notice_str);
			send_notice(notice_id, notice_str);
		}
	}

	void cchat_mgr::send_chat_new( e_chat_content_info* chat_content_array, int32 chat_content_num, std::string common_string, int32 chat_type, guid_64 addressee_guid)
	{
		if (m_player_ptr == nullptr)
		{
			return;
		}

		if (chat_content_array != nullptr)
		{
			if (std::strlen(chat_content_array[0].audio_id) > 0)
			{
				if (invalid_ansi_word::include_invalid_ansi_str(common_string) == true)
				{
					send_failed_process(e_chat_voice_message_forbidden_world, chat_type);
					return;
				}
			}
		}

		if (m_player_ptr->get_role_right(e_role_right_allow_chat) > 0)
		{
			send_failed_process(e_chat_is_baning, chat_type);
			return;
		}
		//if (chat_type == e_chat_type_world
		//	&& m_player_ptr->get_unit_info(e_role_info_exp_level) < GAMECONFIG->WorldChatLevelLimit)
		//{
		//	send_failed_process(e_item_string_unkown, chat_type);
		//	return;
		//}

		if (chat_type == e_chat_type_server
			&& m_player_ptr->get_unit_info(e_role_info_exp_level) < GAMECONFIG->WorldChatLevelLimit)
		{
			m_player_ptr->send_notice("90201661");
			return;
		}

		if (chat_type == e_chat_type_near
			&& m_player_ptr->get_unit_info(e_role_info_exp_level) < GAMECONFIG->NearChatLevelLimit)
		{
			send_failed_process(e_item_string_unkown, chat_type);
			return;
		}
		int64 now_time = utility::get_tick_count();
		chat_proto_chat_result temp_result;
		temp_result.set_chat_type(chat_type);
		int32 send_result = 0;
		//if (chat_type == e_chat_type_world)
		//{
		//	if (m_player_ptr->get_unit_info(e_role_info_vip_level) < 1  && now_time < m_last_send_chat_time)
		//	{
		//		send_failed_process(e_chat_cant_send_chat, chat_type);
		//		return;
		//	}
		//	if (now_time < m_last_chat_time && m_last_chat_str == common_string)
		//	{
		//		send_failed_process(e_chat_is_same_str, chat_type);
		//		return;
		//	}
		//}

		if (e_chat_type_server == chat_type)
		{
			if (now_time < m_last_send_server_chat_time)
			{
				int64 need_sec = (m_last_send_server_chat_time - now_time) / second_tick_time;
				std::vector<std::string> vec_str_time;
				vec_str_time.push_back("90096660");
				vec_str_time.push_back(template_manager::get_instance().int_to_string(need_sec));
				std::string notice_str = init_unit::implode(vec_str_time);
				m_player_ptr->send_notice(notice_str);
				return;
			}
		}

		if (chat_content_num <= 0)
		{
			send_failed_process(e_chat_is_empty, chat_type);
			return; 
		}

		s_item_info chat_item_array[chat_max_item];
		int32 send_item_num = 0;
		for (int32 i = 0; i < chat_content_num; ++i)
		{
			const e_chat_content_info& temp_info = chat_content_array[i];
			if (temp_info.is_item()
				&& send_item_num < chat_max_item)
			{
				for (int32 j = 0 ; j < e_item_info_max; ++j)
				{
					chat_item_array[send_item_num].data_ary[j] = temp_info.item_info[j];
				}
				send_item_num++;
			}
		}
		if (is_had_special_word(common_string))
		{
			send_failed_process(e_chat_illegal_string, chat_type);
			return;
		}
		

		if ((chat_type == e_chat_type_private || chat_type == e_chat_type_friend) && (!addressee_guid.is_valid()))
		{
			send_failed_process(e_chat_no_in_player, chat_type);
			return;
		}
		if (chat_content_num > chat_max_word_num)
		{
			send_failed_process(e_chat_too_long, chat_type);
			return;
		}

		if (addressee_guid == m_player_ptr->get_unit_guid())
		{
			return;
		}

		server_log::chat_log(m_player_ptr->get_third_info(), m_player_ptr->get_unit_info_inst(), addressee_guid
			, chat_type, common_string, chat_item_array, send_item_num, m_player_ptr->get_login_type());


		if (chat_type == e_chat_type_near)
		{
			forbidden_word_check(chat_content_array, chat_content_num);

			//等级低于GAMECONFIG->ChatCostLimitLv 附近和世界频道需要花费金币
			if (m_player_ptr->get_unit_info(e_role_info_exp_level) < GAMECONFIG->ChatCostLimitLv)
			{
				if (GAMECONFIG->NearChatCoinCount.size() < e_money_tuple_max || GAMECONFIG->NearChatCoinCount.size() % e_money_tuple_max != 0)
				{
					return;
				}
				//int32 	CostCoin = GAMECONFIG->NearChatCoinCount;
				if (!m_player_ptr->can_cut_money(e_money_type(GAMECONFIG->NearChatCoinCount[e_money_tuple_id]), GAMECONFIG->NearChatCoinCount[e_money_tuple_num]))
				{
					return;
				}	
				m_player_ptr->cut_money(e_money_type(GAMECONFIG->NearChatCoinCount[e_money_tuple_id]), GAMECONFIG->NearChatCoinCount[e_money_tuple_num], e_server_log_cut_money_chat);
			}

			chat_proto_chat_content msg;
			msg.set_chat_typ(chat_type);
			msg.set_sender_guid(m_player_ptr->get_unit_guid().server_64);
			msg.set_sender_template_id(m_player_ptr->get_unit_info(e_role_info_template_id));
			msg.set_sender_name(m_player_ptr->get_name());
			msg.set_relation_template_id(m_player_ptr->get_unit_info(e_role_info_template_id));
			msg.set_relation_level(m_player_ptr->get_unit_info(e_role_info_exp_level));
			msg.set_chat_guid(guid_gen::make_guid().server_64);

			// add by wangsonghao : 玩家VIP爵位信息通过 chat_content 同步给客户端
			msg.set_vip_title_template_id(-1);
			bool bFoundTitle = false;
			for (int32 vip_level = m_player_ptr->get_unit_info(e_role_info_vip_level); vip_level >= 0; vip_level--)
			{
				if (bFoundTitle)
				{
					break;
				}
				VIPTemplate* vip_template_ptr = template_manager::get_instance().get_template_by_vip_level(vip_level);
				if (nullptr == vip_template_ptr || (vip_template_ptr->ItemGetArray.size() % 2 != 0))
				{
					continue;
				}
				int32 total_item_num = vip_template_ptr->ItemGetArray.size() / 2;
				for (int32 i = 0; i < total_item_num; ++i)
				{
					int32 item_id = vip_template_ptr->ItemGetArray[i * 2];
					ItemTemplate* temp_template_ptr = GET_TEMPLATE(ItemTemplate, item_id);
					if (temp_template_ptr == nullptr)
					{
						continue;
					}
					PropBasicAttributeTemplate* prop_template = GET_TEMPLATE(PropBasicAttributeTemplate, temp_template_ptr->logic_id);
					if (prop_template == nullptr)
					{
						continue;
					}
					if (prop_template->TitleGet > 0)
					{
						msg.set_vip_title_template_id(prop_template->TitleGet);
						bFoundTitle = true;
						break;
					}
				}
				// 这里不判断该爵位称号是否已经获得或者穿戴，只判断VIP等级是否达到该爵位对应的VIP等级
				// 				ctitle_mgr& title_mgr = m_player_ptr->get_title_mgr();
				// 				if (title_mgr.if_own_title_by_template_id(vip_template_ptr->TitleId))
				// 				{
				// 					msg.sender_vip_title_template_id = vip_template_ptr->TitleId;
				// 					break;
				// 				}
			}
			//////////////////////////////////////////////////////////////////////////

			for (int32 i = 0; i < chat_content_num; ++i)
			{
				e_chat_content_info temp_info = chat_content_array[i];
				chat_proto_chat_content_info* temp_info_msg = msg.add_content_info_array();
				if (temp_info.is_item())
				{
					for (int32 j = 0; j < e_item_info_max; ++j)
					{
						temp_info_msg->add_item_data(temp_info.item_info[j]);
					}
				}
				else if (temp_info.is_loction())
				{
					temp_info_msg->add_location(temp_info.location[0]);
					temp_info_msg->add_location(temp_info.location[1]);
					temp_info_msg->add_location(temp_info.location[2]);
					temp_info_msg->set_map_id(temp_info.map_id);
					temp_info_msg->set_line_id(temp_info.line_id);
				}
				else if (temp_info.template_id != 0)
				{
					temp_info_msg->set_template_id(temp_info.template_id);
				}
				else
				{
					temp_info_msg->set_commontext(temp_info.common_text);
				}
				temp_info_msg->set_audio_id(temp_info.audio_id);
				temp_info_msg->set_audio_duration_time(temp_info.duration_time);
				temp_info_msg->set_face_finish_img_id(temp_info.face_finish_img_id);
			}
			msg.set_is_finish_send(true);
			m_player_ptr->send_message_to_aoi(&msg, e_msgindex_s2c_receive_chat_new);
			temp_result.set_chat_result_id(0);
			m_player_ptr->send_message_to_self(&temp_result, e_msgindex_s2c_send_chat_result);
			return;
		}
		else
		{
			if (chat_type == e_chat_type_team)
			{
				if (m_player_ptr->is_player_in_team() == false)
				{
					temp_result.set_chat_result_id(e_chat_player_no_team);
					m_player_ptr->send_message_to_self(&temp_result, e_msgindex_s2c_send_chat_result);
					return;
				}
			}
			else if (chat_type == e_chat_type_legion)
			{
				if (false == m_player_ptr->get_legion_guid().is_valid())
				{
					temp_result.set_chat_result_id(e_chat_sender_no_legion);
					m_player_ptr->send_message_to_self(&temp_result, e_msgindex_s2c_send_chat_result);
					return;
				}
			}

			cs2ws_send_chat_to_ws_new msg;
			msg.sender_guid = m_player_ptr->get_unit_guid();
			msg.sender_server_id = m_player_ptr->get_unit_info(e_role_info_server_id);
			msg.sender_template_id = m_player_ptr->get_unit_info(hld::e_role_info_template_id);
			msg.sender_exp_level = m_player_ptr->get_unit_info(e_role_info_exp_level);
			// add by wangsonghao : 玩家VIP爵位信息通过 chat_content 同步给客户端
			msg.sender_vip_title_template_id = -1;
			msg.sender_head_frame_id = m_player_ptr->get_unit_info(e_role_info_head_frame);
			bool bFoundTitle = false;
			for (int32 vip_level = m_player_ptr->get_unit_info(e_role_info_vip_level); vip_level >= 0; vip_level--)
			{
				if (bFoundTitle)
				{
					break;
				}
				VIPTemplate* vip_template_ptr = template_manager::get_instance().get_template_by_vip_level(vip_level);
				if (nullptr == vip_template_ptr || (vip_template_ptr->ItemGetArray.size() % 2 != 0))
				{
					continue;
				}
				int32 total_item_num = vip_template_ptr->ItemGetArray.size() / 2;
				for (int32 i = 0; i < total_item_num; ++i)
				{
					int32 item_id = vip_template_ptr->ItemGetArray[i * 2];
					ItemTemplate* temp_template_ptr = GET_TEMPLATE(ItemTemplate, item_id);
					if (temp_template_ptr == nullptr)
					{
						continue;
					}
					PropBasicAttributeTemplate* prop_template = GET_TEMPLATE(PropBasicAttributeTemplate, temp_template_ptr->logic_id);
					if (prop_template == nullptr)
					{
						continue;
					}
					if (prop_template->TitleGet > 0)
					{
						msg.sender_vip_title_template_id = prop_template->TitleGet;
						bFoundTitle = true;
						break;
					}
				}
				// 这里不判断该爵位称号是否已经获得或者穿戴，只判断VIP等级是否达到该爵位对应的VIP等级
// 				ctitle_mgr& title_mgr = m_player_ptr->get_title_mgr();
// 				if (title_mgr.if_own_title_by_template_id(vip_template_ptr->TitleId))
// 				{
// 					msg.sender_vip_title_template_id = vip_template_ptr->TitleId;
// 					break;
// 				}
			}
			//////////////////////////////////////////////////////////////////////////
			msg.addressee_guid = addressee_guid;
			msg.chat_type = chat_type;
			msg.content_info_num = chat_content_num;

			if (chat_type == e_chat_type_system && addressee_guid.is_valid())
			{
				msg.content_info[0].template_id = addressee_guid.B;
				int32 count_num = 1;
				for (int32 i = 0; i < chat_content_num  && count_num < chat_max_word_num; i++)
				{
					msg.content_info[count_num] = chat_content_array[i];
					count_num++;
				}
				msg.content_info_num = count_num;
			}
			else
			{

				for (int32 i = 0; i < chat_content_num; ++i)
				{
					msg.content_info[i] = chat_content_array[i];
				}
			}

			memcpy(&msg.sender_name, m_player_ptr->get_name(), max_name_size);

			if (chat_type == e_chat_type_server)
			{
				m_last_send_server_chat_time = now_time + chat_interval_time;
			}

			if (chat_type == e_chat_type_world)
			{
				//世界聊天模式需要增加CD
				m_last_send_chat_time = now_time + chat_interval_time;
				m_last_chat_str = common_string;
				m_last_chat_time = now_time + chat_same_str_time;
				//等级低于GAMECONFIG->ChatCostLimitLv 附近和世界频道需要花费金币
				//if (m_player_ptr->get_unit_info(e_role_info_exp_level) < GAMECONFIG->ChatCostLimitLv)
				//{
				//	if (GAMECONFIG->WorldChatCoinCount.size() < e_money_tuple_max || GAMECONFIG->WorldChatCoinCount.size() % e_money_tuple_max != 0)
				//	{
				//		return;
				//	}
				//	//int32 	CostCoin = GAMECONFIG->WorldChatCoinCount;
				//	if (!m_player_ptr->can_cut_money(e_money_type(GAMECONFIG->WorldChatCoinCount[e_money_tuple_id]), GAMECONFIG->WorldChatCoinCount[e_money_tuple_num]))
				//	{
				//		return;
				//	}	
				//	m_player_ptr->cut_money(e_money_type(GAMECONFIG->WorldChatCoinCount[e_money_tuple_id]), GAMECONFIG->WorldChatCoinCount[e_money_tuple_num], e_server_log_cut_money_chat);
				//}
			}

			if (chat_type == e_chat_type_system)
			{
				if (addressee_guid.is_valid() == false)
				{
					return;
				}
				int32 item_id = addressee_guid.B;
				int32 goods_id = 0;
				ItemTemplate *item_ptr = GET_TEMPLATE(ItemTemplate, item_id);
				if (item_ptr == nullptr)
				{
					return;
				}
				if (item_ptr->item_type != e_item_type_expendable)
				{
					return;
				}

				if (item_ptr->sub_type != e_prop_sub_type_horn && item_ptr->sub_type != e_prop_sub_type_area_horn)
				{
					return;
				}
				if (item_system::can_cost_item(m_player_ptr, e_bag_type_bag, item_id, 1) == false)
				{
					if (item_ptr->goods_id.size() < 1)
					{
						return;
					}
					goods_id = item_ptr->goods_id[0];
					RegionTemplate* region_template_ptr = globle_data::get_instance().get_region_template_ptr();
					if (region_template_ptr == nullptr)
					{
						return;
					}
					int32 region_money_type_index = region_template_ptr->RegionCode * 2;
					int32 region_money_num_index = region_money_type_index + 1;
					GoodsTemplate* goods_ptr = GET_TEMPLATE(GoodsTemplate, goods_id);
					if (goods_ptr == nullptr || goods_ptr->NewPrice.size() < region_money_num_index)
					{
						return;
					}
					int32 money_type = goods_ptr->NewPrice[region_money_type_index];
					int32 money_num = goods_ptr->NewPrice[region_money_num_index];

					if (!m_player_ptr->can_cut_money((e_money_type)money_type, money_num))
					{
						temp_result.set_chat_result_id(-1);
						m_player_ptr->send_message_to_self(&temp_result, e_msgindex_s2c_send_chat_result);
						return;
					}
					m_player_ptr->cut_money((e_money_type)money_type, money_num, e_server_log_cut_money_far_speak, item_id);
				}
				if (goods_id == 0)
				{
					item_system::cost_item_from_bag(m_player_ptr, e_bag_type_bag, item_id, 1);
					//m_player_ptr->get_item_set().cost_item_by_id_with_lock_states(e_server_log_del_item_chat_far_speak, 0, item_id, 1, e_item_locked);
				}
				if (item_ptr->sub_type == e_prop_sub_type_area_horn)
				{
					msg.addressee_guid.A = 1;

				}
			}
			connection_mgr::getInstance().send_to_ws( &msg, msg.get_pak_length());
		    temp_result.set_chat_result_id(0);
			if (chat_type == e_chat_type_system)
			{
				temp_result.set_chat_result_id(1);
			}
			m_player_ptr->send_message_to_self(&temp_result, e_msgindex_s2c_send_chat_result);
			
		}
		if (m_player_ptr->get_write_log())
		{
			server_log::chat_channel_log(chat_type, common_string, m_player_ptr->get_unit_guid(), addressee_guid, chat_content_array[0].audio_id, chat_content_array[0].duration_time);

			set_log_var(log_head);

			m_player_ptr->get_log_common_head_info(log_head);

			server_log::serverChat(log_head, init_unit::change_i64_to_string(addressee_guid.server_64), chat_type, common_string);

		}
	 }

	void cchat_mgr::send_failed_process(int32 send_result, int32 chat_type)
	{
		if (m_player_ptr == nullptr)
		{
			return;
		}
		chat_proto_chat_result temp_result;
		temp_result.set_chat_type(chat_type);
		temp_result.set_chat_result_id(send_result);
		m_player_ptr->send_message_to_self(&temp_result, e_msgindex_s2c_send_chat_result);
	}

	void cchat_mgr::masking_word_check(std::string& temp_text)
	{
		if (invalid_ansi_word::include_invalid_ansi_str(temp_text))
		{
			invalid_ansi_word::replace_invalid_ansi_str(temp_text);
		}
	}

	bool cchat_mgr::is_had_special_word(std::string& temp_text)
	{
		return false;
	}

	// check whether chat info includes forbidden content
	// change the chat info array if exist
	void cchat_mgr::forbidden_word_check( e_chat_content_info* chat_content_ary, int32 chat_content_num)
	{
		std::vector<int>	chat_composition;
		std::string			str_to_be_checked;
		for(int i = 0; i < chat_content_num; ++i)
		{
			 e_chat_content_info& temp_chat_info = chat_content_ary[i];
			 if (temp_chat_info.is_only_text())
			 {
				 str_to_be_checked += temp_chat_info.common_text;
				 chat_composition.push_back(strlen(temp_chat_info.common_text));
			 }
			 else
			 {
				 chat_composition.push_back(0);
			 }
			 
		}
		check_forbidden_and_reconstruct(str_to_be_checked, chat_composition, chat_content_ary);
		
	}

	// change the str to lowercase for checking, reconstruct the chat content array after checking
	void cchat_mgr::check_forbidden_and_reconstruct(std::string& str_to_be_checked, const std::vector<int32>& chat_composition, e_chat_content_info* chat_content_ary)
	{
		int32 pos = 0;		//current checking position in string
		trie_filter::get_instance().find_forbidden(str_to_be_checked);

		for (int32 j = 0; j < chat_composition.size(); ++j)
		{
			std::string temp;
			
			if (chat_composition[j] == 0) // if the content is not text
			{
				continue;
			}

			for (int32 k = pos; k < pos + chat_composition[j]; ++k)
			{
				// if the content is forbidden and be replaced by ***
				if (chat_composition[j] >= 1 && str_to_be_checked[k] == '*')
				{
					temp = "*";
					break;
				}
				temp += str_to_be_checked[k];
			}
			pos += chat_composition[j];
			memset(chat_content_ary[j].common_text, 0, chat_content_info_max + 1);
			int32 str_len = temp.size() > chat_content_info_max ? chat_content_info_max : temp.size();
			memcpy(chat_content_ary[j].common_text, temp.c_str(), str_len);
		}
	}
	void cchat_mgr::save_chat_record(int32 save_type)
	{
		if (m_player_ptr == nullptr)
		{
			return;
		}
		if (m_player_ptr->is_valid() == false)
		{
			return;
		}
		bool is_use = template_manager::get_instance().is_message_use_lua(e_msgindex_cs2dp_save_chat_record);
		if (!is_use)
		{
			cs2dp_save_chat_record req;
			req.save_type_ex = save_type;
			req.role_guid = m_player_ptr->get_unit_guid();
			req.unit_array_index = m_player_ptr->get_array_index();
			int32 data_count = 0;
			for (int32 i = 0; i < chat_record_max_num; i++)
			{
				if (m_chat_record_array[i].record[0] == '\0')
				{
					continue;
				}
				req.data_list[data_count].pos = m_chat_record_array[i].pos;
				memcpy(req.data_list[data_count].record, m_chat_record_array[i].record, chat_record_max_len);
				data_count++;
			}
			req.data_num = data_count;
			m_player_ptr->send_message_to_dp(&req, req.get_pak_length());
			return;
		}
		hld::cs2dp_proto::save_role_chat_record msg;

		msg.set_role_guid(m_player_ptr->get_unit_guid().server_64);
		msg.set_unit_array_index(m_player_ptr->get_array_index());
		msg.set_save_type_ex(save_type);

		hld::cs2dp_proto::role_chat_record_db * db_data = msg.mutable_db_data();

		int32 data_count = 0;
		for (int32 i = 0; i < chat_record_max_num; i++)
		{
			if (m_chat_record_array[i].record[0] == '\0')
			{
				continue;
			}
			hld::cs2dp_proto::role_chat_record_row * db_row = db_data->add_row_data();
			db_row->set_pos(m_chat_record_array[i].pos);
			db_row->set_record(m_chat_record_array[i].record);
			data_count++;
		}
		db_data->set_row_count(data_count);
		if (data_count == 0)
		{
			db_data->add_row_data();
		}
		m_player_ptr->send_message_to_dp_lua(&msg, e_msgindex_cs2dp_save_chat_record);	
	}

	void cchat_mgr::load_chat_record_by_db(const s_chat_record * record_data, int32 load_num)
	{
		for (int32 i = 0; i < load_num && i < chat_record_max_num; i++)
		{
			int32 pos = record_data[i].pos;
			if (pos < 0 || pos >= chat_record_max_num)
			{
				continue;
			}
			m_chat_record_array[pos].pos = pos;
			memcpy(m_chat_record_array[pos].record, record_data[i].record, chat_record_max_len);
		}
		sync_all_chat_record_to_client();
		//sync_one_chat_record_to_client(m_chat_record_array + 0);
	}
	bool cchat_mgr::load_chat_record_by_db_lua(const char *data_ptr, int32 data_len)
	{
		if (data_ptr == nullptr || !data_len)
		{
			return false;
		}
		hld::cs2dp_proto::role_chat_record_db msg;
		bool is_sucess = parse_msg::getInstance().parse_buffer_to_proto(&msg, data_ptr, data_len);
		if (!is_sucess)
		{
			return false;
		}

		xchar *p_data = parse_msg::getInstance().get_cache(msg.row_count() * sizeof(s_chat_record));
		if (p_data == nullptr)
		{
			return false;
		}
		s_chat_record *p_row = (s_chat_record *)p_data;
		for (int32 i = 0; i < msg.row_count(); i++)
		{
			hld::cs2dp_proto::role_chat_record_row db_row = msg.row_data(i);
			p_row->pos = db_row.pos();
			parse_msg::getInstance().my_memcopy_string(p_row->record, chat_record_max_len, db_row.record());
			p_row++;
		}
		load_chat_record_by_db((const s_chat_record *)p_data, msg.row_count());
		return true;

	}
	void cchat_mgr::sync_all_chat_record_to_client()
	{
		if (m_player_ptr == nullptr)
		{
			return;
		}
		if (m_player_ptr->is_valid() == false)
		{
			return;
		}
		chat_proto_chat_all_record msg_all;
		msg_all.add_role_guid(m_player_ptr->get_unit_guid().A);
		msg_all.add_role_guid(m_player_ptr->get_unit_guid().B);

		for (int32 i = 0; i < chat_record_max_num; i++)
		{
			if (m_chat_record_array[i].record[0] == '\0')
			{
				continue;
			}
			chat_proto_chat_one_record * one_msg = msg_all.add_chat_array();
			one_msg->set_pos(m_chat_record_array[i].pos);
			one_msg->set_record(m_chat_record_array[i].record);
		}
		m_player_ptr->send_message_to_self(&msg_all, e_msgindex_s2c_chat_record_all);
	}

	void cchat_mgr::sync_one_chat_record_to_client(const s_chat_record * record_data)
	{
		if (m_player_ptr == nullptr)
		{
			return;
		}
		if (m_player_ptr->is_valid() == false)
		{
			return;
		}
		chat_proto_chat_one_record one_msg;
		one_msg.set_pos(record_data->pos);
		one_msg.set_record(record_data->record);
		m_player_ptr->send_message_to_self(&one_msg, e_msgindex_s2c_chat_record_one);
	}
    int32 cchat_mgr::get_offset(xchar value)
	{
		int32 offset = 0;

		for (int32 i = 0; i < 4; i++)
		{
			if (value & 1 << (7 - i))
			{
				offset++;
			}
			else
			{
				break;
			}
		}
		if (offset == 0)
		{
			offset = 1;
		}
		return offset;
	}
	int32 cchat_mgr::calc_string_len(xstring& record)
	{
		int32 len = record.length();
		const xchar *temp = record.c_str();

		int32 count = 0;
		int32 offset = 1;
		for (int32 i = 0; i < len; i += offset)
		{
			offset = get_offset(temp[i]);
			count++;

		}
		return count;
	}

	void cchat_mgr::chat_oper(int32 pos, xstring record)
	{
		if (pos < 0 || pos >= chat_record_max_num)
		{
			return;
		}
		if (record.length() > chat_real_max_length)
		{
			send_to_result(pos, chat_error_max_len);
			return;
		}
		if (calc_string_len(record) > chat_real_max_length / 3)
		{
			send_to_result(pos, chat_error_max_len);
			return;
		}
		if (invalid_ansi_word::include_invalid_ansi_str(record))
		{
			send_to_result(pos, chat_error_invalid_word);
			return;
		}
		m_chat_record_array[pos].pos = pos;
		memset(m_chat_record_array[pos].record, 0, chat_record_max_len);
		memcpy(m_chat_record_array[pos].record, record.c_str(), record.length());
		sync_one_chat_record_to_client(m_chat_record_array + pos);
		send_to_result(pos, chat_error_sucess);
	}
	void cchat_mgr::send_to_result(int32 pos, int32 result)
	{
		if (m_player_ptr == nullptr)
		{
			return;
		}
		if (m_player_ptr->is_valid() == false)
		{
			return;
		}
		chat_proto_chat_record_oper_end msg;
		msg.set_pos(pos);
		msg.set_result(result);
		m_player_ptr->send_message_to_self(&msg, e_msgindex_s2c_chat_record_end);
	}

}