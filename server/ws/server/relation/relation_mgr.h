/********************************************************************
	file name:	relation.h
	created:	2017/05/26 13:49
	author:		zhy
	purpose:	
*********************************************************************/
#ifndef _RELATION_MGR_H_
#define _RELATION_MGR_H_

#include "relation_set.h"

namespace faith
{
	class client_session;
	class relation_proto_c2s_operate_other_add_req;
	class relation_proto_s2c_ret_friend_userinfo;

	//结婚相关存储
	struct marriage_data
	{
	public:
		enum
		{
			e_lover_nostatus = -1,	//无效状态
			e_lover_normal,			//正常结婚-不能游行
			e_lover_luxury,			//奢华结婚-游行之前
			e_lover_luxury_over,	//奢华结婚-游行之后
			e_lover_break,			//离婚状态-防止再结婚
			e_lover_maxstatus,		//最大值
		};
	public:
		void clean_up_data();
		void serialize_from_db(s_relation_info(&datas)[MAX_FRIEND_NUM * e_relationlist_type_max]);
		void serialize_to_db(s_relation_info(&datas)[MAX_FRIEND_NUM * e_relationlist_type_max]);
	public:
		guid_64 m_lover_guid;		//恋人GUID（存储）
		int8	m_lover_status;		//恋人状态（存储）
		time_t	m_marrage_time;		//结婚/离婚时间（存储）
		guid_64	m_promising_id;		//求婚对方GUID（运行时数据）
	};

	class relation_mgr
	{
	public:
		relation_mgr(client_session* client_session_ptr);
		~relation_mgr();

	public:
		void							clear_data();
		void							clear_relation_list();
		void							del_relation(const guid_64& unit_guid, e_relationlist_type type = e_relationlist_type_friend);
		void							find_player_to_add_friend();
		void							find_player_to_add_friend_by_name(const xstring& szname);
		void                            find_player_to_add_friend_by_pattern(const xstring& pattern);
		void							req_marrage(int32 req_type, client_session& req, client_session& obj);
		void							send_operate_result_to_client(e_relation_operate_result result_id);
	public:
		relation_set*					get_relation_list(e_relationlist_type relation_list_type);
		void							set_last_find_user_time(int64 time);
		int64							get_last_find_user_time();
		void							update_relation_state(bool is_send_to_client = false, bool is_send_friend_online_notice = false);
		void							load_relation_from_db(const s_relation_info(&datas)[MAX_FRIEND_NUM * e_relationlist_type_max], int32 data_num);
		void							save_relation_to_db(bool is_tick);
		void							req_load_relation_from_db();
		void							send_friend_online_notice(const xstring& friend_name, client_session* session);
		
		void							add_chat(client_session* client_session_ptr);
		void							recv_chat(guid_64 send_guid);
		void							add_relation(e_relationlist_type relation_type, client_session* client_session_ptr, bool send_client, bool remove_other);
		void							add_relation(e_relationlist_type relation_type, const guid_64& guid, int32 server_id = 0);
		int32							get_relation_num(e_relationlist_type relation_type);
		bool							is_relation_full(e_relationlist_type relation_type);
		bool							is_relation_exist(e_relationlist_type relation_type, const guid_64& friendGuid);
		bool							is_relation_exist(const guid_64& friendGuid);								//好友是否存在
		bool							add_unit_to_relation_list(const s_relation_info& relation_info, bool remove_other, bool req_add_friend);						//添加好友		
		bool							del_unit_from_relation_list(e_relationlist_type relation_type, const guid_64& friend_guid);					//删除好友
		void							sync_relation_list_to_client();										//同步全部好友列表给客户端

		bool							add_player_from_other_req(const guid_64& guid);
		void							operate_player_from_other_req(const faith::relation_proto_c2s_operate_other_add_req& packet);
		bool							send_other_add_msg_to_self(relation* req_player);

		void							req_relation_info_from_db(int32 relation_type, const guid_64& other_guid);									//从db中请求玩家数据
		void							recv_other_relation_info_from_db(const s_relation_info& other_realtion_info);									//接收db发来的load结果

		void							sync_full_relation_list_to_cs(bool is_add);									//同步所有关系人的guid到cs上


		void							add_friendliness(const guid_64& relation_guid, int32 add_value);		//增加友好度
		void							send_relation_friendliness_to_self(const guid_64& relation_guid);		//发送好友的友好度给自己
		void							fuben_add_friendliness_func(const guid_64 team_member_guid[max_team_member_num - 1], int32 team_num);		//副本增加友好度
		void							send_gift_add_friendliness_func(const guid_64& addreessee_guid, int32 gift_id, int32 gift_count);		//增加友好度
		//rename																																	
		void							change_player_name_func(const guid_64& role_guid, const xstring& role_name);	//角色改名同步
		void							send_change_name_mail_to_all_friend(const xstring& original_name, const xstring& current_name);
		void							add_relation_to_dp(guid_64 role_guid, s_relation_info relation_info);
		void							del_relation_to_dp(guid_64 role_guid, guid_64 target_role_guid, int32 relation_type);
		void							add_friend_success_chat_and_send_advices(guid_64 target_guid, s_relation_info relation_info); //同意添加好友给对方发送私聊消息
		int32							get_vip_title_template_id(client_session* client_session_ptr);																	//获得角色VIP等级称号id
		//void							send_relation_info_one(guid_64 member_guid, e_relation_data info_index);
	private:
		void							add_ret_friend_userinfo(relation_proto_s2c_ret_friend_userinfo& pak, client_session* session);

	/*	婚姻是独立的管理器	*/
	public:
		void							update_lover_name(const guid_64& rGuid, const std::string& rName);	//更新配偶名称（主要是结婚称号中的）
		void							sync_lover_to_client();												//同步伴侣信息

		guid_64							get_promosing() { return m_marriage.m_promising_id; }				//是否处于求婚状态
		void							set_promosing(guid_64 nid) { m_marriage.m_promising_id = nid; }		//设置求婚状态

		guid_64							get_lover_guid();													//得到当前对方GUID
		bool							set_lover_guid(guid_64 loverguid);									//变为婚姻关系
		int8							get_lover_status();													//得到婚姻状态
		void							set_lover_status(int8 nValue);										//变更婚姻状态

		bool							can_marry(client_session& ruser, bool btips);						//结婚判断
		void							marry(client_session& ruser);										//结婚接口

		bool							is_parade_expired();												//对于游行是否过期
		bool							can_parade(client_session& rUser, bool bTips);						//游行判断
		void							marry_parade(client_session& rUser);								//游行接口

		bool							is_breaking_heart();												//是否度过了离婚过渡期
		bool							can_divorce(bool bTips);											//离婚判断
		void							divorce();															//离婚接口
		void							on_divorce();														//离婚操作		
	private:
		client_session*					m_client_session_ptr;
		relation_set					m_releation_list[e_relationlist_type_max];							//玩家好友列表
		marriage_data					m_marriage;
		int64							m_last_update_friend_user_info_time;								//更新玩家好友信息冷却时间
		int64							m_last_find_friend_user_info_time;
		bool							m_load_data;
	};

	//结婚流程管理
	class marry_process
	{
	public:
		/*marry_process(map_cs& rscene);*/
		~marry_process() {}
	public:
		void			cleanup();
		void			heart_tick(const int64& new_time);
		void			start(const guid_64& op1, const guid_64& op2);
		void			nextstep();
		bool			isdoing();
	private:
		guid_64			m_operator1;
		guid_64			m_operator2;
		int32			m_step;
		//map_cs&			m_rscene;
		int32			m_overtime;
	};

	//结婚游街管理
	class marry_parade
	{
	public:
		//marry_parade(map_cs& rScene);
		~marry_parade() {}
	public:
		void				clean_up();
		void				heart_tick(const int64& new_time);
		void				start(const guid_64& op1, const guid_64& op2);
		bool				is_doing();
		void				end();
	private:
		guid_64				m_operator1;
		guid_64				m_operator2;
		/*map_cs&				m_rscene;*/
		std::vector<int32>	m_npcids;
		int32				m_overtime;
	};
}

#endif
