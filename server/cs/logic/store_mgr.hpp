 /********************************************************************
created: 2016年7月4日13:56:28
file base: store_mgr
file ext: hpp
author: zhangminghai

purpose: about store system's store_mgr
*********************************************************************/
#pragma once

#include "goods.h"
#include "Logic/goods_def.hpp"

namespace faith
{
	class goods_proto_update_store_goods;
	typedef	std::vector<int32>    fresh_vec;//刷新数组
	typedef std::map<int32, fresh_vec>  fresh_map;//商店刷新map

	class store_mgr
	{
	public:
		store_mgr() { m_server_opentime = 0; };
		~store_mgr(void) {};
	public:
		static store_mgr& get_instance()
		{
			static store_mgr s_store_mgr;
			return s_store_mgr;
		}
	public:
		void							init_manager();	//初始化管理
		void							heart_tick(const int64& new_time);
		void							load_good_from_db();
		void							save_goods_to_db(int32 save_type);
		bool							is_need_update_record(int32 player_last_update, int32 store_id);
		int32							get_last_update_time(int32 store_id);
		int32							get_goods_id_by_store_id_and_item_id(int32 item_id, int32 store_id);
		int32							get_server_open_time() { return m_server_opentime; }
		int32							get_time_stamp(int32 y, int32 m, int32 d, int32 h, int32 min, int32 s);
		void							init_all_store_time();
	public:
		fresh_map						m_timed_fresh_map;
		fresh_map						m_interval_fresh_map;
	private:
		std::map<int32, int32>			m_store_interval;
		int32							m_server_opentime;
		std::vector<int32>				m_last_fresh_time;
		float							update_interval = 0.0f;

	};


	struct s_store_goods
	{
		int32 store_id;
		cgoods goods_data_array[goods_max_num];
		bool is_valid() 
		{ 
			return store_id > 0;
		}
		s_store_goods()
		{
			clear_data();
		}
		void clear_data()
		{
			memset(this, 0, sizeof(*this));
		}
		void add_goods(const s_goods_info& goods_info)
		{
			for (int32 i = 0; i < goods_max_num; ++i)
			{
				cgoods& goods_ref = goods_data_array[i];
				if (goods_ref.is_data_use() == false)
				{
					goods_ref.set_goods_data(goods_info);
					goods_ref.set_data_use(true);
					break;
				}
			}
		}
		bool remove_goods(const int32& goods_id)
		{
			for (int32 i = 0; i < goods_max_num; ++i)
			{
				cgoods& goods_ref = goods_data_array[i];
				if (goods_ref.get_goods_data_by_index(e_goods_info_id) == goods_id)
				{
					goods_ref.clear_data();
					return true;
				}
			}
			return false;
		}
		cgoods* get_goods_by_id(const int32& goods_id)
		{
			if (goods_id == 0)
			{
				return nullptr;
			}
			for (int32 i = 0; i < goods_max_num; ++i)
			{
				cgoods& goods_ref = goods_data_array[i];
				if (goods_ref.get_goods_data_by_index(e_goods_info_id) == goods_id)
				{
					return &goods_ref;
				}
			}
			return nullptr;
		}
		cgoods* get_goods_by_idex(const int32& idex)
		{
			if (idex>=goods_max_num||idex<0)
			{
				return nullptr;
			}
			cgoods& goods_ref = goods_data_array[idex];
			return &goods_ref;
		}
		/////////////////////////////////
		int32  get_store_id()
		{
			return store_id;
		}

	};
	class player_store_helper
	{
	public:
		player_store_helper() { m_unit_array_index = 0; clear_data(); }
		~player_store_helper() {}
		void							heart_tick(const int64& new_time);			//心跳管理
		void							set_player_ptr(const int32& unit_index) { m_unit_array_index = unit_index; }
		void							clear_data();
		s_store_goods*					get_empty_store_goods();
		s_store_goods*					get_store_goods_by_id(const int32& store_id);
		cgoods*							get_goods_by_id(const int32& store_id, const int32& goods_id);
	//	void							update_goods_list();					//更新商品列表
	//	void							update_goods_info(int32 goods_id);		//更新商品信息
		bool                            can_buy(int32 goods_id, int32 store_id);
		int32							can_buy_num(int32 goods_id,int32 store_id );			//// 0表示是不可购买 ; <0 表示可无限购买 ;>0 表示实际可购买的数量
		void							add_buy_info(int32 goods_id, int32 goods_num, int32 store_id);		//	添加购买记录
		std::vector<int32>				get_cost_of_goods(int32 goods_id, int32 goods_num);	//获取购买需要的金额
		void							create_record_by_inst(const s_goods_info& goods_info);
		cgoods							create_goods_by_template(int32 goods_id, int32 store_id);
		void							load_purchase_record_from_db(const s_goods_info* goods_data, int32 goods_num);
		bool                            load_purchase_record_by_db_lua(const char *data_ptr, int32 data_len);
		void							save_purchase_record_to_db(int32 save_type);
		void							update_record(bool is_send, int32 update_type);		// 刷新
		void							send_store_goods_record_all();
		void							send_server_open_time();
		void							remove_record_by_goods_id(int32 goods_id, int32 store_id);
		void							get_goods_msg(goods_proto_update_store_goods& goods_msg, s_store_goods& store_ptr);
		int32							buy_goods(int32 goods_id, int32 goods_num, int32 store_id, bool is_auto_buy);
		bool							check_is_store_open_time(int32 store_id,int32 goods_id);
		bool							check_ladder_store_can_buy(int32 store_id);
		bool							check_is_goods_open_time(int32 goods_id);
		int32							GetTimeSecond(int32 day, int32 hour, int32 min, int32 sec);
		int32							get_goods_buy_count(int32 goods_id, int32 store_id = -1);
	private:

		s_store_goods					m_purchase_record[e_store_type_max];
		int32							m_unit_array_index;
	public:
		s_store_goods*                  get_s_store_goods_by_index(int32 index);
		bool                            is_use_lua();


		
	};
}