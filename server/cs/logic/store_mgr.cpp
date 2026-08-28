/********************************************************************
created: 2016年7月4日13:56:28
file base: store_mgr
file ext: hpp
author: zhangminghai

purpose: about store system's store_mgr
*********************************************************************/

#include "internet/cs2dp.pb.h"
#include "internal/goods_msg.hpp"
#include "logic/item_set.h"
#include "logic/player.hpp"
#include "logic/unit_man.h"
#include "lua/script_mgr.h"
#include "server_log.hpp"
#include "store_mgr.hpp"
#include "system/item/item_system.h"
#include "template/StringConst_S.h"
#include "template/template_manager.h"
#include "time_def.hpp"
#include "utility/cs_date.hpp"
#include <Utility/parse_msg.h>
#include "internet/goods.pb.h"
#include "utility/init_unit.h"


namespace hld
{
	void store_mgr::init_manager()
	{
		init_all_store_time();
	}


	void store_mgr::heart_tick(const int64& new_time)
	{
	}

	void store_mgr::load_good_from_db()
	{
		
	}

	int32 store_mgr::get_goods_id_by_store_id_and_item_id(int32 item_id,int32 store_id)
	{
		int32 goods_id = 0;
		StoreTemplate* temp_store_ptr = template_manager::get_instance().get_store_template_by_id(store_id);
		if (temp_store_ptr != nullptr)
		{
			for (int32 i = 0; i < temp_store_ptr->StoreGoods.size(); i++)
			{
				GoodsTemplate* temp_goods_ptr = GET_TEMPLATE(GoodsTemplate, temp_store_ptr->StoreGoods[i]);
				if (temp_goods_ptr != nullptr)
				{
					if (item_id == temp_goods_ptr->ItemId)
					{
						goods_id = temp_store_ptr->StoreGoods[i];
					}
				}
				temp_goods_ptr = nullptr;
			}
		}
		return goods_id;

	}

	
	bool store_mgr::is_need_update_record(int32 player_last_update, int32 store_id) 
	{
		StoreTemplate* temp_store_ptr = template_manager::get_instance().get_store_template_by_id(store_id);
		if (temp_store_ptr == nullptr)
		{
			return false;
		}
		if (temp_store_ptr->FreshType == e_store_update_null)
		{//不刷新
			return false;
		}
		if (player_last_update < m_last_fresh_time[store_id])
		{
			return true;
		}
		return false;
	}

	int32	store_mgr::get_time_stamp(int32 y, int32 m, int32 d, int32 h, int32 min, int32 s)
	{
		time_t now_time;
		time(&now_time);
		tm temp_time = *localtime(&now_time);
		temp_time.tm_year = y - 1900;    //设置年
		temp_time.tm_mon = m - 1;     //设置月
		temp_time.tm_mday = d;    //设置日
		temp_time.tm_hour = h;    //设置小时
		temp_time.tm_min = min;     //设置分钟
		temp_time.tm_sec = s;     //设置秒	
		return mktime(&temp_time);
	}

	void store_mgr::init_all_store_time()
	{
		//记开服的当天0:00:00
		tm open_time;
		time_t now_time;
		time(&now_time);
		open_time = *localtime(&now_time);
		open_time.tm_sec = 0;  //修改时间
		open_time.tm_min = 0;
		open_time.tm_hour = 0;
		//记录开服时间
		m_server_opentime = mktime(&open_time);
		m_timed_fresh_map.clear();
		m_interval_fresh_map.clear();
		m_last_fresh_time.clear();
		//所有商店的初始刷新时间
		for (int32 i = 0; i < e_store_type_max; ++i)
		{
			m_last_fresh_time.push_back(m_server_opentime);

			//读表,是否需要记录刷新时间

			StoreTemplate* temp_store_ptr = template_manager::get_instance().get_store_template_by_id(i);
			if (temp_store_ptr != nullptr)
			{
				//常驻商店 定时刷新
				if (temp_store_ptr->OpenType == e_store_open_type_foever && temp_store_ptr->FreshTime.size() > 0)
				{
					fresh_vec fresh_times;
					for (int32 j = 0; j < temp_store_ptr->FreshTime.size(); ++j)
					{
						fresh_times.push_back(temp_store_ptr->FreshTime[j]);
					}
					m_timed_fresh_map.insert({ i,fresh_times });
					continue;
				}
				//随开服计算时间
				if (temp_store_ptr->OpenType == e_store_open_type_server)
				{
					if (temp_store_ptr->StoreOpenTimeLimit.size() == 5)
					{
						open_time.tm_year += temp_store_ptr->StoreOpenTimeLimit[0];
						open_time.tm_mon += temp_store_ptr->StoreOpenTimeLimit[1];
						open_time.tm_mday += temp_store_ptr->StoreOpenTimeLimit[2];
						open_time.tm_hour += temp_store_ptr->StoreOpenTimeLimit[3];
						open_time.tm_min += temp_store_ptr->StoreOpenTimeLimit[4];
					}

					int32 store_open_time = mktime(&open_time);
					fresh_vec fresh_times;
					fresh_times.push_back(store_open_time);//添加开启时间
					fresh_times.push_back(temp_store_ptr->FreshInterval);//添加刷新间隔
					m_interval_fresh_map.insert({ i,fresh_times });
				}
				//现实时间计算
				if (temp_store_ptr->OpenType == e_store_open_type_time)
				{
					if (temp_store_ptr->StoreOpenTimeLimit.size() == 5)
					{

						int32 store_open_time = get_time_stamp(
							temp_store_ptr->StoreOpenTimeLimit[0],
							temp_store_ptr->StoreOpenTimeLimit[1],
							temp_store_ptr->StoreOpenTimeLimit[2],
							temp_store_ptr->StoreOpenTimeLimit[3],
							temp_store_ptr->StoreOpenTimeLimit[4], 0);
						fresh_vec fresh_times;
						fresh_times.push_back(store_open_time);//添加开启时间
						fresh_times.push_back(temp_store_ptr->FreshInterval);//添加刷新间隔
						m_interval_fresh_map.insert({ i,fresh_times });
					}
				}
			}
		}
	}
	int32 store_mgr::get_last_update_time(int32 store_id)
	{
		if (0 <= store_id && store_id < e_store_type_max)
		{
			return m_last_fresh_time[store_id];
		}
		return 0;
	}




	//======================== 下面是玩家商城信息管理 ===========================================================================
	void player_store_helper::heart_tick(const int64& new_time)
	{

	}
	void player_store_helper::clear_data()
	{
		memset(m_purchase_record, 0, sizeof(m_purchase_record));
	}
	s_store_goods* player_store_helper::get_empty_store_goods()
	{
		for (int32 i = 0; i < e_store_type_max; ++i)
		{
			s_store_goods & store_goods_ref = m_purchase_record[i];
			if (store_goods_ref.is_valid() == false)
			{
				return &store_goods_ref;
			}
		}
		return nullptr;
	}
	s_store_goods* player_store_helper::get_store_goods_by_id(const int32& store_id)
	{
		if (0 == store_id)
		{
			return nullptr;
		}
		for (int32 i = 0; i < e_store_type_max; ++i)
		{
			s_store_goods & store_goods_ref = m_purchase_record[i];
			if (store_goods_ref.store_id == store_id)
			{
				return &store_goods_ref;
			}
		}
		return nullptr;
	}
	cgoods* player_store_helper::get_goods_by_id(const int32& store_id, const int32& goods_id)
	{
		s_store_goods* store_goods_ptr = get_store_goods_by_id(store_id);
		if (nullptr == store_goods_ptr)
		{
			return nullptr;
		}
		return store_goods_ptr->get_goods_by_id(goods_id);
	}
	void	player_store_helper::create_record_by_inst(const s_goods_info& goods_info)
	{
		int32 goods_id = goods_info.data_ary[e_goods_info_id];
		if (goods_id == 0)
		{
			return;
		}

		StoreTemplate* store_template_ptr = template_manager::get_instance().get_store_template_by_id(goods_info.data_ary[e_goods_info_owner_store]);
		if (nullptr == store_template_ptr)
		{
			return;
		}

		if (std::count(store_template_ptr->StoreGoods.begin(), store_template_ptr->StoreGoods.end(), goods_info.data_ary[e_goods_info_id]) <= 0)
		{
			return;
		}
		s_store_goods* store_goods_ptr = get_store_goods_by_id(goods_info.data_ary[e_goods_info_owner_store]);
		if (store_goods_ptr)
		{
			store_goods_ptr->add_goods(goods_info);
		}
		else
		{
			s_store_goods* empty_store_goods_ptr = get_empty_store_goods();
			if (empty_store_goods_ptr)
			{
				empty_store_goods_ptr->store_id = goods_info.data_ary[e_goods_info_owner_store];
				empty_store_goods_ptr->add_goods(goods_info);
			}
		}			
	}

	void player_store_helper::load_purchase_record_from_db(const s_goods_info* goods_data, int32 goods_num)
	{
		for (int32 i = 0; i < goods_num; i++)
		{
			create_record_by_inst(goods_data[i]);		
		}

		send_server_open_time();
		send_store_goods_record_all();
	}
	bool player_store_helper::load_purchase_record_by_db_lua(const char *data_ptr, int32 data_len)
	{
		if (data_ptr == nullptr || !data_len)
		{
			return false;
		}
		hld::cs2dp_proto::role_goods_db msg;
		bool is_sucess = parse_msg::getInstance().parse_buffer_to_proto(&msg, data_ptr, data_len);
		if (!is_sucess)
		{
			return false;
		}

		xchar *p_data = parse_msg::getInstance().get_cache(msg.row_count() * sizeof(s_goods_info));
		if (p_data == nullptr)
		{
			return false;
		}
		s_goods_info *p_row = (s_goods_info *)p_data;
		for (int32 i = 0; i < msg.row_count(); i++)
		{
			hld::cs2dp_proto::role_goods_row db_row = msg.row_data(i);
			for (int32 j = 0; j < db_row.data_ary_size(); j++)
			{
				p_row->data_ary[j] = db_row.data_ary(j);
			}
			p_row++;
		}
		load_purchase_record_from_db((const s_goods_info *)p_data, msg.row_count());
		return true;
	}


	void  player_store_helper::send_server_open_time()
	{
		script_mgr::get_instance().call_func("store_mgr", "send_server_open_time_cpp", 0, false, "%d", m_unit_array_index);
		/*player& player_ref = unit_man::get_player(m_unit_array_index);
		goods_proto_goods_get_server_opent_time msg;
		msg.set_server_open_time(store_mgr::get_instance().get_server_open_time());
		player_ref.send_message_to_self(&msg, e_msgindex_s2c_server_open_time);*/
	}

	void player_store_helper::send_store_goods_record_all()
	{
		script_mgr::get_instance().call_func("store_mgr", "send_store_goods_record_all_cpp", 0, false, "%d", m_unit_array_index);
		/*player& player_ref = unit_man::get_player(m_unit_array_index);
		goods_proto_update_store_goods update_msg;
		for (int32 i = 0; i < e_store_type_max; ++i)
		{
			s_store_goods & store_goods_ref = m_purchase_record[i];
			if (store_goods_ref.is_valid())
			{
				get_goods_msg(update_msg, store_goods_ref);
			}
		}
		player_ref.send_message_to_self(&update_msg, e_msgindex_s2c_update_store);*/
	}

	void player_store_helper::get_goods_msg(goods_proto_update_store_goods& goods_msg, s_store_goods& store_ptr)
	{
		goods_proto_store_goods* store_msg_ptr = goods_msg.add_store_list();
		store_msg_ptr->set_store_id(store_ptr.store_id);
		for (int32 i = 0; i < goods_max_num; i++)
		{
			cgoods& goods_ref = store_ptr.goods_data_array[i];
			if (goods_ref.is_data_use())
			{
				goods_proto_goods_info* goods_info = store_msg_ptr->add_goods_list();
				goods_info->add_goods_info(goods_ref.get_goods_data_by_index(e_goods_info_id));
				goods_info->add_goods_info(store_ptr.store_id);
				goods_info->add_goods_info(goods_ref.get_goods_data_by_index(e_goods_info_purchased_num));
				goods_info->add_goods_info(0);
			}
		}
	}

	void player_store_helper::update_record(bool is_send, int32 update_type)
	{
		bool  not_modify_record = false;
		for (int32 i = 0; i < e_store_type_max; ++i)
		{
			s_store_goods & store_goods_ref = m_purchase_record[i];
			if (store_goods_ref.is_valid() == false)
			{
				continue;
			}
			for (int32 j = 0; j < goods_max_num; ++j)
			{//是否是永久限购
				cgoods& goods_ref = store_goods_ref.goods_data_array[j];
				if (goods_ref.is_data_use() == false)
				{
					continue;
				}
				GoodsTemplate* goods_template_ptr = goods_ref.get_goods_template_ptr();
				if (nullptr == goods_template_ptr)
				{
					continue;
				}
				
				if (goods_template_ptr->LimitType == update_type)
				{
					goods_ref.set_goods_data_by_index(e_goods_info_purchased_num, 0);
					not_modify_record = true;
				}
			}
		}
		if (not_modify_record && is_send)
		{
			send_store_goods_record_all();
		}
		
	}

	void player_store_helper::remove_record_by_goods_id(int32 goods_id,int32 store_id)
	{
		for (int32 i = 0; i < e_store_type_max; ++i)
		{
			s_store_goods & store_goods_ref = m_purchase_record[i];
			if (store_goods_ref.store_id != store_id)
			{
				continue;
			}
			if (store_goods_ref.remove_goods(goods_id))
			{
				break;
			}
		}
	}

	void player_store_helper::save_purchase_record_to_db(int32 save_type)
	{
		player& player_ref = unit_man::get_player(m_unit_array_index);
		bool is_use = template_manager::get_instance().is_message_use_lua(e_msgindex_cs2dp_save_char_goods);
		if (is_use == false)
		{
			cs2dp_save_char_goods req;
			req.save_type_ex = save_type;
			req.role_guid = player_ref.get_unit_guid();
			req.unit_array_index = player_ref.get_array_index();
			uint32 item_count = 0;

			for (int32 i = 0; i < e_store_type_max; ++i)
			{
				s_store_goods & store_goods_ref = m_purchase_record[i];
				if (store_goods_ref.is_valid())
				{
					for (int32 j = 0; j < goods_max_num; ++j)
					{
						cgoods& goods_ref = store_goods_ref.goods_data_array[j];
						if (goods_ref.is_data_use())
						{
							memcpy(&(req.data_list[item_count]), &(goods_ref.get_goods_inst()), sizeof(s_goods_info));
							++item_count;
						}
					}
				}
			}
			req.data_num = item_count;
			player_ref.send_message_to_dp(&req, req.get_pak_length());
		}
		else
		{
			hld::cs2dp_proto::save_role_goods msg;
			msg.set_role_guid(player_ref.get_unit_guid().server_64);
			msg.set_unit_array_index(m_unit_array_index);
			msg.set_save_type_ex(save_type);
			hld::cs2dp_proto::role_goods_db *db_data = msg.mutable_db_data();
			if (db_data == nullptr)
			{
				return ;
			}
			uint32 row_count = 0;

			for (int32 i = 0; i < e_store_type_max; ++i)
			{
				s_store_goods & store_goods_ref = m_purchase_record[i];
				if (store_goods_ref.is_valid() == false)
				{
					continue;
				}
				for (int32 j = 0; j < goods_max_num; ++j)
				{
					cgoods& goods_ref = store_goods_ref.goods_data_array[j];
					if (goods_ref.is_data_use())
					{
						const s_goods_info& tmp = goods_ref.get_goods_inst();
						hld::cs2dp_proto::role_goods_row *db_row = db_data->add_row_data();
						if (db_row == nullptr)
						{
							return;
						}
						for (int32 k = 0; k < e_goods_info_max; k++)
						{
							db_row->add_data_ary(tmp.data_ary[k]);
						}
						++row_count;
					}
				 }		
			  }
			db_data->set_row_count(row_count);
			if (row_count == 0)
			{
				db_data->add_row_data();
			}
			player_ref.send_message_to_dp_lua(&msg, e_msgindex_cs2dp_save_char_goods);
		}
	}

	std::vector<int32> player_store_helper::get_cost_of_goods(int32 goods_templateid,int32 goods_num)
	{
		std::vector<int32> price_ary;
		price_ary.clear();
		RegionTemplate* region_template_ptr = globle_data::get_instance().get_region_template_ptr();
		if (region_template_ptr == nullptr)
		{
			return price_ary;
		}
		int32 money_type_index = region_template_ptr->RegionCode * 2;
		int32 money_num_index = money_type_index + 1;
	
		GoodsTemplate* goods_ptr = GET_TEMPLATE(GoodsTemplate, goods_templateid);
		if (goods_ptr != nullptr
			&& goods_ptr->NewPrice.size() > money_num_index)
		{
			price_ary.push_back(goods_ptr->NewPrice[money_type_index]);
			int32 price = (int32)(goods_ptr->NewPrice[money_num_index]);
			price_ary.push_back(price * goods_num);
		}
		return price_ary;
	}

	bool player_store_helper::can_buy(int32 goods_id, int32 store_id)
	{
		//处理能否购买的逻辑
		if (check_is_store_open_time(store_id, goods_id) == false)
		{
			return false;
		}	
		return true;
	}
	bool  player_store_helper::check_ladder_store_can_buy(int32 store_id)
	{
		StoreTemplate* temp_store_ptr = template_manager::get_instance().get_store_template_by_id(store_id);
		if (temp_store_ptr == nullptr)
		{
			return false;
		}
		if (temp_store_ptr->StoreType == e_store_type_cross_ladder_store)
		{
			player& player_ref = unit_man::get_player(m_unit_array_index);
			int32 is_buy = player_ref.get_competition_mgr().get_competition_info().is_buy;
			int32 manual_lv = player_ref.get_competition_mgr().get_competition_info().level;
			if (is_buy == 0 || manual_lv < role_competition_max)
			{
				return false;
			}
		}
		return true;
	}

	int32	player_store_helper::can_buy_num(int32 goods_id,int32 store_id) 
	{// 0表示是不可购买 ; <0 表示可无限购买 ;>0 表示实际可购买的数量
		int32 cur_purchased_num = 0;
		cgoods* goods_ptr = get_goods_by_id(store_id, goods_id);
		if (goods_ptr != nullptr)
		{//是可能找不到的！压根没买过这个东西就是找不到的！
		 //并且之后不能直接用这个goods_ptr!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
			cur_purchased_num = goods_ptr->get_goods_data_by_index(e_goods_info_purchased_num);
		}
		int32 temp_can_buy_num = 0;

		//GoodsTemplate* goods_template_ptr = goods_ptr->get_goods_template_ptr();
		//不能用这个goods_ptr!!!!!!!!!!!可能是空的！！！！！！！！！！！！！！！

		GoodsTemplate* goods_template_ptr = GET_TEMPLATE(GoodsTemplate, goods_id);
		if (nullptr == goods_template_ptr)
		{
			return 0;
		}
		if (goods_template_ptr->LimitType == e_goods_purchase_limit_type_none)
		{
			temp_can_buy_num = -1;
		}		
		else
		{
			temp_can_buy_num = (goods_template_ptr->RoleMaxNum - cur_purchased_num) > 0 ? goods_template_ptr->RoleMaxNum - cur_purchased_num : 0;
		}		
		
		return temp_can_buy_num;
	}

	void player_store_helper::add_buy_info(int32 goods_id, int32 goods_num,int32 store_id)
	{
		cgoods* goods_ptr = get_goods_by_id(store_id, goods_id);
		int32 store_last_fresh_time = store_mgr::get_instance().get_last_update_time(store_id);
		if (nullptr == goods_ptr)
		{
			s_goods_info new_goods_record_info;
			new_goods_record_info.data_ary[e_goods_info_id] = goods_id;
			new_goods_record_info.data_ary[e_goods_info_owner_store] = store_id;
			new_goods_record_info.data_ary[e_goods_info_purchased_num] = goods_num;
			new_goods_record_info.data_ary[e_goods_info_last_update_time] = store_last_fresh_time;
			create_record_by_inst(new_goods_record_info);
			return;
		}
		int32 old_num = goods_ptr->get_goods_data_by_index(e_goods_info_purchased_num);
		goods_ptr->set_goods_data_by_index(e_goods_info_purchased_num, old_num + goods_num);
		goods_ptr->set_goods_data_by_index(e_goods_info_last_update_time, store_last_fresh_time);
	}

	int32 player_store_helper::buy_goods(int32 goods_id, int32 goods_num, int32 store_id, bool is_auto_buy)
	{
		//////////////////////////////////////////////////////////////////////////////////
		if (is_use_lua())
		{
			int32 i_ret = e_item_string_unkown;
			script_mgr::get_instance().call_func("store_mgr", "buy_goods", 1, false, "%d%d%d%d%b>%d", m_unit_array_index,goods_id,goods_num, store_id,is_auto_buy, &i_ret);
			return i_ret;
		}

		//////////////////////////////////////////////////////////////////////////////////

		player& player_ref = unit_man::get_player(m_unit_array_index);
		if (goods_num <= 0)
		{
			return e_buy_goods_end_goods_num_is_zero;
		}
		GoodsTemplate* temp_goods_ptr = GET_TEMPLATE(GoodsTemplate, goods_id);
		if (temp_goods_ptr == nullptr)
		{
			return e_item_string_unkown;
		}
		ItemTemplate* temp_item_ptrs = GET_TEMPLATE(ItemTemplate, temp_goods_ptr->ItemId);

		if (temp_item_ptrs == nullptr)
		{
			return e_item_string_unkown;
		}
		StoreTemplate* temp_store_ptr = template_manager::get_instance().get_store_template_by_id(store_id);
		if (temp_store_ptr == nullptr)
		{
			return e_item_string_unkown;
		}

		// 判断商品是否在商店中
		bool is_in_store = false;
		for (int32 i = 0; i < temp_store_ptr->StoreGoods.size(); i++)
		{
			if (temp_store_ptr->StoreGoods[i] == goods_id)
			{
				is_in_store = true;
				break;
			}
		}
		if (!is_in_store)
		{
			return e_item_string_unkown;
		}

		bool is_full_exc_att = false;
		if (temp_store_ptr->StoreType == e_store_type_hope_point_first
			|| temp_store_ptr->StoreType == e_store_type_hope_point_second
			|| temp_store_ptr->StoreType == e_store_type_hope_point_third
			|| temp_store_ptr->StoreType == e_store_type_magic_crystal_third)
		{
			is_full_exc_att = true;
		}

		//判断物品是否上架
		if (!check_is_goods_open_time(goods_id))
		{
			return e_buy_goods_end_in_time_limit;
		}
		// 判断玩家的等级是否达到了能够购买此商品的要求
		int32 player_level = player_ref.get_unit_info(e_role_info_exp_level);
		if (temp_goods_ptr->Levellimit.size() < 2)
		{
			return  e_item_string_level;
		}

		if (player_level < temp_goods_ptr->Levellimit[0] || player_level > temp_goods_ptr->Levellimit[1])
		{
			if (temp_item_ptrs->item_type == e_item_type_expendable && (temp_item_ptrs->sub_type== e_prop_sub_type_hp_min 
				|| temp_item_ptrs->sub_type == e_prop_sub_type_hp_mld || temp_item_ptrs->sub_type == e_prop_sub_type_hp_max
				|| temp_item_ptrs->sub_type == e_prop_sub_type_hp_shop || temp_item_ptrs->sub_type == e_prop_sub_type_hp_essence))
			{
				return e_item_string_not_belong_level;
			}
			else
			{
				return e_item_string_level;
			}
		}

		if (player_ref.get_vip_level() < temp_goods_ptr->NeedVIPLevel)
		{
			return e_item_string_vip_level;
		}

		//军团相关已经在ws上判断过，此处不再判断
		//// 如果购买改物品需要军团等级达到一定的等级才行,就在这里判断当前军团的等级是否足够
		//if (temp_goods_ptr->NeedLegionLevel > 0)
		//{
		//	guid_64 legion_guid = player_ref.get_legion_guid();
		//	if (false == legion_guid.is_valid())
		//	{
		//		return e_buy_goods_end_role_not_in_legion;
		//	}

		//	int32 legion_level = player_ref.get_player_legion_level();
		//	if (legion_level < temp_goods_ptr->NeedLegionLevel)
		//	{
		//		return e_buy_goods_end_legion_level_limit;
		//	}
		//}

		if (can_buy(goods_id, store_id) == false)
		{
			return e_item_string_unkown;
		}
		//能否购买
		int32 i_can_buy_num = can_buy_num(goods_id, store_id);
		if (i_can_buy_num == 0 || (i_can_buy_num != -1 && (i_can_buy_num - goods_num) < 0))
		{
			//达到购买上限
			return e_buy_goods_end_state_num_limit;
		}
		

		//背包格子数是否足够
		int32 need_slot_num = 0;
		int32 total_item_num = goods_num * temp_goods_ptr->GoodsNum;
		need_slot_num = total_item_num;
		ItemTemplate* temp_item_ptr = GET_TEMPLATE(ItemTemplate, temp_goods_ptr->ItemId);
		if (temp_item_ptr != nullptr)
		{
			if (temp_item_ptr->max_pile_num != 0)
			{
				need_slot_num = total_item_num / (temp_item_ptr->max_pile_num);
				if (total_item_num % temp_item_ptr->max_pile_num != 0)
				{
					need_slot_num += 1;
				}
			}
		}
		else
		{
			return e_item_string_unkown;
		}
		e_bag_type need_bag = init_unit::get_item_bag_type(temp_goods_ptr->ItemId);
		if (player_ref.get_item_set().is_empty_slot_enough(need_bag, need_slot_num) == false)
		{
			return e_buy_goods_end_state_bag;
		}

		if (temp_goods_ptr->NeedItemId.size() >= 2)
		{//需求物品
			if (item_system::can_cost_item(&player_ref, e_bag_type_bag, temp_goods_ptr->NeedItemId[0], temp_goods_ptr->NeedItemId[1] * goods_num) == false)
			{
				return e_buy_goods_end_need_item_not_enough;
			}
		}

		if (i_can_buy_num == -1 || (i_can_buy_num - goods_num) >= 0)
		{
			std::vector<int32> total_price = get_cost_of_goods(goods_id, goods_num);
			if (total_price.size() != 2)
			{
				return e_item_string_unkown;
			}

			//能否花钱
			if (player_ref.can_cut_money((e_money_type)total_price[0], total_price[1]) == false)
			{
				return e_buy_goods_end_state_money;
			}
			//添加物品

			citem* temp_item = player_ref.get_item_set().create_item_by_template(e_server_log_add_item_buy_goods, goods_id, temp_goods_ptr->ItemId, goods_num * temp_goods_ptr->GoodsNum, temp_goods_ptr->IsLock,  -temp_goods_ptr->EffectiveTime);
			if (temp_item == nullptr)
			{
				return e_item_string_unkown;
			}

			if (temp_item_ptr->item_type == e_item_type_spirit)
			{//精灵默认设置为1级
				temp_item->set_data_info(e_item_info_upgrade_count, 1);
			}

			if (temp_goods_ptr->NeedItemId.size() >= 2)
			{	//消耗物品
				item_system::cost_item_from_bag(&player_ref, e_bag_type_bag, temp_goods_ptr->NeedItemId[0], temp_goods_ptr->NeedItemId[1] * goods_num);
			}
			player_ref.get_item_set().put_in_bag(temp_item);

			//花钱
			player_ref.cut_money((e_money_type)total_price[0], total_price[1], e_server_log_cut_money_buy_goods, goods_id, goods_num);

			if (total_price[0] == e_money_type_jewel)
			{
				player_ref.get_welfare_mgr().set_active_degree_info(e_daily_active_degree_type_cost_one_diamond, 1);
			}

			//添加购买记录
			add_buy_info(goods_id, goods_num, store_id);
			send_store_goods_record_all();

			player_ref.get_mission_mgr().target_check(e_mission_end_type_purchase_goods, goods_id, goods_num);

			if (temp_store_ptr->StoreType == e_store_type_recommend || temp_store_ptr->StoreType == e_store_type_daily || temp_store_ptr->StoreType == e_store_type_expendables)
			{
				//记录钻石商店购买事件
				player_ref.get_time_limit_activity_mgr().activity_behavior_done(e_time_limit_behavior_type_buy_in_diamond_store);
			}

			if (player_ref.get_write_log())
			{
				server_log::buy_goods_role_log(player_ref.get_third_info(), player_ref.get_unit_info_inst(), temp_store_ptr->attribute_id, goods_id, goods_num, player_ref.get_login_type(), player_ref.get_time_data(e_time_type_login_time));
			}
			return 0;
			//这里成功就是返回 0，因为各个系统买、兑换东西都走这个逻辑，但是成功后的提示不同
			//所以返回0，每个界面自己做不同的成功提示
		}
		return e_item_string_unkown;
	}


	bool player_store_helper::check_is_store_open_time(int32 store_id, int32 goods_id)
	{
		//检查是是否商店是否开启
		StoreTemplate* temp_store_ptr = template_manager::get_instance().get_store_template_by_id(store_id);
		if (temp_store_ptr == nullptr )
		{
			return false;
		}
		int32 now_time = time_helper::get_cur_time_new().second;
		if (temp_store_ptr->OpenType != e_store_open_type_foever)
		{
			if (temp_store_ptr->StoreOpenTimeLimit.size() != 5)
			{
				return false;
			}
			int32 open_time = 0;
			if (temp_store_ptr->OpenType != e_store_open_type_time)
			{
				open_time = store_mgr::get_instance().get_time_stamp(temp_store_ptr->StoreOpenTimeLimit[0],
					temp_store_ptr->StoreOpenTimeLimit[1],
					temp_store_ptr->StoreOpenTimeLimit[2],
					temp_store_ptr->StoreOpenTimeLimit[3],
					temp_store_ptr->StoreOpenTimeLimit[4], 0);
			}
			else if(temp_store_ptr->OpenType != e_store_open_type_server)
			{
				open_time = store_mgr::get_instance().get_server_open_time() + temp_store_ptr->StoreOpenTimeLimit[3] * 3600 + temp_store_ptr->StoreOpenTimeLimit[4] * 60;
			}
			
			int32 passed_time = difftime(now_time, open_time);//从开启时间到现在的时间差
			if (passed_time < 0 || passed_time >= temp_store_ptr->LastTime)
			{
				return false;
			}			
		}

		//检查商品是否上架
		
		GoodsTemplate* temp_goods_ptr = GET_TEMPLATE(GoodsTemplate,goods_id );
		if (nullptr == temp_goods_ptr)
		{
			return false;
		}
		//存在上下架时间
		if (temp_goods_ptr->AddTime.size() == 5 && temp_goods_ptr->RemoveTime.size() == 5)
		{
			int32 on_sale_time = store_mgr::get_instance().get_time_stamp(temp_goods_ptr->AddTime[0],
				temp_goods_ptr->AddTime[1],
				temp_goods_ptr->AddTime[2],
				temp_goods_ptr->AddTime[3],
				temp_goods_ptr->AddTime[4], 0);//上架时间
			int32 off_sale_time = store_mgr::get_instance().get_time_stamp(temp_goods_ptr->RemoveTime[0],
				temp_goods_ptr->RemoveTime[1],
				temp_goods_ptr->RemoveTime[2],
				temp_goods_ptr->RemoveTime[3],
				temp_goods_ptr->RemoveTime[4], 0);//下架时间

			if (now_time < on_sale_time || now_time > off_sale_time)
			{
				return false;
			}
		}
		return true;
	}

	bool player_store_helper::check_is_goods_open_time(int32 goods_id)
	{
		GoodsTemplate* temp_goods_ptr = GET_TEMPLATE(GoodsTemplate, goods_id);
		if (nullptr == temp_goods_ptr)
		{
			return false;
		}
		if (temp_goods_ptr->TimelimitOnType == e_time_limit_goods_on_type_none)
		{
			return true;
		}
		if (temp_goods_ptr->TimelimitOnTime.size() != 5 || temp_goods_ptr->DurationTime.size() != 5)		//(时间:年,月,日,时,分)
		{
			return false;
		}
		int32 now_time = time_helper::get_cur_time_new().second;
		int32 open_time = 0;
		switch ((e_time_limit_goods_on_type)temp_goods_ptr->TimelimitOnType)
		{
		case e_time_limit_goods_on_type_none:
		{
			return true;
		}
		break;
		case e_time_limit_goods_on_type_open_server_days:
		{
			if (temp_goods_ptr->TimelimitOnTime.size() < 1)		//(开服第几天)
			{
				return false;
			}
			//活动开服当天0点的时间戳
			tm temp_time = globle_data::get_instance().get_server_open_tm();
			temp_time.tm_hour = 0;    
			temp_time.tm_min = 0;     
			temp_time.tm_sec = 0;     	
			
			open_time = mktime(&temp_time) + (temp_goods_ptr->TimelimitOnTime[2] * day_time_second) + 
				(temp_goods_ptr->TimelimitOnTime[3] * (hour_tick_time / 1000)) +
				(temp_goods_ptr->TimelimitOnTime[4] * (minute_tick_time / 1000));
		}
		break;
		case e_time_limit_goods_on_type_open_time:
		{
			open_time = store_mgr::get_instance().get_time_stamp(temp_goods_ptr->TimelimitOnTime[0],
				temp_goods_ptr->TimelimitOnTime[1],
				temp_goods_ptr->TimelimitOnTime[2],
				temp_goods_ptr->TimelimitOnTime[3],
				temp_goods_ptr->TimelimitOnTime[4], 0);
		}
		break;
		default:
			break;
		}
		int32 end_time = GetTimeSecond(temp_goods_ptr->DurationTime[2],
			temp_goods_ptr->DurationTime[3],
			temp_goods_ptr->DurationTime[4], 0);

		int32 passed_time = difftime(now_time, open_time);		//从开启时间到现在的时间差
		if (passed_time < 0 || passed_time >= end_time)	//判断是否超过上架时间
		{
			return false;
		}
		return true;
	}
	int32 player_store_helper::GetTimeSecond(int32 day, int32 hour, int32 min, int32 sec)
	{
		return ((day * (hld::day_time_second)) + (hour * 3600) + (min * 60) + sec);
	}

	int32 player_store_helper::get_goods_buy_count(int32 goods_id, int32 store_id)
	{
		int32 buy_num = 0;
		if (store_id < 0)
		{
			for (int32 i = 0; i < e_store_type_max; ++i)
			{
				cgoods* goods_ptr = get_goods_by_id(i, goods_id);
				if (goods_ptr != nullptr)
				{
					buy_num += goods_ptr->get_goods_data_by_index(e_goods_info_purchased_num);
				}
			}
		}
		else
		{
			cgoods* goods_ptr = get_goods_by_id(store_id, goods_id);
			if (goods_ptr != nullptr)
			{
				buy_num += goods_ptr->get_goods_data_by_index(e_goods_info_purchased_num);
			}
		}

		return buy_num;
	}

	s_store_goods* player_store_helper::get_s_store_goods_by_index(int32 index)
	{
		if (index < 0 || index >= e_store_type_max)
		{
			return nullptr;
		}
		return &m_purchase_record[index];
	}

	bool player_store_helper::is_use_lua()
	{
		//script_mgr::get_instance().call_func("reload", 0);
		bool bRet = false;
		script_mgr::get_instance().call_func("store_mgr", "is_use_lua", 1, false, ">%b", &bRet);
		return bRet;
	}
}
