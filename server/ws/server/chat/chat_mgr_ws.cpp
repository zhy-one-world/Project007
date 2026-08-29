#include "chat_mgr_ws.h"
#include "../client_session_mgr.hpp"
#include "ws_client.hpp"
#include <legion_msg.hpp>
#include "Logic/time_def.hpp"
#include "time.hpp"
#include "template/template_manager.h"
#include "Logic/activity_def.hpp"
#include "../mail/event_ws_mgr.h"
#include "utility/init_unit.h"
#include "invalid_word/trie_filter.h"

namespace faith
{
	chat_mgr_ws::chat_mgr_ws()
	{
	}

	chat_mgr_ws::~chat_mgr_ws()
	{

	}

	void chat_mgr_ws::forbidden_word_check(e_chat_content_info* chat_content_ary, int32 chat_content_num)
	{
		std::vector<int>	chat_composition;
		std::string			str_to_be_checked;
		for (int i = 0; i < chat_content_num; ++i)
		{
			e_chat_content_info& temp_chat_info = chat_content_ary[i];
			if (temp_chat_info.is_only_text())
			{
				str_to_be_checked += temp_chat_info.common_text;
				chat_composition.push_back(strlen(temp_chat_info.common_text));
			}
			else
			{
				chat_composition.push_back(0);
			}

		}
		check_forbidden_and_reconstruct(str_to_be_checked, chat_composition, chat_content_ary);

	}

	void chat_mgr_ws::check_forbidden_and_reconstruct(std::string& str_to_be_checked, const std::vector<int32>& chat_composition, e_chat_content_info* chat_content_ary)
	{
		int32 pos = 0;		//current checking position in string
		trie_filter::get_instance().find_forbidden(str_to_be_checked);

		for (int32 j = 0; j < chat_composition.size(); ++j)
		{
			std::string temp;

			if (chat_composition[j] == 0) // if the content is not text
			{
				continue;
			}

			for (int32 k = pos; k < pos + chat_composition[j]; ++k)
			{
				// if the content is forbidden and be replaced by ***
				if (chat_composition[j] >= 1 && str_to_be_checked[k] == '*')
				{
					temp = "*";
					break;
				}
				temp += str_to_be_checked[k];
			}
			pos += chat_composition[j];
			memset(chat_content_ary[j].common_text, 0, chat_content_info_max + 1);
			int32 str_len = temp.size() > chat_content_info_max ? chat_content_info_max : temp.size();
			memcpy(chat_content_ary[j].common_text, temp.c_str(), str_len);
		}
	}
}
