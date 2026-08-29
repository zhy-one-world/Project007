#ifndef _PERSON_INFORMATION_MGR_H_
#define _PERSON_INFORMATION_MGR_H_

#include "item.h"

namespace faith
{
	class person_information_mgr
	{
	public:
		person_information_mgr();
		~person_information_mgr();

	public:
		void									set_unit_index(int32 array_index) { m_array_index = array_index; };
		void									get_person_information_to_dp();

		void									get_other_person_information_to_dp(guid_64 target_guid, bool is_ask_player_base_info);

		void									get_other_person_information_to_dp_end(guid_64 target_guid, s_role_person_information role_info, bool is_ask_player_base_info);

		void									update_person_infomation(s_role_person_information role_info);

		bool                                    load_role_person_infomation_by_db_lua(const char *data_ptr, int32 data_len);

		void									save_to_db(e_save_role_data_type eType);

		void									sync_to_client();

		void									get_reward();

		void									clear_data();

		void									sync_to_ws();

		void									delete_from_ws();

		s_role_person_information&				get_person_info() { return m_cur_person_info;};

		void									mofify_person_information_lua(xstring gender, xstring personalized_signature, int32 birthday_year, int32 birthday_mouth, int32 birthday_day, xstring city, xstring hometown, xstring online_state);
	
		void									add_person_information_tag(int32 tag_id);

		void									clear_person_information_tags();

		bool									check_person_information(xstring personalized_signature, xstring city, xstring hometown);
	private:
		int32									m_array_index;

		s_role_person_information				m_cur_person_info;
	};


}
#endif
