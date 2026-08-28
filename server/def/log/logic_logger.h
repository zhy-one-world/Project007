/********************************************************************
	created:	2014/07/25
	created:	25:7:2014   19:34
	file base:	logic_logger
	file ext:	h
	author:		zhy
	
	purpose:	
*********************************************************************/
#ifndef _LOGIC_LOGGER_H_
#define _LOGIC_LOGGER_H_

#include <xchar.hpp>
#include <singleton.hpp>
#include "logic/type_def.hpp"

namespace hld
{

	typedef uint32 unix_time;

	class log_field;

	extern log_field log_null;

	class log_field
	{
	public:
		log_field(i16 _number);
		log_field(ui16 _number);
		log_field(int32 _number);
		log_field(uint32 _number);
		log_field(i64 _number);
		log_field(ui64 _number);
		log_field(float _number);
		log_field(xstring _str);
		log_field(guid_64 _number);
		log_field():_null(true){}
	public:
		bool is_null(){return _null;}
		xstring& get_string(){return data_s;}
	protected:
		bool _null;
		xstring data_s;
	};

	class log_field_time : public log_field
	{
	public:
		log_field_time(unix_time t);
		log_field_time();
	};

	class log_field_seconds : public log_field
	{
	public:
		log_field_seconds(uint32 seconds);
	};

	class low_logger : public singleton< low_logger >
	{
	public:
		enum log_content
		{
			log_begin = 100,
			log_login,						//用户登录
			log_logout,						//用户登出
			log_enter_scene,				//进入场景
			log_exit_scene,					//离开场景
			log_create_character,			//创建角色
			log_del_character,				//删除角色
			log_revive_character,			//恢复角色
			log_kick_account,				//玩家被踢
			log_role_online,				//角色上线
			log_role_offline,				//角色下线
			log_market_buyitem,				//商城购买物品	
			log_market_buydolly,			//商城购物车购买
			log_market_gift,				//赠送商城物品
			log_market_group,				//发起团购
			log_market_groupjoin,			//加入团购
			log_market_group_getback,		//团购失败，领取元宝
			log_market_getgift,				//领取商城物品
			log_item_trade,					//交易
			log_item_npc_trade,				//NPC交易
			log_item_npc_item,				//NPC交易物品（NPC交易的从表）
			log_item_pick,					//拾取物品
			log_item_drop,					//丢弃物品
			log_item_consume,				//物品消耗
			log_item_levelup,				//道具升级
			log_item_enchase,				//道具镶嵌
			log_item_fuse,					//道具融合
			log_item_hecheng,				//灵石合成
			log_item_stiletto,				//道具打孔
			log_item_reehence,				//属性重抽
			log_item_auction_sell,			//寄卖物品
			log_item_auction_takeitem,		//取寄卖物品
			log_item_auction_takemoney,		//取寄卖所得游戏币
			log_item_auction_takeyb,		//取寄卖所得元宝
			log_item_auction_buy,			//游戏币竞买
			log_item_auction_buyyb,			//元宝竞买
			log_item_auction_tackback_sell_item,			//撤销寄卖物品
			log_item_giftbox,				//捡到礼品箱
			log_item_moneybox,				//捡到钱箱
			log_item_ybbox,					//捡到元宝箱
			log_item_yb_trade,				//元宝交易
			log_dopot_item,					//仓库存取物品
			log_dopot_pet,					//仓库存取兵魂
			log_dopot_money,				//仓库存取游戏币
			log_mail_send,					//发送邮件
			log_mail_receive,				//接受邮件
			log_mail_delete,				//删除邮件
			log_yb_consume,					//元宝消耗
			log_item_vitiate,				//物品变质
			log_fortune_change,				//金钱变化
			log_market_encourage,			//节点奖励
			log_item_zhaichu,				//摘除灵石
			log_cast_cfaithose,				//制作道具
			log_quest_accept,				//接任务
			log_quest_submit,				//交任务
			log_quest_reward,				//任务奖励道具
			log_quest_save,					//保存任务进度
			log_player_addexp,				//玩家获得经验
			log_player_useexp,				//玩家使用经验
			log_player_levelup,				//玩家升级
			log_player_die,					//玩家死亡
			log_player_revive,				//原地复活
			log_friend_add,					//添加好友
			log_friend_remove,				//解除好友
			log_blacklist_add,				//添加黑名单
			log_blacklist_remove,			//移除黑名单
			log_add_speciality,				//学习生活技能
			log_del_speciality,				//遗忘生活技能
			log_add_cfaithose,				//学习配方
			//聊天记录
			log_chat_world,					//世界频道
			log_chat_camp,					//世家频道
			log_chat_team,					//队伍频道
			log_chat_guild,					//家族频道
			log_chat_trade,					//交易频道
			log_chat_aoi,					//附近频道
			log_chat_map,					//地区频道
			log_chat_private,				//私聊频道
			log_chat_room,					//自建聊天室频道
			log_pet_get_from_item,			//兵魂获得
			log_pet_get,					//兵魂获得
			log_pet_smelt,					//兵魂祈灵
			log_pet_ghost,					//兵魂洗魂
			log_pet_skill,					//兵魂学习技能
			log_pet_drop,					//兵魂丢弃		
			log_pet_seal,					//兵魂封印
			log_pet_unseal,					//兵魂解封
			log_pet_feed,					//兵魂喂养
			log_pet_change,					//兵魂技
			log_pet_trade,					//兵魂幻化
			log_pet_enhence,				//兵魂星级融合
			log_pet_extend,	

			log_guild_create,				//家族创建
			log_guild_join,					//加入家族
			log_guild_exit,					//退出家族
			log_guild_destroy,				//解散家族
			log_guild_duty,					//家族职位任命
			log_guild_present,				//家族奖励

			log_camp_join,					//加入世家
			log_camp_change,				//世家变更
			log_camp_elect,					//世家参选
			log_camp_vote,					//世家选举投票
			log_camp_point_get,				//获得世家点数
			log_camp_point_consume,			//消耗世家点数
			log_camp_buy,					//购买世家物品

			log_ym_create,					//义盟创建
			log_ym_join,					//加入义盟
			log_ym_leave,					//退出义盟
			log_ym_dismiss,					//义盟解散
			log_ym_levelup,					//义盟升级

			log_ys_levelup,					//元神升级
			log_ys_redeem,					//元神修炼
			log_ys_consume,					//元神消耗
			log_ys_reset,					//元神洗点

			log_ride_enhance,				//坐骑强化

			log_player_count,				//在线人数统计
			log_player_instance,			//角色进出副本
			log_money_overload,				//系统删除金钱

			log_active_branch,				//切换技能分支
			log_add_skill,					//学习技能
			log_use_skill,					//使用技能
			log_fly,						//固定路线飞行
			log_gm_fortune,					//通过GM制定得到钱或元宝
			log_offline_exp,				//离线经验
			log_gift_money,					//时长元宝

			log_quest_consume,				// 任务扣除物品
			log_guild_levelup,				//家族升级
			log_guild_addmoney,				//家族捐款
			log_guild_addmaterial,			//家族捐献材料
			log_vip_info,					//帐号VIP

			log_market_group_finish,		// 2012-02-22 330版本：增加团购成功LOG


			log_ghost_pet,					//兵魂洗魂
			log_fuse_pet,
			//...

			//////////////////////////////////////////////////////////////////////////
			//
			//	2011-12-13: Zeguo.Gao 增加Log类型: 增加兵魂槽位消耗
			//
			//////////////////////////////////////////////////////////////////////////
			log_pet_add_pet_slot,			// 增加兵魂槽位

			log_change_camp_point,			// 元宝兑换精力值

			log_qingling_pet,	
			log_paiyang_pet,	
			log_item_synthesis,				// 物品合成

			log_title_info,					// 称号信息

			log_online_time,				// 在线时间
			// 2012-04-26 Zeguo.Gao 增加LOG类型
			log_yuanbao_auction_sale,		// 寄卖元宝
			log_yuanbao_auction_buy,		// 购买元宝
			log_yuanbao_auction_take_back,	// 寄卖元宝挂单下架

			low_buff_cost_yb_begin,			// 242 ~ 342为buff扣除元宝类型
			low_buff_cost_yb_end = low_buff_cost_yb_begin + 100,

			//////////////////////////////////////////////////////////////////////////
			//	2012-05-21 高泽国增加：世界聊天扣除元宝
			//////////////////////////////////////////////////////////////////////////
			log_chat_world_deduct_yuanbao,
			//////////////////////////////////////////////////////////////////////////
			log_starteam,					// 星阵
			//////////////////////////////////////////////////////////////////////////
			//	2012-05-30 高泽国：击杀野外BOSS保存LOG
			//////////////////////////////////////////////////////////////////////////
			log_kill_world_boss,
			//////////////////////////////////////////////////////////////////////////

			log_pet_transfer,

			//////////////////////////////////////////////////////////////////////////
			//	2012-06-27 高泽国：付费邮件扣除
			//////////////////////////////////////////////////////////////////////////
			// 支付金钱
			log_mail_pay_money,
			// 支付金元宝
			log_mail_pay_yuanbao,
			//////////////////////////////////////////////////////////////////////////

			//////////////////////////////////////////////////////////////////////////
			//
			//	2012-05-30 祝黄清：拒绝家族敌对元宝消耗
			//
			//////////////////////////////////////////////////////////////////////////
			log_refuse_guild_hostility,
			//////////////////////////////////////////////////////////////////////////

			//////////////////////////////////////////////////////////////////////////
			//
			//	2012-08-02 715增加内容：当前主分支最大数：349
			//
			//////////////////////////////////////////////////////////////////////////
			log_715_use_begin			= 350,
			log_quest_abort				= log_715_use_begin,	// 放弃任务
			log_yuanshen,				// 元神
			log_kill_npc,				// 杀怪
			log_buff,					// buff
			log_change_xiewang_card,	// 金钱兑换蟹王卡
			log_change_yanshen_card,	// 金钱兑换炎神卡
			log_change_jushou_card,		// 金钱兑换巨兽卡
			log_change_mozun_card,		// 金钱兑换魔尊卡
			low_repair_item,			// 修理物品
			//////////////////////////////////////////////////////////////////////////
			log_star_eat,				// 星魂喂食

			low_use_item,				//使用物品扣除元宝
			low_pk_kill,				//杀戮值LOG
			low_mail_pay_info,			//支付信息LOG

			max_content_id,		
		};

		void init_table_name()
		{
			log_tables[log_login]					+= "login";
			log_tables[log_logout]					+= "logout";
			log_tables[log_enter_scene]				+= "enter_scene";
			log_tables[log_exit_scene]				+= "exit_scene";
			log_tables[log_create_character]		+= "create_character";
			log_tables[log_del_character]			+= "del_character";
			log_tables[log_revive_character]		+= "revive_character";
			log_tables[log_kick_account]			+= "kick_account";
			log_tables[log_role_online]				+= "role_online";
			log_tables[log_role_offline]			+= "role_offline";

			log_tables[log_market_buyitem]			+= "market_buyitem";
			log_tables[log_market_buydolly]			+= "market_buydolly";
			log_tables[log_market_gift]				+= "market_gift";
			log_tables[log_market_group]			+= "market_group";
			log_tables[log_market_groupjoin]		+= "market_groupjoin";
			log_tables[log_market_getgift]			+= "market_getgift";
			log_tables[log_market_encourage]		+= "market_encourage";

			log_tables[log_item_trade]				+= "item_trade";
			log_tables[log_item_npc_trade]			+= "item_npc_trade";
			log_tables[log_item_npc_item]			+= "npc_trade_item";
			log_tables[log_item_pick]				+= "item_pick";
			log_tables[log_item_drop]				+= "item_drop";
			log_tables[log_item_consume]			+= "item_consume";
			log_tables[log_item_levelup]			+= "item_levelup";
			log_tables[log_item_enchase]			+= "item_enchase";
			log_tables[log_item_auction_sell]		+= "item_auction_sell";
			log_tables[log_item_auction_takeitem]	+= "item_auction_takeitem";
			log_tables[log_item_auction_takemoney]	+= "item_auction_takemoney";
			log_tables[log_item_auction_takeyb]		+= "item_auction_takeyb";
			log_tables[log_item_auction_buy]		+= "item_auction_buy";
			log_tables[log_item_auction_buyyb]		+= "item_auction_buyyb";
			log_tables[log_item_auction_tackback_sell_item]		+= "item_auction_tackback_sell_item";
			log_tables[log_item_giftbox]			+= "item_giftbox";
			log_tables[log_item_moneybox]			+= "item_moneybox";
			log_tables[log_item_ybbox]				+= "item_ybbox";
			log_tables[log_item_zhaichu]			+= "item_zhaichu";
			log_tables[log_item_yb_trade]			+= "item_yb_trade";
			log_tables[log_item_fuse]				+= "item_fuse";
			log_tables[log_item_hecheng]			+= "item_hecheng";
			log_tables[log_item_stiletto]			+= "item_stiletto";
			log_tables[log_item_reehence]			+= "item_reehence";
			log_tables[log_yb_consume]				+= "yb_consume";
			log_tables[log_item_vitiate]			+= "item_vitiate";
			log_tables[log_fortune_change]			+= "fortune_change";

			log_tables[log_dopot_item]				+= "dopot_item";
			log_tables[log_dopot_pet]				+= "dopot_pet";
			log_tables[log_dopot_money]				+= "dopot_money";
			log_tables[log_mail_send]				+= "mail_send";
			log_tables[log_mail_receive]			+= "mail_receive";
			log_tables[log_mail_delete]				+= "mail_delete";

			log_tables[log_cast_cfaithose]			+= "cast_cfaithose";

			log_tables[log_quest_accept]			+= "quest_accept";
			log_tables[log_quest_submit]			+= "quest_submit";
			log_tables[log_quest_reward]			+= "quest_reward";
			log_tables[log_quest_save]				+= "quest_save";
			log_tables[log_quest_abort]				+= "quest_abort";
			log_tables[log_yuanshen]				+= "yuanshen";

			log_tables[log_player_addexp]			+= "player_addexp";
			log_tables[log_player_useexp]			+= "player_useexp";
			log_tables[log_player_levelup]			+= "player_levelup";
			log_tables[log_player_die]				+= "player_die";
			log_tables[log_player_revive]			+= "player_revive";
			log_tables[log_friend_add]				+= "friend_add";
			log_tables[log_friend_remove]			+= "friend_remove";
			log_tables[log_blacklist_add]			+= "blacklist_add";
			log_tables[log_blacklist_remove]		+= "blacklist_remove";
			log_tables[log_add_speciality]			+= "add_speciality";
			log_tables[log_del_speciality]			+= "del_speciality";
			log_tables[log_add_cfaithose]				+= "add_cfaithose";

			log_tables[log_chat_world]				+= "chat_recored";
			log_tables[log_chat_camp]				+= "chat_recored";
			log_tables[log_chat_team]				+= "chat_recored";
			log_tables[log_chat_guild]				+= "chat_recored";
			log_tables[log_chat_trade]				+= "chat_recored";
			log_tables[log_chat_aoi]				+= "chat_recored";
			log_tables[log_chat_map]				+= "chat_recored";
			log_tables[log_chat_private]			+= "chat_recored";
			log_tables[log_chat_room]				+= "chat_recored";

			log_tables[log_pet_get]					+= "pet_get";
			log_tables[log_pet_smelt]				+= "pet_smelt";
			log_tables[log_pet_ghost]				+= "pet_ghost";
			log_tables[log_pet_enhence]				+= "pet_enhence";
			log_tables[log_pet_skill]				+= "pet_skill";
			log_tables[log_pet_drop]				+= "pet_drop";
			log_tables[log_pet_trade]				+= "pet_trade";
			log_tables[log_pet_seal]				+= "pet_seal";
			log_tables[log_pet_unseal]				+= "pet_unseal";
			log_tables[log_pet_feed]				+= "pet_feed";
			log_tables[log_pet_change]				+= "pet_change";

			log_tables[log_guild_create]			+= "guild_create";
			log_tables[log_guild_join]				+= "guild_join";
			log_tables[log_guild_exit]				+= "guild_exit";
			log_tables[log_guild_destroy]			+= "guild_destroy";
			log_tables[log_guild_duty]				+= "guild_duty";

			log_tables[log_camp_join]				+= "camp_join";
			log_tables[log_camp_change]				+= "camp_change";
			log_tables[log_camp_elect]				+= "camp_elect";
			log_tables[log_camp_vote]				+= "camp_vote";
			log_tables[log_camp_point_get]			+= "camp_point_get";
			log_tables[log_camp_point_consume]		+= "camp_point_consume";
			log_tables[log_camp_buy]				+= "camp_buy";

			log_tables[log_ym_create]				+= "ym_create";
			log_tables[log_ym_join]					+= "ym_join";
			log_tables[log_ym_leave]				+= "ym_leave";
			log_tables[log_ym_dismiss]				+= "ym_dismiss";
			log_tables[log_ym_levelup]				+= "ym_levelup";

			log_tables[log_ys_levelup]				+= "ys_levelup";
			log_tables[log_ys_redeem]				+= "ys_redeem";
			log_tables[log_ys_consume]				+= "ys_consume";
			log_tables[log_ys_reset]				+= "ys_reset";

			log_tables[log_ride_enhance]			+= "ride_enhance";

			log_tables[log_player_count]			+= "player_count";
			log_tables[log_player_instance]			+= "player_instance";
			log_tables[log_money_overload]			+= "money_overload";
			log_tables[log_quest_consume]			+= "quest_consume";
			log_tables[log_guild_levelup]			+= "guild_levelup";
			log_tables[log_guild_addmoney]			+= "guild_addmoney";
			log_tables[log_guild_addmaterial]		+= "guild_addmaterial";
			log_tables[log_vip_info]				+= "vip_info";

			log_tables[log_market_group_finish]		+= "market_group_finish";

			log_tables[log_item_synthesis]			+= "item_synthesis";

			log_tables[log_title_info]				+= "title_info";

			log_tables[log_online_time]				+= "online_time";
			log_tables[log_yuanbao_auction_sale]	+= "log_yuanbao_auction";
			log_tables[log_yuanbao_auction_buy]		+= "log_yuanbao_auction";
			log_tables[log_yuanbao_auction_take_back]	+= "log_yuanbao_auction";

			log_tables[log_starteam]				+= "starteam";

			log_tables[log_kill_world_boss]			+= "log_kill_world_boss";


			log_tables[log_pet_transfer]			+= "pet_transfer";

			log_tables[log_star_eat]				+= "star_eat";
			log_tables[low_pk_kill]					+= "log_pk_kill";
			log_tables[low_mail_pay_info]			+= "mail_pay_log";

		}

		low_logger(void);
		~low_logger(void);

		//log( low_logger::table_type, field1, field2, ... );
		//faild can be: int32,uint,float,log_field_time() 
		//example: log( low_logger::log_login, account, uint32(ipaddr), log_field_time(utility::time()) );
		bool log(log_content t, 
			log_field f1, 
			log_field f2 = log_null, 
			log_field f3 = log_null, 
			log_field f4 = log_null, 
			log_field f5 = log_null, 
			log_field f6 = log_null, 
			log_field f7 = log_null, 
			log_field f8 = log_null, 
			log_field f9 = log_null, 
			log_field f10 = log_null, 
			log_field f11 = log_null, 
			log_field f12 = log_null, 
			log_field f13 = log_null, 
			log_field f14 = log_null, 
			log_field f15 = log_null, 
			log_field f16 = log_null, 
			log_field f17 = log_null,
			log_field f18 = log_null,
			log_field f19 = log_null,
			log_field f20 = log_null,
			log_field f21 = log_null,
			log_field f22 = log_null,
			log_field f23 = log_null,
			log_field f24 = log_null);

		ui64 get_log_id();
		bool get_log_switch(log_content t);
		void send(const void* data, size_t length);
	private:
		xstring log_tables[max_content_id];
	};

}


#endif
