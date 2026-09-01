
/********************************************************************
created: 2016/8/22
created: 22:8:2016 19:33
file base: ranking_def
file ext: hpp
author: xinglong

purpose:define ranking
*********************************************************************/
#ifndef _RANKING_DEF_HPP_
#define _RANKING_DEF_HPP_

#include "logic/type_def.hpp"
#include "char_def.hpp"
#include "login_def.hpp"

namespace faith
{
#pragma pack(push,1)
	
	const int32 TopMaxNum = 500;//每个排行榜最大人数
	const int32 ShowTopMaxNum = 100;//每个排行榜需要显示最大人数
	const int32 GuidWorshipMax = 3500;//排行榜崇拜值最大数量	原本为2000,这里改为3500,假定7个排行榜角色均不同
	const float FreshTimeInterval = 120.0f;//每个排行榜最大人数
	const int32 DaliyWorshipTime = 10;//排行榜崇拜值最大数量(这里需要与gameconfig中DaliyWorshipTime保持一致)
	const int32 WingOrMountValueRateNum = 9;	//坐骑或翅膀value比率数

	const int32 MinRankingPost = 0;	//最小排名公告
	const int32 MaxRankingPost = 9;//最大排名公告
	
	const int32 LegionBossRankingPara1 = 1000000;	//军团BOSS积分计算参数1
	const int32 LegionBossRankingPara2 = 10000;		//军团BOSS积分计算参数1
	const int32 LegionBossRankingPara3 = 100;		//军团BOSS积分计算参数3

	const int32 service_rank_first_template_id = 133000000;

	const int32 oracle_trial_rank_info = 3;	//天启试炼需要前三的单独显示
	const int32 oracle_trial_first_play_notice_id = 93000295;	//天启试炼第一名公告 无军团
	const int32 oracle_trial_first_play_and_legion_notice_id = 93000294;	 //天启试炼第一名公告 有军团
	enum e_ranking_operation_type
	{
		e_ranking_operation_ranking_type_fighting,			// 战力排行榜     
		e_ranking_operation_ranking_type_max
	};
	 


	enum e_sync_cs2ws_ranking_type
	{
		e_sync_cs2ws_ranking_gs_value,
		e_sync_cs2ws_ranking_level,
		e_sync_cs2ws_ranking_money,
		e_sync_cs2ws_ranking_demonstower,
		e_sync_cs2ws_ranking_wing,
		e_sync_cs2ws_ranking_mount,
		e_sync_cs2ws_ranking_worship,		//威望值应该是不需要同步的
		e_sync_cs2ws_ranking_max
	};



	enum EWorshipEndType
	{
		EWorshipEndType_success = 0,
		EWorshipEndType_time,
		EWorshipEndType_same_person,
		EWorshipEndType_money_not_enough,
		EWorshipEndType_unknown,
		EWorshipEndType_max
	};
	
	enum e_ranking_get_other_player_info	//排行榜获取其他玩家信息
	{
		e_ranking_get_other_player_info_base,
		e_ranking_get_other_player_info_equiping,
		e_ranking_get_other_player_info_buff_equip_arr,
		e_ranking_get_other_player_info_special_name_arr,
		e_ranking_get_other_player_info_group_name,
		e_ranking_get_other_player_info_spirit,
		e_ranking_get_other_player_info_wing,
		e_ranking_get_other_player_info_mount,
		e_ranking_get_other_player_info_buff,
		e_ranking_get_other_player_info_belief,
		e_ranking_get_other_player_info_feather,
		e_ranking_get_other_player_info_max
	};

	///**********************************************************/////

	//db
	struct s_ranking_info_db
	{
		guid_64				role_guid;
		xchar				role_name[max_name_size + 1];
		int64				ranking_type;
		int64				ranking_value;
		int64				time;
		guid_64				supporting_guid;
		xchar				supporting_name[max_name_size + 1];
		int32				vip_level;
		int32				Peak;
		int32				server_id;
		xchar				game_channel[e_login_fixed_data_lenght + 1];
		int64				standby_parameter;
		void set_role_name(xstring p_role_name)
		{
			memset(role_name, 0, sizeof(role_name));
			memset(supporting_name, 0, sizeof(supporting_name));
			memcpy(role_name, p_role_name.c_str(), p_role_name.size() > sizeof(role_name) ? sizeof(role_name) : p_role_name.size());
		}
		s_ranking_info_db()
		{
			memset(this, 0, sizeof(*this));
		}
		void clear_data()
		{
			memset(this, 0, sizeof(*this));
		}
	};

	struct s_ranking_worship_db
	{
		guid_64				role_guid;
		int32				worship;
		int32				role_template_id;
		int32				wing_id;
		int32				mount_id;
		s_ranking_worship_db()
		{
			clear_data();
		}
		void clear_data()
		{
			role_guid = 0;
			worship = 0;
			role_template_id = 0;
			wing_id = 0;
			mount_id = 0;
		}
	};

	struct s_ranking_player_info
	{
		guid_64				role_guid;
		xchar				role_name[max_name_size + 1];
		int64				ranking_type;
		int64				ranking_value;
		int64				save_time;
		guid_64				supporting_guid;						//备用
		xchar				supporting_name[max_name_size + 1];
		int32				vip_level;
		int32				Peak;
		int32				server_id;
		xchar				game_channel[e_login_fixed_data_lenght + 1];
		int64				standby_parameter;						//备用参数
		void set_role_name(xstring p_role_name)
		{
			memset(role_name, 0, sizeof(role_name));
			memcpy(role_name, p_role_name.c_str(), p_role_name.size() > max_name_size ? max_name_size: p_role_name.size());
		}
		void set_supporting_name(xstring p_supporting_name)
		{
			memset(supporting_name, 0, sizeof(supporting_name));
			memcpy(supporting_name, p_supporting_name.c_str(), p_supporting_name.size() > max_name_size ? max_name_size : p_supporting_name.size());
		}
		void set_game_channel(xstring p_game_channel)
		{
			memset(game_channel, 0, sizeof(game_channel));
			memcpy(game_channel, p_game_channel.c_str(), p_game_channel.size() > e_login_fixed_data_lenght ? e_login_fixed_data_lenght : p_game_channel.size());
		}
		s_ranking_player_info()
		{
			clear_data();
		}
		void clear_data()
		{
			memset(this, 0, sizeof(*this));
		}
		void set_data(const s_ranking_info_db& other_data)
		{
			role_guid = other_data.role_guid;
			set_role_name(other_data.role_name);
			ranking_value = other_data.ranking_value;
			save_time = other_data.time;
			ranking_type = other_data.ranking_type;
			supporting_guid = other_data.supporting_guid;
			set_supporting_name(other_data.supporting_name);
			vip_level = other_data.vip_level;
			Peak = other_data.Peak;
			server_id = other_data.server_id;
			set_game_channel(other_data.game_channel);
			standby_parameter = other_data.standby_parameter;
		}

		int compare(s_ranking_player_info& rank_player_info) const
		{
			if (Peak > rank_player_info.Peak)
			{
				return 1;
			}
			else if (Peak < rank_player_info.Peak)
			{
				return -1;
			}
			else
			{
				if (ranking_value > rank_player_info.ranking_value)
				{
					return 1;
				}
				else if (ranking_value < rank_player_info.ranking_value)
				{
					return -1;
				}
				else
				{
					return 0;
				}
			}
		}
		void to_proto(faith::st_proto::st_ranking_player_info * st_rank_ptr)
		{
			if (st_rank_ptr == nullptr)
			{
				return;
			}
			st_rank_ptr->set_role_guid(role_guid.server_64);
			st_rank_ptr->set_role_name(role_name);
			st_rank_ptr->set_ranking_type(ranking_type);
			st_rank_ptr->set_ranking_value(ranking_value);
			st_rank_ptr->set_save_time(save_time);
			st_rank_ptr->set_supporting_guid(supporting_guid.server_64);						//备用
			st_rank_ptr->set_supporting_name(supporting_name);
			st_rank_ptr->set_vip_level(vip_level);
			st_rank_ptr->set_peak(Peak);
			st_rank_ptr->set_server_id(server_id);
		    st_rank_ptr->set_game_channel(game_channel);
			st_rank_ptr->set_standby_parameter(standby_parameter);						//备用参数

		}

		void my_memcopy_string(void* dst, size_t max_len, const xstring &str)
		{
			size_t len = str.size();
			if (max_len < len)
			{
				len = max_len;
			}
			memcpy(dst, str.c_str(), len);
		}
		void from_proto(const faith::st_proto::st_ranking_player_info& st_rank_ref)
		{

			role_guid.server_64 = st_rank_ref.role_guid();
			my_memcopy_string(role_name, max_name_size, st_rank_ref.role_name());

			ranking_type = st_rank_ref.ranking_type();
			ranking_value = st_rank_ref.ranking_value();
			save_time =  st_rank_ref.save_time();
			supporting_guid.server_64 = st_rank_ref.supporting_guid();						//备用
			my_memcopy_string(supporting_name, max_name_size, st_rank_ref.supporting_name());

			vip_level = st_rank_ref.vip_level();
			Peak = st_rank_ref.peak();
			server_id = st_rank_ref.server_id();
			my_memcopy_string(game_channel, e_login_fixed_data_lenght, st_rank_ref.game_channel());
			standby_parameter = st_rank_ref.standby_parameter();
		}

	};


	struct s_ranking_top_info
	{
		s_ranking_player_info		data_ary[TopMaxNum];

		void clear_data()
		{
			for (int32 i = 0; i < TopMaxNum; i++)
			{
				data_ary[i].clear_data();
			}
		}
	};

	struct s_ranking_worship_info
	{
		guid_64						role_guid;
		int32						worship_value;
		int64						save_time;
		void clear_data()
		{
			memset(this, 0, sizeof(s_ranking_worship_info));
		}
	};

	struct s_ranking_guid_wordship_map
	{
		guid_64						role_guid;
		int32						worship;

		s_ranking_guid_wordship_map()
		{
			clear_data();
		}

		void clear_data()
		{
			role_guid = 0;
			worship = 0;
		}
	};


	struct worship_record
	{
		int32						ranking_index;
		guid_64						target_guid;
		worship_record()
		{
			clear_data();
		}
		void clear_data()
		{
			ranking_index = 0;
			target_guid = 0;
		}
	};

	struct worship_record_db
	{
		guid_64						role_guid;
		int32						ranking_index;
		guid_64						target_guid;
		worship_record_db()
		{
			clear_data();
		}
		void clear_data()
		{
			role_guid = 0;
			ranking_index = 0;
			target_guid = 0;
		}
	};

	struct s_service_rank_player_info
	{
		guid_64						role_guid;
		int32						rank_type;
		int32						role_num;
		s_service_rank_player_info()
		{
			clear_data();
		}
		void clear_data()
		{
			role_guid = 0;
			rank_type = 0;
			role_num = 0;
		}
	};

	struct s_service_rank_info
	{
		s_service_rank_player_info	data_ary[ShowTopMaxNum];
		s_service_rank_info()
		{
			clear_data();
		}
		void clear_data()
		{
			for (int32 i = 0; i < ShowTopMaxNum; i++)
			{
				data_ary[i].clear_data();
			}
		}
	};

	struct s_service_rank_reward_state
	{
		int32						rank_type;
		int32						reward_state;
		s_service_rank_reward_state()
		{
			clear_data();
		}
		void clear_data()
		{
			rank_type = 0;
			reward_state = 0;
		}
	};

	struct s_service_rank_reward_state_db
	{
		guid_64						role_guid;
		s_service_rank_reward_state data_info;
		s_service_rank_reward_state_db()
		{
			clear_data();
		}
		void clear_data()
		{
			role_guid = 0;
			data_info.clear_data();
		}
	};

	typedef std::list<s_ranking_player_info>					ranking_list;
	typedef ranking_list::iterator								ranking_list_ite;
	typedef	std::map<ui64,s_service_rank_player_info>			service_rank_map;
	typedef	service_rank_map::iterator							service_rank_map_ite;

#pragma pack(pop)
}

#endif

