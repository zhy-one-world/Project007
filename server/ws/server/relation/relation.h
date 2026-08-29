/********************************************************************
	file name:	relation.h
	created:	2017/05/26 13:49
	author:		zhy
	purpose:	
*********************************************************************/
#ifndef _RELATION_H_
#define _RELATION_H_

#include "Logic/relation_def.h"
#include "Logic/team_def.hpp"



namespace faith
{
	class client_session;
	//关系人数据
	class relation
	{
	public:
		relation();
	public:
		void					clear_data();				//清除数据
		bool					build_relation(client_session* puser);
		bool					is_valid();
		void					set_is_use(bool is_use) { m_is_use = is_use; }
		bool					serialize_to_db(s_relation_info& datas);	//数据拷贝到存储结构中
		bool					serialize_from_db(const s_relation_info& datas);//存储结构中数据拷贝

	public:
		s_relation_info&		get_data_all() { return m_relation_info; }
		guid_64&				get_data_guid() { return m_relation_info.relation_guid; };
		void					set_data_guid(guid_64 guid) { m_relation_info.relation_guid = guid; };
		xchar*					get_data_name() { return m_relation_info.relation_name; };
		void                    set_data_name(const xchar *role_name);
		int32					get_data_value(e_relation_data infotype);
		bool					set_data_value(e_relation_data infotype, int32 value);
		int64					get_data_gs_value() { return m_relation_info.gs_value; };
		void					set_data_gs_value(int64 gs_value);
	
	private:
		s_relation_info			m_relation_info;			//好友的信息
		bool					m_is_use;
	};
}

#endif
