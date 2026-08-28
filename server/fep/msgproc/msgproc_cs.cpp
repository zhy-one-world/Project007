/********************************************************************
	created:	2014/06/05
	created:	5:6:2014   11:31
	file base:	msgproc_cs
	file ext:	cpp
	author:		zhy
	
	purpose:	
*********************************************************************/
#include "msgproc_cs.hpp"
#include "../server/client_session.hpp"
#include "../server/security_communication_layer.hpp"
#include "../server/fep_client.hpp"
#include "../server/proxy_service_cli.hpp"
#include <core.hpp>
#include <Utility/serialize_msg.h>
#include <time.hpp>

namespace hld
{
	void cs2fep_aoi_msg_func(uint32 connindex, const void *data_ptr, size_t data_len)
	{
		const cs2fep_aoi_msg* pdata = static_cast<const cs2fep_aoi_msg*>(data_ptr);
		if (NULL == pdata)
			return;
		for (int32 i = 0; i < pdata->data_num; ++i)
		{
			client_session* client_session_ptr = proxy_service_cli::getInstance().get_session_by_id(pdata->client_uid[i].fepsession_uid);
			if (nullptr == client_session_ptr || client_session_ptr->is_vaild() == false || client_session_ptr->get_client_uid() != pdata->client_uid[i])
			{
				continue;
			}
			security_communication_layer::getInstance().send_to_session(client_session_ptr->get_conn_index(), pdata->header, pdata->data, pdata->data_size);
		}
	}
	void cs2fep_in_game_msg_func(uint32 connindex, const void *data_ptr, size_t data_len)
	{
		const cs2fep_in_game* pdata = static_cast<const cs2fep_in_game*>(data_ptr);
		if (NULL == pdata)
			return;
		fep2cs_in_game msg;
		memcpy(msg.account, pdata->account, max_account_length);
		msg.array_index = pdata->array_index;
		msg.role_guid = pdata->role_guid;
		client_session* client_session_ptr = proxy_service_cli::getInstance().get_session_by_id(pdata->client_uid.fepsession_uid);
		if (nullptr == client_session_ptr)
		{
			msg.be_in_game = false;
			fep_client::getInstance().send_message_to_cs(&msg, sizeof(msg), connindex);
			return;
		}
		if (strcmp(pdata->account, client_session_ptr->get_account()) != 0)
		{
			msg.be_in_game = false;
			fep_client::getInstance().send_message_to_cs(&msg, sizeof(msg), connindex);
			return;
		}
		msg.be_in_game = true;
		fep_client::getInstance().send_message_to_cs(&msg, sizeof(msg), connindex);
	}

	void cs2fep_month_recharge_msg_func(uint32 connindex, const void *data_ptr, size_t data_len)
	{
		const cs2fep_month_recharge_num* pdata = static_cast<const cs2fep_month_recharge_num*>(data_ptr);
		if (NULL == pdata)
			return;
		client_session* client_session_ptr = proxy_service_cli::getInstance().get_session_by_id(pdata->client_uid.fepsession_uid);
		if (nullptr == client_session_ptr)
		{
			return;
		}
		if(client_session_ptr->get_age() < 18)
		{
			int32 old_month_recharge_num = client_session_ptr->get_month_recharge_num();
			client_session_ptr->set_month_recharge_num(old_month_recharge_num + pdata->recharge_num);
			client_session_ptr->set_month_recharge_time(time_helper::get_time());
		}
	}

}
