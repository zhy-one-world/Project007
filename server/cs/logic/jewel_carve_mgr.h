#ifndef  JEWEL_CARVE_MGR_H
#define  JEWEL_CARVE_MGR_H

#include "logic/type_def.hpp"


namespace faith
{
	const int32 jewel_carve_error_sucess = 0;
	const int32 jewel_carve_error_unknow = -99;
	const int32 jewel_carve_errror_open_level = -1;
	const int32 jewel_carve_error_need_jewel_num = -2;
	const int32 jewel_carve_error_config = -3;
	const int32 jewel_carve_error_cost_item = -4;
	const int32 jewel_carve_error_not_enough_lucky = -5;
	const int32 jewel_carve_error_not_exist_skill_id = -6;
	


	enum e_jewel_carve_oper
	{
		e_jewel_carve_oper_random,  //随机技能
		e_jewel_carve_oper_up,     //上位
		e_jewel_carve_oper_cancel,   //取消
		e_jewel_carve_oper_choose, //幸运选择
	};

	enum e_jewel_carve_param
	{
		e_jewel_carve_param_open_level,
		e_jewel_carve_param_lucky_max,
		e_jewel_carve_param_need_jewel_num,
		e_jewel_carve_param_random_open_2,  //95,
		e_jewel_carve_param_random_to_2,   //50,
		e_jewel_carve_param_max,
	};
	enum e_jewel_carve_real_type
	{
		e_jewel_carve_real_type_add_attr = 1,
		e_jewel_carve_real_type_jewel_num,
		e_jewel_carve_real_type_level_sum,

	};


	class player;
	class citem;
	struct JewelCarveTemplate;
	

	class jewel_carve_mgr
	{
	public:
		jewel_carve_mgr();
		~jewel_carve_mgr();
	public:
		void clear_data();
		void set_player_ptr(const int32 array_index);
	public:
		void  on_oper(guid_64 item_guid, int32 type, int32 param);
		void  oper_result(player &player_ref, guid_64 item_guid, int32 type, int32 result);
		int32 on_random(player &player_ref, guid_64 item_guid);
		int32 on_up(player &player_ref,guid_64 item_guid);
		int32 on_up_equ_skill_idx(player &player_ref, citem *item_ptr, int32 idx);
		int32 on_cancel(player &player_ref, guid_64 item_guid);
		int32 on_choose(player &player_ref,guid_64 item_guid, int32 param);
	public:
		int32 get_config_param(e_jewel_carve_param e_type);
		int32 get_jewel_open_on_num(player &player_ref,guid_64 item_guid);
		static JewelCarveTemplate *get_jewel_carve_tmplate_by_id(int32 jewel_carve_id);

		JewelCarveTemplate* get_jewel_carve_tmplate_by_guid_id(guid_64 item_guid);

		bool  is_add_lucky_same_skill(citem *item_ptr);
		int32 is_high_replace_low(citem *item_ptr);
		bool  is_exist_skill(const std::vector<int32>& random_skill_set, int32 skill_id);


		int32 get_skill_num(citem *item_ptr);


		bool  is_can_inherit(citem* strip_item_ptr, citem* inherited_item_ptr);

		void  set_inherit(citem* strip_item_ptr, citem* inherited_item_ptr);

		int32 inherit_skill_num(citem* item_ptr);
	public:
		void  set_skill(citem *item_ptr, bool is_add);
		int32 get_jewel_num(citem *item_ptr);
		int32 get_jewel_level_sum(citem *item_ptr);
		void  set_skill_attribute(citem *citem_ptr, std::vector<int32>& RealValue,  bool is_add);
		void  set_skill_jewel_num(citem *citem_ptr, std::vector<int32>& RealValue, bool is_add);
		void  set_skill_level_sum(citem *citem_ptr, std::vector<int32>& RealValue, bool is_add);
		int64 get_jewel_fight(citem *item_ptr, const double* att_array);
		int64 get_skill_attribute_fight(citem *citem_ptr, std::vector<int32>& RealValue, const double* att_array);
		int64 get_skill_jewel_num_fight(citem *citem_ptr, std::vector<int32>& RealValue, const double* att_array);
		int64 get_skill_level_num_fight(citem *citem_ptr, std::vector<int32>& RealValue, const double* att_array);
	public:
		static bool my_greater(int32 skill_id_a, int32 skill_id_b);
		void  get_random_skill(std::vector<int32>& vec_skill_set_id, int32& skill_id1, int32& skill_id2, int32 open_num);



	   

	private:
		int32  m_array_index;


	};

}














#endif