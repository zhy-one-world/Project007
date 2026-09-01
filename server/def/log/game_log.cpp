/********************************************************************
	created:	2014/07/25
	created:	25:7:2014   19:27
	file base:	game_log
	file ext:	cpp
	author:		zhy
	
	purpose:	
*********************************************************************/
#include "game_log.hpp"
#include "logic_logger.h"

namespace faith
{

	// private--------------------------------------------------------
	bool cgame_log_enactment::load_resource( xstring strFilename )
	{
// 		att_xml_doc Doc;
// 		if ( !Doc.open( strFilename.c_str() ) )
// 		{
// 			show_message( strFilename, "load file fail" );
// 			return false;
// 		}
// 		bool bEof = true;
// 		while ( bEof )
// 		{
// 			bool bValue = false;
// 			// 解析数据
// 			read_log_enactment( Doc );
// 			bEof = Doc.next_att();
// 		}

		return true;
	}

	void cgame_log_enactment::read_log_enactment( /*att_xml_doc& Doc */)
	{
// 		if ( Doc.is_empty() )
// 		{
// 			return;
// 		}
// 		int32 Data[low_logger::max_content_id - low_logger::log_login];
// 		memset(Data, 0, sizeof(Data));
// #define LOG_SWITCH(id)\
// 	Doc.get_value( #id, Data[low_logger::id - low_logger::log_login] );
// 		LOG_SWITCH(log_login)
// 			LOG_SWITCH(log_logout)
// 			LOG_SWITCH(log_enter_scene)
// 			LOG_SWITCH(log_exit_scene)
// 			LOG_SWITCH(log_create_character)
// 			LOG_SWITCH(log_del_character)
// 			LOG_SWITCH(log_revive_character)
// 			LOG_SWITCH(log_role_online)
// 			LOG_SWITCH(log_role_offline)
// 			LOG_SWITCH(log_market_buyitem)
// 			LOG_SWITCH(log_market_buydolly)
// 			LOG_SWITCH(log_market_gift)
// 			LOG_SWITCH(log_market_group)
// 			LOG_SWITCH(log_market_groupjoin)
// 			LOG_SWITCH(log_market_getgift)
// 			LOG_SWITCH(log_kick_account)
// 			LOG_SWITCH(log_item_trade)
// 			LOG_SWITCH(log_item_npc_trade)
// 			LOG_SWITCH(log_item_pick)
// 			LOG_SWITCH(log_item_drop)
// 			LOG_SWITCH(log_item_consume)
// 			LOG_SWITCH(log_item_levelup)
// 			LOG_SWITCH(log_item_enchase)
// 			LOG_SWITCH(log_item_fuse)
// 			LOG_SWITCH(log_item_hecheng)
// 			LOG_SWITCH(log_item_reehence)
// 			LOG_SWITCH(log_item_stiletto)
// 			LOG_SWITCH(log_item_auction_sell)
// 			LOG_SWITCH(log_item_auction_takeitem)
// 			LOG_SWITCH(log_item_auction_takemoney)
// 			LOG_SWITCH(log_item_auction_takeyb)
// 			LOG_SWITCH(log_item_auction_buy)
// 			LOG_SWITCH(log_item_auction_buyyb)
// 			LOG_SWITCH(log_item_auction_tackback_sell_item)
// 			LOG_SWITCH(log_item_giftbox)
// 			LOG_SWITCH(log_item_moneybox)
// 			LOG_SWITCH(log_item_ybbox)
// 			LOG_SWITCH(log_item_yb_trade)
// 			LOG_SWITCH(log_yb_consume)
// 			LOG_SWITCH(log_item_vitiate)
// 			LOG_SWITCH(log_fortune_change)
// 			LOG_SWITCH(log_dopot_item)
// 			LOG_SWITCH(log_dopot_pet)
// 			LOG_SWITCH(log_dopot_money)
// 			LOG_SWITCH(log_mail_send)
// 			LOG_SWITCH(log_mail_receive)
// 			LOG_SWITCH(log_mail_delete)
// 			LOG_SWITCH(log_market_encourage)
// 			LOG_SWITCH(log_item_zhaichu)
// 			LOG_SWITCH(log_quest_accept)
// 			LOG_SWITCH(log_quest_submit)
// 			LOG_SWITCH(log_quest_abort)
// 			LOG_SWITCH(log_quest_reward)
// 			LOG_SWITCH(log_quest_save)
// 			LOG_SWITCH(log_player_addexp)
// 			LOG_SWITCH(log_player_useexp)
// 			LOG_SWITCH(log_player_levelup)
// 			LOG_SWITCH(log_player_die)
// 			LOG_SWITCH(log_player_revive)
// 			LOG_SWITCH(log_friend_add)
// 			LOG_SWITCH(log_friend_remove)
// 			LOG_SWITCH(log_blacklist_add)
// 			LOG_SWITCH(log_blacklist_remove)
// 			LOG_SWITCH(log_cast_cfaithose)
// 			LOG_SWITCH(log_add_speciality)
// 			LOG_SWITCH(log_del_speciality)
// 			LOG_SWITCH(log_add_cfaithose)
// 			LOG_SWITCH(log_chat_world)
// 			LOG_SWITCH(log_chat_camp)
// 			LOG_SWITCH(log_chat_team)
// 			LOG_SWITCH(log_chat_guild)
// 			LOG_SWITCH(log_chat_trade)
// 			LOG_SWITCH(log_chat_aoi)
// 			LOG_SWITCH(log_chat_map)
// 			LOG_SWITCH(log_chat_private)
// 			LOG_SWITCH(log_chat_room)
// 			LOG_SWITCH(log_pet_get)
// 			LOG_SWITCH(log_pet_smelt)
// 			LOG_SWITCH(log_pet_ghost)
// 			LOG_SWITCH(log_pet_enhence)
// 			LOG_SWITCH(log_pet_skill)
// 			LOG_SWITCH(log_pet_drop)
// 			LOG_SWITCH(log_pet_trade)
// 			LOG_SWITCH(log_pet_seal)
// 			LOG_SWITCH(log_pet_unseal)
// 			LOG_SWITCH(log_pet_feed)
// 			LOG_SWITCH(log_pet_change)
// 			LOG_SWITCH(log_pet_transfer)
// 			LOG_SWITCH(log_guild_create)
// 			LOG_SWITCH(log_guild_join)
// 			LOG_SWITCH(log_guild_exit)
// 			LOG_SWITCH(log_guild_destroy)
// 			LOG_SWITCH(log_guild_duty)
// 			LOG_SWITCH(log_ys_levelup)
// 			LOG_SWITCH(log_ys_redeem)
// 			LOG_SWITCH(log_ys_consume)
// 			LOG_SWITCH(log_ys_reset)
// 			LOG_SWITCH(log_yuanshen)
// 			LOG_SWITCH(log_ym_create)
// 			LOG_SWITCH(log_ym_join)
// 			LOG_SWITCH(log_ym_leave)
// 			LOG_SWITCH(log_ym_dismiss)
// 			LOG_SWITCH(log_ym_levelup)
// 			LOG_SWITCH(log_camp_join)
// 			LOG_SWITCH(log_camp_change)
// 			LOG_SWITCH(log_camp_elect)
// 			LOG_SWITCH(log_camp_vote)
// 			LOG_SWITCH(log_camp_point_get)
// 			LOG_SWITCH(log_camp_point_consume)
// 			LOG_SWITCH(log_camp_buy)
// 			LOG_SWITCH(log_ride_enhance)
// 			LOG_SWITCH(log_player_count)
// 			LOG_SWITCH(log_player_instance)
// 			LOG_SWITCH(log_money_overload)
// 			LOG_SWITCH(log_quest_consume)
// 			LOG_SWITCH(log_guild_levelup)
// 			LOG_SWITCH(log_guild_addmoney)
// 			LOG_SWITCH(log_guild_addmaterial)
// 			LOG_SWITCH(log_vip_info)
// 			LOG_SWITCH(log_market_group_finish)
// 			LOG_SWITCH(log_item_synthesis)
// 			LOG_SWITCH(log_title_info)
// 			LOG_SWITCH(log_online_time)
// 			LOG_SWITCH(log_yuanbao_auction_sale)
// 			LOG_SWITCH(log_yuanbao_auction_buy)
// 			LOG_SWITCH(log_yuanbao_auction_take_back)
// 			LOG_SWITCH(log_starteam)
// 			LOG_SWITCH(log_kill_world_boss)
// 			LOG_SWITCH(log_star_eat)
// 			LOG_SWITCH(low_pk_kill)
// 			LOG_SWITCH(low_mail_pay_info)
// 			xstring Msg	   = "";
// 		bool		bValue = false;
// 		for ( int32 i = 0; i < low_logger::max_content_id - low_logger::log_login; ++i )
// 		{
// 			if ( 0 == Data[i] )
// 			{
// 				bValue = false;
// 			}
// 			else
// 			{
// 				bValue = true;
// 			}
// 			//if ( false == m_mLogEnactment.insert( std::make_pair( i + low_logger::log_login, bValue ) ).second )
// 			//{
// 			//	Msg = "第 " + boost::lexical_cast< xstring >( i + 1 ) + " 个设置Insert失败！";
// 			//	show_message( Doc.GetFileName(), Msg );
// 			//}
// 			m_log_enactment.insert( std::make_pair( i + low_logger::log_login, bValue ) ).second;
// 			//自动设置从表开关
// 			if (i == low_logger::log_item_npc_trade-low_logger::log_login)
// 				m_log_enactment.insert( std::make_pair( (int32)low_logger::log_item_npc_item, bValue ) );
// 		}
	}

	// public---------------------------------------------------------
	bool cgame_log_enactment::load(xstring strResPath)
	{
// 		cres_file_parse Rfp( strResPath, "GameLog_Enactment" );
// 		xstring strFilename = "";
// 		while ( true )
// 		{
// 			strFilename = Rfp.get_file_name();
// 			if ( "" == strFilename )
// 			{
// 				break;
// 			}
// 			load_resource( strFilename );
// 		}

		return true;
	}

	void cgame_log_enactment::unload()
	{
		m_log_enactment.clear();
	}

	bool cgame_log_enactment::get_log_enactment( uint32 eType )
	{
		cgame_log_info_iter iter = m_log_enactment.find( eType );
		if ( iter != m_log_enactment.end() )
		{
			return iter->second;
		}
		return false;
	}

}
