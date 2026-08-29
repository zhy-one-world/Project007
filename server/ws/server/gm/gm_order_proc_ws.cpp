#include "gm_order_proc_ws.h"
#include "Logic/gm_order_def.hpp"
#include "template/template_manager.h"
#include "server/mail/mail_event_ws.h"

using namespace std;

namespace faith
{
	gm_order_proc_ws::gm_order_proc_ws()
	{
	
	}
	gm_order_proc_ws::~gm_order_proc_ws()
	{
	
	}
	
	void gm_order_proc_ws::proc_gm_order(e_gm_order_type order_type, const int32 *order_param, int32 param_num)
	{
		if (order_type >= egot_max)
		{
			return;
		}
		switch (order_type)
		{
		case egot_load_config:
		{
			template_manager::get_instance().init();
		}
		break;
		case egot_send_mail:
		{
			int32 server_id;
			guid_64 target_guid;
			std::vector<s_item_template_info> drop_list;
			int32 money_type1;
			int32 money_type2;
			int32 mail_count;
			if (param_num < 8)
			{
				return;
			}
			server_id = order_param[0];
			target_guid.A = order_param[1];
			target_guid.B = order_param[2];
			drop_list.push_back({ order_param[3] , 1});
			drop_list.push_back({ order_param[4] , 1});
			money_type1 = order_param[5];
			money_type2 = order_param[6];
			mail_count = order_param[7];
			for (int32 i = 0; i < mail_count; i++)
			{
				mail_ws_mgr::get_instance().send_mail_system(target_guid, server_id, drop_list, "title", "content", money_type1, 100, money_type2, 100);
			}
		}
		break;
		default:
			break;
		}
	}

}