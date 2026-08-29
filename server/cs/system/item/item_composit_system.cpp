#include "utility/init_unit.h"
#include "internet/item.pb.h"
#include "item_composit_system.h"
#include "item_system.h"
#include "logic/player.hpp"
#include "server_log.hpp"
#include "template/template_manager.h"
#include "utility/random.h"
#include "internet/net.pb.h"

using namespace faith;

void item_composit_system::composit_operate_message(player* player_ptr, const item_proto_item_composit& msg)
{
	ZoneScoped;
	auto composit_id = msg.composit_template_id();
	auto composit_num = msg.composit_num();
	auto composit_template = GET_TEMPLATE(CompositTemplate, composit_id);
	if (composit_template == nullptr)
	{
		CONSOLE_ERROR("composit_template is null composit_id:{}", composit_id);
		return;
	}
	if (composit_template->MaterialList.empty())
	{
		CONSOLE_ERROR("MaterialList is null composit_id:{}", composit_id);
		return;
	}
	if (composit_template->CostMoney.empty())
	{
		CONSOLE_ERROR("CostMoney is null composit_id:{}", composit_id);
		return;
	}
	if (composit_template->Rate <= 0)
	{
		CONSOLE_ERROR("Rate is null composit_id:{} Rate:{}", composit_id, composit_template->Rate);
		return;
	}
	auto create_item_template = GET_TEMPLATE(ItemTemplate, composit_template->CompositItemTemplateID);
	if (create_item_template == nullptr)
	{
		CONSOLE_ERROR("create_item_template is null composit_id:{} create_item_id:{}", composit_id, composit_template->CompositItemTemplateID);
		return;
	}
	for (int32 i = 0; i < composit_template->MaterialList.size(); i += 2)
	{
		auto need_item_id = composit_template->MaterialList[i];
		auto need_item_num = composit_template->MaterialList[i + 1];
		auto need_item_template = GET_TEMPLATE(ItemTemplate, need_item_id);
		if (need_item_template == nullptr)
		{
			CONSOLE_ERROR("need_item_template is null composit_id:{} need_item_id:{}", composit_id, need_item_id);
			return;
		}
		e_bag_type bag_type = init_unit::get_item_bag_type(need_item_id);
		if (bag_type == e_bag_type_none)
		{
			CONSOLE_ERROR("bag_type is null composit_id:{} need_item_id:{}", composit_id, need_item_id);
			return;
		}
		if (item_system::can_cost_item(player_ptr, bag_type, need_item_id, need_item_num) == false)
		{
			CONSOLE_ERROR("use_item_count is not enough composit_id:{} need_item_id:{} need_item_num:{}", composit_id, need_item_id, need_item_num);
			return;
		}
	}
	for (int32 i = 0; i < composit_template->CostMoney.size(); i += 2)
	{
		auto need_money_id = composit_template->CostMoney[i];
		auto need_money_num = composit_template->CostMoney[i + 1];
		if (!player_ptr->can_cut_money(e_money_type(need_money_id), need_money_num))
		{
			CONSOLE_ERROR("money is not enough composit_id:{} need_money_id:{} need_money_num:{}", composit_id, need_money_id, need_money_num);
			return;
		}
	}
	for (int32 i = 0; i < composit_template->MaterialList.size(); i += 2)
	{
		auto need_item_id = composit_template->MaterialList[i];
		auto need_item_num = composit_template->MaterialList[i + 1];
		auto need_item_template = GET_TEMPLATE(ItemTemplate, need_item_id);
		if (need_item_template == nullptr)
		{
			CONSOLE_ERROR("need_item_template is null composit_id:{} need_item_id:{}", composit_id, need_item_id);
			return;
		}
		e_bag_type bag_type = init_unit::get_item_bag_type(need_item_id);
		if (bag_type == e_bag_type_none)
		{
			CONSOLE_ERROR("bag_type is null composit_id:{} need_item_id:{}", composit_id, need_item_id);
			return;
		}
		item_system::cost_item_from_bag(player_ptr, bag_type, need_item_id, need_item_num);
	}
	for (int32 i = 0; i < composit_template->CostMoney.size(); i += 2)
	{
		auto need_money_id = composit_template->CostMoney[i];
		auto need_money_num = composit_template->CostMoney[i + 1];
		player_ptr->cut_money(e_money_type(need_money_id), need_money_num, e_server_log_cut_money_composit, composit_id);
	}
	auto success_field = composit_template->Rate * 100.f;
	auto random_num = random_gen::get_random(1, 100);
	if (success_field < random_num)
	{
		CONSOLE_INFO("composit fail composit_id:{} success_field:{} random_num:{}", composit_id, success_field, random_num);
		return;
	}
	auto create_item_ent = item_system::create_item(player_ptr, composit_template->CompositItemTemplateID);
	item_system::put_item_into_bag(player_ptr, create_item_ent);
	item_system::send_item_one(player_ptr, { create_item_ent });
	item_system::send_promp_msg_to_client(player_ptr, { composit_template->CompositItemTemplateID, 1 });

	item_proto_item_composit_end send_msg;
	send_msg.set_composit_item_template_id(composit_id);
	send_msg.add_role_guid(player_ptr->get_unit_guid().A);
	send_msg.add_role_guid(player_ptr->get_unit_guid().B);
	player_ptr->send_message_to_self(&send_msg, e_msgindex_s2c_item_composit);
}