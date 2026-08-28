#ifndef _LEGION_DEF_HPP_
#define _LEGION_DEF_HPP_

#include "logic/type_def.hpp"
#include "Logic/char_def.hpp"
#include "Logic/item_def.hpp"
#include "Logic/ranking_def.hpp"
#include "count_def.hpp"
#include "template/ServerTemplateHead.h"

namespace hld
{
#pragma pack(push,1)

	const int32 legion_name_size_min_limit = 2;				// 这个是军团名称实际的最小长度限制
	const int32 legion_name_size_max_limit = 12;			// 这个是军团名称实际的最大长度限制
	const int32 max_legion_count = 999;						// 军团的最大数量
	const int32 db_legion_count = 100;						// 军团单次传输的数量
	const int32 once_legion_list_get_num = 15;				// 单次获取军团列表的长度
	const int32 max_legion_announcement_size = 400;			// 军团公告的字符数限制
	const int32 max_legion_announcement_content_size = 200;	// 军团公告的字数限制
	const int32 max_legion_member_num = 250;				// 军团最大人数限制
	const int32 max_cross_legion_member_num = 50;			// 开启跨服后预计军团最大活跃人数
	const int64	convenient_join_legion_cd = 10;				// 一键加入军团功能的CD
	const int32 max_hurt_monster_player_num = 500;			// 给一个怪物造成伤害的玩家数

	const int32 max_legion_applicant_num = 50;				// 申请列表可以保存的申请数目的最大值
	const int32 max_legion_event_num = 500;					// 一个军团能保存的军团事件的最大数目
	const int32 once_legion_event_list_get_num = 10;		// 查询军团事件列表每次能够获得的事件条目个数
	const int32 max_once_legion_event_size = 256;			// 军团事件的字符数限制
	const int32 max_legion_boss_record_num = 20;			// 军团BOSS的最大数目

	const int32 first_legion_template_id = 38000001;		// 军团表里第一个条目的ID
	const int32 legion_shop_template_id = 34000017;			// 军团商店的模板表格ID

	const int32 city_master_wing_id = 31040044;				// 城主翅膀模板ID
	const int32 city_master_buff_id = 10130011;				// 城主buff
	const int32 city_master_title_id = 39000011;			// 城主称号
	const int32 city_master_mount_id = 0;					// 城主坐骑
	const int32 city_master_union_mount_id = 31030050;		// 城主所在工会
	const int32 lord_legion_mem_mount_id = 0;				// 霸主成员坐骑
	const int32 city_war_winner_title_id = 39000012;		// 霸主成员title

	const int32 city_war_cross_master_title_id = 39000043;			// 跨服城主称号
	const int32 city_war_cross_winner_title_id = 39000044;			// 跨服霸主成员称号
	
	const int32 max_take_part_in_banquet_per_role = 10; // 每名角色能够参加王城舞会的最大次数

	const int32 max_apply_city_war_legion_num = 3;				// 单个领地的城战申请的名额
	const int32 city_war_bid_base_price = 10000;				// 城战申请基本竞标价格
	const int32 city_war_bid_add_price = 1000;					// 每次申请城战竞标的价格增量
	const int32 city_war_map_id = 71080001;						// 军团城战的地图ID
	const int32 max_glory_glow = int32_MAX_NUM - 300000;		// 生命之辉最大值
	const int32 territory_occupier_price_flag = max_glory_glow + 1;	// 占领标记
	const int32 territory_occupier_bid_idx = 0;					// 占领后存储在哪个索引
	const int32 max_territory_num = 10;
	const int32 max_territory_num_cross_server = 7;
	const int32 min_territory_num_cross_server = 4;

	const int32 all_territory_bid_info_num = (max_territory_num_cross_server + 1) * max_apply_city_war_legion_num;
	const int32 overlord_war_territory_flag = 121000000;		// CityWarTerritoryTemplate表中的id
	const int32 legion_construction_num = 4;		// 军团建筑数量
	const int32 max_legion_bonus_mission_num = 10;//分红任务上限
	const int32 cross_server_territory_war_flag = 121000005;		//跨服领地战起始id

	const int32 first_legion_bonus_template_id = 132000000;											//第一个分红任务ID
	const int32	legion_city_master_online_notice_id = 93000008;
	const int32	new_legion_city_master_online_notice_id = 93000284;	//新霸主上线公告
	const int32 new_begin_cross_city_war_notice_id = 93000285;//新城战开启公告
	const int32	legion_city_war_open_notice_id = 93000022;
	const int32	legion_city_dance_open_notice_id = 93000023;
	const int32 legion_send_overload_notice_id = 93000040;
	const int32 legion_add_member_notice_id = 93000047;
	const int32 legion_del_member_notice_id = 93000048;
	const int32 legion_change_job_title_notice_id = 93000049;
	const int32 legion_on_create_bonfire_notice_id = 93000060;
	const int32 legion_occupied_territory_notice_id = 93000061;
	const int32 legion_on_legion_boss_notice_id = 93000062;
	const int32 legion_world_mistress_notice_id = 93000063;
	const int32 legion_world_mistress_open_notice_id = 93000068;
	const int32 legion_maintenance_money_not_enough_notice_id = 93000072;
	const int32 legion_city_maintenance_money_not_enough_notice_id = 93000073;
	const int32 legion_start_legion_boss_map_id = 71060000;
	const int32 legion_donate_notice_id = 93000151;
	const int32 legion_add_bonfire_successfull_notice_id = 93000201;
	const int32 legion_get_bonus_reward_notice_id = 93000230;										//获得军团分红奖励公告
	const int32 legion_stimulate_bonus_notice_id = 93000231;										//军团激励分红公告
	const int32 legion_member_change_name_notice_id = 93000255;										//军团成员改名公告
	const int32 legion_warehouse_store_notice_id = 93000264;                                        //军团仓库捐赠公告
	const int32 legion_warehouse_get_notice_id = 93000265;                                          //军团仓库获得公告
	const int32 legion_member_change_legion_name_notice_id = 93000276;								//军团名改名公告
	const int32	legion_cross_server_city_war_open_notice_id = 93000280;

	const int32 legion_boss_cur_legion_notice_id = 93000421;										//军团BOSS击杀当前军团内部伤害
	const int32 legion_boss_all_legion_notice_id = 93000422;										//军团BOSS击杀所有军团排名

	const int32 legion_init_announcement_str_id = 90095257;										 	// 军团默认公告
	const int32 legion_event_member_leave_str_model_id = 90095121;									// 成员退出
	const int32 legion_event_member_join_str_model_id = 90095122;									// 成员加入
	const int32 legion_event_member_post_change_str_model_id = 90095123;							// 军团成员职务变更
	const int32 legion_event_donate_money_and_item_str_model_id = 90095124;							// 有人捐献金币或者物品
	const int32 legion_event_kill_boss_str_model_id = 90095125;										// 有人击杀了军团BOSS
	const int32 legion_event_win_overlord_city_war_str_model_id = 90095128;							// 世界霸主胜利
	const int32 legion_event_occupy_city_str_model_id = 90095129;									// 占领城市
	const int32 legion_event_lose_city_by_war_str_model_id = 90095130;								// 城战失败丢失城市
	const int32 legion_event_lose_city_by_maintain_money_not_enough_str_model_id = 90095131;		// 维护资金不足丢失城市
	const int32 legion_event_on_create_bonfire_str_model_id = 90095132;								// 篝火创建
	const int32 legion_event_legion_construction_level_up_str_model_id = 90095133;					// 军团建筑升级
	const int32 legion_event_donate_money_and_item_no_battle_achievement_str_model_id = 90095134;	// 有人捐献金币或者物品,无战功
	const int32 legion_event_bid_win_str_model_id = 90095135;										// 竞标成功
	const int32 legion_event_bid_fail_str_model_id = 90095136;										// 竞标失败
	const int32 legion_main_construction_name_id = 90090132;

	const int32 first_legion_skill_id = 128000001;													//第一个军团技能ID
	const int32 finally_legion_skill_id = 128000010;												//最后一个军团技能ID

	const int32 legion_member_change_name_event_id = 90096117;										//军团内成员改名事件id
	
	const int32 first_legion_answer_template_id = 144000001;										//军团答题配置表起始id
	const int32 inform_series_right_nums = 10;														//连续正确答题发公告需要的数量
	const int32 inform_all_right_nums = 14;														    //总共正确答题发公告需要的数量
	const int32 first_right_question_notice_id = 93000256;											//第一正确答题公告id
	const int32 series_right_question_notice_id = 93000257;											//连续正确答题公告id
	const int32 all_right_question_notice_id = 93000258;											//总共正确答题公告id
	const int32 prepare_legion_answer_notice_id = 93000259;											//准备开始答题公告id
	const int32 start_legion_answer_notice_id = 93000262;											//开始答题公告id

	const int32	first_answer_true_notice_id = 93000400;												//抢答成功的军团公告id
	const int32 answer_failure_notice_id = 93000401;												//军团答题失败的公告id
	const int32 play_answer_num_notice_id = 93000402;												//玩家在本次活动抢答次数到达指定次数的公告id
	const int32 legion_answer_end_notice_id = 93000403;												//本次军团答题结束的公告
	const int32 all_legion_answer_end_one_play_notice_id = 93000404;								//全部军团答题结束的公告一个军团上榜
	const int32 all_legion_answer_end_two_play_notice_id = 93000405;								//全部军团答题结束的公告二个军团上榜
	const int32 all_legion_answer_end_three_play_notice_id = 93000406;								//全部军团答题结束的公告三个军团上榜

	const int32 max_legion_warehouse_info = 100;                                                    //军团仓库物品的最大数量
	const int32 max_legion_warehouse_log = 20;                                                      //军团日志的最大数量
	const int32 calculate_legion_average_lv_need_member_num = 20;									//计算军团平均等级所需成员数目

	const int32 max_cross_server_all_city_num = 15;													//跨服霸主战总军团数
	const int32 max_cross_server_one_group_city_num = 3;											//一组霸主战军团数
	const int32 max_cross_server_city_group_num = 5;												//跨服霸主战组数

	const int32 cross_server_territory_city_max = 7;												//跨服城战城池最大个数
	const int32 cross_server_legion_call_operate_params_max = 5;									//跨服城战召集参数最大数量
	const int32 cross_server_territory_city_max_params = 11;										//跨服城战请求的信息为所有表中配置的城战信息，在客户端做过滤处理		

	const int32 cross_server_territory_city_bid_info_max = cross_server_territory_city_max * max_apply_city_war_legion_num;

	const int32 cross_server_player_legion_save_time = 180;											//跨服玩家军团缓存记录Sec
	const int32 max_legion_answer_num = 14;															//军团答题最大题数
	const int32 rank_show_num = 3;																	//军团答题最大显示数量

	const int32 overload_city_war_reward_first_temp_id = 155000001;									// 霸主战奖励首ID
	const int32 max_overload_city_war = 15;															// 霸主战奖励最大数

	const int32 legion_welfare_activity_award_notice_id = 93000425;									// 军团福利活动排名奖励公告
	const int32 legion_welfare_kill_boss_notice_id = 93000426;										// 军团福利击杀boss奖励公告
	const int32 legion_welfare_grant_num = 1;														// 军团福利每人发放数量

	const int32 legion_strategy_world_boss_num = 3;                                                 //军团攻略世界boss数量  

	enum e_del_overlord_stuff_event
	{
		e_del_overlord_stuff_event_none,
		e_del_overlord_stuff_event_all_legion,		//删除整个军团 包括城主 和成员的 奖励
		e_del_overlord_stuff_event_all_member,		//所有成员
		e_del_overlord_stuff_event_only_master,		//只有城主
		e_del_overlord_stuff_event_only_one_member,	//只有一个成员
		e_del_overlord_stuff_event_max,

	};


	// 王城舞会的级别,举办不同级别的舞会需要不同的层次的花费,得到的收益也不同
	enum e_banquet_level
	{
		e_banquet_level_ordinary,		// 普通的舞会
		e_banquet_level_hearty,			// 丰盛的舞会
		e_banquet_level_luxury,			// 豪华的舞会
		e_banquet_level_max
	};

	enum e_overload_notice_type
	{
		e_overload_notice_type_legion_begin,//发给军团的开始公告
		e_overload_notice_type_world_begin,//发给世界的开始公告
		e_overload_notice_type_cross_city_war_begin_world,//活动开启公告发给世界
		e_overload_notice_type_cross_city_war_begin_legion,//活动开启公告发给军团
		e_overload_notice_type_max
	};

	enum e_overload_city_war_type
	{
		e_overload_city_war_type_team_ready,//处于已分好组状态
		e_overload_city_war_type_start_city_war,//处于开始状态
	};

	enum e_city_war_interact_npc
	{
		e_city_war_interact_npc_stronghold = 76300011,
		e_city_war_interact_npc_crystal_s = 76300100,
		e_city_war_interact_npc_crystal_m = 76300101,
		e_city_war_interact_npc_crystal_l = 76300102,
		e_city_war_interact_npc_crystal_xl = 76300103,
	};

	enum ELegionInfo
	{
		ELegionInfo_construction_level_main,	// 军团主建筑等级
		ELegionInfo_construction_level_1,		// 军团副建筑1等级
		ELegionInfo_construction_level_2,		// 军团副建筑2等级
		ELegionInfo_construction_level_3,		// 军团副建筑3等级
		ELegionInfo_asset_money,				// 军团财产--资金
		ELegionInfo_asset_item_1,				// 军团财产--物品1
		ELegionInfo_asset_item_2,				// 军团财产--物品2
		ELegionInfo_asset_item_3,				// 军团财产--物品3
		ELegionInfo_asset_item_4,				// 军团财产--物品4
		ELegionInfo_fighting_power,             // 军团战斗力
		ELegionInfo_auto_accept_new_member,     // 是否自动接受入团申请
		ELegionInfo_last_refresh_time,			// 上次每周更新的时间
		ELegionInfo_glory_glow,					// 荣耀之辉
		ELegionInfo_act_score_raid,				// 军团副本得分
		ELegionInfo_act_score_bonfire,			// 篝火得分
		ELegionInfo_legion_welfare_num,			// 本周军团福利宝箱数量
		ELegionInfo_max
	};
	
	struct s_legion_info
	{
		guid_64	legion_guid;
		xchar	legion_name[max_name_size + 1];
		xchar	announcement[max_legion_announcement_size + 1];
		int64	gs_value;
		i64		data_ary[ELegionInfo_max];

		s_legion_info()
		{
			clear_data();
		}

		void clear_data()
		{
			memset(this, 0, sizeof(s_legion_info));
			data_ary[ELegionInfo_construction_level_main] = 1;
			data_ary[ELegionInfo_construction_level_1] = 1;
			data_ary[ELegionInfo_construction_level_2] = 1;
			data_ary[ELegionInfo_construction_level_3] = 1;
			data_ary[ELegionInfo_asset_money] = 0;
		}
	};

	enum e_legion_job_title
	{
		e_legion_job_title_none,             // 无职位
		e_legion_job_title_commander,        // 指挥官
		e_legion_job_title_assistant_chief,  // 副军团长
		e_legion_job_title_chief,            // 军团长
		e_legion_job_title_max
	};

	enum e_legion_log_type
	{
		e_legion_log_type_create,			// 创建军团
		e_legion_log_type_del,				// 删除军团
		e_legion_log_type_join,				// 加入军团
		e_legion_log_type_leave,			// 离开军团
		e_legion_log_type_change_name,		// 改军团名
		e_legion_log_type_announcement,		// 修改公告
		e_legion_log_type_appoint,			// 更改职位
		e_legion_log_type_max
	};

	struct s_legion_city_war_member_info
	{
		guid_64  role_guid;
		int64	 use_type;
		xchar    role_name[max_name_size + 1];
		int64	 player_temp_id;
		int64	 week_contribution;
		int64    job_title;
		s_legion_city_war_member_info()
		{
			clear_data();
		}

		void clear_data()
		{
			memset(this, 0, sizeof(s_legion_city_war_member_info));
		}

		bool is_valid()
		{
			return role_guid.is_valid();
		}

		void set_role_name(xstring _role_name)
		{
			memset(role_name, 0, max_name_size + 1);
			memcpy(role_name, _role_name.c_str(), _role_name.size() > sizeof(role_name) ? sizeof(role_name) : _role_name.size());
		}
	};

	struct s_legion_city_war_member_info_db
	{
		guid_64							legion_guid;
		s_legion_city_war_member_info	legion_member_info;
	};

	struct s_legion_member_info
	{
		guid_64  role_guid;
		xchar    role_name[max_name_size + 1];
		int64	 gs_value;
		int64    data_ary[e_legion_member_info_max];

		s_legion_member_info()
		{
			clear_data();
		}

		void clear_data()
		{
			memset(this, 0, sizeof(s_legion_member_info));
		}

		bool is_valid()
		{
			return role_guid.is_valid();
		}

		void set_role_name(xstring _role_name)
		{
			memset(role_name, 0, max_name_size + 1);
			memcpy(role_name, _role_name.c_str(), _role_name.size() > sizeof(role_name) ? sizeof(role_name) : _role_name.size());
		}

		bool operator < (const s_legion_member_info& other_info) const 
		{
			return data_ary[e_legion_member_info_week_contribution] < other_info.data_ary[e_legion_member_info_week_contribution];
		}

		bool operator > (const s_legion_member_info& other_info) const
		{
			return data_ary[e_legion_member_info_week_contribution] > other_info.data_ary[e_legion_member_info_week_contribution];
		}

		bool to_proto(hld::st_proto::st_legion_member_info *st_member_ptr)
		{
			if (st_member_ptr == nullptr)
			{
				return false;
			}
			st_member_ptr->set_role_guid(role_guid.server_64);
			st_member_ptr->set_role_name(role_name);
			st_member_ptr->set_gs_value(gs_value);
			for (int32 i = 0; i < e_legion_member_info_max; i++)
			{
				st_member_ptr->add_data_ary(data_ary[i]);
			}
			return true;

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

		void  from_proto(const hld::st_proto::st_legion_member_info& st_member_ref)
		{
			role_guid.server_64 = st_member_ref.role_guid();
			my_memcopy_string(role_name, max_name_size, st_member_ref.role_name());

			gs_value = st_member_ref.gs_value();
			for (int32 i = 0; i < st_member_ref.data_ary_size(); i++)
			{
				data_ary[i] = st_member_ref.data_ary(i);
			}
		}
	};

	struct s_legion_member_info_db
	{
		guid_64					legion_guid;
		s_legion_member_info	legion_member_info;
	};

	struct s_overload_legion_info_db
	{
		guid_64					legion_guid;
		xchar					city_master_name[max_name_size + 1];
	};

	// 用来保存军团成员的分数
	struct legion_member_score
	{
		guid_64	legion_guid;
		guid_64	member_guid;
		xchar	member_name[max_name_size + 1];
		int32	member_level;
		int32	score;
		int32	server_id;

		legion_member_score()
		{
			clear_data();
		}

		void clear_data()
		{
			legion_guid.clear_data();
			member_guid.clear_data();
			member_level = 0;
			score = 0;
			server_id = 0;
			memset(member_name, 0, max_name_size + 1);
		}

		void set_member_name(xstring role_name)
		{
			memset(member_name, 0, max_name_size + 1);
			memcpy(member_name, role_name.c_str(), role_name.size() > sizeof(member_name) ? sizeof(member_name) : role_name.size());
		}
	};

	struct s_legion_answer_info
	{
		int32					answer_successfully_num;						//本次答题成功的数量
		guid_64					play_guid[max_legion_answer_num];				//用来记录本次活动抢答成功的玩家
		bool					is_answer;										//是否抢答成功了
		guid_64					the_answer_play_guid[max_legion_member_num];	//本题所有回答成功的玩家列表
		int32					the_answer_play_num;							//回答成功玩家的数量
		s_legion_answer_info()
		{
			clear_data();
		}

		void clear_data()
		{
			memset(this, 0, sizeof(s_legion_answer_info));
		}
		/*void set_legion_name(const xstring role_name)
		{
			memset(legion_name, 0, sizeof(legion_name));
			memcpy(legion_name, role_name.c_str(), role_name.size() > sizeof(legion_name) ? sizeof(legion_name) : role_name.size());
		}*/
		int32 get_play_answer_num(guid_64 role_guid)		//返回这个玩家抢答的次数
		{
			int32 num = 0;
			for (int32 i = 0; i < max_legion_answer_num; ++i)
			{
				if (play_guid[i] == role_guid)
				{
					++num;
				}
			}
			return num;
		}
		guid_64 get_max_num_play()	//获得抢答次数最多的玩家id
		{
			guid_64 role_guid;
			int32 num = 0;
			for (int32 i = 0; i < max_legion_answer_num; ++i)
			{
				if (play_guid[i].is_valid() && get_play_answer_num(play_guid[i]) > num)
				{
					num = get_play_answer_num(play_guid[i]);
					role_guid = play_guid[i];
				}
			}
			return role_guid;
		}
	};

	enum e_legion_apply_stage
	{
		e_legion_apply_stage_send_failed,	// 入团申请发送失败
		e_legion_apply_stage_send_success,	// 入团申请发送成功
		e_legion_apply_stage_is_accepted,	// 对方接受了入团申请
		e_legion_apply_stage_is_refused,	// 对方拒绝了入团申请
		e_legion_apply_stage_auto_accepted,	// 对方自动接收新成员
		e_legion_apply_stage_max
	};

	enum ELegionApplicantInfo
	{
		ELegionApplicantInfo_level,
		ELegionApplicantInfo_player_template_id,
		ELegionApplicantInfo_fighting_power,
		ELegionApplicantInfo_vip_level,
		ELegionApplicantInfo_max
	};

	struct s_legion_applicant_info
	{
		guid_64	legion_guid;
		guid_64	role_guid;
		xchar	role_name[max_name_size + 1];
		float	exist_time;
		int64	gs_value;
		i32		data_ary[ELegionApplicantInfo_max];

		s_legion_applicant_info()
		{
			clear_data();
		}

		void clear_data()
		{
			memset(this, 0, sizeof(s_legion_applicant_info));
		}

		bool is_valid()
		{
			return role_guid.is_valid();
		}

		void set_applicant_name(xstring _role_name)
		{
			memset(role_name, 0, max_name_size + 1);
			memcpy(role_name, _role_name.c_str(), _role_name.size() > sizeof(role_name) ? sizeof(role_name) : _role_name.size());
		}
	};

	// 军团事件的类型
	enum e_legion_event_type
	{
		e_legion_event_type_none,
		e_legion_event_type_legion_create,								// 创建军团
		e_legion_event_type_member_join,								// 成员加入
		e_legion_event_type_member_leave,								// 成员退出
		e_legion_event_type_donate_money,								// 有人捐献金钱
		e_legion_event_type_donate_item,								// 有人捐献物品
		e_legion_event_type_kill_boss,									// 有人击杀了军团BOSS
		e_legion_event_type_max
	};

	// 军团事件可携带的额外数据,相比直接保存描述事件的字符串内容可以减小存储量和数据包大小.
	// 每个额外数据根据s_legion_event_type的值的不同具有不同的意义,不一定所有的额外数据都会被用到
	enum e_legion_event_extra_data
	{
		e_legion_event_extra_data_0,
		e_legion_event_extra_data_1,
		e_legion_event_extra_data_2,
		e_legion_event_extra_data_max
	};

	struct s_legion_event
	{
		guid_64				legion_guid;
		int32				event_idx;
		xchar				extra_data[max_once_legion_event_size + 1];
		uint32				happen_time;

		s_legion_event()
		{
			clear_data();
		}

		void clear_data()
		{
			memset(this, 0, sizeof(s_legion_event));
		}

		void set_extra_data(xstring data_str)
		{
			memset(extra_data, 0, sizeof(extra_data));
			memcpy(extra_data, data_str.c_str(), data_str.size() > sizeof(extra_data) ? sizeof(extra_data) : data_str.size());
		}
	};

	enum ELegionBossRecord
	{
		ELegionBossRecord_template_id,	// 对应的军团活动的副本地图模板ID
		ELegionBossRecord_unlock,		// 相应的BOSS活动地图是否解锁
		ELegionBossRecord_killed,		// 相应的军团BOSS是否已经被击杀
		ELegionBossRecord_max
	};

	struct s_legion_boss_record
	{
		guid_64	legion_guid;
		guid_64	boss_map_guid;
		xchar	killer_name[max_name_size + 1];
		int32	data_ary[ELegionBossRecord_max];

		s_legion_boss_record()
		{
			clear_data();
		}

		void clear_data()
		{
			memset(this, 0, sizeof(s_legion_boss_record));
		}

		void set_killer_name(const xchar* _killer_name, int32 str_size)
		{
			memcpy(killer_name, _killer_name, max_name_size > str_size ? str_size : max_name_size);
		}

		void clear_killer_name()
		{
			memset(killer_name, '\0', max_name_size + 1);
		}
	};

	// 下面罗列了城战过程中相关操作的枚举,如占领据点和采集水晶
	enum e_city_war_operate_type
	{
		e_city_war_operate_type_none,
		e_city_war_operate_type_occupy_stronghold,		// 占领据点
		e_city_war_operate_type_interrupt_occupy,		// 中断占领
		e_city_war_operate_type_finish_occupy,			// 完成占领
		e_city_war_operate_type_gather_crystal,			// 采集水晶
		e_city_war_operate_type_interrupt_gather,		// 中断采集
		e_city_war_operate_type_finish_gather,			// 完成采集
		e_city_war_operate_type_max
	};
	
	// 用来保存军团的分数
	struct s_city_war_legion_score
	{
		guid_64	legion_guid;
		char	legion_name[max_name_size + 1];
		int32	score;
		int32	bid_price;

		s_city_war_legion_score()
		{
			clear_data();
		}

		void clear_data()
		{
			memset(this, 0, sizeof(s_city_war_legion_score));
		}

		bool operator < (const s_city_war_legion_score & other)const
		{
			return score > other.score;
		}
		bool to_proto(hld::st_proto::st_city_war_legion_score* st_city_war_ptr)
		{
			if (st_city_war_ptr == nullptr)
			{
				return false;
			}
			st_city_war_ptr->set_legion_guid(legion_guid.server_64);
			st_city_war_ptr->set_legion_name(legion_name);
			st_city_war_ptr->set_score(score);
			st_city_war_ptr->set_bid_price(bid_price);
			return true;
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
		void from_proto(const hld::st_proto::st_city_war_legion_score& st_city_war_ref)
		{
			legion_guid.server_64 = st_city_war_ref.legion_guid();
			my_memcopy_string(legion_name, max_name_size, st_city_war_ref.legion_name());
			score = st_city_war_ref.score();
			bid_price = st_city_war_ref.bid_price();
		}
	};

	struct s_city_war_bid_info
	{
		int32	territory_id;		//领地配置id
		int32	index_in_terr;
		guid_64	legion_guid;
		int32	cur_bid_price;		//占领后值为territory_occupier_price_flag
		int32	is_prior_maintain;	//占领后是否优先维护
		int32   occupy_time_stamp;
		int32	server_id;
		xchar	legion_name[max_name_size + 1];

		s_city_war_bid_info()
		{
			clear_data();
			index_in_terr = 0;
			territory_id = 0;
		}

		void clear_data()
		{
			legion_guid.clear_data();
			cur_bid_price = 0;
			is_prior_maintain = 0;
			occupy_time_stamp = 0;
			server_id = 0;
		}

		bool is_valid()
		{
			return legion_guid.is_valid();
		}

		bool is_data_valid()
		{
			return territory_id > 0;
		}
		void set_legion_name(xstring m_legion_name) {
			memset(legion_name, 0, sizeof(legion_name));
			memcpy(legion_name, m_legion_name.c_str(), m_legion_name.size() > sizeof(legion_name) ? sizeof(legion_name) : m_legion_name.size());
		}
	};
	struct s_cross_server_city_war_bid_info
	{
		int32   serverid;			//服务器ID
		int32	territory_id;		//领地配置id
		int32	index_in_terr;
		guid_64	legion_guid;
		int32	cur_bid_price;		//占领后值为territory_occupier_price_flag
		int32	is_prior_maintain;	//占领后是否优先维护
		int32   occupy_time_stamp;

		s_cross_server_city_war_bid_info()
		{
			clear_data();
		}

		void clear_data()
		{
			serverid = 0;
			territory_id = 0;
			index_in_terr = 0;
			legion_guid.clear_data();
			cur_bid_price = 0;
			is_prior_maintain = 0;
			occupy_time_stamp = 0;
		}

		bool is_valid()
		{
			return legion_guid.is_valid();
		}

		bool is_data_valid()
		{
			return territory_id > 0;
		}
	};
	typedef s_city_war_bid_info s_city_war_bid_info_db;
// 	struct s_city_war_bid_info_db
// 	{
// 		int32				bid_id;
// 		s_city_war_bid_info	bid_info;
// 
// 		s_city_war_bid_info_db()
// 		{
// 			bid_id = 0;
// 		}
// 	};
	
	struct s_city_war_territory_info
	{
		int32				territory_id;
		//guid_64			occupier_legion_guid;
		guid_64				city_war_map_guid; //map有效时说明正在争夺 军团战进行中重启服务器需要重新开打
		uint32				city_war_finish_stamp;
		s_city_war_bid_info	bid_legions[max_apply_city_war_legion_num];

		s_city_war_territory_info()
		{
			clear_data();
			territory_id = 0;
		}

		void clear_data()
		{
			//occupier_legion_guid.clear_data();
			city_war_map_guid.clear_data();
			for (int32 i = 0; i < max_apply_city_war_legion_num; i++)
			{
				bid_legions[i].clear_data();
				bid_legions[i].index_in_terr = i;
			}
			city_war_finish_stamp = 0;
		}
		void set_territory_id(int32 id)
		{
			territory_id = id;
			for (int32 i = 0; i < max_apply_city_war_legion_num; i++)
			{
				bid_legions[i].territory_id = id;
			}
		}
		s_city_war_bid_info* get_bid_info(int32 idx)
		{
			if (idx < 0 || idx >= max_apply_city_war_legion_num)
			{
				return nullptr;
			}
			return &bid_legions[idx];
		}
		s_city_war_bid_info* get_bid_info(guid_64 legion)
		{
			if (!legion.is_valid())
			{
				return nullptr;
			}
			for (int32 i = 0; i < max_apply_city_war_legion_num; i++)
			{
				if (bid_legions[i].legion_guid == legion)
				{
					return &bid_legions[i];
				}
			}
			return nullptr;
		}
		void del_bid_info()
		{
			for (int32 i = 0; i < max_apply_city_war_legion_num; i++)
			{
				bid_legions[i].clear_data();
			}
		}
	};
	typedef std::map<int32, s_city_war_territory_info> territory_lists_type;

	struct s_city_war_legion_rank_info
	{
		int32  server_id;
		guid_64 legion_guid;
		int32	occupy_terr_num;
		int32	occupy_terr_score;
		int32	occupy_terr_highest_score;

		s_city_war_legion_rank_info()
		{
			clear_data();
		}
		void clear_data()
		{
			server_id = 0;
			legion_guid.clear_data();
			occupy_terr_num = 0;
			occupy_terr_score = 0;
			occupy_terr_highest_score = 0;
		}
		bool operator > (const s_city_war_legion_rank_info& info) const
		{
			if (occupy_terr_score != info.occupy_terr_score)
			{
				return occupy_terr_score > info.occupy_terr_score;
			}
			if (occupy_terr_num != info.occupy_terr_num)
			{
				return occupy_terr_num > info.occupy_terr_num;
			}
			return occupy_terr_highest_score > info.occupy_terr_highest_score;
		}
		bool operator < (const s_city_war_legion_rank_info& info) const
		{
			return !(*this > info); //设计决定不会=
		}
	};
	typedef std::list<s_city_war_legion_rank_info> city_war_rank_type;

	struct s_city_war_member_add_gg
	{
		guid_64 mem_guid;
		int32	mem_got_gg;

		s_city_war_member_add_gg()
		{
			clear_data();
		}
		void clear_data()
		{
			mem_guid.clear_data();
			mem_got_gg = 0;
		}
	};
	typedef std::vector<s_city_war_member_add_gg> legion_mem_gg_info_list;
	typedef std::map<guid_64, legion_mem_gg_info_list> legion_add_gg_info;


	// 用于cs上存储玩家军团简要信息的结构
	struct s_player_legion_info
	{
		guid_64				legion_guid;
		xchar				legion_name[max_name_size + 1];
		int32				legion_level;
		int32				legion_role_num;
		e_legion_job_title	job_title;
		int32				occupied_territorys[max_territory_num];
		int32				legion_construction_level[legion_construction_num];
		int64               legion_power;

		s_player_legion_info()
		{
			clear_data();
		}

		void clear_data()
		{
			memset(this, 0, sizeof(s_player_legion_info));
		}

		void set_legion_name(std::string name)
		{
			memcpy(legion_name, name.c_str(), name.size() > max_name_size ? max_name_size : name.size());
		}
		bool to_proto(hld::st_proto::st_player_legion_info *st_legion_ptr)
		{
			if (st_legion_ptr == nullptr)
			{
				return false;
			}
			st_legion_ptr->set_legion_guid(legion_guid.server_64);
			st_legion_ptr->set_legion_name(legion_name);
			st_legion_ptr->set_legion_level(legion_level);
			st_legion_ptr->set_legion_role_num(legion_role_num);
			st_legion_ptr->set_job_title(job_title);
			for (int32 i = 0; i < max_territory_num; i++)
			{
				st_legion_ptr->add_occupied_territorys(occupied_territorys[i]);
			}
			for (int32 i = 0; i < legion_construction_num; i++)
			{
				st_legion_ptr->add_legion_construction_level(legion_construction_level[i]);
			}
			return true;
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
		void from_proto(const hld::st_proto::st_player_legion_info& st_legion_ref)
		{
			legion_guid.server_64 = st_legion_ref.legion_guid();
			my_memcopy_string(legion_name, max_name_size, st_legion_ref.legion_name());
	
			legion_level = st_legion_ref.legion_level();
			legion_role_num = st_legion_ref.legion_role_num();
			job_title = (e_legion_job_title)st_legion_ref.job_title();
			for (int32 i = 0; i < max_territory_num; i++)
			{
				occupied_territorys[i] = st_legion_ref.occupied_territorys(i);
			}
			for (int32 i = 0; i < legion_construction_num; i++)
			{
				legion_construction_level[i] = st_legion_ref.legion_construction_level(i);
			}
		}
	};

	// 用于跨服上存储玩家军团简要信息的结构
	struct s_cross_player_legion_info
	{
		s_player_legion_info	legion_info;
		int32				save_time_stamp;

		s_cross_player_legion_info()
		{
			clear_data();
		}

		void clear_data()
		{
			memset(this, 0, sizeof(s_cross_player_legion_info));
		}
	};

	struct s_legion_boss_award_get_log
	{
		guid_64 role_guid;
		int32	get_logs_num;
		int32	get_logs[max_legion_boss_record_num];

		s_legion_boss_award_get_log()
		{
			memset(this, 0, sizeof(s_legion_boss_award_get_log));
		}
		bool to_proto(hld::st_proto::st_legion_boss_award_get_log* st_log_ptr)
		{
			if (st_log_ptr == nullptr)
			{
				return false;
			}
			st_log_ptr->set_role_guid(role_guid.server_64);
			st_log_ptr->set_get_logs_num(get_logs_num);
			for (int32 i = 0; i < max_legion_boss_record_num; i++)
			{
				st_log_ptr->add_get_logs(get_logs[i]);
			}
			return true;
		}
		void from_proto(const hld::st_proto::st_legion_boss_award_get_log& st_log_ref)
		{
			role_guid.server_64 = st_log_ref.role_guid();
			get_logs_num = st_log_ref.get_logs_num();
			for (int32 i = 0 ; i < st_log_ref.get_logs_size(); i++)
			{
				get_logs[i] = st_log_ref.get_logs(i);
			}
		}
	};

	struct s_legion_boss_award_get_log_db
	{
		guid_64	role_guid;
		int32	boss_map_id;

		s_legion_boss_award_get_log_db()
		{
			role_guid.clear_data();
			boss_map_id = 0;
		}
	};

	struct s_legion_member_contribution
	{
		guid_64 role_guid;
		int32   contribution_get;
		s_legion_member_contribution()
		{
			clear_data();
		}
		void clear_data() 
		{
			memset(this, 0, sizeof(s_legion_member_contribution));
		}
	};

	enum e_legion_construction_level_up_error
	{
		e_legion_construction_level_up_error_none,							// 没错误
		e_legion_construction_level_up_error_job_title_not_enough,			// 职位不够
		e_legion_construction_level_up_error_invalid_index,					// 建筑索引不正确
		e_legion_construction_level_up_error_sub_constr_level_not_enough,	// 升级主军团建筑但是副军团建筑等级不足
		e_legion_construction_level_up_error_sub_constr_level_too_high,		// 升级副军团建筑但是该建筑等级不低于主军团建筑
		e_legion_construction_level_up_error_table_data_error,				// 表格相关错误
		e_legion_construction_level_up_error_money_not_enough,				// 升级所需的军团资金不足
		e_legion_construction_level_up_error_item_not_enougn,				// 升级所需的物品不足
		e_legion_construction_level_up_error_already_top_level,				// 建筑已经达到最高等级
		e_legion_construction_level_up_error_max
	};

	enum e_legion_error
	{
		e_legion_error_none,
		e_legion_error_template_error,								// 军团模板错误
		e_legion_error_member_full,									// 军团人员已满
		e_legion_error_applicant_list_full,							// 军团的入团申请列表已满
		e_legion_error_applicant_already_exist,						// 已经向军团发送过一份入团申请
		e_legion_error_player_added_not_online,						// 要添加的成员当前不在线
		e_legion_error_player_added_is_legion_member,				// 要添加的成员已经是当前军团的成员
		e_legion_error_player_added_is_already_join_other_legion,	// 要添加的成员已经加入了别的军团
		e_legion_error_used_all_take_part_in_banquet_times,			// 所有的参加舞会的次数已经用完
		e_legion_error_banquet_already_over,						// 舞会已经结束
		e_legion_error_no_city_war_qualification,					// 没有城战资格
		e_legion_error_city_war_not_begin,							// 城战还没有开始
		e_legion_error_can_not_dissolve_during_city_war,			// 城战期间不能解散军团(仅对参战军团有效)
		e_legion_error_can_not_leave_legion_during_city_war,		// 城战期间不能退出军团(仅对参战军团有效)
		e_legion_error_error_legion_name_illegal,					// 军团名称中含有非法字符
		e_legion_error_can_not_dissolve_during_banquet,				// 王城舞会举办期间不能解散军团
		e_legion_error_can_not_kickout_member_during_city_war,		// 城战期间不能踢出成员(仅对参战军团有效)
		e_legion_error_repeat_legion_name,							// 重复的军团名
		e_legion_error_full_legion_num,								// 军团数量已达到本服上限,无法再创建军团
		e_legion_error_create_legion_invalid_role_state,			// 角色当前的状态不能创建军团
		e_legion_error_create_legion_no_enough_level,				// 角色等级不足不能创建军团
		e_legion_error_create_legion_no_enough_money,				// 角色金钱不足不能创建军团
		e_legion_error_invalid_banquet_type,						// 错误的舞会类型
		e_legion_error_can_cur_time_error,							// 获取当前时间时发生错误
		e_legion_error_can_not_hold_banquet_in_legion_war_day,		// 不能在军团城战的举办日申请举办宴会
		e_legion_error_invalid_hold_banquet_time,					// 当前时间不能举办宴会
		e_legion_error_request_hold_banquet_again,					// 已经申请了宴会的举办
		e_legion_error_already_occupy_stronghold,					// 已经占领了该据点
		e_legion_error_already_get_occupation_daily_award,			// 已经领取了当日的王城占领奖励
		e_legion_error_next_day_get_occupation_daily_award,			// 领取当日王城占领奖励条件不足
		e_legion_error_city_war_gather_crystal_field_not_open,		// 城战水晶采集区域还未开放
		e_legion_error_stronghold_occupied_by_others,				// 城战据点已经被其他军团占领(用来打断自己的操作)
		e_legion_error_crystal_gathered_by_others,					// 城战水晶已经被其他军团采集(用来打断自己的操作)
		e_legion_error_job_title_num_max,							// 要委托的职位已达到上限
		e_legion_error_player_joined_other_legion,					// 玩家已经加入了其他军团
		e_legion_error_is_in_legion_answer_time,					// 在军团答题时间内不允许创建军团
		e_legion_error_max
	};

	enum e_legion_warehouse_error
	{
		e_legion_warehouse_error_none,
		e_legion_unknown_error,                                    //未知错误
		e_legion_cross_server_error,                               //跨服状态不允许存储
		e_legion_not_in_legion_error,                              //没有军团
		e_legion_not_has_item_error,                               //该装备不存在
		e_legion_warehouse_is_full,                                //仓库容量已满
		e_legion_warehouse_item_type_error,                        //要存储的装备 类型必须是武器或防具
		e_legion_warehouse_item_equip_quality_error,               //要存储的装备 必须是紫色以上
		e_legion_warehouse_item_equip_equip_on_error,              //要存储的装备 已经绑定
		e_legion_warehouse_equip_bag_id_full,                      //装备背包已满 不能获取道具
		e_legion_warehouse_score_not_enough,                       //军团仓库积分不够
		e_legion_warehouse_lack_of_authority,                      //权限不足 不能销毁道具
		e_legion_warehouse_none_item_by_destroy,                   //没有道具可被销毁
		e_legion_warehouse_item_tempelete_none,                    //物品表里面没有配置该道具
		e_legion_warehouse_max
	};

	enum e_legion_daily_rwd_flag
	{
		// 0到29给城战预留着
		e_legion_daily_rwd_flag_daily_everyone = 30,
		e_legion_daily_rwd_flag_bonfire,
	};

	enum e_legion_gm_order
	{
		e_legion_gm_order_money,//军团资金
		e_legion_gm_order_achievement,//战功
		e_legion_gm_order_emblem,//生命之徽记
		e_legion_gm_order_item1,//秘银
		e_legion_gm_order_item2,//黄铜
		e_legion_gm_order_item3,//白金
		e_legion_gm_order_item4,//暗钢
		e_legion_gm_order_max
	};

	enum e_legion_get_contribution_type
	{
		e_legion_get_contribution_type_donate_money,
		e_legion_get_contribution_type_donate_item,
		e_legion_get_contribution_type_fuel_bonfire,
		e_legion_get_contribution_type_city_war,
		e_legion_get_contribution_type_legion_boss,
		e_legion_get_contribution_type_add_glory_glow,
		e_legion_get_contribution_type_max
	};

	enum e_legion_act_type
	{
		e_legion_act_type_raid,				// 军团副本
		e_legion_act_type_bonfire,			// 篝火
		e_legion_act_type_cross_boss,		// 跨服世界boss
		e_legion_act_type_max,
	};

	enum e_legion_rank_type
	{
		e_legion_rank_type_fighting_power,	// 军团战力
		e_legion_rank_type_city_war,		// 城战
		e_legion_rank_type_world_elite,		// 世界精英
		e_legion_rank_type_world_boss,		// 世界boss
		e_legion_rank_type_raid,			// 军团副本
		e_legion_rank_type_bonfire,			// 篝火
		e_legion_rank_type_cross_world_boss,// 跨服世界boss
		e_legion_rank_type_max,
	};
	enum e_legion_warehouse_operation_type
	{
		e_legion_warehouse_operation_store,    //存入
		e_legion_warehouse_operation_out,      //取出
		e_legion_warehouse_operation_destroy,  //销毁道具
	};
	
	struct s_legion_member_attend_activity
	{
		guid_64 mem_legion_guid;
		guid_64 mem_guid;
		int64   mem_score;
		int64   mem_rank_num;
		s_legion_member_attend_activity()
		{
			clear_data();
		}
		void clear_data() 
		{
			memset(this, 0, sizeof(s_legion_member_attend_activity));
		}
	};
	typedef std::map<guid_64, s_legion_member_attend_activity> legion_act_attend_mems_map;

	
	struct s_legion_rank_info
	{
		guid_64				legion_guid;
		xchar				legion_name[max_name_size + 1];
		guid_64				chief_guid;
		xchar				chief_name[max_name_size + 1];
		int64				legion_score;

		s_legion_rank_info()
		{
			clear_data();
		}
		void clear_data()
		{
			memset(this, 0, sizeof(s_legion_rank_info));
		}
	};
	//typedef std::list<s_legion_rank_info> legion_rank_list;
	typedef std::list<s_ranking_player_info> legion_rank_list;

	struct s_legion_bonus_info
	{
		guid_64	legion_guid;		
		int32	mission_id;		//任务id
		int32	finish_count;

		s_legion_bonus_info()
		{
			clear_data();
			legion_guid.clear_data();
			mission_id = 0;
		}

		void clear_data()
		{

			finish_count = 0;
		}

		bool is_valid()
		{
			return legion_guid.is_valid();
		}

		bool is_data_valid()
		{
			return mission_id > 0;
		}
	};
	struct s_legion_bonus_info_db
	{
		guid_64					legion_guid;
		s_legion_bonus_info		legion_bonus_info;
	};

	enum e_legion_bonus_type
	{
		e_legion_bonus_type_donate_count,
		e_legion_bonus_type_legion_boss_man,
		e_legion_bonus_type_bonfire_count,
		e_legion_bonus_city_war_count,
		e_legion_bonus_world_elite,
		e_legion_bonus_world_boss,
		e_legion_bonus_cross_world_boss,
		e_legion_bonus_type_max,
	};

	enum e_add_applicant_ret
	{
		e_add_applicant_ok,
		e_add_applicant_legion_full,
		e_add_applicant_repeat,
		e_add_applicant_have_legion,
	};

	const int32 legion_job_name[e_legion_job_title_max] =
	{
		90201036,
		90201039,
		90201038,
		90201037,
	};

	struct s_legion_warehouse_info
	{
		guid_64 legion_guid;
		int64 add_time;
		s_item_info	item_info;
		void clear_data()
		{
			memset(this, 0, sizeof(*this));
			item_info.clear_data();
		}

		s_legion_warehouse_info()
		{
			clear_data();
		}
	};

	struct s_legion_warehouse_log
	{
		guid_64 role_guid;
		guid_64 item_guid;
		guid_64 legion_guid;
		int64 add_time;
		int32 job;
		xchar role_name[max_name_size + 1];
		int32 operation_type;
		int32 item_template_id;
		void clear_data()
		{
			memset(this, 0, sizeof(*this));
		}
		s_legion_warehouse_log()
		{
			clear_data();
		}
	};

	enum e_legion_name_error
	{
		e_legion_name_error_change_name_succeed, //更改军团名成功
		e_legion_name_error_not_chief,			//玩家不是军团长
		e_legion_name_error_formal_error,		//军团名格错误
		e_legion_name_error_repetition,			//军团名重复
		e_legion_name_error_contains_illegal_characters,	//军团名包含违规字符
		e_legion_name_error_cross_server,		//跨服状态下不允许修改
		e_legion_name_error_available,			//军团名可以更改
		e_legion_name_error_player_not_legion,	//玩家没有军团
		e_legion_name_error_item_not_enough,    //军团改名卡数量不足		
	};

	enum e_change_cross_city_type
	{
		e_change_cross_city_type_change_chief,//改军团长
		e_change_cross_city_type_change_legion_name,//改军团名
		e_change_cross_city_type_del_legion,//解散军团
	};
	
	enum e_legion_use_contribution_type
	{
		e_legion_use_contribution_type_city_war,		//城战奖励
		e_legion_use_contribution_type_legion_welfare,	//军团福利
	};

	struct s_cross_db_city_info
	{
		guid_64 legion_guid;
		int32	server_id;
		guid_64	chief_guid;
		int32	legion_rank;
		xchar	legion_name[max_name_size + 1];
		xchar	chief_name[max_name_size + 1];
		void clear_data()
		{
			memset(this, 0, sizeof(*this));
		}

		s_cross_db_city_info()
		{
			clear_data();
		}

		void set_legion_name(xstring role_name)
		{
			memset(legion_name, 0, sizeof(legion_name));
			memcpy(legion_name, role_name.c_str(), role_name.size() > sizeof(legion_name) ? sizeof(legion_name) : role_name.size());
		}
		void set_chief_name(xstring role_name)
		{
			memset(chief_name, 0, sizeof(chief_name));
			memcpy(chief_name, role_name.c_str(), role_name.size() > sizeof(chief_name) ? sizeof(chief_name) : role_name.size());
		}

		bool is_vaild()
		{
			return legion_guid.is_valid();
		}
	};

	struct s_cross_city_info
	{
		guid_64 legion_guid;
		int32	server_id;
		guid_64	chief_guid;
		xchar	legion_name[max_name_size + 1];
		xchar	chief_name[max_name_size + 1];
		void clear_data()
		{
			memset(this, 0, sizeof(*this));
		}

		s_cross_city_info()
		{
			clear_data();
		}

		void set_legion_name(xstring role_name)
		{
			memset(legion_name, 0, sizeof(legion_name));
			memcpy(legion_name, role_name.c_str(), role_name.size() > sizeof(legion_name) ? sizeof(legion_name) : role_name.size());
		}
		void set_chief_name(xstring role_name)
		{
			memset(chief_name, 0, sizeof(chief_name));
			memcpy(chief_name, role_name.c_str(), role_name.size() > sizeof(chief_name) ? sizeof(chief_name) : role_name.size());
		}

		bool is_vaild()
		{
			return legion_guid.is_valid();
		}
	};
	struct territory_war_bid_info
	{
		s_city_war_bid_info	bid_infos[cross_server_territory_city_bid_info_max];
		int32 bid_infos_num;
		void clear_data()
		{
			memset(this, 0, sizeof(*this));
		}

		territory_war_bid_info()
		{
			clear_data();
		}
	};

	struct s_city_war_all_member_info
	{
		guid_64 legion_guid;
		guid_64	all_member_guid[max_legion_member_num];
		void clear_data()
		{
			memset(this, 0, sizeof(*this));
		}
		s_city_war_all_member_info()
		{
			clear_data();
		}
	};
#pragma pack(pop)
}

#endif