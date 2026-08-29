#include "legion_ws_applicant_list.h"
#include "ws_client.hpp"
#include "Logic/time_def.hpp"
#include "../ws/server/legion/legion_ws_mgr.h"
#include <legion_msg.hpp>
#include "../cache_ws_mgr.hpp"


namespace faith
{
	legion_ws_applicant_list::legion_ws_applicant_list()
	{
		m_tick_time = 0.0f;
		memset(m_applicant_ptr_arr, 0, sizeof(m_applicant_ptr_arr));
	}

	legion_ws_applicant_list::~legion_ws_applicant_list()
	{
		for (int32 i = 0; i < max_legion_applicant_num; i++)
		{
			if (nullptr == m_applicant_ptr_arr[i])
			{
				continue;
			}
			cache_ws_mgr::get_instance().back_legion_ws_applicant_ptr(m_applicant_ptr_arr[i]);
		}
	}

	int32 legion_ws_applicant_list::get_empty_pos()
	{
		for (int32 i = 0; i < max_legion_applicant_num; ++i)
		{
			if (nullptr == m_applicant_ptr_arr[i] || m_applicant_ptr_arr[i]->is_valid() == false)
			{
				return i;
			}
		}
		return -1;
	}

	bool legion_ws_applicant_list::is_full()
	{
		return get_empty_pos() < 0 ? true : false;
	}

	bool legion_ws_applicant_list::exist_applicant(guid_64 applicant_guid)
	{
		for (int32 i = 0; i < max_legion_applicant_num; ++i)
		{
			if (nullptr != m_applicant_ptr_arr[i] && m_applicant_ptr_arr[i]->get_applicant_guid() == applicant_guid)
			{
				return true;
			}
		}

		return false;
	}

	bool legion_ws_applicant_list::can_add_member_applicant(guid_64 applicant_guid)
	{
		legion_ws* legion_ws_ptr = legion_ws_mgr::get_instance().get_unit_legion(applicant_guid);
		if (nullptr == legion_ws_ptr)
		{
			return true;
		}
		return false;
	}

	// ����ֵ˵��:
	//  1 ����ִ��
	// -1 ���������б�����
	// -2 ��������Ϣ�Ѿ��������б���
	e_add_applicant_ret legion_ws_applicant_list::add_applicant(legion_ws_applicant& applicant)
	{
		int32 empty_pos = get_empty_pos();
		if (empty_pos < 0)
		{
			return e_add_applicant_legion_full;
		}

		if (exist_applicant(applicant.get_applicant_guid()) == true)
		{
			return e_add_applicant_repeat;
		}

		if (false == can_add_member_applicant(applicant.get_applicant_guid()))
		{
			return e_add_applicant_have_legion;
		}
		legion_ws_applicant* legion_ws_applicant_ptr = cache_ws_mgr::get_instance().get_legion_ws_applicant_ptr();
		if (nullptr == legion_ws_applicant_ptr)
		{
			return e_add_applicant_legion_full;
		}
		legion_ws_applicant_ptr->set_legion_applicant_info(applicant.get_legion_applicant_info());
		m_applicant_ptr_arr[empty_pos] = legion_ws_applicant_ptr;

		return e_add_applicant_ok;
	}

	void legion_ws_applicant_list::save_to_db()
	{
		for (int32 i = 0; i < max_legion_applicant_num; i++)
		{
			if (nullptr == m_applicant_ptr_arr[i] || !m_applicant_ptr_arr[i]->is_valid())
			{
				break;
			}
			m_applicant_ptr_arr[i]->save_to_db();
			cache_ws_mgr::get_instance().back_legion_ws_applicant_ptr(m_applicant_ptr_arr[i]);
			m_applicant_ptr_arr[i] = nullptr;
		}
		
	}

	void legion_ws_applicant_list::del_applicant_to_dp(guid_64 role_guid)
	{
		//ws2dp_del_legion_applicant_one delete_applicant_info_msg;
		//delete_applicant_info_msg.role_guid = role_guid;
		//ws_client::getInstance().send_to_dp(&delete_applicant_info_msg, sizeof(delete_applicant_info_msg));

		script_mgr::get_instance().call_func("ws2dp_send", "send_delete_legion_applicant_lua", 0, false, "%l", role_guid.server_64);

	}

	void legion_ws_applicant_list::del_applicant(guid_64 applicant_guid)
	{
		for (int32 i = 0; i < max_legion_applicant_num; ++i)
		{
			if (m_applicant_ptr_arr[i] != nullptr && m_applicant_ptr_arr[i]->get_applicant_guid() == applicant_guid)
			{
				del_applicant_to_dp(applicant_guid);
				guid_64 legion_guid = m_applicant_ptr_arr[i]->get_legion_guid();
				cache_ws_mgr::get_instance().back_legion_ws_applicant_ptr(m_applicant_ptr_arr[i]);
				m_applicant_ptr_arr[i] = nullptr;

				legion_ws* legion_ws_ptr = legion_ws_mgr::get_instance().get_legion(legion_guid);
				if (nullptr != legion_ws_ptr)
				{
					legion_ws_ptr->send_applicant_info_list_to_have_job_member();
				}

				break;
			}
		}
	}

	int32 legion_ws_applicant_list::get_applicant_list_size()
	{
		int32 len = 0;
		for (int32 i = 0; i < max_legion_applicant_num; ++i)
		{
			if (m_applicant_ptr_arr[i] != nullptr && m_applicant_ptr_arr[i]->is_valid())
			{
				++len;
			}
		}

		return len;
	}

	legion_ws_applicant* legion_ws_applicant_list::get_applicant(guid_64 applicant_guid)
	{
		for (int32 i = 0; i < max_legion_applicant_num; ++i)
		{
			if (m_applicant_ptr_arr[i] != nullptr && m_applicant_ptr_arr[i]->get_applicant_guid() == applicant_guid)
			{
				return m_applicant_ptr_arr[i];
			}
		}

		return nullptr;
	}

	void legion_ws_applicant_list::tick(const uint64& tick_time)
	{
		// �����߼�¼48Сʱ�Żᱻ����,�ò�������tick,���۵�60��tickһ�ο����������
		if (tick_time > m_tick_time)
		{
			m_tick_time = tick_time + minute_tick_time;
			for (int32 i = 0; i < max_legion_applicant_num; ++i)
			{
				if (nullptr != m_applicant_ptr_arr[i] && m_applicant_ptr_arr[i]->is_valid())
				{
					//if (false == can_add_member_applicant(m_applicant_arr[i].get_applicant_guid()))
					//{
					//	del_applicant(m_applicant_arr[i].get_applicant_guid());
					//	continue;
					//}
					if (m_applicant_ptr_arr[i]->is_out_of_data(tick_time))
					{
						del_applicant(m_applicant_ptr_arr[i]->get_applicant_guid());
						continue;
					}
				}
			}
		}
	}
	
	// ����ֵ˵��:
	//  1 ����ִ��
	// -1 û��������(��������Ϊ��)
	// -2 ��������µ����������ݰ�ʧ��
	// -3 �����������Ϣ�����ݰ�ʧ��
	int32 legion_ws_applicant_list::fill_get_legion_applicant_list_msg(legion_proto_get_applicant_info_list_end& get_legion_applicant_list_end_msg)
	{
		// �Ȱ�Ҫ�������ݰ������������������Ϊ0,���ʾ������������Ĭ��Ϊʧ�ܵ�,ֻ�н��µ��������
		// ��������ȷ�˲Ż����ó���ȷ�ĳ���(�ͻ��˿��Ը����б��ĳ���ȷ������˵Ĵ���Ƿ���ȷ)
		get_legion_applicant_list_end_msg.set_list_len(0);

		int32 list_len = 0;
		for (int32 i = 0; i < max_legion_applicant_num; ++i)
		{
			if (nullptr == m_applicant_ptr_arr[i] || m_applicant_ptr_arr[i]->is_valid() == false)
			{
				continue;
			}
			legion_proto_applicant_info* applicant_info_msg = get_legion_applicant_list_end_msg.add_applicant_list();
			if (nullptr == applicant_info_msg)
			{
				return -2;
			}
			if (m_applicant_ptr_arr[i]->fill_legion_application_info_msg(*applicant_info_msg) == false)
			{
				return -3;
			}

			++list_len;
		}

		get_legion_applicant_list_end_msg.set_list_len(list_len);
		return 1;
	}

	void legion_ws_applicant_list::del_applicant_with_join_legion(guid_64 role_guid)
	{
		for (int32 i = 0; i < max_legion_applicant_num; ++i)
		{
			if (m_applicant_ptr_arr[i] != nullptr && m_applicant_ptr_arr[i]->is_valid())
			{
				if (m_applicant_ptr_arr[i]->get_applicant_guid() == role_guid)
				{
					del_applicant(role_guid);
				}
			}
		}
	}
}