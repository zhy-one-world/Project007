/********************************************************************
	file name:	relation_set.h
	created:	2017/05/26 13:49
	author:		zhy
	purpose:	
*********************************************************************/
#ifndef _RELATION_SET_H_
#define _RELATION_SET_H_

#include "relation.h"
#include "Logic/team_def.hpp"



namespace hld
{
	class client_session;
	typedef std::list<relation> relation_list;
	typedef relation_list::iterator relation_list_it;
	//玩家好友列表
	class relation_set
	{
	public:
		relation_set();
	public:
		//好友操作接口
		void					clear_data();
		void					set_client_session(client_session* client_session_ptr);							//设置拥有者类型
		void					set_relation_type(int32 itype);													//设置关系人的关系类型
		bool					is_relation(const guid_64& relation_guid);										//关系人是否已经存在
		bool					add_relation(const s_relation_info& other_realtion, bool send_client);		//增加关系人
		bool					del_relation(const guid_64& relation_guid);									//删除关系人
		bool					is_full();
		int32					get_relation_num();														//得到关系人数量
		relation*				get_relation(const guid_64& relation_guid);									//获得关系人
		int32					get_max_firendliness();													//获得好感度最高的数据
		relation_list&			get_relation_list() { return m_relation_array; }
		//数据存储接口
	public:
		bool					serialize_to_db(s_relation_info* data_list, int32& list_count);
		bool					serialize_from_db(const s_relation_info& datas);
		void					send_relation_add_msg_to_self(relation& relation_ref);
		void					send_relation_del_msg_to_self(guid_64 relation_guid);
		void					send_sync_relation_list_to_client(int32 e_msg_relation_type);
	private:
		client_session*			m_client_session_ptr;
		int32					m_relation_type;														//关系列表类型
		relation_list			m_relation_array;														//玩家关系人列表
	};
}

#endif
