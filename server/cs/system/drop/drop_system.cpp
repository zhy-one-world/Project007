#include "drop_def.hpp"
#include "drop_system.h"
#include "server_log.hpp"
#include "template/template_manager.h"
#include "utility/random.h"
#include <rlog.hpp>

using namespace faith;

std::vector<s_item_template_info> drop_system::get_drop_list(int32 drop_id)
{
	ZoneScoped;
	std::vector<s_item_template_info> res;
	if (drop_id <= 0)
	{
		CONSOLE_ERROR("drop_id:{} is nullprt", drop_id);
		return res;
	}
	drop_system::gen_drop_id_list(drop_id, res, DROP_MAX_WEIGHT, 0);
	return res;
}

void drop_system::gen_drop_id_list(int32 drop_id, std::vector<s_item_template_info>& drop_list, int32 drop_weight, int32 curstep)
{
	ZoneScoped;
	curstep++;
	if (curstep > 10)
	{
		CONSOLE_ERROR("curstep > 10 drop_id:{}", drop_id);
		return; //强制保护,因为掉落包可以套掉落包,填错了就会递归包含,最终导致死循环,所以调用深度超过10,则强制返回;
	}

	if (drop_list.size() > 100)
	{
		CONSOLE_ERROR("drop_id:{} is nullprt", drop_id);
		return;//一个包掉落了超过100个物品? 强制退出,很可能是出问题了;
	}

	bool rand_ret = rand_trigger(drop_weight, DROP_MAX_WEIGHT);
	if (false == rand_ret)
	{
		return;
	}

	DropTemplate* line_ptr = GET_TEMPLATE(DropTemplate, drop_id);
	if (nullptr == line_ptr)
	{
		CONSOLE_ERROR("drop_id:{} is nullprt", drop_id);
		return;
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
		CONSOLE_ERROR("size_equ is error drop_id:{} size_id:{} size_idtype:{} size_count:{} size_bound:{} size_weight:{}", drop_id, size_id, size_idtype, size_count, size_bound, size_weight);
		return;//表填错了,列的数量对不上
	}

	//新掉落逻辑
	switch (drop_type)
	{
	case e_dropbox_ran_type_all:
		sub_drop_rand_all(line_ptr->attribute_id, drop_list, curstep);
		break;
	case e_dropbox_ran_type_one:
		sub_drop_rand_one(line_ptr->attribute_id, drop_list, curstep);
		break;
	default:
		CONSOLE_ERROR("drop_id:{}  drop_type:{}", drop_id, drop_type);
		break;
	}
}

bool drop_system::rand_trigger(int32 drop_weight, int32 drop_denominator)
{
	ZoneScoped;
	int32 random = random_gen::get_random(1, drop_denominator);

	if (random <= drop_weight)
	{
		return true;
	}

	return false;
}


void drop_system::sub_drop_rand_all(int32 drop_id, std::vector<s_item_template_info>& drop_list, int32 curstep)
{
	ZoneScoped;
	DropTemplate* line_ptr = GET_TEMPLATE(DropTemplate, drop_id);
	if (nullptr == line_ptr)
	{
		CONSOLE_ERROR("drop_id:{} is nullprt", drop_id);
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
		CONSOLE_ERROR("size_equ is error drop_id:{} size_id:{} size_idtype:{} size_count:{} size_bound:{} size_weight:{}", drop_id, size_id, size_idtype, size_count, size_bound, size_weight);
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
			CONSOLE_ERROR("size_equ is error drop_id:{} size_id:{} i:{}", drop_id, size_id, i);
			continue;
		}

		if (e_dropbox_typ_drop_box == temp_type)//掉落盒里的物品是另一个掉落盒，再随机一遍……
		{
			for (int32 ii = 0; ii < temp_count; ii++)
			{
				gen_drop_id_list(temp_id, drop_list, temp_weight, curstep);
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
					drop_list.push_back(s_item_template_info(temp_id, 1, temp_bound));
		        }
		    }
		}
		else if (e_dropbox_typ_money == temp_type)//掉落盒里是物品，就加入掉落列表
		{
			bool rand_temp = rand_trigger(temp_weight, DROP_MAX_WEIGHT);
			if (true == rand_temp)
			{
				drop_list.push_back(s_item_template_info(temp_id, temp_count));
			}
		}
		else 
		{
			CONSOLE_ERROR("size_equ is error drop_id:{} size_id:{} i:{} temp_type:{}", drop_id, size_id, i, temp_type);
		    continue; //非法值, 继续下一物品~
		}
	}

}

void drop_system::add_item_id_to_drop_list(int32 item_template_id, std::vector<s_item_template_info>& drop_list)
{
	ZoneScoped;
	auto item_template = GET_TEMPLATE(ItemTemplate, item_template_id);
	if (item_template == nullptr)
	{
		CONSOLE_ERROR("item_template_id:{} is nullprt", item_template_id);
		return;
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
	drop_list.push_back(s_item_template_info(item_template_id, 1));
}
void drop_system::sub_drop_rand_one(int32 drop_id, std::vector<s_item_template_info>& drop_list, int32 curstep)
{
	ZoneScoped;
	DropTemplate* line_ptr = GET_TEMPLATE(DropTemplate, drop_id);
	if (nullptr == line_ptr)
	{
		CONSOLE_ERROR("drop_id:{} is nullprt", drop_id);
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
		CONSOLE_ERROR("size_equ is error drop_id:{} size_id:{} size_idtype:{} size_count:{} size_bound:{} size_weight:{}", drop_id, size_id, size_idtype, size_count, size_bound, size_weight);
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
		_RLOG_(MINFO, ::faith::log_detail::format_message("random empty item. drop_id:{}, choosed_item_index:{}, total_weight:{} random_value:{} cur_weight:{}",  drop_id,  choosed_item_index,  total_weight,  random_value,  cur_weight));
		return;
	}

	if (choosed_item_index >= size_id)
	{
		CONSOLE_ERROR("size_equ is error drop_id:{} choosed_item_index:{} size_id:{}", drop_id, choosed_item_index, size_id);
		return;//随到的物品下标越界了
	}

	int32 cur_id_typ = line_ptr->IDTypes[choosed_item_index];
	if (cur_id_typ == e_dropbox_typ_non)
	{
		CONSOLE_ERROR("size_equ is error drop_id:{} choosed_item_index:{}  cur_id_typ:{} ", drop_id, choosed_item_index, cur_id_typ);
		return;//0可能就是表有问题……
	}

	int32 real_get_id = line_ptr->IDs[choosed_item_index];
	int32 item_lock_state = line_ptr->Bounds[choosed_item_index];
	if (real_get_id == 0)//物品ID不可能是0
	{
		CONSOLE_ERROR("real_get_id is error drop_id:{} choosed_item_index:{} real_get_id:{} ", drop_id, choosed_item_index, real_get_id);
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
			gen_drop_id_list(real_get_id, drop_list, DROP_MAX_WEIGHT, curstep);
		}
		//因为此时已经确认该物品包必然掉落，所以递归时直接传入满权重
	}
	else if (e_dropbox_typ_item == cur_id_typ)//掉落盒里是物品，就加入掉落列表
	{
		for (int32 cur_item_num = 0; cur_item_num < total_item_num; cur_item_num++)
		{
			drop_list.push_back({ real_get_id, 1, item_lock_state });
		}
	}
	else if (e_dropbox_typ_money == cur_id_typ)//掉落盒里是物品，就加入掉落列表
	{
		drop_list.push_back({ real_get_id, total_item_num });
	}
}