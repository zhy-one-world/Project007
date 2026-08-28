#include "person_infor_ws_mgr.h"
#include "template/template_manager.h"
#include "utility/random.h"
#include "server/client_session_mgr.hpp"
#include "server/relation/marry_mgr_ws.hpp"
#include <marry_msg.hpp> 
#include "utility/init_unit.h"
#include "relation.pb.h"
#include "net.pb.h"

namespace hld
{
	person_infor_ws_mgr::person_infor_ws_mgr()
	{
	}

	person_infor_ws_mgr::~person_infor_ws_mgr()
	{

	}
	void person_infor_ws_mgr::sync_person_info(guid_64 role_guid, s_role_person_information person_info)
	{
		if (person_info_map.find(role_guid) != person_info_map.end())
		{
			//有数据
			person_info_map[role_guid] = person_info;
			get_relation_person(role_guid);
			get_one_relation_person(role_guid);
			return;
		}
		person_info_map.insert(std::make_pair(role_guid, person_info));
	}
	std::vector<guid_64> person_infor_ws_mgr::get_relation_person(guid_64 role_guid)
	{
		std::vector<guid_64> random_list;
		int32 my_tags[person_information_tag_num_max] = {0};
		if (person_info_map.find(role_guid) != person_info_map.end())
		{
			//有数据
			for (int32 i = 0 ; i < person_information_tag_num_max;i++)
			{
				my_tags[i] = person_info_map[role_guid].tags[i];
			}
		}
		std::set<int32> aim_tags;
		for (int32 index = 0;index < person_information_tag_num_max;index++)
		{
			if (my_tags[index] == 0)
			{
				continue;;
			}
			PersonalSignatureTemplate* question_marry_template = GET_TEMPLATE(PersonalSignatureTemplate, my_tags[index]);
			if (nullptr == question_marry_template)
			{
				continue;
			}
			for (int32 answer_index = 0; answer_index < question_marry_template->answer.size();answer_index++)
			{
				int32 answer = question_marry_template->answer[answer_index];
				if (aim_tags.find(answer) == aim_tags.end())
				{
					aim_tags.insert(answer);
				}
			}
		}
		int32 cur_num = 0;
		if (aim_tags.size() == 0)
		{
			random_list = random_relation_person(role_guid);
		}
		else {
			std::set<guid_64> cur_guid_set;
			//优先取
			for (std::set<int32>::iterator it = aim_tags.begin();it != aim_tags.end(); ++it)
			{
				for (std::map<guid_64, s_role_person_information>::iterator iter = person_info_map.begin(); iter != person_info_map.end(); ++iter)
				{
					s_role_person_information& role_info = iter->second;
					if (role_info.role_guid == role_guid)
					{
						continue;
					}
					if (cur_guid_set.find(role_info.role_guid) != cur_guid_set.end())
					{
						continue;
					}
					//已结婚的不考虑
					if (nullptr != marry_mgr_ws::get_instance().find_marry_record_info(role_info.role_guid))
					{
						continue;
					}
					for (int32 target_index = 0; target_index < person_information_tag_num_max;target_index++)
					{
						if (role_info.tags[target_index] == 0)
						{
							continue;
						}
						if (role_info.tags[target_index] == *it)
						{
							random_list.push_back(role_info.role_guid);
							cur_guid_set.insert(role_info.role_guid);
							cur_num++;
							if (cur_num >= relation_push_num_max)
							{
								return random_list;
							}
							break;
						}
					}
				}
			}
			if (random_list.size() == 0)
			{
				//没有找到合适的
				random_list = random_relation_person(role_guid);
			}
			cur_guid_set.clear();
		}
		return random_list;
	}

	std::vector<guid_64>  person_infor_ws_mgr::random_relation_person(guid_64 role_guid)
	{
		std::vector<guid_64> random_list;
		std::vector<guid_64> values;

		for (std::map<guid_64, s_role_person_information>::iterator iter = person_info_map.begin(); iter != person_info_map.end(); ++iter)
		{
			if (iter->first == role_guid)
			{
				continue;
			}
			//已结婚的不考虑
			if (nullptr != marry_mgr_ws::get_instance().find_marry_record_info(role_guid))
			{
				continue;
			}
			values.push_back(iter->first);
		}
		if (values.size() > 0)
		{
			if (values.size() <= relation_push_num_max)
			{
				random_list = values;//全部
			}
			else
			{
				for (int32 i = 0;i < relation_push_num_max;++i)
				{
					int32 rand_num = random_gen::get_random(0, values.size() - 1);
					random_list.push_back(values[rand_num]);
					std::iter_swap(values.begin() + rand_num, values.end() - 1);
					values.pop_back();
				}
			}
		}
		return random_list;
	}
	guid_64 person_infor_ws_mgr::get_one_relation_person(guid_64 role_guid)
	{
		guid_64 random_guid;
		std::vector<guid_64> values;
		for (std::map<guid_64, s_role_person_information>::iterator iter = person_info_map.begin(); iter != person_info_map.end(); ++iter)
		{
			if (iter->first == role_guid)
			{
				continue;
			}
			//已结婚的不考虑
			if (nullptr != marry_mgr_ws::get_instance().find_marry_record_info(role_guid))
			{
				continue;
			}
			values.push_back(iter->first);
		}
		if (values.size() == 0)
		{
			return random_guid;
		}
		int32 rand_num = random_gen::get_random(0, values.size() - 1);
		random_guid = values[rand_num];
		return random_guid;
	}
	
	void person_infor_ws_mgr::remove_person_info(guid_64 role_guid)
	{
		if (person_info_map.find(role_guid) != person_info_map.end())
		{
			//有数据
			person_info_map.erase(role_guid);
		}
	}

	void person_infor_ws_mgr::send_lucky_info_by_type(const guid_64& role_guid, e_lucky_info_req_type type)
	{
		client_session* session = client_session_mgr::getInstance().get_session(role_guid);
		if (nullptr == session)
			return;
		switch (type)
		{
		case e_lucky_info_req_type_vec:
		{
			const auto& tmpVec = get_relation_person(role_guid);
			if (tmpVec.empty())
				return;
			hld::relation_proto_s2c_ret_lucky_info_vec msg;
			for (auto iter = tmpVec.begin(); iter != tmpVec.end(); ++iter)
			{
				client_session* tmp_session = client_session_mgr::getInstance().get_session(*iter);
				if (nullptr != tmp_session && tmp_session->get_role_guid() != role_guid)
				{
					auto *item = msg.add_user_info();
					if (nullptr == item)
						break;
					if (msg.ByteSize() > packege_length_max)
						break;
					auto tmp_iter = person_info_map.find(*iter);
					if (tmp_iter == person_info_map.end())
						continue;
					item->set_role_guid_a(tmp_session->get_role_guid().A);
					item->set_role_guid_b(tmp_session->get_role_guid().B);
					item->set_role_name(tmp_session->m_role_info.role_name);
					item->set_role_vip(tmp_session->m_role_info.data_ary[e_role_info_vip_level]);
					item->set_role_template(tmp_session->m_role_info.data_ary[e_role_info_template_id]);
					item->set_role_level(tmp_session->m_role_info.data_ary[e_role_info_exp_level]);
					item->set_role_fight(init_unit::change_i64_to_string(tmp_session->get_role_gs_value()));
					item->set_role_time(tmp_iter->second.online_state);
					item->set_role_desc(tmp_iter->second.personalized_signature);
				}
			}
			session->send_to_client(&msg, e_msgindex_s2c_ret_lucky_info_vec);
			return;
		}
		break;
		case e_lucky_info_req_type_special_info:
		{
			client_session* tmp_session = client_session_mgr::getInstance().get_session(get_one_relation_person(role_guid));
			if (nullptr != tmp_session && tmp_session->get_role_guid() != role_guid)
			{
				hld::relation_proto_s2c_ret_special_lucky_info msg;
				msg.set_role_guid_a(tmp_session->get_role_guid().A);
				msg.set_role_guid_b(tmp_session->get_role_guid().B);
				msg.set_role_name(tmp_session->m_role_info.role_name);
				msg.set_role_template(tmp_session->m_role_info.data_ary[e_role_info_template_id]);
				session->send_to_client(&msg, e_mgsindex_s2c_ret_special_lucky_info);
				//策划说这个提示不要了
				//session->send_notice("90096796");
				return;
			}
			session->send_notice("90096795");
			hld::ws2cs_return_special_item_times ret_msg;
			ret_msg.target_guid = role_guid;
			bool is_use = proto_by_lua(e_msgindex_ws2cs_return_special_item_times);
			if (is_use == false)
			{
				session->send_to_cs(&ret_msg, sizeof(ws2cs_return_special_item_times));
			}
			else
			{
				hld::ws2cs_proto::return_special_item_times pro_msg;
				ret_msg.to_proto(pro_msg);
				session->send_to_cs_lua(&pro_msg, e_msgindex_ws2cs_return_special_item_times);
			}
			return;
		}
		break;
		default:
			break;
		}
	}
}
