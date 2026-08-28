#ifndef _WS_CHAT_MGR_H_
#define _WS_CHAT_MGR_H_

#include "Logic/chat_def.hpp"

namespace hld
{
	class chat_mgr_ws
	{
	public:
		explicit chat_mgr_ws();
		~chat_mgr_ws();

		static chat_mgr_ws& get_instance()
		{
			static chat_mgr_ws instance;
			return instance;
		}

		void forbidden_word_check(e_chat_content_info* chat_content_ary, int32 chat_content_num);
		void check_forbidden_and_reconstruct(std::string& str_to_be_checked, const std::vector<int32>& chat_composition, e_chat_content_info* chat_content_ary);	
	};
}

#endif