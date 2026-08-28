/********************************************************************
created: 2016/04/13
created: 14:7:2014 19:40
file base: drop
file ext: cpp
author: Steven.Han

purpose:
*********************************************************************/

#include "logic/drop.h"
#include "template/template_manager.h"
#include "server_log.hpp"
#include "utility/random.h"

namespace hld
{
	int32 cdrop::get_drop_id(int32 drop_template_id)
	{
		ZoneScoped;
		int32 return_value = 0;
		DropTemplate* line_ptr = GET_TEMPLATE(DropTemplate, drop_template_id);
		if (nullptr == line_ptr)
		{
			CONSOLE_ERROR("drop_template_id:{} is nullprt", drop_template_id);
			return return_value;
		}
		int32 size_id = line_ptr->IDs.size();
		int32 size_idtype = line_ptr->IDTypes.size();
		int32 size_count = line_ptr->Counts.size();
		int32 size_bound = line_ptr->Bounds.size();
		int32 size_weight = line_ptr->Weights.size();

		bool size_equ = (size_id == size_idtype) && (size_id * 2 == size_count) && (size_id == size_bound) && (size_id == size_weight);
		if (false == size_equ)
		{
			return return_value;//表填错了,列的数量对不上
		}

		int32 choosed_drop_box_index = -1;
		int32 total_weight = 0;
		for (int32 i = 0; i < size_weight; i++)
		{

			total_weight += line_ptr->Weights[i];
		}

		if (total_weight < DROP_MAX_WEIGHT)
		{
			total_weight = DROP_MAX_WEIGHT;
		}

		//确定随到第几个
		//此时的随机上限已经做过处理，在实际总权重值不足“DROP_MAX_WEIGHT”时，已经将其设置为“DROP_MAX_WEIGHT”，所以可能出现随不到的情况
		int32 random_value = random_gen::get_random(0, total_weight);
		int32 cur_weight = 0;
		for (int32 i = 0; i < size_weight; i++)
		{
			cur_weight += line_ptr->Weights[i];
			if (cur_weight >= random_value)
			{
				choosed_drop_box_index = i;
				break;
			}
		}

		if (choosed_drop_box_index < 0)
		{
			//没随到，啥都不干
			return return_value;
		}
		else
		{
			if (choosed_drop_box_index >= size_id)
			{
				return return_value;//随到的包下标越界了
			}
			return_value = line_ptr->IDs[choosed_drop_box_index];
		}
		return return_value;
	}

	void cdrop::gen_drop_list_by_drop_id(int32 drop_template_id, std::vector<s_item_template_info>& drop_items_list_ref, int32 class_type, int32 exp_level)
	{
		ZoneScoped;
		if (drop_template_id <= 0)
		{
			CONSOLE_ERROR("drop_template_id:{} is nullprt", drop_template_id);
			return;
		}
		std::vector<s_item_template_info> item_drop_list_att;

		cdrop::gen_drop_id_list(drop_template_id, item_drop_list_att, DROP_MAX_WEIGHT, 0, class_type, exp_level);
		if (item_drop_list_att.empty() == false)
		{
			cdrop::merge_drop_bag(item_drop_list_att, drop_items_list_ref, class_type, exp_level);
		}
	}

	bool cdrop::meet_level(DropTemplate* drop_ptr, int32 owner_level)
	{
		ZoneScoped;
		if (nullptr == drop_ptr)
		{
			return false;
		}

		if (2 != drop_ptr->DropLevelLimit.size()) {
			return true;
		}
		int32 drop_box_level_limit_min = drop_ptr->DropLevelLimit[0];;
		int32 drop_box_level_limit_max = drop_ptr->DropLevelLimit[1];
		if (drop_box_level_limit_min <= owner_level && owner_level <= drop_box_level_limit_max)
		{
			return true;
		}
		return false;
	}

	std::vector<s_item_template_info> cdrop::mission_drop(MissionTemplate* mission_ptr, player* player_ptr)
	{
		ZoneScoped;
		std::vector<s_item_template_info> temp_list_with_num;

		std::vector<s_item_template_info> item_drop_list_with_att;

		gen_drop_id_list(mission_ptr->FinishItemDropId, item_drop_list_with_att, DROP_MAX_WEIGHT, 0, player_ptr->get_unit_info(e_role_info_class_type), player_ptr->get_unit_info(e_role_info_exp_level));
		merge_drop_bag(item_drop_list_with_att, temp_list_with_num, player_ptr->get_unit_info(e_role_info_class_type));
		return temp_list_with_num;
	}

	void cdrop::gen_drop_id_list(int32 drop_template_id, std::vector<s_item_template_info>& item_drop_with_att, int32 drop_weight, int32 curstep, int32 class_type, int32 exp_level)
	{
		ZoneScoped;
		curstep++;
		if (curstep > 10)
		{
			CONSOLE_ERROR("drop_template_id:{} is nullprt", drop_template_id);
			return; //强制保护,因为掉落包可以套掉落包,填错了就会递归包含,最终导致死循环,所以调用深度超过10,则强制返回;
		}

		if (item_drop_with_att.size() > 100)
		{
			CONSOLE_ERROR("drop_template_id:{} is nullprt", drop_template_id);
			return;//一个包掉落了超过100个物品? 强制退出,很可能是出问题了;
		}

		bool rand_ret = rand_trigger(drop_weight, DROP_MAX_WEIGHT);
		if (false == rand_ret)
		{
			return;
		}

		DropTemplate* line_ptr = GET_TEMPLATE(DropTemplate, drop_template_id);
		if (nullptr == line_ptr)
		{
			CONSOLE_ERROR("drop_template_id:{} is nullprt", drop_template_id);
			return;
		}

		if (line_ptr->LevelChangeId.size() > 0 && line_ptr->LevelChangeId.size() % 3 == 0 && exp_level >= 0)
		{
			std::vector<int32> level_change_id_arr = line_ptr->LevelChangeId;
			for (int32 i = 0; i < level_change_id_arr.size() / 3; i++)
			{
				if (level_change_id_arr[i * 3] <= exp_level && level_change_id_arr[i * 3 + 1] >= exp_level)
				{
					line_ptr = GET_TEMPLATE(DropTemplate, level_change_id_arr[i * 3 + 2]);
					break;
				}
			}
			if (nullptr == line_ptr)
			{
				return;
			}
		}

		//职业限制
		if (line_ptr->ClassType > hld::e_class_type_none && class_type > hld::e_class_type_none)
		{
			if (class_type != line_ptr->ClassType)
			{
				return;
			}
		}

		int32 size_id = line_ptr->IDs.size();
		int32 drop_type = line_ptr->DropType; //0为逐个掉落，可能掉落多个物品，每个物品的掉落概率=权重/1000000 1为归一掉落，圆桌理论，最多只会掉落一个物品，所有id总权重小于1000000时，可能出现不掉落物品的情况
		int32 size_idtype = line_ptr->IDTypes.size();
		int32 size_count = line_ptr->Counts.size();
		int32 size_bound = line_ptr->Bounds.size();
		int32 size_weight = line_ptr->Weights.size();

		bool size_equ = (size_id == size_idtype) && (size_id * 2 == size_count) && (size_id == size_bound) && (size_id == size_weight);
		if (false == size_equ)
		{
			CONSOLE_ERROR("size_equ is error drop_template_id:{} size_id:{} size_idtype:{} size_count:{} size_bound:{} size_weight:{}", drop_template_id, size_id, size_idtype, size_count, size_bound, size_weight);
			return;//表填错了,列的数量对不上
		}

		//sub_drop_rand_one(drop_template_id, item_id_list_out, curstep);
		//测试用

		//新掉落逻辑
		switch (drop_type)
		{
		case e_dropbox_ran_type_all:
			sub_drop_rand_all(line_ptr->attribute_id, item_drop_with_att, curstep, class_type);
			break;
		case e_dropbox_ran_type_one:
			sub_drop_rand_one(line_ptr->attribute_id, item_drop_with_att, curstep, class_type);
			break;
		default:
			CONSOLE_ERROR("drop_template_id:{}  drop_type:{}", drop_template_id, drop_type);
			break;
		}
	}

	void cdrop::merge_drop_bag(const std::vector<s_item_template_info>& drop_list_in, std::vector<s_item_template_info>& drop_list_out, int32 class_type, int32 exp_level)
	{
		ZoneScoped;
		for (auto& in_it : drop_list_in)
		{
			auto item_template = GET_TEMPLATE(ItemTemplate, in_it.m_item_id);
			if (item_template == nullptr)
			{
				CONSOLE_ERROR("item_template_id:{} is nullprt", in_it.m_item_id);
				return;
			}
			if (class_type != e_class_type_none)
			{
				if (item_template->class_type.empty() == false)
				{
					bool in_class = false;
					for (int32 i = 0; i < item_template->class_type.size(); ++i)
					{
						if (item_template->class_type[i] == class_type || e_class_type_none == item_template->class_type[i])
						{
							in_class = true;
							break;
						}
					}
					if (in_class == false)
					{
						return;
					}
				}
			}
			bool is_add = false;
			for (auto& out_it : drop_list_out)
			{
				if (in_it.m_item_id == out_it.m_item_id)
				{
					if (out_it.m_item_num < item_template->max_pile_num)
					{
						out_it.m_item_num++;
						is_add = true;
						break;
					}
				}
			}
			if (is_add == false)
			{
				drop_list_out.push_back(in_it);
			}
		}
	}

	bool cdrop::rand_trigger(int32 drop_weight, int32 drop_denominator)
	{
		ZoneScoped;
		int32 random = random_gen::get_random(1, drop_denominator);

		if (random <= drop_weight)
		{
			return true;
		}

		return false;
	}


	void cdrop::sub_drop_rand_all(int32 drop_template_id, std::vector<s_item_template_info>& item_drop_with_att, int32 curstep, int32 class_type, int32 exp_level)
	{
		ZoneScoped;
		DropTemplate* line_ptr = GET_TEMPLATE(DropTemplate, drop_template_id);
		if (nullptr == line_ptr)
		{
			CONSOLE_ERROR("drop_template_id:{} is nullprt", drop_template_id);
			return;
		}

		int32 size_id = line_ptr->IDs.size();
		int32 size_idtype = line_ptr->IDTypes.size();
		int32 size_count = line_ptr->Counts.size();
		int32 size_bound = line_ptr->Bounds.size();
		int32 size_weight = line_ptr->Weights.size();
		int32 size_const_att = line_ptr->ConstAtt.size();

		bool size_equ = (size_id == size_idtype) && (size_id * 2 == size_count) && (size_id == size_bound) && (size_id == size_weight);
		if (false == size_equ)
		{
			CONSOLE_ERROR("size_equ is error drop_template_id:{} size_id:{} size_idtype:{} size_count:{} size_bound:{} size_weight:{}", drop_template_id, size_id, size_idtype, size_count, size_bound, size_weight);
			return;//表填错了,列的数量对不上
		}

		for (int32 i = 0; i < size_id; i++)
		{
			int32 temp_id = line_ptr->IDs[i];
			int32 temp_type = line_ptr->IDTypes[i];
			int32 temp_count = 0;
			int32 temp_bound = line_ptr->Bounds[i];
			int32 temp_weight = line_ptr->Weights[i];

			int32 temp_count1 = line_ptr->Counts[2*i];
			int32 temp_count2 = line_ptr->Counts[2*i+1];

			int32 temp_const_att = 0;
			if (size_const_att > i)
			{
				temp_const_att = line_ptr->ConstAtt[i];
			}
		    if (temp_count2 > temp_count1)
		    {
		        temp_count = random_gen::get_random(temp_count1, temp_count2);
		    }
		    else
		    {
		        temp_count = random_gen::get_random(temp_count2, temp_count1);
		    }


			if (0 == temp_id) //物品ID不可能是0
			{
				CONSOLE_ERROR("size_equ is error drop_template_id:{} size_id:{} i:{}", drop_template_id, size_id, i);
				continue;
			}

		    if (e_dropbox_typ_drop_box == temp_type)//掉落盒里的物品是另一个掉落盒，再随机一遍……
		    {
				for (int32 ii = 0; ii < temp_count; ii++)
				{
					gen_drop_id_list(temp_id, item_drop_with_att, temp_weight, curstep, class_type, exp_level);
					//此时还不确定这个包掉不掉，所以要传真实权重做判断
				}
		    }
		    else if(e_dropbox_typ_item == temp_type )//掉落盒里是物品，就加入掉落列表
		    {
		        bool rand_temp = rand_trigger(temp_weight, DROP_MAX_WEIGHT);
				//bool rand_temp = rand_trigger(temp_weight, temp_weight+1);
		        if (true == rand_temp)
		        {
		            for (int32 ii=0; ii < temp_count; ii++)
		            {
						item_drop_with_att.push_back(s_item_template_info(temp_id, 1, temp_bound));
		            }
		        }
		    }
			else if (e_dropbox_typ_money == temp_type)//掉落盒里是物品，就加入掉落列表
			{
				bool rand_temp = rand_trigger(temp_weight, DROP_MAX_WEIGHT);
				if (true == rand_temp)
				{
					item_drop_with_att.push_back(s_item_template_info(temp_id, temp_count));
				}
			}
		    else 
			{
				CONSOLE_ERROR("size_equ is error drop_template_id:{} size_id:{} i:{} temp_type:{}", drop_template_id, size_id, i, temp_type);
		        continue; //非法值, 继续下一物品~
		    }
		}

	}

	void cdrop::add_item_id_to_drop_list(int32 item_template_id, std::vector<s_item_template_info>& drop_list, int32 item_const_att, int32 class_type, int32 exp_level)
	{
		ZoneScoped;
		auto item_template = GET_TEMPLATE(ItemTemplate, item_template_id);
		if (item_template == nullptr)
		{
			CONSOLE_ERROR("item_template_id:{} is nullprt", item_template_id);
			return;
		}
		if (class_type != e_class_type_none)
		{
			if (item_template->class_type.empty() == false)
			{
				bool in_class = false;
				for (int32 i = 0; i < item_template->class_type.size(); ++i)
				{
					if (item_template->class_type[i] == class_type || e_class_type_none == item_template->class_type[i])
					{
						in_class = true;
						break;
					}
				}
				if (in_class == false)
				{
					return;
				}
			}
		}
		for (int32 list_index = 0; list_index < drop_list.size();list_index++)
		{
			if (item_template_id == drop_list[list_index].m_item_id)
			{
				if (drop_list[list_index].m_item_num < item_template->max_pile_num)
				{
					drop_list[list_index].m_item_num++;
					return;
				}
				else
				{
					//当前item的位置堆满了，还要看之后的物品，有没有空余的堆叠数
				}
			}
		}
		drop_list.push_back(s_item_template_info(item_template_id, 1, 0));
	}
	void cdrop::sub_drop_rand_one(int32 drop_template_id, std::vector<s_item_template_info>& item_drop_with_att, int32 curstep, int32 class_type, int32 exp_level)
	{
		ZoneScoped;
		DropTemplate* line_ptr = GET_TEMPLATE(DropTemplate, drop_template_id);
		if (nullptr == line_ptr)
		{
			CONSOLE_ERROR("drop_template_id:{} is nullprt", drop_template_id);
			return;
		}

		int32 size_id = line_ptr->IDs.size();
		int32 size_idtype = line_ptr->IDTypes.size();
		int32 size_count = line_ptr->Counts.size();
		int32 size_bound = line_ptr->Bounds.size();
		int32 size_weight = line_ptr->Weights.size();

		bool size_equ = (size_id == size_idtype) && (size_id * 2 == size_count) && (size_id == size_bound) && (size_id == size_weight);
		if (false == size_equ)
		{
			CONSOLE_ERROR("size_equ is error drop_template_id:{} size_id:{} size_idtype:{} size_count:{} size_bound:{} size_weight:{}", drop_template_id, size_id, size_idtype, size_count, size_bound, size_weight);
			return;//表填错了,列的数量对不上
		}

		int32 choosed_item_index = -1;
		int32 total_weight = 0;
		for (int32 i = 0; i < size_weight; i++)
		{
			total_weight += line_ptr->Weights[i];
		}

		if (total_weight < DROP_MAX_WEIGHT)
		{
			total_weight = DROP_MAX_WEIGHT;
		}

		//确定随到第几个
		//此时的随机上限已经做过处理，在实际总权重值不足“DROP_MAX_WEIGHT”时，已经将其设置为“DROP_MAX_WEIGHT”，所以可能出现随不到的情况
		int32 random_value = random_gen::get_random(0, total_weight);
		int32 cur_weight = 0;
		for (int32 i = 0; i < size_weight; i++)
		{
			cur_weight += line_ptr->Weights[i];
			if (cur_weight>= random_value)
			{
				choosed_item_index = i;
				break;
			}
		}

		if (choosed_item_index < 0)
		{
			//没随到，啥都不干
			CONSOLE_INFO("random empty item. drop_template_id:{}, choosed_item_index:{}, total_weight:{} random_value:{} cur_weight:{}", drop_template_id, choosed_item_index, total_weight, random_value, cur_weight);
			return;
		}

		if (choosed_item_index >= size_id)
		{
			CONSOLE_ERROR("size_equ is error drop_template_id:{} choosed_item_index:{} size_id:{}", drop_template_id, choosed_item_index, size_id);
			return;//随到的物品下标越界了
		}

		int32 cur_id_typ = line_ptr->IDTypes[choosed_item_index];
		if (cur_id_typ == e_dropbox_typ_non)
		{
			CONSOLE_ERROR("size_equ is error drop_template_id:{} choosed_item_index:{}  cur_id_typ:{} ", drop_template_id, choosed_item_index, cur_id_typ);
			return;//0可能就是表有问题……
		}

		int32 real_get_id = line_ptr->IDs[choosed_item_index];
		int32 item_lock_state = line_ptr->Bounds[choosed_item_index];
		if (real_get_id == 0)//物品ID不可能是0
		{
			CONSOLE_ERROR("real_get_id is error drop_template_id:{} choosed_item_index:{} real_get_id:{} ", drop_template_id, choosed_item_index, real_get_id);
			return;
		}

		//确认生成几个物品
		int32 total_item_num = 0;
		int32 temp_count1 = line_ptr->Counts[2 * choosed_item_index];
		int32 temp_count2 = line_ptr->Counts[2 * choosed_item_index + 1];

		int32 temp_const_att = 0;
		if (line_ptr->ConstAtt.size() > choosed_item_index)
		{
			temp_const_att = line_ptr->ConstAtt[choosed_item_index];
		}
		if (temp_count2 > temp_count1)
		{
			total_item_num = random_gen::get_random(temp_count1, temp_count2);
		}
		else
		{
			total_item_num = random_gen::get_random(temp_count2, temp_count1);
		}

		if (e_dropbox_typ_drop_box == cur_id_typ)//掉落盒里的物品是另一个掉落盒，再随机一遍……
		{
			for (int32 cur_item_num = 0; cur_item_num < total_item_num; cur_item_num++)
			{
				gen_drop_id_list(real_get_id, item_drop_with_att, DROP_MAX_WEIGHT, curstep, class_type, exp_level);
			}
			//因为此时已经确认该物品包必然掉落，所以递归时直接传入满权重
		}
		else if (e_dropbox_typ_item == cur_id_typ)//掉落盒里是物品，就加入掉落列表
		{
			for (int32 cur_item_num = 0; cur_item_num < total_item_num; cur_item_num++)
			{
				item_drop_with_att.push_back({ real_get_id, 1, item_lock_state });
			}
		}
		else if (e_dropbox_typ_money == cur_id_typ)//掉落盒里是物品，就加入掉落列表
		{
			item_drop_with_att.push_back({ real_get_id, total_item_num });
		}
	}

}