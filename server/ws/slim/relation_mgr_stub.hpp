#pragma once
#include "logic/type_def.hpp"
#include <Logic/relation_def.h>

namespace faith
{
	class client_session;

	class relation_mgr
	{
	public:
		explicit relation_mgr(client_session*) {}
		~relation_mgr() = default;
		void clear_data() {}
		void update_relation_state(bool = false, bool = false) {}
		void save_relation_to_db(bool) {}
		void req_load_relation_from_db() {}
		void sync_full_relation_list_to_cs(bool) {}
		void change_player_name_func(const guid_64&, const xstring&) {}
	};
}
