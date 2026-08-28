#ifndef _WS_LEGION_WS_WAREHOUSE_INFO_H_
#define _WS_LEGION_WS_WAREHOUSE_INFO_H_

#include "Logic/legion_def.hpp"
#include "legion.pb.h"
#include <core.hpp>

namespace hld
{
	class legion_ws_warehouse
	{
	public:
		legion_ws_warehouse();
		~legion_ws_warehouse();
		void clear_data();

		std::list<s_legion_warehouse_info>&			get_legion_warehouse_list() { return m_legion_warehouse_list; }
		int32										get_legion_warehouse_count() const { return m_legion_warehouse_list.size(); }
		bool                                        add_legion_warehouse(const s_legion_warehouse_info& legion_warehouse_info, bool is_from_db);
		void                                        add_legion_log(const s_legion_warehouse_log& log, bool is_from_db);
		void                                        del_legion_warehouse(const guid_64& item_guid);
		s_legion_warehouse_info						get_item(const guid_64& item_guid);
		bool										is_in_legion_warehouse(const guid_64& item_guid);
		void                                        send_item_list_to_client(uint64 role_guid);
		void                                        send_log_list_to_client(uint64 role_guid);
		void										make_package_info(legion_proto_legion_warehouse_item_info* target_info,const s_legion_warehouse_info& source_info);
		void                                        operation_store_legion_warehouse(const cs2ws_legion_warehouse_operation& info);
		int32                                       operation_store(const cs2ws_legion_warehouse_operation& info);
		void                                        operation_out_legion_warehouse(const cs2ws_legion_warehouse_operation& info);
		int32                                       operation_out(const cs2ws_legion_warehouse_operation& info);
		void                                        destroy_item(const cs2ws_legion_warehouse_operation& info);
		void                                        clear_legion_warehouse_info_in_db(const guid_64& legion_guid);
		void                                        send_notice(e_legion_warehouse_operation_type operation_type,const guid_64& role_guid,int32 item_id);
		void										save_legion_warehouse(const s_legion_warehouse_info& info);
		void										save_legion_warehouse_log(const s_legion_warehouse_log& log);
	private:
		std::list<s_legion_warehouse_info>			m_legion_warehouse_list;
		std::list<s_legion_warehouse_log>           m_legion_warehouse_log;
	};
}

#endif