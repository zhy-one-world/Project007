/********************************************************************
  created: 2014/07/16
  created: 16:7:2014 15:07
  file base: item_def
  file ext: hpp
  author: zhy
  
  purpose: 
*********************************************************************/
#ifndef _ITEM_DEF_HPP_
#define _ITEM_DEF_HPP_

#include "logic/type_def.hpp"
#include "char_def.hpp"
namespace faith
{
#pragma pack(push,1)

/************************************************************************/
/*	                    容器限制                                        */
/************************************************************************/


#define			All_MAX_ITEM_BAG_NUM					300												// 最大数量
#define			MAX_ITEM_BAG_NUM						500												// 背包最大数量
#define			MAX_ITEM_TREASURE_NUM					100												// 背包最大数量

#define			MAX_WING_NUM							200
#define			MAX_WING_FEATHER_NUM					5
#define			MAX_WING_SOUL_NUM						1
#define			MAX_WING_SPIRIT_NUM						1
#define			MAX_WING_ELEMENTS_NUM					(MAX_WING_NUM + MAX_WING_FEATHER_NUM + MAX_WING_SOUL_NUM + MAX_WING_SPIRIT_NUM)

#define			MAX_MOUNT_ILLUSION_NUM					200
#define			MAX_MOUNT_MOUNT_NUM						50
#define			MAX_MOUNT_NUM							MAX_MOUNT_ILLUSION_NUM+MAX_MOUNT_MOUNT_NUM

#define			MAX_TIME_LIMIT_ITEM_NUM					MAX_MOUNT_NUM + MAX_WING_ELEMENTS_NUM

#define			MAX_BEAST_SPIRIT_NUM				50
	const int32 send_item_max_num = 80;
	const int32 max_proctect_mark_equip_slot_num = 8;													// 守护印记装备栏上限
	const int32 max_proctect_mark_bag_slot_num = 36;													// 守护印记栏上限

	const int32 max_proctect_tinder_prop_slot_num = 10;													// 火种道具装备栏上限

	const int32 element_heart_bag_num = 200;															// 元素之心背包最大数量
	const int32 element_heart_equip_num = 10;															// 元素之心装备最大数量
	const int32 core_element_num = 4;																	// 核心符文最大数目

	const int32 belief_rune_bag_num = 200;                                                              // 信仰符文背包的最大数量
	const int32 belief_rune_equip_slot_each_type_num = 7;												// 每类信仰的装备槽数量
	const int32 belief_rune_equip_bag_num = e_belief_series_type_max * belief_rune_equip_slot_each_type_num; // 信仰符文装备的最大数量
	const int32 tinder_prop_bag_num = 10;																//火种道具背包数量

	const int32 sprite_bag_num = 200;																	//精灵背包数量

	const int32 max_hope_item_bag_size = 100;

	const int32 max_fasion_item_bag_size = 100;

	const int32 max_same_rune_equip_num = 3;

	const int32 max_item_advance_num = 3;

	const int32 special_mission_wing_id = 31040129;														//任务给的翅膀ID
	const int32 special_wing_buff_id = 10992004;														//特殊翅膀的显示BUFF ID

	const int32 max_item_storage_num = 100;
	const int32 template_storage_id = 33010000;

	const int32 max_auto_buy_medicine_num = 99;

	const int32 max_feather_heart_bag_num = 100;

	const int32 sky_equip_slot_num = 10;
	const int32 skygod_equip_slot_num = 10;
	const int32 supreme_equip_slot_num = 10;

	const int32 max_wedding_equip_num = 1;																//目前只有一个婚戒
	const int32 max_goddess_equip_num = 28;																//4个女神物品以及每个女神6件装备
	const int32 per_goddess_type_equip_num = 6;															//每个女神装备数目
	const int32	max_enchant_type_num = 100;																//附魔种类最大数量
	const int32 max_each_type_addition_buff_num = 50;													//每种额外buff类型的最大数量

	const int32 init_wedding_fashion = 31130490;														//初始化送的结婚时装
	const int32	heart_value_template_begin_id = 58102001;												//灵犀值称号其实id

	const int32 sky_equip_buff_id = 10992053;															//天空套装显示buffId
	const int32 skygod_equip_buff_id = 10992125;														//天神套装显示buffId
	const int32 supreme_equip_buff_id = 10992150;														//神王套装显示buffId
/************************************************************************/
/*		              功能限制                                          */
/************************************************************************/
#define			EQUIP_QUALITY_MAX						8						//普通装备升阶需要道具等级

#define			EQUIP_GRADE_MAX					    	10						//普通装备品阶最高值

#define         FEATHER_EQUIP_OPEN                      5                                               // 坐骑开启翎羽功能的等级
#define			ADD_SPIRIT_OPEN							5												// 注灵开启功能的等级
#define			ADD_SOUL_OPEN							5	

	const int32 gods_reel = 31020005;						//神佑神石
	const int32 must_success_props_item = 31000062;			//露西菲尔的祝福
	const int32 not_downgrade_props_item = 31000063;		//亘古神佑卷轴

	const int32 item_illusion_max_num = 10;																// 物品幻化数量上限
	const int32 item_illusion_byte_array[item_illusion_max_num] = { 1,2,4,8,16,32,64,128,256,512 };			// 物品幻化位标记数组

	const float item_tidy_interval = 3.0f; // 背包整理的间隔

	const int32 hope_and_hunt_record_num = 50; // 祈福和猎取记录存储数量
	const int32 draw_record_total_num = hope_and_hunt_record_num * 2; // 祈福和猎取各50

	const int32 min_add_on_notice = 6;								// 最小公告追加等级
	const int32 max_item_name_size = 40;							// 物品名最大字符数 拍卖行用
	const int32 difference_from_upgrade_star_id_and_item_logic_id = 1000;	//魂星与翅膀或坐骑logic id的差值
	const int32 difference_from_wing_psychic_id_and_item_logic_id = 2000;   //羽翼通灵与翅膀logic id的差值 

	const int32 init_mount_template_id = 56010000;					//第一个坐骑模板id

	const int32 max_item_num = MAX_ITEM_BAG_NUM + e_role_equip_slot_max + MAX_WING_ELEMENTS_NUM + MAX_MOUNT_NUM
		+ max_proctect_mark_bag_slot_num + max_proctect_mark_equip_slot_num + max_hope_item_bag_size + element_heart_bag_num
		+ element_heart_equip_num + max_fasion_item_bag_size+ e_fashion_equip_slot_max + MAX_ITEM_TREASURE_NUM + max_item_storage_num
		+ belief_rune_bag_num + belief_rune_equip_bag_num + sprite_bag_num + sky_equip_slot_num + skygod_equip_slot_num + max_goddess_equip_num + max_wedding_equip_num + core_element_num;

	const int32 role_show_item_num = e_role_equip_slot_max + e_fashion_equip_slot_max + 4; //4 是展示翅膀+展示翅膀的原区间id+展示坐骑+展示坐骑的原区间

	const int32 fake_item_num = role_show_item_num + element_heart_equip_num + belief_rune_equip_bag_num;//雕像需要的物品 装备+时装+符文+信仰符文

	const int32 bag_is_full_notice_id = 93000185;					//包已满清请清理背包公告id
	const int32 mail_with_item_notice_id = 93000186;				//奖励以邮件形式发送过去公告id
	const int32 mail_with_item_rune_notice_id = 93000253;			//符文奖励以邮件形式发送过去公告id
	const int32 add_buff_need_equip_num = 8;						//强化追加洗练产生效果的数量

	const int32	upgrade_promote_spirit_max = 50;				//星图精灵最大数量
	const int32	break_will_max = 100;					//星图精灵最大数量
	const int32	awaken_fetter_bag_max = 20;				//觉醒大师物品最大数量
	//物品整理用数组大小  ***必须大于所有背包格子数***

	//***************************************
	//*********必须大于所有背包格子数********
	//***************************************

	// 武器种类类型(武器模板数据的子类型)
	enum e_weap_sub_type
	{
		e_weap_sub_type_none,			// 徒手熟练度
		// 单手武器
		e_weap_sub_type_jian,			// 剑
		e_weap_sub_type_fu,				// 斧
		e_weap_sub_type_faqi,			// 法器
		e_weap_sub_type_duanzhang,		// 短杖
		// 双持武器
		e_weap_sub_type_shuangdao,		// 双刀
		e_weap_sub_type_shuanggun,		// 棍
		// 巨型武器
		e_weap_sub_type_jujian,			// 巨剑
		e_weap_sub_type_juchu,			// 巨杵
		// 轮刺长柄武器
		e_weap_sub_type_qianggun,		// 枪棍
		e_weap_sub_type_changzhang,		// 长杖
		// 挥砍长柄武器
		e_weap_sub_type_jiyue,			// 戟钺
		// 弓类武器
		e_weap_sub_type_gong,			// 弓
		e_weap_sub_type_gongjian,		// 箭
		// 单手武器
		e_weap_sub_type_dao,			// 刀
		// 弓类武器
		e_weap_sub_type_zhonggong,		// 重弓
		e_weap_sub_type_assistant,		// 盾牌		
		e_weap_sub_type_max,
	};

	enum e_item_lock_status
	{
		e_item_lock_status_unlocked,    // 非绑定
		e_item_lock_status_locked,      // 绑定
		e_item_lock_status_ignore,      // 不关心绑不绑定
		e_item_lock_status_max
	};

	enum e_item_get_hope_item_typ
	{
		e_item_get_hope_item_typ_one_free,
		e_item_get_hope_item_typ_one_voucher,
		e_item_get_hope_item_typ_ten_voucher,
		e_item_get_hope_item_typ_one_cost,
		e_item_get_hope_item_typ_ten_cost,
		e_item_get_hope_item_typ_max,
	};

	enum e_quest_sub_type
	{// 任务物品子类型(任务物品模板数据的子类型SubType)
		e_quest_sub_type_none,				// 缺省，未指定类型
		e_quest_sub_type_unexpendable,		// 不可消耗类
		e_quest_sub_type_expendable,		// 可消耗类
		e_quest_sub_type_max,
	};

	const int32 max_jewel_slot_open_by_equip_level = e_item_info_jewel_vip_slot_0 - e_item_info_jewel_slot_0;
	const int32 max_jewel_slot_open_by_vip_level = e_item_info_jewel_vip_slot_1 - e_item_info_jewel_vip_slot_0;

	enum item_temp_succinct_property				//洗练属性临时存储
	{
		item_temp_succinct_property_1,
		item_temp_succinct_property_2,
		item_temp_succinct_property_3,
		item_temp_succinct_property_4,
		item_temp_succinct_property_max

	};

	enum item_temp_forge_property				//重铸属性临时存储
	{
		item_temp_forge_property_1,
		item_temp_forge_property_2,
		item_temp_forge_property_3,
		item_temp_forge_property_4,
		item_temp_forge_property_5,
		item_temp_forge_property_6,
		item_temp_forge_property_max
	};


	enum item_succinct_money_cost_typ				//洗练时消耗的货币种类
	{
		item_succinct_money_cost_non = 0,
		item_succinct_money_cost_gold,
		item_succinct_money_cost_diamond
	};

	enum item_succinct_change_range_read_index
	{
		item_succinct_change_des_max = 0,		//最大减值,如-5
		item_succinct_change_des_min,			//最小减值,如-1
		item_succinct_change_ins_min,			//最小加值,如 1
		item_succinct_change_ins_max,			//最大加值,如 5
		item_succinct_change_max
	};

	//enum wing_type_read
	//{
	//	wing_type_non				= 0,		//翅膀类型，无
	//	wing_type_normal			= 1,		//普通翅膀
	//	wing_type_shape				= 2,		//外形翅膀
	//};

	enum mount_type_read
	{
		mount_type_non = 0,				//坐骑类型，无
		mount_type_normal = 1,			//普通坐骑
		mount_type_special = 2,			//特殊坐骑
		mount_type_warlord = 3,			//城主坐骑
		mount_type_illusion = 4,		//坐骑幻化
		mount_type_lordlegionmem = 5,	//霸主军团成员坐骑
	};

	enum e_item_transform_type
	{
		e_item_transform_type_none,
		e_item_transform_type_two,	//二元数组
		e_item_transform_type_three,	//三元数组
		e_item_transform_type_four,	//四元数组
		e_item_transform_type_max
	};

	struct s_item_cd
	{
		int32 item_template_id;
		float cd_time;
		s_item_cd()
		{
			item_template_id = 0;
			cd_time = 0;
		}
	};


	struct s_item_template_info
	{
		int32 m_item_id;
		int64 m_item_num;
		int32 m_lock;
		e_bag_type m_bag_type;
		s_item_template_info()
		{
			m_item_id = 0;
			m_item_num = 0;
			m_lock = false;
			m_bag_type = e_bag_type_none;
		}
		s_item_template_info(int32 item_id)
		{
			m_item_id = item_id;
			m_item_num = 1;
			m_lock = 1;
			m_bag_type = e_bag_type_none;
		}
		s_item_template_info(int32 item_id, int64 add_num)
		{
			m_item_id = item_id;
			m_item_num = add_num;
			m_lock = 1;
			m_bag_type = e_bag_type_none;
		}
		s_item_template_info(int32 item_id, int64 add_num, int32 lock)
		{
			m_item_id = item_id;
			m_item_num = add_num;
			m_lock = lock;
			m_bag_type = e_bag_type_none;
		}
		s_item_template_info(int32 item_id, int64 add_num, int32 lock, e_bag_type bag_type)
		{
			m_item_id = item_id;
			m_item_num = add_num;
			m_lock = lock;
			m_bag_type = bag_type;
		}
	};
	struct s_del_item_info
	{
		int32 m_item_id;
		int32 m_del_num;
		s_del_item_info(int32 item_id, int32 del_num)
		{
			m_item_id = item_id;
			m_del_num = del_num;
		}
		s_del_item_info(int32 item_id)
		{
			m_item_id = item_id;
			m_del_num = 0;
		}
	};
	struct s_del_item_guid
	{
		guid_64 m_item_guid;
		int32 m_del_num;
		s_del_item_guid(guid_64 item_guid, int32 del_num)
		{
			m_item_guid = item_guid;
			m_del_num = del_num;
		}
		s_del_item_guid(guid_64 item_guid)
		{
			m_item_guid = item_guid;
			m_del_num = 0;
		}
	};
	struct s_item_info
	{
		guid_64	item_guid;									// 物品的GUID	
		int32	data_ary[e_item_info_max];					// 物品实例数据
		xchar	param_str[max_recharge_order_length + 1];	// 字符参数，现在主要用作补单钻石卡的订单号
		s_item_info()
		{
			clear_data();
		}
		bool is_valid()
		{
			return item_guid.is_valid();
		}
		void clear_data()
		{
			memset(this, 0, sizeof(*this));
		}
		void set_param_str(xstring _str)
		{
			memset(param_str, 0, sizeof(param_str));
			memcpy(param_str, _str.c_str(), _str.size() > max_recharge_order_length ? max_recharge_order_length : _str.size());
		}

		void  my_memcopy_string(void* dst, size_t max_len, const xstring &str)
		{
			size_t len = str.size();
			if (max_len < len)
			{
				len = max_len;
			}
			memcpy(dst, str.c_str(), len);
		}
	};

	struct s_item_info_db
	{
		guid_64 role_guid;
		s_item_info data_info;
		s_item_info_db()
		{
			clear_data();
		}
		void clear_data()
		{
			role_guid.clear_data();
			data_info.clear_data();
		}
	};
	//合并记录
	struct s_merge_stat
	{
		guid_64 item_guid;
		int32 total_num;
		s_merge_stat()
		{
			item_guid.clear_data();
			total_num = 0;
		}
	};
	enum e_item_operate_money
	{
		e_item_operate_money_gold,		//使用金币
		e_item_operate_money_diamond	//使用钻石
	};
	enum e_item_lock_states 
	{
		e_item_unlocked,
		e_item_locked,
	};
	//enum e_item_wing_type
	//{
	//	e_item_wing_type_non,		// 空
	//	e_item_wing_type_normal,	// 普通翅膀
	//	e_item_wing_type_shape_no_illusion,		// 无幻化外形翅膀
	//	e_item_wing_type_shape_illusion, // 有幻化的外形翅膀
	//	e_item_wing_type_illusion,		// 幻化的翅膀
	//};
	enum e_wing_illusion_originalid_range
	{
		e_wing_illusion_originalid_range_low,
		e_wing_illusion_originalid_range_high,
		e_wing_illusion_originalid_range_max,
	};
	enum e_mount_illusion_originalid_range
	{
		e_mount_illusion_originalid_range_low,
		e_mount_illusion_originalid_range_high,
		e_mount_illusion_originalid_range_max,
	};
	enum e_wing_rush	//翅膀三段冲
	{
		e_wing_rush_first,
		e_wing_rush_second,
		e_wing_rush_third,
		e_wing_rush_max,
	};

	enum e_item_advance_type
	{
		e_item_advance_ban,			// 不可进阶
		e_item_advance_normal,		// 普通进阶
		e_item_advance_godassembly,	// 神装再造

	};

	enum EItemPropertyIdex
	{
		EItemProperty_Level,
		EItemProperty_PropertyID,
		EItemProperty_Points,
		EItemProperty_Percent,
		EItemProperty_IsShow,
		EItemProperty_Max
	};

	enum e_luck_draw_record_type
	{
		e_luck_draw_record_type_sprite,
		e_luck_draw_record_type_hope_item,
		e_luck_draw_record_type_max
	};

	struct s_hope_hunt_record_info
	{
		guid_64		info_guid;
		guid_64		role_guid;
		xchar		role_name[max_name_size + 1];
		int32		item_template_id;
		int32		get_type;					//获取方式，单抽还是十连
		int32		record_type;				//记录类型，祈福还是精灵
		int64		record_time;

		s_hope_hunt_record_info()
		{
			clear_data();
		}

		void clear_data()
		{
			memset(this, 0, sizeof(*this));
		}

		void set_role_name(const xstring& p_role_name)
		{
			memset(role_name, 0, sizeof(role_name));
			memcpy(role_name, p_role_name.c_str(), p_role_name.size() > max_name_size ? max_name_size : p_role_name.size());
		}
	};

	enum e_composit_type
	{
		e_composit_type_none,
		e_composit_type_yinji,
		e_composit_type_chibang,
		e_composit_type_zhuangbeisuipian,
		e_composit_type_zhuangbeicailiao,
		e_composit_type_zuoqi,
		e_composit_type_jingling,
		e_composit_type_chengzhuang,
		e_composit_type_hongzhuang,
		e_composit_type_jewel,
		e_composit_type_item_advance,
		e_composit_type_god_assembly,
		e_composit_type_god_assembled,
		e_composit_type_item_cast,
 		e_composit_type_skygod,
		e_composit_type_skylist,
		e_composit_type_skygodlist,
		e_composit_type_supreme,
		e_composit_type_supreme_list,
		e_composit_type_fu_neng,
		e_composit_type_max
	};

	enum e_map_count
	{
		e_map_count_map_id,
		e_map_count_gain_treasure_type,
	};

	enum e_develop_open_type
	{
		e_develop_open_type_non,
		e_develop_open_type_tinder,					// 火种开放上限
		e_develop_open_type_mount_upgrade,			// 坐骑升阶上限
		e_develop_open_type_mount_starupgrade,		// 坐骑魂星等级上限
		e_develop_open_type_mount_illusionupgrade,	// 坐骑幻化等级上限
		e_develop_open_type_wing_upgrade,			// 羽翼升阶上限
		e_develop_open_type_wing_starupgrade,		// 羽翼魂星上限
		e_develop_open_type_wing_illusionupgrade,	// 羽翼幻化上限
		e_develop_open_type_element_preview,		// 符文预览品质上限
		e_develop_open_type_fashion_upgrade,		// 时装升级上限
		e_develop_open_type_max,					// 上限值 永远在最后一个

	};
#pragma pack(pop)
}

#endif
