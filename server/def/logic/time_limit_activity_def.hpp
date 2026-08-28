/********************************************************************
created: 2018/07/28
file base: time_limit_activity_def
file ext: hpp
author: shangxiaoliang

purpose:
*********************************************************************/
#ifndef _TIME_LIMIT_ACTIVITY_DEF_HPP_
#define _TIME_LIMIT_ACTIVITY_DEF_HPP_
#include "logic/type_def.hpp"
#include "char_def.hpp"

namespace hld
{
#pragma pack(push,1)
	const int32 time_limit_activity_schedule_num = 20;							//进度记录最大条目
	const int32 time_limit_activity_get_reward_flag_index_max = 64;				//奖励标记位最大个数
	const int32 time_limit_activity_common_data_max = 5;						//限时活动公用数据最大个数
	const int32 max_limit_act_temp_size = 512;									//限时活动表数据最大长度
	const int32 max_load_time_limit_temp_num = 30;								//限时活动表单次load最大数量
	const int32 max_save_num_with_one_player = 300;								//记录玩家的最大条数
	const int32 max_send_cs2ws_once_num = 20;									//同步给WS单次最大数目
	const int32 time_limit_day_mark_num = 10000000;								//限时活动表格时间标记前两位
	const int32 time_limit_server_mark_num = 10000;								//限时活动表格服务器标记中间三位
	const int32 time_limit_activity_added_drop_drop_template_id = 70999999;		//限时活动额外掉落掉落包id
	const int32 time_limit_activity_param_string_max_size = 3200;				//字符串参数最大长度
	const int32 time_limit_activity_string_max_size = 600;						//cs存储字符串参数最大长度
	const int32 time_limit_activity_dp_send_size = 50;							//dp显示活动发送数据
	const int32 time_feed_back_to_db_num = 30;									//时光回馈存储大小	

	enum e_time_limit_activity_cycle_param_type
	{
		e_time_limit_activity_cycle_param_type_none,
		e_time_limit_activity_cycle_param_type_num,
		e_time_limit_activity_cycle_param_type_server_level,
		e_time_limit_activity_cycle_param_type_max
	};

	enum e_time_limit_activity_open_type
	{
		e_time_limit_activity_open_type_none,
		e_time_limit_activity_open_type_server_date,
		e_time_limit_activity_open_type_spencific_date,
		e_time_limit_activity_open_type_cross_server,
		e_time_limit_activity_open_type_merge_server,
		e_time_limit_activity_open_type_max
	};

	enum e_compare_temp_result
	{
		e_compare_temp_result_error,// 比对出现错误，表为空
		e_compare_temp_result_new,//需要加新表
		e_compare_temp_result_change,//需要换表
		e_compare_temp_result_none,//不做处理
		e_compare_temp_result_refresh_guid,
		e_compare_temp_result_max
	};

	enum e_time_limit_ws_act_info
	{
		e_time_limit_ws_act_info_type,
		e_time_limit_ws_act_info_act_guid,
		e_time_limit_ws_act_info_refresh_time,
		e_time_limit_ws_act_info_branch_temp_id,
		e_time_limit_ws_act_info_schedule,
		e_time_limit_ws_act_info_flag,
		e_time_limit_ws_act_info_max
	};

	enum e_time_limit_template_from_type
	{
		e_time_limit_template_from_type_time_limit_activity,
		e_time_limit_template_from_type_operator_use,
		e_time_limit_template_from_type_max
	};

	enum e_time_limit_activity_delete_type
	{
		e_time_limit_activity_delete_type_normal,
		e_time_limit_activity_delete_type_operator_use,
		e_time_limit_activity_delete_type_time_limit_activity,
		e_time_limit_activity_delete_type_all,
		e_time_limit_activity_delete_type_max
	};

	enum e_time_limit_activity_string_param_index
	{
		e_time_limit_activity_string_param_index_1,
		e_time_limit_activity_string_param_index_2,
		e_time_limit_activity_string_param_index_max
	};

	struct s_time_limit_activity_info
	{
		int32 data_ary[e_time_limit_activity_info_max];
		int32 activity_schedule[time_limit_activity_schedule_num];
		xchar schedule_str[time_limit_activity_string_max_size + 1];		
		int64 reward_flag; 
		s_time_limit_activity_info()
		{
			clear_data();
		}

		void clear_data()
		{
			memset(this, 0, sizeof(*this));
			//默认类型是-1 没有这个类型 判断时不会出错
			data_ary[e_time_limit_activity_info_type] = -1;
		}
		void set_schedule_str(xstring temp_name)
		{
			memset(schedule_str, 0, sizeof(schedule_str));
			memcpy(schedule_str, temp_name.c_str(), temp_name.size() > time_limit_activity_string_max_size ? time_limit_activity_string_max_size : temp_name.size());
		}
	};

	struct s_time_limit_activity_info_db
	{
		guid_64 role_guid;
		s_time_limit_activity_info data_info;
		s_time_limit_activity_info_db()
		{
			clear_data();
		}
		void clear_data()
		{
			data_info.clear_data();
			memset(this, 0, sizeof(*this));
		}
		
	};


	struct s_time_limit_activity_branch_temp_db
	{
		int32				ActGuid;
		int32				AttributeId;
		int32				ActivityType;
		int32				CycleIndex;
		xchar				Condition[max_limit_act_temp_size];
		xchar				Reward[max_limit_act_temp_size];
		xchar				TextImage[max_limit_act_temp_size];
		xchar				ParamArr1[max_limit_act_temp_size];
		xchar				ParamArr2[max_limit_act_temp_size];
		xchar				ParamArr3[max_limit_act_temp_size];
		xchar				ParamArr4[max_limit_act_temp_size];
		xchar				ParamArr5[max_limit_act_temp_size];
		s_time_limit_activity_branch_temp_db()
		{
			clear_data();
		}
		void set_Condition(xstring _string)
		{
			memset(Condition, 0, sizeof(Condition));
			memcpy(Condition, _string.c_str(), _string.size() > sizeof(Condition) ? sizeof(Condition) : _string.size());
		}
		void set_Reward(xstring _string)
		{
			memset(Reward, 0, sizeof(Reward));
			memcpy(Reward, _string.c_str(), _string.size() > sizeof(Reward) ? sizeof(Reward) : _string.size());
		}
		void set_TextImage(xstring _string)
		{
			memset(TextImage, 0, sizeof(TextImage));
			memcpy(TextImage, _string.c_str(), _string.size() > sizeof(TextImage) ? sizeof(TextImage) : _string.size());
		}
		void set_ParamArr5(xstring _string)
		{
			memset(ParamArr5, 0, sizeof(ParamArr5));
			memcpy(ParamArr5, _string.c_str(), _string.size() > sizeof(ParamArr5) ? sizeof(ParamArr5) : _string.size());
		}
		void set_ParamArr4(xstring _string)
		{
			memset(ParamArr4, 0, sizeof(ParamArr4));
			memcpy(ParamArr4, _string.c_str(), _string.size() > sizeof(ParamArr4) ? sizeof(ParamArr4) : _string.size());
		}
		void set_ParamArr3(xstring _string)
		{
			memset(ParamArr3, 0, sizeof(ParamArr3));
			memcpy(ParamArr3, _string.c_str(), _string.size() > sizeof(ParamArr3) ? sizeof(ParamArr3) : _string.size());
		}
		void set_ParamArr2(xstring _string)
		{
			memset(ParamArr2, 0, sizeof(ParamArr2));
			memcpy(ParamArr2, _string.c_str(), _string.size() > sizeof(ParamArr2) ? sizeof(ParamArr2) : _string.size());
		}
		void set_ParamArr1(xstring _string)
		{
			memset(ParamArr1, 0, sizeof(ParamArr1));
			memcpy(ParamArr1, _string.c_str(), _string.size() > sizeof(ParamArr1) ? sizeof(ParamArr1) : _string.size());
		}
		void clear_data()
		{
			memset(this, 0, sizeof(*this));
		}
		bool  to_proto(hld::st_proto::st_time_limit_activity_branch_temp_db* st_act_branch_ptr)
		{
			if (st_act_branch_ptr == nullptr)
			{
				return false;
			}
			st_act_branch_ptr->set_actguid(ActGuid);		
			st_act_branch_ptr->set_attributeid(AttributeId);			
			st_act_branch_ptr->set_activitytype(ActivityType);			
			st_act_branch_ptr->set_cycleindex(CycleIndex);			
			st_act_branch_ptr->set_condition(Condition);			
			st_act_branch_ptr->set_reward(Reward);		
			st_act_branch_ptr->set_textimage(TextImage);			
			st_act_branch_ptr->set_paramarr1(ParamArr1);		
			st_act_branch_ptr->set_paramarr2(ParamArr2);		
			st_act_branch_ptr->set_paramarr3(ParamArr3);		
			st_act_branch_ptr->set_paramarr4(ParamArr4);		
			st_act_branch_ptr->set_paramarr5(ParamArr5);			
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
		void from_proto(const hld::st_proto::st_time_limit_activity_branch_temp_db& st_act_branch_ref)
		{
			ActGuid = st_act_branch_ref.actguid();
			AttributeId = st_act_branch_ref.attributeid();
			ActivityType = st_act_branch_ref.activitytype();
			CycleIndex = st_act_branch_ref.cycleindex();
			my_memcopy_string(Condition, max_limit_act_temp_size, st_act_branch_ref.condition());
			my_memcopy_string(Reward, max_limit_act_temp_size, st_act_branch_ref.reward());
			my_memcopy_string(TextImage, max_limit_act_temp_size, st_act_branch_ref.textimage());
			my_memcopy_string(ParamArr1, max_limit_act_temp_size, st_act_branch_ref.paramarr1());
			my_memcopy_string(ParamArr2, max_limit_act_temp_size, st_act_branch_ref.paramarr2());
			my_memcopy_string(ParamArr3, max_limit_act_temp_size, st_act_branch_ref.paramarr3());
			my_memcopy_string(ParamArr4, max_limit_act_temp_size, st_act_branch_ref.paramarr4());
			my_memcopy_string(ParamArr5, max_limit_act_temp_size, st_act_branch_ref.paramarr5());
		}
	};

	struct s_time_limit_activity_temp_db
	{
		int32				ActGuid;
		int32				AttributeId;
		int32				ActivityType;
		int32				CycleParamType;
		xchar				CycleParamNum[max_limit_act_temp_size];
		xchar				CycleFirstId[max_limit_act_temp_size];
		int32				OpenType;
		int32				OpenServerDate;
		xchar				OpenSpencificDate[max_limit_act_temp_size];
		int32				FirstOpenDayOfWeek;
		int32				DurationDays;
		int32				CycleDays;
		int32				ShowType;
		xchar				TextImage[max_limit_act_temp_size];
		xchar				ParamArr1[max_limit_act_temp_size];
		xchar				ParamArr2[max_limit_act_temp_size];
		xchar				ParamArr3[max_limit_act_temp_size];
		xchar				ParamArr4[max_limit_act_temp_size];
		xchar				ParamArr5[max_limit_act_temp_size];
		int32				IsEndAct;
		xchar				OriginalCycleFirstId[max_limit_act_temp_size];
		xchar				OldCycleFirstId[max_limit_act_temp_size];
		int32				CreateTime;
		int32				EndTime;
		int32				TemplateFromType;
		int32				RefreshTime;
		s_time_limit_activity_temp_db()
		{
			clear_data();
		}
		void set_ParamArr5(xstring _string)
		{
			memset(ParamArr5, 0, sizeof(ParamArr5));
			memcpy(ParamArr5, _string.c_str(), _string.size() > sizeof(ParamArr5) ? sizeof(ParamArr5) : _string.size());
		}
		void set_ParamArr4(xstring _string)
		{
			memset(ParamArr4, 0, sizeof(ParamArr4));
			memcpy(ParamArr4, _string.c_str(), _string.size() > sizeof(ParamArr4) ? sizeof(ParamArr4) : _string.size());
		}
		void set_ParamArr3(xstring _string)
		{
			memset(ParamArr3, 0, sizeof(ParamArr3));
			memcpy(ParamArr3, _string.c_str(), _string.size() > sizeof(ParamArr3) ? sizeof(ParamArr3) : _string.size());
		}
		void set_ParamArr2(xstring _string)
		{
			memset(ParamArr2, 0, sizeof(ParamArr2));
			memcpy(ParamArr2, _string.c_str(), _string.size() > sizeof(ParamArr2) ? sizeof(ParamArr2) : _string.size());
		}
		void set_ParamArr1(xstring _string)
		{
			memset(ParamArr1, 0, sizeof(ParamArr1));
			memcpy(ParamArr1, _string.c_str(), _string.size() > sizeof(ParamArr1) ? sizeof(ParamArr1) : _string.size());
		}
		void set_TextImage(xstring _string)
		{
			memset(TextImage, 0, sizeof(TextImage));
			memcpy(TextImage, _string.c_str(), _string.size() > sizeof(TextImage) ? sizeof(TextImage) : _string.size());
		}
		void set_OpenSpencificDate(xstring _string)
		{
			memset(OpenSpencificDate, 0, sizeof(OpenSpencificDate));
			memcpy(OpenSpencificDate, _string.c_str(), _string.size() > sizeof(OpenSpencificDate) ? sizeof(OpenSpencificDate) : _string.size());
		}
		void set_CycleFirstId(xstring _string)
		{
			memset(CycleFirstId, 0, sizeof(CycleFirstId));
			memcpy(CycleFirstId, _string.c_str(), _string.size() > sizeof(CycleFirstId) ? sizeof(CycleFirstId) : _string.size());
		}
		void set_CycleParamNum(xstring _string)
		{
			memset(CycleParamNum, 0, sizeof(CycleParamNum));
			memcpy(CycleParamNum, _string.c_str(), _string.size() > sizeof(CycleParamNum) ? sizeof(CycleParamNum) : _string.size());
		}
		void set_OriginalCycleFirstId(xstring _string)
		{
			memset(OriginalCycleFirstId, 0, sizeof(OriginalCycleFirstId));
			memcpy(OriginalCycleFirstId, _string.c_str(), _string.size() > sizeof(OriginalCycleFirstId) ? sizeof(OriginalCycleFirstId) : _string.size());
		}
		void set_OldCycleFirstId(xstring _string)
		{
			memset(OldCycleFirstId, 0, sizeof(OldCycleFirstId));
			memcpy(OldCycleFirstId, _string.c_str(), _string.size() > sizeof(OldCycleFirstId) ? sizeof(OldCycleFirstId) : _string.size());
		}
		void clear_data()
		{
			memset(this, 0, sizeof(*this));
		}
		bool to_proto(hld::st_proto::st_time_limit_activity_temp_db* st_act_ptr)
		{
			if (st_act_ptr == nullptr)
			{
				return false;
			}
			st_act_ptr->set_actguid(ActGuid);
			st_act_ptr->set_attributeid(AttributeId);	
			st_act_ptr->set_activitytype(ActivityType);	
			st_act_ptr->set_cycleparamtype(CycleParamType);				
			st_act_ptr->set_cycleparamnum(CycleParamNum);		
			st_act_ptr->set_cyclefirstid(CycleFirstId);		
			st_act_ptr->set_opentype(OpenType);	
			st_act_ptr->set_openserverdate(OpenServerDate);			
			st_act_ptr->set_openspencificdate(OpenSpencificDate);		
			st_act_ptr->set_firstopendayofweek(FirstOpenDayOfWeek);		
			st_act_ptr->set_durationdays(DurationDays);			
			st_act_ptr->set_cycledays(CycleDays);	
			st_act_ptr->set_showtype(ShowType);
			st_act_ptr->set_textimage(TextImage);				
			st_act_ptr->set_paramarr1(ParamArr1);	
			st_act_ptr->set_paramarr2(ParamArr2);
			st_act_ptr->set_paramarr3(ParamArr3);
			st_act_ptr->set_paramarr4(ParamArr4);
			st_act_ptr->set_paramarr5(ParamArr5);
			st_act_ptr->set_isendact(IsEndAct);
			st_act_ptr->set_originalcyclefirstid(OriginalCycleFirstId);				
			st_act_ptr->set_oldcyclefirstid(OldCycleFirstId);				
			st_act_ptr->set_createtime(CreateTime);			
			st_act_ptr->set_endtime(EndTime);
			st_act_ptr->set_templatefromtype(TemplateFromType);			
			st_act_ptr->set_refreshtime(RefreshTime);
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
		void from_proto(const hld::st_proto::st_time_limit_activity_temp_db& st_act_ref)
		{
			ActGuid = st_act_ref.actguid();
			AttributeId = st_act_ref.attributeid();
			ActivityType = st_act_ref.activitytype();
			CycleParamType = st_act_ref.cycleparamtype();

			my_memcopy_string(CycleParamNum, max_limit_act_temp_size, st_act_ref.cycleparamnum());
			my_memcopy_string(CycleFirstId, max_limit_act_temp_size, st_act_ref.cyclefirstid());
	
			OpenType = st_act_ref.opentype();
			OpenServerDate = st_act_ref.openserverdate();
			my_memcopy_string(OpenSpencificDate, max_limit_act_temp_size, st_act_ref.openspencificdate());
			FirstOpenDayOfWeek = st_act_ref.firstopendayofweek();
			DurationDays = st_act_ref.durationdays();
			CycleDays = st_act_ref.cycledays();
			ShowType = st_act_ref.showtype();

			my_memcopy_string(TextImage, max_limit_act_temp_size, st_act_ref.textimage());
			my_memcopy_string(ParamArr1, max_limit_act_temp_size, st_act_ref.paramarr1());
			my_memcopy_string(ParamArr2, max_limit_act_temp_size, st_act_ref.paramarr2());
			my_memcopy_string(ParamArr3, max_limit_act_temp_size, st_act_ref.paramarr3());
			my_memcopy_string(ParamArr4, max_limit_act_temp_size, st_act_ref.paramarr4());
			my_memcopy_string(ParamArr5, max_limit_act_temp_size, st_act_ref.paramarr5());
			IsEndAct = st_act_ref.isendact();

			my_memcopy_string(OriginalCycleFirstId, max_limit_act_temp_size, st_act_ref.originalcyclefirstid());
			my_memcopy_string(OldCycleFirstId, max_limit_act_temp_size, st_act_ref.oldcyclefirstid());

			CreateTime = st_act_ref.createtime();
			EndTime = st_act_ref.endtime();
			TemplateFromType = st_act_ref.templatefromtype();
			RefreshTime = st_act_ref.refreshtime();

		}
	};

	struct s_time_limit_activity_temp
	{
		int32				ActGuid;
		int32				AttributeId;
		int32				ActivityType;
		int32				CycleParamType;
		std::vector<int32>	CycleParamNum;
		std::vector<int32>	CycleFirstId;
		int32				OpenType;
		int32				OpenServerDate;
		std::vector<int32>	OpenSpencificDate;
		int32				FirstOpenDayOfWeek;
		int32				DurationDays;
		int32				CycleDays;
		int32				ShowType;
		std::vector<int32>	TextImage;
		std::vector<int32>	ParamArr1;
		std::vector<int32>	ParamArr2;
		std::vector<int32>	ParamArr3;
		std::vector<int32>	ParamArr4;
		std::vector<int32>	ParamArr5;
		int32				IsEndAct;//是否通过GM指令以及其他原因终止该活动
		std::vector<int32>	OriginalCycleFirstId;
		std::vector<int32>	OldCycleFirstId;
		int32				CreateTime;
		int32				EndTime;
		int32				TemplateFromType;
		int32				RefreshTime;
		s_time_limit_activity_temp()
		{
			clear_data();
		}

		void clear_data()
		{
			memset(this, 0, sizeof(*this));
			CycleParamNum.clear();
			CycleFirstId.clear();
			OpenSpencificDate.clear();
			TextImage.clear();
			ParamArr1.clear();
			ParamArr2.clear();
			ParamArr3.clear();
			ParamArr4.clear();
			ParamArr5.clear();
		}
		bool is_valid() const
		{
			return ActGuid > 0;
		}
	};

	struct s_time_limit_activity_branch_temp
	{
		int32				ActGuid;
		int32				AttributeId;
		int32				ActivityType;
		int32				CycleIndex;
		std::vector<int32>	Condition;
		std::vector<int32>	Reward;
		std::vector<int32>	TextImage;
		std::vector<int32>	ParamArr1;
		std::vector<int32>	ParamArr2;
		std::vector<int32>	ParamArr3;
		std::vector<int32>	ParamArr4;
		std::vector<int32>	ParamArr5;
		s_time_limit_activity_branch_temp()
		{
			clear_data();
		}

		void clear_data()
		{
			memset(this, 0, sizeof(*this));
			Condition.clear();
			Reward.clear();
			TextImage.clear();
			ParamArr1.clear();
			ParamArr2.clear();
			ParamArr3.clear();
			ParamArr4.clear();
			ParamArr5.clear();
		}

		bool is_valid() const
		{
			return ActGuid > 0;
		}
	};

	struct s_time_limit_ws_info
	{
		int32			act_info[e_time_limit_ws_act_info_max];
		xchar			param_string_1[time_limit_activity_param_string_max_size];
		xchar			param_string_2[time_limit_activity_param_string_max_size];
		s_time_limit_ws_info()
		{
			clear_data();
		}
		void clear_data()
		{
			memset(this, 0, sizeof(*this));
		}
		bool is_valid()
		{
			return act_info[e_time_limit_ws_act_info_type] >= 0 && act_info[e_time_limit_ws_act_info_type] < e_time_limit_activity_type_max;
		}
	};


	struct s_time_limit_activity_ws_info
	{
		int32 open_time;
		int32 ranking_type;
		int32 cycle_num;
		int32 cycle_index;
		s_time_limit_ws_info info_data;
		s_time_limit_activity_ws_info()
		{
			clear_data();
		}

		void clear_data()
		{
			memset(this, 0, sizeof(*this));
			set_activity_info(e_time_limit_ws_act_info_type, -1);
			ranking_type = e_RankingIndex_max;
		}

		s_time_limit_ws_info get_ws_db_info()
		{
			return info_data;
		}

		void set_ws_db_info(const s_time_limit_ws_info& s_info)
		{
			info_data = s_info;
		}

		int32 get_activity_info(e_time_limit_ws_act_info e_type)
		{
			if (e_type < 0 || e_type >= e_time_limit_ws_act_info_max)
			{
				return 0;
			}
			return info_data.act_info[e_type];
		}

		void set_activity_info(e_time_limit_ws_act_info e_type, int32 value)
		{
			if (e_type < 0 || e_type >= e_time_limit_ws_act_info_max)
			{
				return;
			}
			info_data.act_info[e_type] = value;
		}

		void add_activity_info(e_time_limit_ws_act_info e_type, int32 add_value)
		{
			if (e_type < 0 || e_type >= e_time_limit_ws_act_info_max)
			{
				return;
			}
			info_data.act_info[e_type] += add_value;
		}

		xstring get_activity_string_param(int32 param_index)
		{
			switch (param_index)
			{
			case e_time_limit_activity_string_param_index_1:
			{
				return info_data.param_string_1;
			}
				break;
			case e_time_limit_activity_string_param_index_2:
			{
				return info_data.param_string_2;
			}
			break;
			default:
				break;
			}
			return "";
		}

		void set_activity_string_param(const xstring& param_string, int32 param_index)
		{
			clear_activity_string_param(param_index);
			switch (param_index)
			{
			case e_time_limit_activity_string_param_index_1:
			{
				memcpy(info_data.param_string_1, param_string.c_str(), param_string.size() >= time_limit_activity_param_string_max_size ? time_limit_activity_param_string_max_size : param_string.size());
			}
			break;
			case e_time_limit_activity_string_param_index_2:
			{
				memcpy(info_data.param_string_2, param_string.c_str(), param_string.size() >= time_limit_activity_param_string_max_size ? time_limit_activity_param_string_max_size : param_string.size());
			}
			break;
			default:
				break;
			}
		}

		void clear_activity_string_param(int32 param_index = -1)
		{
			if (param_index == -1)
			{
				memset(info_data.param_string_1, 0, time_limit_activity_param_string_max_size);
				memset(info_data.param_string_2, 0, time_limit_activity_param_string_max_size);
				return;
			}
			switch (param_index)
			{
			case e_time_limit_activity_string_param_index_1:
			{
				memset(info_data.param_string_1, 0, time_limit_activity_param_string_max_size);
			}
			break;
			case e_time_limit_activity_string_param_index_2:
			{
				memset(info_data.param_string_2, 0, time_limit_activity_param_string_max_size);
			}
			break;
			default:
				break;
			}
		}

		void clear_all_activity_info()
		{
			int32 activity_type_temp = get_activity_info(e_time_limit_ws_act_info_type);
			int32 activity_guid_temp = get_activity_info(e_time_limit_ws_act_info_act_guid);
			int32 ranking_type_temp = ranking_type;

			clear_data();

			set_activity_info(e_time_limit_ws_act_info_type, activity_type_temp);
			set_activity_info(e_time_limit_ws_act_info_act_guid, activity_guid_temp);
			ranking_type = ranking_type_temp;
		}

		bool is_valid()
		{
			return info_data.is_valid();
		}

		bool is_open()
		{
			return get_activity_info(e_time_limit_ws_act_info_refresh_time) > 0 && get_activity_info(e_time_limit_ws_act_info_branch_temp_id) > 0
				&& get_activity_info(e_time_limit_ws_act_info_act_guid) > 0;
		}
	};

	struct s_time_feed_back_info
	{
		int32		template_id;			// 活动主表Id	
		int32		sub_template_id;		// 活动主表Id	
		int32		info_state;				// 活动状态
		int32		get_time;				// 可领取奖励时间
		int32		end_time;				// 活动结束时间
		int32		money_type;				// 奖励货币Id
		int32		money_num;				// 奖励货币数量
		s_time_feed_back_info()
		{
			memset(this, 0, sizeof(*this));
		}

		void clear_data()
		{
			memset(this, 0, sizeof(*this));
		}
		bool is_valid()
		{
			return template_id > 0 && sub_template_id > 0;
		}
	};

	struct s_time_feed_back_db_info
	{
		guid_64					role_guid;
		s_time_feed_back_info	_info;
		s_time_feed_back_db_info()
		{
			memset(this, 0, sizeof(*this));
		}

		void clear_data()
		{
			memset(this, 0, sizeof(*this));
		}
	};

	enum e_time_feed_back_info_state
	{
		e_time_feed_back_info_state_no_buy,				// 未购买
		e_time_feed_back_info_state_buy,				// 已购买
		e_time_feed_back_info_state_receive_award,		// 已领取
		e_time_feed_back_info_state_end,
	};

	enum e_time_feed_back_operation_type
	{
		e_time_feed_back_operation_type_buy,			// 购买
		e_time_feed_back_operation_type_receive_award,	// 领取
	};

	enum e_time_feed_back_operation_end_type
	{
		e_time_feed_back_operation_end_buy_succeed,		// 购买成功
		e_time_feed_back_operation_end_get_succeed,		// 领取成功
		e_time_feed_back_operation_end_template_error,	// 表格错误
		e_time_feed_back_operation_end_money_error,		// 购买错误 - 货币不足
		e_time_feed_back_operation_end_item_error,		// 购买错误 - 物品创建失败
		e_time_feed_back_operation_end_time_error,		// 领取错误 - 时间未到
		e_time_feed_back_operation_end_error_01,		// 领取错误 - 错误码01
	};

	enum e_buy_time_limit_item_end_type
	{
		e_buy_time_limit_item_end_type_buy_succeed,		// 购买成功
		e_buy_time_limit_item_end_type_template_error,	// 表格错误
		e_buy_time_limit_item_end_type_money_error,		// 购买错误 - 货币不足
		e_buy_time_limit_item_end_type_item_error,		// 购买失败 - 物品无法创建
		e_buy_time_limit_item_end_type_error_01,		// 领取错误 - 错误码01
	};


	struct s_diamond_shop_info
	{
		int32			goods_index;
		int32			shop_level;
		int32			total_weight;
		bool			is_set;
		s_diamond_shop_info()
		{
			memset(this, 0, sizeof(*this));
		}

		void clear_data()
		{
			memset(this, 0, sizeof(*this));
		}
	};


#pragma pack(pop)
}

#endif
