/********************************************************************
	created:	2016年8月9日13:12:31
	file base:	globle_data
	file ext:	h
	author:		zhy
	
	purpose:	
*********************************************************************/
#pragma once

#include "logic/type_def.hpp"
#include "Logic/pk_community_def.hpp"
#include "Logic/item_def.hpp"
#include "Logic/mail_def.hpp"
#include "Logic/drop_def.hpp"
#include "Logic/chat_def.hpp"
#include "Logic/auction_def.h"

namespace hld
{
	typedef std::vector<fvector> map_point_vec ;
	struct RegionTemplate;
	struct VersionTemplate;

	enum e_item_mail_type
	{
		e_item_mail_type_no_send,//不发送邮件
		e_item_mail_type_nomal,//默认邮件
		e_item_mail_type_auction_item,//交易行
		e_item_mail_type_auction_cancel_sell_item,//交易行取消出售
		e_item_mail_type_feather_heart_put_bag,//翎羽之心放入背包
		e_item_mail_type_blessing,//
		e_item_mail_type_arena_reward,//
		e_item_mail_type_jewel_send_mail,//
		e_item_mail_type_item_recovery,//
		e_item_mail_type_hope_item,//

	};


	struct s_map_block
	{
		s_map_block() : region_id(-1), priority(0), region_type(e_region_type_zhongli)
		{
			points.clear();
		}

		int32 region_id;
		int32 priority;
		int32 region_type;
		map_point_vec points;
	};

	struct s_drop_list
	{
		s_drop_list() {
			clear_data();
		}

		void clear_data() {
			drop_list_vec.clear();
			drop_owner_guid.clear_data();
		}
		std::vector<s_item_template_info>		   drop_list_vec;
		guid_64					   drop_owner_guid;
	};

	class c_mail_info
	{
	public:
		guid_64 TargetPlayerGuid;
		guid_64 SenderPlayerGuid;
		std::string SenderName;
		std::string title;
		std::string contentText;
		int32 money_typ1;
		int32 money_num1;
		int32 money_typ2;
		int32 money_num2;
		const s_item_info* item_list;
		int32 item_num;

		c_mail_info()
		{
			clear_data();
		}

		c_mail_info(std::string _SenderName, std::string _title, std::string _contentText)
		{
			clear_data();
			SenderName = _SenderName;
			title = _title;
			contentText = _contentText;
		}

		void clear_data()
		{
			TargetPlayerGuid.clear_data();
			SenderPlayerGuid.clear_data();
			SenderName = "";			
			title = "";					
			contentText = "";			
			money_typ1 = 0;
			money_num1 = 0;
			money_typ2 = 0;
			money_num2 = 0;
			item_list = nullptr;
			item_num = 0;  
		}
	};
	//c_mail_info	default_mail_info;
	//c_mail_info	bag_is_full_mail_info("90303013", "90009004", "90303016");
	typedef std::vector<s_map_block> map_safe_point_vec;
	typedef std::unordered_map<int32, map_safe_point_vec> map_safe_point_map;
	typedef std::vector<std::string>	word_vector;
	typedef word_vector::iterator		word_vector_it;
	typedef std::unordered_map<std::string, int32> npc_name_map;
	typedef npc_name_map::iterator npc_name_map_it;
	typedef boost::function<void(const void*, size_t, int32)>	sender_ws_type;

	class globle_data
	{
	public:
		static globle_data& get_instance()
		{
			static globle_data s_globle_data;
			return s_globle_data;
		}
	public:
		void init_manager();
		void init_for_client();
		void init_server_open_time();
		void init_server_open_time(std::string server_open_str);
		void init_server_open_time(uint32 server_open_stamp);
		void init_send_ws(sender_ws_type send_ws) { m_send_ws = send_ws; }
	public:
		s_chat_notice_info& get_char_notice_info() { return m_chat_notice_info; }
		int32 get_notice_id() { return m_notice_id; }
		void clear_char_notice_info() {
			m_notice_id = 0;
			m_chat_notice_info.clear_data();
		}
		void set_notice_id(int32 notice_id) { m_notice_id = notice_id; }
		void add_other_name(const xstring& temp_other_name) { m_chat_notice_info.other_name.push_back(temp_other_name); }
		void add_describe_str(const xstring& temp_describe_str) { m_chat_notice_info.describe_str.push_back(temp_describe_str); }
		void add_item_name(const xstring& temp_item_name) { m_chat_notice_info.item_name.push_back(temp_item_name); }
		void add_num_type_data(const int32& temp_num_type_data) { m_chat_notice_info.num_type_data.push_back(temp_num_type_data); }
		void add_item_data(const s_item_info& temp_item_data) { m_chat_notice_info.item_data.push_back(temp_item_data); }
		const std::string&	get_server_open_tm_str() { return m_server_open_time_str; }
		const tm&			get_server_open_tm() { return m_server_open_time_struct; }
		const std::string&	get_server_open_state_end_tm_str() { return m_server_open_state_end_time_str; }
		RegionTemplate* get_region_template_ptr() { return m_region_template_ptr; }
		int32           get_region_code();
		VersionTemplate* get_version_template_ptr() { return m_version_template_ptr; }
		map_safe_point_vec* get_map_save_point_ary(int32 map_template_id);
		float get_att_gs(int32 index) const{if (e_unit_attack_att_none < index && index < e_unit_attack_att_max){return m_unit_gs_value[index];}return 0;}
		uint32 app_ansi_str_crc(const char* Data);
		word_vector& get_char_can_use() { return m_char_can_use_vector; }
		word_vector& get_char_no_use() { return m_char_no_use_vector; }
		bool is_ancient_battle(int32 map_template_id);
		bool is_belief_cloister_battle(int32 map_template_id);
		int32 get_community_group_relation_by_type(int32 type1, int32 type2);
		s_community_group_relation& get_community_group_relation() { return m_community_group_relation; }
		const xchar* get_mail_common_text_id(e_mail_common_text id_index);
		const xchar* get_mail_common_text(int32 id_index);
		void send_mail_to_another_player_by_system(
			guid_64 target_player_guid,
			guid_64 sender_guid,
			int32 receiver_server_id,
			e_item_mail_type mail_type,
			int32 money_typ1,
			int32 money_num1,
			int32 money_typ2,
			int32 money_num2,
			const s_item_info* item_list,
			int32 item_num);
		void send_mail_to_another_player_by_system(
			guid_64 target_player_guid,
			guid_64 sender_guid,
			int32 receiver_server_id,
			std::string SenderName,
			std::string title,
			std::string contentText,
			int32 money_typ1,
			int32 money_num1,
			int32 money_typ2,
			int32 money_num2,
			const s_item_info* item_list,
			int32 item_num);

		void send_mail_with_item(
			guid_64 player_guid, 
			int32 receiver_server_id, 
			const std::vector<s_item_template_info>& item_list, 
			std::string title, 
			std::string contentText, 
			int32 money_typ1 = 0, 
			int32 money_num1 = 0, 
			int32 money_typ2 = 0, 
			int32 money_num2 = 0);
		void merge_money_to_array(std::vector<int32>& money_array, int32 new_money_id, int32 new_money_value);
		void add_to_drop_list(std::vector<s_drop_list>& drop_list, const guid_64& ref_player_guid, s_item_template_info& _drop_list);
		void add_to_drop_list(std::vector<s_item_template_info>& drop_list, s_item_template_info& drop_info);
		void get_item_list_by_drop_bg(int32 drop_bg_id, std::vector<s_item_template_info>& _drop_list_with_num, int32 recursion_times = 0, int32 drop_level = -1);
		void get_item_list_by_rank_rwd(int32 _rank, std::vector<int32>& rank_rwd_list, std::vector<s_item_template_info>& _drop_list_with_num, int32 drop_level = -1);
		int32 get_drop_bag_by_rank_rwd(int32 _rank, std::vector<int32>& rank_rwd_list);
		void get_exp_add_percent(int32 player_level, int32 down_level, int32 up_level, float& add_percent, int32& buff_template_id);
		int32 get_rwd_num_by_rank_rwd(int32 _rank, std::vector<int32>& rank_rwd_list);

		std::vector<int32> get_item_by_class_type(const e_class_type class_type, const std::vector<int32>& item_array, const int32& item_locked_state);
		
		bool make_auction_info(s_auction_info& target_info, s_item_info item_info, int32 sell_num, int32 trade_type = 0,  
			guid_64 seller_guid = guid_64(), std::string seller_name = "", int32 seller_vip_level = 0,
			int32 sell_money_type = 0, int64 sell_total_money = 0);

		std::vector<int32> get_extra_buy_money_arr(int32 map_template_id);
		std::vector<int32> get_int_arr_by_str_arr(std::string str_arr);
		
		xstring get_money_str_by_type(int32 money_type);
		xstring get_class_name_by_type(int32 class_type);
		
	public:
		int32 get_server_on_days(int32 time_stamp = -1);
		int32 get_server_days() { return m_on_server_day; };
		int32 get_legion_boss_temp_id();
		void set_server_days();
	private:
		globle_data(){}
		void init_map_safe_point();
		void init_att_gs();
		void init_crc();
		void init_char(int32 begin_id, int32 end_id, word_vector& char_set);

		void init_community_group_relation();
	private:
		map_safe_point_map m_map_safe_point_map;
		RegionTemplate* m_region_template_ptr;
		VersionTemplate* m_version_template_ptr;
		float	m_unit_gs_value[e_unit_attack_att_max];//属性的战力加成
		uint32 m_GCRCTable[256];
		word_vector m_char_can_use_vector;
		word_vector m_char_no_use_vector;
		npc_name_map m_npc_name_map;
		int32 m_on_server_day;
		int32 m_notice_id;
		s_chat_notice_info m_chat_notice_info;
		sender_ws_type m_send_ws;
		const xchar* const mail_common_text_array[e_mail_common_text_max] =
		{
			"90303009",						//PK之王结算标题
			"90303010",						//PK之王结算正文
			"90303017",						//破碎虚空结算标题
			"90303018",						//破碎虚空结算正文
			"90303021",						//城战结算标题
			"90303019",						//城战胜利方结算内容
			"90303020",						//城战失败方结算内容
			"90303011",						//资源找回标题
			"90303012",						//资源找回正文
			"90303025",						//退出副本时获得的物品邮件标题
			"90303026",						//退出副本时获得的物品邮件正文
			"90303013",						//邮件发件人 系统
			"90090915",						//祈福抽奖邮件标题
			"90090916",						//祈福抽奖邮件正文
			"90095104",						//交易行购买成功标题
			"90095105",						//交易行购买成功正文
			"90095102",						//交易行出售成功标题
			"90095103",						//交易行出售成功正文
			"90095117",						//交易行取消成功标题
			"90095118",						//交易行取消成功玩家掉线正文
			"90095119",						//交易行取消成功玩家包满正文
			"90095150",						//交易行分红标题
			"90095151",						//交易行分红杀BOSS正文
			"90095152",						//交易行分红活动正文
			"90095153",						//交易行竞标失败标题
			"90095154",						//交易行竞标失败正文
			"90095155",						//交易行上架失败标题
			"90095156",						//交易行上架失败正文
			"90095189",						//强制离婚失败标题
			"90095190",						//强制离婚失败正文
			"90303035",						//军团排行分红
			"90303036",						//军团排行分红正文
			"90095242",						//e_mail_common_text_active_code_gift_title
			"90095243",						//e_mail_common_text_active_code_gift_context
			"90303040",						//崇拜失败标题
			"90303041",						//崇拜失败正文
			"90303046",						//加入军团邮件标题
			"90303047",						//加入军团邮件正文
			"90303029",						//日常结算包满标题
			"90303030",						//日常结算包满正文
			"90303031",						//讨伐结算包满标题
			"90303032",						//讨伐结算包满正文
			"90305017",						//e_mail_common_text_active_recharge_package_title
			"90305018",						//e_mail_common_text_active_recharge_package_context
			"90091122",						//宝石自动卸除发邮件标题
			"90091123",						//宝石自动卸除发邮件内容
			"90303048",						//竞技场领奖邮件标题
			"90303049",						//竞技场领奖邮件正文
			"90303050",						//改军团公告邮件标题
			"90303051",						//改军团公告邮件正文
			"90303052",						//任命军团职位邮件标题
			"90303053",						//任命军团邮件正文
			"0",							//霸主战奖励邮件标题
			"0",							//霸主战奖励邮件正文
			"90303054",						//竞标被超邮件标题
			"90303055",						//竞标被超邮件正文
			"90303056",						//云购保底奖励邮件标题
			"90303057",						//云购保底奖励邮件正文
			"90303058",						//云购大奖邮件标题
			"90303059",						//云购大奖邮件正文
			"90303060",						//云购购买失败邮件标题
			"90303061",						//云购购买失败邮件正文
			"90303062",						//限时活动排行奖励标题-累计充值
			"90303063",						//限时活动排行奖励正文-累计充值
			"90303064",						//限时活动排行奖励标题-累计消费
			"90303065",						//限时活动排行奖励正文-累计消费
			"90303066",						//限时活动排行奖励标题-转盘
			"90303067",						//限时活动排行奖励正文-转盘
			"90303068",						//改名成功通知军团内玩家标题
			"90303069",						//改名成功通知军团内玩家正文
			"90303070",						//改名成功通知好友标题
			"90303071",						//改名成功通知好友正文
			"90303072",						//七日排行奖励超时未领取邮件标题
			"90303073",						//七日排行奖励超时未领取邮件内容
			"90305247",						//跨服竞技未进入16强奖励邮件标题
			"90305248",						//跨服竞技未进入16强奖励邮件内容
			"90305249",						//跨服竞技进入16强奖励邮件标题
			"90305250",						//跨服竞技进入16强奖励邮件内容
			"90305251",						//跨服竞技进入8强奖励邮件标题
			"90305252",						//跨服竞技进入8强奖励邮件内容
			"90305253",						//跨服竞技进入4强奖励邮件标题
			"90305254",						//跨服竞技进入4强奖励邮件内容
			"90305255",						//跨服竞技进入2强奖励邮件标题
			"90305256",						//跨服竞技进入2强奖励邮件内容
			"90305257",						//跨服竞技冠军奖励邮件标题
			"90305258",						//跨服竞技冠军奖励邮件内容
			"90203440",						//每周守护值结算邮件标题
			"90203441",						//每周守护值结算邮件内容
			"90203442",						//每周服务器繁荣度结算邮件标题
			"90203443",						//每周服务器繁荣度结算邮件内容
			"90203486",						//军团改名邮件标题
			"90203487",						//军团改名邮件内容
			"90203561",						//跨服城战军团排名邮件标题
			"90203562",						//跨服城战军团排名邮件内容
			"90203563",						//跨服城战军团长排名邮件标题
			"90203564",						//跨服城战军团长排名邮件内容
			"90203577",						//跨服天梯-退还钻石
			"90203577",						//跨服天梯-退还钻石
			"90203578",						//跨服天梯-单次比赛奖励标题
			"90203579",						//跨服天梯-单次比赛奖励内容
			"90203580",						//跨服天梯-赛季段位奖励标题
			"90203581",						//跨服天梯-赛季段位奖励内容
			"90203582",						//跨服天梯-王者段位奖励标题
			"90203533",						//跨服天梯-王者段位奖励内容
			"90091352",                     //技能书替换背包满标题
			"90091353",                     //技能书替换背包满内容  
			"90203690",						//跨服霸主战地图邮件标题
			"90203689",						//跨服霸主战地图邮件内容
			"90203691",                     //跨服城战结算邮件标题
			"90203692",                     //跨服城战结算邮件内容
			"90305374",                     //战力提升排行奖励邮件标题
			"90305376",                     //战力提升排行奖励邮件内容
			"90305377",                     //坐骑提升排行奖励邮件标题
			"90305378",                     //坐骑提升排行奖励邮件内容
			"90305379",                     //翅膀提升排行奖励邮件标题
			"90305380",                     //翅膀提升排行奖励邮件内容
			"90305381",                     //装备提升排行奖励邮件标题
			"90305382",                     //装备提升排行奖励邮件内容
			"90305408",                     //精灵提升排行奖励邮件标题
			"90305409",                     //精灵提升排行奖励邮件内容
			"90305383",                     //跨服战力提升排行奖励邮件标题
			"90305384",                     //跨服战力提升排行奖励邮件内容
			"90305385",                     //跨服坐骑提升排行奖励邮件标题
			"90305386",                     //跨服坐骑提升排行奖励邮件内容
			"90305387",                     //跨服翅膀提升排行奖励邮件标题
			"90305388",                     //跨服翅膀提升排行奖励邮件内容
			"90305389",                     //跨服装备提升排行奖励邮件标题
			"90305390",                     //跨服装备提升排行奖励邮件内容
			"90305410",                     //跨服精灵提升排行奖励邮件标题
			"90305411",                     //跨服精灵提升排行奖励邮件内容
			"90096786",                     //夫妻榜祝福邮件标题
			"90096787",                     //夫妻榜祝福邮件内容
			"90096788",                     //夫妻结婚邮件标题
			"90096789",                     //夫妻结婚邮件内容
			"90096790",                     //夫妻离婚邮件标题
			"90096791",                     //夫妻离婚邮件内容
			"90096862",                     //天启试炼副本结束奖励邮件标题
			"90096863",                     //天启试炼1星副本结束奖励邮件内容
			"90096864",                     //天启试炼2星副本结束奖励邮件内容
			"90096865",                     //天启试炼3星副本结束奖励邮件内容
			"90203992",                     //跨服天梯段位提升奖励邮件标题
			"90203991",                     //跨服天梯段位提升奖励邮件内容
			"90203995",                     //世界霸主分组标题
			"90203994",                     //世界霸主分组内容
			"90203998",                     //世界霸主比赛结果标题
			"90203999",                     //世界霸主比赛结果内容
			"90204028",                     //跨服世界霸主分组标题
			"90204029",                     //跨服世界霸主分组内容
			"90204031",                     //跨服世界霸主比赛结果标题
			"90204032",                     //跨服世界霸主比赛结果内容
			"90204058",                     //世界霸主比赛军团长标题
			"90204059",                     //世界霸主比赛军团长内容
			"90204086",                     //世界霸主结算标题
			"90204087",                     //世界霸主结算内容
			"90204091",                     //跨服天梯赛季开始
			"90204092",                     //跨服天梯赛季开始
			"90096951",						//交易行超时标题
			"90096952",						//交易行超时正文
			"90204148",						//助战奖励标题
			"90204149",						//助战奖励正文
			"90204150",						//求助奖励标题
			"90204151",						//求助奖励正文
			"90204298",						//城战结束胜利方发奖励标题
			"90204299",						//城战结束胜利方发奖励正文
			"90204300",                     //跨服天梯赛季排名奖励
			"90204301",                     //跨服天梯赛季排名奖励
			"90204334",                     //军团福利奖励邮件标题
			"90204335",                     //军团福利奖励邮件内容
			"90305819",                     //元素争霸赛季奖励标题
			"90305820",                     //元素争霸赛季奖励内容
			"90305832",                     //元素争霸赛季开启邮件标题
			"90305833",                     //元素争霸赛季开启邮件内容
			"90305821",                     //元素争霸百强奖励邮件标题
			"90305822",                     //元素争霸百强奖励邮件内容
			"90305817",                     //元素争霸升段奖励邮件标题
			"90305818",                     //元素争霸升段奖励邮件内容
			"90305886",                     //宝库清理邮件标题
			"90305887",                     //宝库清理邮件内容
			"90306038",						//世界BOSS攻略奖励邮件标题
			"90306039",						//世界BOSS攻略奖励邮件内容
			"90306084",						//限时活动奖励标题 -- 跨服充值排行榜
			"90306085",						//限时活动奖励正文 -- 跨服充值排行榜
			"90306328",						//攻城战分组邮件标题
			"90306329",						//攻城战分组邮件正文
			"90306330",						//攻城战个人奖励邮件标题
			"90306331",						//攻城战个人奖励邮件正文
	};
		
	public:
		//阵营关系
		s_community_group_relation	m_community_group_relation;
		//int32 m_community_group_relation[e_community_type_max][e_community_type_max];
		std::string					m_server_open_time_str;
		tm							m_server_open_time_struct;
		std::string					m_server_open_state_end_time_str;
	};
}
