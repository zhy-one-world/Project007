#ifndef _APPEARANCE_INFO_MGR_H_
#define _APPEARANCE_INFO_MGR_H_

#include "Logic/char_def.hpp"
#include "server_def.hpp"
#include "base.hpp"

namespace faith
{
	class appearance_info_mgr
	{
	public:
		explicit appearance_info_mgr();
		~appearance_info_mgr();

		static appearance_info_mgr& get_instance()
		{
			static appearance_info_mgr instance;
			return instance;
		}

		void req_save_appearance_info_to_id(s_client_uid client_uid, const std::string& appearance_info);
		void req_save_appearance_info_to_id_end(s_client_uid client_uid, const std::string& appearance_info, int32 id);
		void req_load_appearance_info_by_id(s_client_uid client_uid, int32 id);
		void req_load_appearance_info_by_id_end(s_client_uid client_uid, int32 id, const std::string& appearance_info);

	private:
		std::map<int32, std::string> m_appearance_info_by_id;
		std::map<std::string, int32> m_appearance_info_by_string;
	};
}

#endif
